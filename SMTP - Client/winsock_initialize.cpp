#include "winsock_initialize.h"
WSADATA wsa;
SOCKET s;
struct sockaddr_in server;
int initialize_socket()
{
	/*----------------INICJALIZACJA WINSOCKA----------------------*/
	cout << "\nInicjalizacja Winsocka...\n";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		/*WSASStartup - funkcja u¿ywana by rozpocz¹æ winsock, MAKEWORD - wersja któr¹ chcemy wczytaæ,
		&wsa - struktura ktora po za³adowaniu winsocka, trzyma dodatkowe informacje */
		printf("Niepowodzenie. Error Code : %d", WSAGetLastError()); //je¿eli powstanie b³¹d, funkcja WSAGetLastError pokaze informacje o b³êdzie
		cout << endl;
		return 1;
	} printf("Zainicjalizowano pomyslnie.\n");
	return 0;
}


void create_socket(){
/*-----------------TWORZENIE SOCKETU----------------------*/
if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) { //funkcja socket tworzy socket i zwraca jego opis, ktory mo¿e byæ wykorzystany w sieciowych komendach
															   //AF_INET - adres z rodziny IPv4  SOCK_STREAM - wykorzystujemy protokó³ TCP, 0 - protokó³
	printf("Nie mozna stworzyc socketu : %d", WSAGetLastError());
}	printf("Socket stworzony.\n");
}

int connect_with_server(const char *address, int port) {
	/*------------------£¥CZENIE Z SERWEREM-----------------------------*/
	server.sin_addr.s_addr = inet_addr(address); //adres IP serwera
	server.sin_family = AF_INET;
	server.sin_port = htons(port); //port HTTP

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("Problem z polaczeniem.");
		return 0;
	}
	puts("Polaczono!");
	return 1;
}


int say_hello(char *comm) { //comm pobiera jako polecenie

	////puts(comm);
	if (send(s, comm, strlen(comm), 0) < 0)
	{
		//puts("Nie udalo sie wyslac danych");
		return 0;
	}

	//puts("Pomyslnie wyslano dane");
	return 1;
}
int send_data(char *comm) { //comm pobiera jako polecenie

	////puts(comm);
	if (send(s, comm, strlen(comm), 0) < 0)
	{
		//puts("Nie udalo sie wyslac danych");
		return 0;
	}

	//puts("Pomyslnie wyslano dane");
	return 1;
}

int send_message(){ //comm pobiera jako polecenie
	/*------------------WYSYLANIE DANYCH-----------------------------*/

	char *message;
	message = new char[2000];
	cin.getline(message, 2000);
	//message = comm;
	//comm = new char[12];
	
	////puts(message);
	if (send(s, message, strlen(message), 0) < 0)
	{
		puts("Nie udalo sie wyslac wiadomosci");
		return 0;
	} 

	//puts("Pomyslnie wyslano dane");
	return 1;
}

char *receive_data() {
	/*------------------ODBIERANIE DANYCH-----------------------------*/
	char server_reply[2000];
	int recv_size;
	if ((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR)
	{
		puts("Niepowodzenie podczas odbioru danych");
	}

	if (recv_size < 1) {/* puts("Nie otrzymano zadnej wiadomosci.");*/ }
	if (recv_size >=1) {
		////puts("Otrzymano odpowiedz od serwera");
		server_reply[recv_size] = '\0';
		//puts(server_reply);
	}

	return server_reply;
}

int close_socket() {
	/*--------ZAMKNIÊCIE SOCKETU-------------*/
	closesocket(s);
	WSACleanup();
	return 0;
}
int send_file(char *file_name)
{
	FILE *File;
	char *Buffer;
	unsigned long Size;

	File = fopen(file_name, "rb");
	if (!File)
	{
		printf("Taki plik nie istnieje!\n");
		getchar();
		return 0;
	}

	fseek(File, 0, SEEK_END);
	Size = ftell(File);
	fseek(File, 0, SEEK_SET);

	Buffer = new char[Size];

	fread(Buffer, Size, 1, File);
	char cSize[MAX_PATH];
	sprintf(cSize, "%i", Size);

	fclose(File);
	send(s, cSize, MAX_PATH, 0); // File size


	int Offset = 0;
	while (Size > Offset)
	{
		int Amount = send(s, Buffer + Offset, Size - Offset, 0);

		if (Amount <= 0)
		{
			cout << "Error: " << WSAGetLastError() << endl;
			break;
		}
		else
		{
			Offset += Amount;
			printf("2\n");
		}
	}


	free(Buffer);
}