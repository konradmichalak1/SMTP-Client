//#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include<string>
#pragma comment(lib,"ws2_32.lib") //Biblioteka Winsock
using namespace std;

//funkcje w winsocku
int initialize_socket();
void create_socket();
int connect_with_server(const char *, int);
int send_data(char *);
int say_hello(char *);
int send_message();
char *receive_data();
int close_socket();
int send_file(char *);

//funkcje w mainie
bool check_state(char *, int);
void first_connection();
void login_screen();
void send_mail();
void switch_screen();
void type_message();
void type_subject();
void quit();
void get_time();
void send_all_info();
