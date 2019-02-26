
// EXAMPLE OF USAGE

#include <iostream>
#include <string>
#include "client.h"

using namespace std;

//any msg send by server will call this function
void MessageReceived(std::string msg_received)
{
	cout << endl;
	std::cout << msg_received << endl;
	cout << "> ";
}

int main()
{
	string inputstr;
	TCPClient *client = new TCPClient;
	string userName;


	if (client->Connect("192.168.1.16", 54000))
	{

		//get message from server first
		client->ListenRecvInThread(MessageReceived);


		Sleep(500);
		//ask user to enter a username
		cout << "Enter username: ";
		getline(cin, userName);
		cout << "Start typing, " << userName << endl;

		while (true)
		{
			cout << "> ";
			std::getline(std::cin, inputstr);
			if (inputstr == "exit") break;

			inputstr = userName + ": " + inputstr;
			client->Send(inputstr);
		}
	}
	else {
		cout << "Connect() fail." << endl;
	}

	delete client;


	cin.get(); // wait
	return 0;
}





































//#include <iostream>
//#include <string>
//#include <WS2tcpip.h>
//
//#pragma comment(lib, "ws2_32.lib")
//
//using namespace std;
//
//void main()
//{
//	string ipAddress = "192.168.1.16"; //ip address of server (local host)
//	int port = 54000; //listening port # ib tge server
//
//	//initialize winsock
//	WSADATA data;
//	WORD ver = MAKEWORD(2, 2);
//	int wsResult = WSAStartup(ver, &data);
//	if (wsResult != 0)
//	{
//		cerr << "Can't start winsock, error #" << wsResult << endl;
//		return;
//	}
//
//	//create socket
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (sock == INVALID_SOCKET)
//	{
//		cerr << "Can't create socket, error #" << WSAGetLastError() << endl;
//		WSACleanup();
//
//		return;
//	}
//
//	// fill in a hint structure
//	sockaddr_in hint;
//	hint.sin_family = AF_INET;
//	hint.sin_port = htons(port);
//	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
//
//	// connect to server
//	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
//	if (connResult == SOCKET_ERROR)
//	{
//		cerr << "Can't connect to server, error #" << WSAGetLastError() << endl;
//		closesocket(sock);
//		WSACleanup();
//
//		return;
//	}
//
//	//do-while loop to send and receive data
//	char buf[4096];
//	string userInput;
//	int bytesReceived = recv(sock, buf, 4096, 0);
//	cout << "SERVER > " << string(buf, 0, bytesReceived) << endl;
//	do
//	{
//		//prompt the user for some text
//		cout << "> ";
//		getline(cin, userInput);
//
//		if (userInput.size() > 0) // make sure the user has typed in something
//		{
//			//send the text
//			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
//
//			if (sendResult != SOCKET_ERROR)
//			{
//				ZeroMemory(buf, 4096);
//				int bytesReceived = recv(sock, buf, 4096, 0);
//				cout << string(buf, 0, bytesReceived) << endl;
//			}
//
//		}
//
//	} while (userInput != "exit");
//
//	userInput = "Good bye, User has disconnected";
//	send(sock, userInput.c_str(), userInput.size() + 1, 0);
//
//
//
//	//gracefully close down everything
//	closesocket(sock);
//	WSACleanup();
//
//}