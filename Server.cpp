// Oliver 1930026012
// Peter 1930026027
//Assignment_2_Group Project


#pragma warning(disable: 4996)
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>


typedef struct socket
{
	SOCKET client;//socket
	char username[20];//username
	char password[20];//password
	char buf[256];//buffer and info
	int flag;//notation of this client number
	char chatMode[30]; //chatting
	int online;
}NewSOCKET;

//maximum 20 people
NewSOCKET sClient[20];

#define DEFAULT_PORT	5019
int count = 0, online = 0;
struct sockaddr_in local, client_addr;

void GroupSendRecv(NewSOCKET* Client) { //client in this code are not global
	char buff[100];
	char temp[2] = ":";
	int ret = 0;
	int i;
	while (1) {
		ret = recv(Client->client, buff, sizeof(buff), 0);//receive the message
		
		if (ret > 0) {
			//send the information to the one to one talk user
			//the friendly notation"+++++++++++++++++++++\n"
			//"\n---------------------\n"
			strcpy(Client->buf, "+++++++++++++++++++++\n");
			strcat(Client->buf, Client->username);
			strcat(Client->buf, temp);
			strcat(Client->buf, buff);
			strcat(Client->buf, "\n---------------------\n");
			for (i = 0; i < count; i++) { // send messages to other clients
				if (i != Client->flag && strlen(buff) != 0 && sClient[i].online == 1) { //only send to the online client
					send(sClient[i].client, Client->buf, sizeof(Client->buf), 0);
				}
			}
		}
	}
}

void SingleSendRecv(NewSOCKET* Client, char* uname) { //sclient are not global
	char buff[100];
	char temp[2] = ":";
	int ret = 0;
	int i;
	printf("You sent information to %s", uname);
	while (1) {
		ret = recv(Client->client, buff, sizeof(buff), 0);
		if (ret > 0) {
			//send the information to everyone
			//the friendly notation"+++++++++++++++++++++\n"
			//"\n---------------------\n"
			strcpy(Client->buf, "+++++++++++++++++++++\n");
			strcat(Client->buf, Client->username);
			strcat(Client->buf, temp);
			strcat(Client->buf, buff);
			strcat(Client->buf, "\n---------------------\n");
			for (i = 0; i < count; i++) { // //send private
				if (strcmp(sClient[i].username, uname) == 0 && sClient[i].online == 1) {//only send to the online client
					send(sClient[i].client, Client->buf, sizeof(Client->buf), 0);
				}
			}
		}
	}
}

int VerifyLogin(char name[20], char pwd[20]){//to verfy the login user
	FILE* fp;
	char nameV[20], pwdV[20];
	/*
	*mode : r
	*Format of the file
			'username'+' '(space)+'password'\n
		E.g.:	Alice 123
				Bob 4566
	*/
	if ((fp = fopen("user.txt", "r")) == NULL)
	{
		printf("cannot open file\n");
		return -1;
	}
	/*
	*determine whether the user is in the file
	*/
	while (!feof(fp))
	{
		fscanf(fp, "%s%s", nameV, pwdV);
		if (strcmp(nameV, name) == 0 && strcmp(pwdV, pwd) == 0)
		{
			printf("The account exists!\n");
			return 1;
		}
	}
	fclose(fp);

	return 0;
}

int VerifyRegister(char name[20]) {// to check whether have this username or not
	FILE* fp;
	char nameV[20], pwdV[20];
	/*
	*mode : r
	*Format of the file
			'username'+' '(space)+'password'\n
		E.g.:	Alice 123
				Bob 4566
	*/
	if ((fp = fopen("user.txt", "r")) == NULL)
	{
		printf("cannot open file\n");
		return -1;
	}
	/*
	*determine whether the user is in the file
	*/
	while (!feof(fp))
	{
		fscanf(fp, "%s%s", nameV, pwdV);
		if (strcmp(nameV, name) == 0)
		{
			return 0;
		}
	}
	fclose(fp);

	return 1;
}

