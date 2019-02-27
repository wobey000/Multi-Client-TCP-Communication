#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "192.168.1.16"; //ip address of server (local host)
	int port = 54000; //listening port # ib tge server

	//initialize winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start winsock, error #" << wsResult << endl;
		system("pause");
		return;
	}

	//create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, error #" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return;
	}

	// fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, error #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		system("pause");
		return;
	}

	//do-while loop to send and receive data
	char buf[4096];
	string userInput;

	do
	{
		//prompt the user for some text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0) // make sure the user has typed in something
		{
			//send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				//wait for a response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					// echo response to console
					cout << "SERVER > " << string(buf, 0, bytesReceived) << endl;

				}
			}

		}

	} while (userInput.size() > 0);

	system("pause");

	//gracefully close down everything

	closesocket(sock);
	WSACleanup();

}