// Oliver 1930026012
// Peter 1930026027
//Assignment_2_Group Project


#pragma warning(disable: 4996)
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>


#define DEFAULT_PORT 5019
SOCKET connect_sock;// the server socket

void Recv() {
	//receive message from server
	char buff[280];//the message
	int t = 0;
	printf("####  Chatting  ####\n");
	while (1) {
		t = recv(connect_sock, buff, sizeof(buff), 0);//wait for server receving
		if (t == SOCKET_ERROR) {
			Sleep(500);
			continue;
		}
		if (strlen(buff) != 0)//if have information then print it
		{
			printf("%s", buff);
		}
	}
	_endthread();

}

int main(int argc, char** argv) {
	char valid[10];//valid(execute correct or not)
	char input[10];//menu response(input a/b/c/d)
	char userName[20];//username
	char password[20];//password
	char uname[20];//the name want to talk to
	char szBuff[100];//info char string
	int msg_len;//message lenth
	//int addr_len;
	struct sockaddr_in server_addr;
	struct hostent* hp;
	WSADATA wsaData;

	char* server_name = "localhost";
	unsigned short	port = DEFAULT_PORT;
	unsigned int	addr;

	/*if (argc != 3) {
		printf("echoscln [server name] [port number]\n");
		return -1;
	}
	else {
		server_name = argv[1];
		port = atoi(argv[2]);
	}*/

	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {
		// stderr: standard error are printed to the screen.
		fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
		//WSACleanup function terminates use of the Windows Sockets DLL. 
		WSACleanup();
		return -1;
	}

	if (isalpha(server_name[0]))
		hp = gethostbyname(server_name);
	else {
		addr = inet_addr(server_name);
		hp = gethostbyaddr((char*)&addr, 4, AF_INET);
	}

	if (hp == NULL)
	{
		fprintf(stderr, "Cannot resolve address: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	//copy the resolved information into the sockaddr_in structure
	memset(&server_addr, 0, sizeof(server_addr));
	memcpy(&(server_addr.sin_addr), hp->h_addr, hp->h_length);
	server_addr.sin_family = hp->h_addrtype;
	server_addr.sin_port = htons(port);


	connect_sock = socket(AF_INET, SOCK_STREAM, 0);	//TCp socket


	if (connect_sock == INVALID_SOCKET) {// check invalid or not
		fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Client connecting to: %s\n", hp->h_name);

	if (connect(connect_sock, (struct sockaddr*)&server_addr, sizeof(server_addr))
		== SOCKET_ERROR) {
		fprintf(stderr, "connect() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}//connect
	
	//login and register menu
	while (1) {
		//the login and register function
		printf("*Welcome to the chat room*\n");
		printf("+---------------+\n");
		printf(" a: Login		\n");
		printf(" b: Register	\n");
		printf("+---------------+\n\n");
		printf("Please choose: ");
		scanf("%s", input);
		if (strcmp(input, "a") == 0) {//choose a then login
			while (1) {
				//get information as well as send and receive
				printf("Please input your username: ");
				scanf("%s", userName);
				printf("Please input your password: ");
				scanf("%s", password);
				send(connect_sock, userName, sizeof(userName), 0);
				send(connect_sock, password, sizeof(password), 0);
				send(connect_sock, "login", sizeof("login"), 0);
				recv(connect_sock, valid, sizeof(valid), 0);// get the response from server
				//valid = wrong then output error inforamtion
				//valid = correct then output the success inforamtion
				if (strcmp(valid, "wrong") == 0) {
					printf("Wrong username or password, please input again!\n");
					continue;
				}
				else {
					break;
				}
			}
		}
		else if (strcmp(input, "b") == 0) {//choose b then register
			while (1) {
				//get information as well as send and receive
				printf("Please input your username: ");
				scanf("%s", userName);
				printf("Please input your password: ");
				scanf("%s", password);
				send(connect_sock, userName, sizeof(userName), 0);
				send(connect_sock, password, sizeof(password), 0);
				send(connect_sock, "register", sizeof("register"), 0);
				recv(connect_sock, valid, sizeof(valid), 0);// get the response from server
				if (strcmp(valid, "wrong") == 0) {//username exit 
					printf("Username already exists, please input again!\n");
					continue;
				}
				else {//this is correct registering then break go to next step
					break;
				}
			}
		}
		else {//this is input not login(1) nor register(2)
			printf("Wrong input!Please input again!\n");
			continue;
		}
		break;
	}


	printf("You have successfully logged in!\n");

	while (1) {
		//the menu for functions
		printf("+------------------------+\n");
		printf("| a: One-to-one chatting |\n");
		printf("| b: Chat in a group     |\n");
		printf("| c: Exit                |\n");
		printf("| d: Delete account      |\n");
		printf("+------------------------+\n\n");

		printf("Please choose: ");
		scanf("%s", input);

		if (strcmp(input, "a") == 0) {//one to one chat
			send(connect_sock, "single", sizeof("single"), 0);// tell the server
			while (1) {
				printf("Please input the username of the people you want to chat with: ");
				scanf("%s", uname);

				if (strcmp(uname, userName) == 0) {//can not write yourself to talk
					printf("You can't chat with yourself!\n");
					continue;
				}
				else {//others
					send(connect_sock, uname, sizeof(uname), 0);
					recv(connect_sock, valid, sizeof(valid), 0);// get the response from server
					if (strcmp(valid, "wrong") == 0) {//is invalid
						printf("The user doesn't exist or is offline!\n");
						continue;//then loop again until correct input
					}
					else {
						break;//input correct out of while
					}
				}
			}
			

		}
		else if (strcmp(input, "b") == 0) {
			send(connect_sock, "group", sizeof("group"), 0);// tell the server
			recv(connect_sock, valid, sizeof(valid), 0);//get the response from server
			//go to the thread
		}
		else if (strcmp(input, "c") == 0) {
			send(connect_sock, "exit", sizeof("exit"), 0);// tell the server
			recv(connect_sock, valid, sizeof(valid), 0);//get the response from server
			printf("Your account is exited, BYE\n");
			//go to the thread
		}
		else if (strcmp(input, "d") == 0) {
			char delete_note[10];
			//delete or not user interface
			printf("Do you want to delete your account? Which means that this account will be removed\n");
			printf("(delete:1; cancel:2)\n");
			scanf("%s", delete_note);
			if (strcmp(delete_note, "1") == 0) {//sure to delete
				send(connect_sock, "delete", sizeof("delete"), 0);//tell the server
				recv(connect_sock, valid, sizeof(valid), 0);//get the response from server
				printf("Your account has already deleted,");
				printf("wish we will meet again.\n");
				//then quit
				printf("BYE\n");
				return 0;
			}
			else {
				continue;//still doing the menu to wait command of user
			}
		}
		else {
			printf("Wrong input!Please input again.\n");//looping until corrct input
			continue;
		}
		
		
		_beginthread(Recv, 0, NULL);//begin thread, turn to func Recv

		while (1) { //sending messages to the server
			gets(szBuff);
			msg_len = send(connect_sock, szBuff, sizeof(szBuff), 0);
			if (msg_len == SOCKET_ERROR) {//error
				fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
				WSACleanup();
				return -1;
			}

			if (msg_len == 0) {//error
				printf("server closed connection\n");
				closesocket(connect_sock);
				WSACleanup();
				return -1;
			}
		}
	}

	// over and closesocket
	closesocket(connect_sock);
	WSACleanup();
	return 0;
}