int Register(char name[20], char pwd[20]) {// add the new user information
	FILE* fp;
	/*
	*mode : a
	*Format of the file
			'username'+' '(space)+'password'\n
		E.g.:	Alice 123
				Bob 4566
	*/
	if ((fp = fopen("user.txt", "a")) == NULL)
	{
		printf("cannot open file\n");
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "%s %s\n", name, pwd);//write new client
	fclose(fp);

	return 1;
}

int VerifySingle(char* name) {
	FILE* fp;
	char nameV[20], pwdV[20];
	/*
	*mode : r
	*Format of the file
			'username'+' '(space)+'password'\n
		E.g.:	Alice 123
				Bob 4566
	*/
	if ((fp = fopen("user.txt", "r")) == NULL)
	{
		printf("cannot open file\n");
		return -1;
	}
	/*
	*determine
	*/
	while (!feof(fp))
	{
		fscanf(fp, "%s%s", nameV, pwdV);
		if (strcmp(nameV, name) == 0)
		{
			return 1;
		}
	}
	fclose(fp);

	return 0;
}

int deleteAccount(char username[20], char password[20]) {//delete the information of user
	/*
	*mode : r,w
	*Format of the file
			'username'+' '(space)+'password'\n
		E.g.:	Alice 123
				Bob 4566
	*/
	//function to delete the acount of the client
	char username_file[20];//the username in the file
	char password_file[20];//the password in the file
	char ch;//the sentence get from the data file
	FILE* f1;//user.txt
	FILE* f2;//the copied.txt
	FILE* data, * user;
	f1 = fopen("user.txt", "r");//read
	f2 = fopen("data.txt", "w");//write
	fscanf(f1, "%s %s", username_file, password_file);
	while (!feof(f1)) {
		if (strcmp(username, username_file) == 0 && strcmp(password, password_file) == 0) {//do not get this
			//do nothing
		}
		else {
			fputs(username_file, f2);
			fputs(" ", f2);
			fputs(password_file, f2);
			fputs("\n", f2);
		}
		fscanf(f1, "%s %s", username_file, password_file);
	}
	fclose(f1);
	fclose(f2);


	//copy the file
	/*
	*mode : r,w
	*Format of the file
			'username'+' '(space)+'password'\n
		E.g.:	Alice 123
				Bob 4566
	*/


	if ((data = fopen("data.txt", "r")) == NULL) {
		printf("canot find the in.txt file!\n");
		exit(0);
	}
	if ((user = fopen("user.txt", "w")) == NULL) {
		printf("canot find the out.txt file!\n");
		exit(0);
	}
	ch = fgetc(data);
	while (ch != EOF) {
		fputc(ch, user);
		ch = fgetc(data);
	}
	fclose(data);
	fclose(user);

	return 1;
}

