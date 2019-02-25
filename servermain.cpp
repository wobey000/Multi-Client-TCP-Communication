#include <iostream>
#include <string>
#include <ws2tcpip.h>
#include <sstream>


#pragma comment (lib, "ws2_32.lib")

using namespace std;


void main()
{
	//initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);

	if (wsok != 0)
	{
		cerr << "Can't initialize winsock! Quitting" << endl;
		return;
	}

	//create socket
	//end point, a number or handle ` just a note to myself of what a socket is
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	//bind the socket to an ip address and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // could also use inet_pton...

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);

	FD_SET(listening, &master);

	while (true)
	{
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];
			if (sock == listening)
			{
				//accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);
				cout << "new client connected\n";


				//add the new connection to the list of connected clients *VERY IMPORTANT PART*
				FD_SET(client, &master);


				// send welcome message to the connected clients
				string welcomeMsg = "Welcome to the chat server!\r\n";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
			}
			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				//receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					//drop the client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else
				{
					//send message to other clients and definiately NOT the listening client
					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock)
						{
							ostringstream ss;
							ss << "SOCKET #" << sock << ": " << buf << "\r";
							string strOut = ss.str();
							 
							send(outSock, strOut.c_str(), strOut.size() + 1, 0);
						}
					}
				}
			}
		}
	}

	//cleanup winsock
	WSACleanup();

}