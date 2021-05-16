#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

#include "TCPClient.h"

TCPClient tcp;
struct termios old = {0};

void sig_exit(int s)
{
	tcp.exit();

	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror ("tcsetattr ~ICANON");
	exit(0);
}

char getch() {
        char buf = 0;
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        return (buf);
}

int main(int argc, char *argv[])
{
	if(argc != 4) {
		cerr << "Usage: ./client ip port message" << endl;
		return 0;
	}
	signal(SIGINT, sig_exit);

	if (tcgetattr(0, &old) < 0)
			perror("tcsetattr()");

	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
	tcp.setup(argv[1],atoi(argv[2]));
	while(1)
	{
		char c = getch();
		tcp.Send(string(1, c));
		string rec = tcp.receive();
		if( rec != "" )
		{
			cout << rec << endl;
		}
		usleep(100);
	}

	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror ("tcsetattr ~ICANON");
	return 0;
}