void ServerThread(NewSOCKET* Client) {
	char lo_re[10];//check what function it will execute
	char uname[20];//the name want to talk to
	int is_online = 0;//to check other user's state
	int i;
	//login and register
	while (1) {
		recv(Client->client, Client->username, sizeof(Client->username), 0); //get name
		recv(Client->client, Client->password, sizeof(Client->password), 0); //get password
		printf("Received username and password\n");
		recv(Client->client, lo_re, sizeof(Client->chatMode), 0); //get state

		if (Client->client == INVALID_SOCKET) {
			fprintf(stderr, "accept() failed with error %d\n", WSAGetLastError());
			WSACleanup();
			//return -1;
		}

		if (strcmp(lo_re, "login") == 0) {
			if (VerifyLogin(Client->username, Client->password) > 0) {//to check meet the name and password and login
				printf("Thread:%d accepted connection from:%s, port:%d, username:%s\n",
					GetCurrentThreadId(),
					inet_ntoa(client_addr.sin_addr),
					htons(client_addr.sin_port),
					Client->username);
				send(Client->client, "correct", sizeof("correct"), 0);//success
				break;
			}
			else {//wrong
				send(Client->client, "wrong", sizeof("wrong"), 0);//failed
			}
		}
		else {
			if (VerifyRegister(Client->username) > 0) {//to check no one is same and add the information of user
				printf("Thread:%d accepted connection from:%s, port:%d, username:%s\n",
					GetCurrentThreadId(),
					inet_ntoa(client_addr.sin_addr),
					htons(client_addr.sin_port),
					Client->username);
				send(Client->client, "correct", sizeof("correct"), 0);//success
				Register(Client->username, Client->password);
				break;
			}
			else {
				send(Client->client, "wrong", sizeof("wrong"), 0);//failed
			}
		}
	}

	Client->online = 1;//notation
	printf("Online %d\n", sClient[Client->flag].online);
	online++;
	/*printf("count = %d\n", count);
	printf("online = %d\n", online);*/
	recv(Client->client, Client->chatMode, sizeof(Client->chatMode), 0);//get state
	printf("Get state: chatting\n");

	if (strcmp(Client->chatMode, "single") == 0) {
		while (1) {
			recv(Client->client, uname, sizeof(uname), 0);//get username
			if (VerifySingle(uname) == 1) {//have that user

				for (i = 0; i < count; i++) { // //send private
					if (strcmp(sClient[i].username, uname) == 0 && sClient[i].online == 1) {//only send to the online client
						is_online = 1;//user is online
						break;
					}
				}
				if (is_online == 1) {// he is online
					//should verify the private user's state
					send(Client->client, "correct", sizeof("correct"), 0);//tell it success
					SingleSendRecv(Client, uname);
				}
				else {// not online
					send(Client->client, "wrong", sizeof("wrong"), 0);//not online
				}

			}
			else {
				send(Client->client, "wrong", sizeof("wrong"), 0);//user not exists
			}
		}
		
	}
	else if (strcmp(Client->chatMode, "group") == 0) {
		send(Client->client, "correct", sizeof("correct"), 0);//tell it success
		GroupSendRecv(Client);
	}
	else if (strcmp(Client->chatMode, "exit") == 0) {
		send(Client->client, "correct", sizeof("correct"), 0);//tell it exit
		//then quit this thread
		printf("\n\nExit:% d EXIT connection from : % s, port : % d, username : % s\n\n", GetCurrentThreadId(), inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), Client->username);
		closesocket(Client->client);
	}
	else if (strcmp(Client->chatMode, "delete") == 0) {
		send(Client->client, "correct", sizeof("correct"), 0);//tell it deleted
		deleteAccount(Client->username, Client->password);//delete
		//then quit this thread
		printf("\n\nExit:% d EXIT and DELETE connection from : % s, port : % d, username : % s\n\n", GetCurrentThreadId(), inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), Client->username);
		closesocket(Client->client);
	}
	//_endthread();
}



int main(int argc, char** argv) {
	int addr_len;
	SOCKET sock;
	WSADATA wsaData;

	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {
		// stderr: standard error are printed to the screen.
		fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
		//WSACleanup function terminates use of the Windows Sockets DLL. 
		WSACleanup();
		return -1;
	}
	// Fill in the address structure
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(DEFAULT_PORT);

	sock = socket(AF_INET, SOCK_STREAM, 0);	//TCP socket

	// verify the socket
	if (sock == INVALID_SOCKET) {
		fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// bind local address to the socket
	if (bind(sock, (struct sockaddr*)&local, sizeof(local)) == SOCKET_ERROR) {
		fprintf(stderr, "bind() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	//waiting for the connections
	if (listen(sock, 5) == SOCKET_ERROR) {
		fprintf(stderr, "listen() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}


	printf("Waiting for connections ........\n");

	// block, until the request arrives
	addr_len = sizeof(client_addr);
	// store the address family of the client in client_addr


	while (1) {
		sClient[count].client = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
		sClient[count].flag = count;
		sClient[count].online = 0;
		//printf("count = %d\n", count);
		//printf("online = %d\n", online);
		_beginthread(ServerThread, 0, &sClient[count]);
		count++;
		//printf("count++ = %d\n", count);

	}

	closesocket(sClient[count].client);
	WSACleanup();

	return 0;
}


