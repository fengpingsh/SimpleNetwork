#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <thread>
#include <algorithm>
#include <cctype>
#include <mutex>

using namespace std;

#define MAXPACKETSIZE 40960
#define MAX_CLIENT 1000
//#define CODA_MSG 4

struct descript_socket{
	int socket     = -1;
	string ip      = "";
	int id         = -1;
	std::string message;
	bool enable_message_runtime = false;
};

class TCPServer
{
public:
	TCPServer();
	int setup(int port, vector<int> opts = vector<int>());
	vector<descript_socket*> getMessage();
	void cleanMessage(descript_socket * message);
	void accepted();
	void Send(string msg, int id);
	void detach(int id);
	void clean(int id);
	bool is_online();
	string get_ip_addr(int id);
	int connection_valid(int id);
	void closed();

private:
	int sockfd, n, pid;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;

	static vector<descript_socket*> newsockfd;
	static vector<descript_socket*> Message;//[CODA_MSG];

	static bool isonline;
	static int last_closed;
	static int num_client;
	static std::mutex mt;
	static std::mutex ct;
	static void * Task(void * argv);
};

#endif
