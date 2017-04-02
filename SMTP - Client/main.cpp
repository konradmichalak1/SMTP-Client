#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
//#include<winsock2.h>
#include<iostream>
#include <ctime>
#include <cstdio>
#include<string>
#include <conio.h>
//#pragma comment(lib,"ws2_32.lib") //Biblioteka Winsock
#include "winsock_initialize.h"
static char *login;
static char *user;
using namespace std;



//sprawdza wysylany przez serwer komunikat. Jezeli jest taki sam jak oczekiwany, zwraca true
bool check_state(char *state, int recv_size) {
	char *server_data = receive_data();
	int flag;

	for (int i = 0; i < recv_size; i++)
	{
		if (state[i] == server_data[i])
			flag = 1;
		else
			flag = 0;
	}
	if (flag == 1) return 1;
	else return 0;
}



//Funkcja wykonywana na samym poczatku, by polaczyc sie z serwerem. Po poprawnym wykonaniu, przechodzi do login_screen
void first_connection()
{
	system("CLS");
	initialize_socket();
	create_socket();
	connect_with_server("192.168.43.11", 8888);
//	system("pause");
	if (check_state("220", 2)==1) {
		say_hello("helo");
//		system("pause");
		if (check_state("250 OK", 5)==1) {
//			system("pause");
			login_screen();
		} else first_connection();
	}
	else {
		//cout << "Wystapil blad podczas laczenia\n";
		Sleep(3000);
		first_connection();
	}
}
//Ekran logowania, przez uzytkownika podawany jest login i haslo, po pomyslnym zalogowaniu, przechodzi do switch_Screen
void login_screen() {
	char *user, *password;
	user = new char[32];
	password = new char[32];
	system("CLS");
	printf("Podaj login: ");
	cin >> user;
	login = user; //login = login wpisany;

	send_data(user);
	if (check_state("250 OK",5)) {
		printf("Podaj haslo: ");
		
		cin >> password;
		send_data(password);
		
		if (check_state("250 OK",5)) {
			switch_screen();
		} else {
			printf("Bledny login lub haslo");
			Sleep(2000);
			login_screen();
		}
	} else {
		printf("Bledny login lub haslo");
		Sleep(2000);
		login_screen();
	}
}
//Funkcja wysylajaca temat wiadmosci
void type_subject() {
	char *message;
	message = new char[32];
	Sleep(10);
	cin >> message;
	send_data(message);

	if (message[0] == '.' && strlen(message) <= 1) { //jezeli wyslalem .  przenies do switch_screen;
		cout << "Message: ";
		send_data("Message: ");
		type_message();
	}
	else {
		type_subject(); //jezeli wyslalem cos innego, moge dalej pisac wiadomosc;
	}
}
//Wysyla tresc wiadomosci
void type_message() {
	char *message;
	message = new char[32];
	Sleep(10);
	cin >> message;
	send_data(message);

	if (message[0] == '.' && strlen(message) <= 1) { //jezeli wyslalem .  przenies do switch_screen;
		cout << "Wiadomosc zostala wyslana.\n";
		Sleep(2000);
		int pom;
		cout << "Wyslac zalacznik? [1]TAK [2]NIE\n";
		cin >> pom;
		if (pom == 1) {
			send_data("send att");
			if (check_state("250 OK", 5)) {
				char *nazwa_pliku;
				nazwa_pliku = new char[32];
				cout << "Podaj nazwe pliku: ";
				cin >> nazwa_pliku;
				send_data(nazwa_pliku);
				send_file(nazwa_pliku);
				cout << "Wyslano zalacznik!\n";

				Sleep(2000);
				switch_screen();
			}
			else switch_screen();

		}
		else switch_screen();
	}
	else {
		type_message(); //jezeli wyslalem cos innego, moge dalej pisac wiadomosc;
	}
}
//Ekran wysylania maila - wysylamy: mailfrom, rcpt to, data;
void send_mail() {
	system("CLS");
	cout << "Nadawca: ";
	cout << login << endl;
	//cin >> user;
	send_data("mail from:");
	send_data(login); //pamieta login uzytkownika
	if (check_state("250 OK",5)) { //Sprawdz czy nadawca jest poprawny
		printf("Odbiorca: ");
		user = new char[32];
		cin >> user;
		send_data("rcpt to:");
		send_data(user);
		if (check_state("250 Accepted",11)) { //Sprawdz czy odbiorca jest poprawny
			send_data("data");
			if (check_state("354",2)) { //Sprawdz czy mozna wyslac wiadomosc
				////////////////////////////////////////////
				send_all_info();
				cout << "Subject: ";
				send_data("Subject: ");
				type_subject();
	
			}	else {
				cout << "Wystapil blad podczas wysylania wiadomosci.\n";
				switch_screen();
			}///////////////////////////////////////////////////////
		}	else {
			cout << "Blad.\n";
			send_mail();
		}
		}	else {
			cout << "Bledny nadawca.\n";
			send_mail();
		}
}

void quit() {
	send_data("quit");
	if (check_state("221", 2)) {
		close_socket();
	}
	else switch_screen();
}

void send_all_info() { //Cala wiadomosc przeslana jeszcze raz po otrzymaniu komunikatu 354
	send_data("Data: ");
	get_time();
	send_data("From: ");
	send_data(login);
	send_data("To: ");
	send_data(user);

}
//Ekran zarzadzania kontem
void switch_screen(){
	system("CLS");
	cout << "MENU\n1.Wyslij mail\n2.Rozlacz z serwerem";
	int nr;

	for (;;){
		cin >> nr;
		switch (nr) {
		case 1: { send_mail(); break; } //wyslij mail
		case 2: { quit(); break; } //rozlacz

		}

	}
}
void get_time() {
	time_t czas;
	time(&czas);
	send_data(ctime(&czas));
}


int main(int argc, char *argv[])
{

	system("pause");
	first_connection(); //od tej funkcji sie zacznie ca³y proces, wiec nic wiecej nie trzeba do maina dawac
	return 0;
}