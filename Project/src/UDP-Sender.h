#ifndef UDP_S_H
#define UDP_S_H

#include <WPILib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Utility.h"
#include "Constants.h"

#define BUFSIZE      2048
#define SERVICE_PORT 8890	/* hard-coded port number */

class UDP_Sender {
private:
	int ourSocket;
	struct sockaddr_in myaddr;

	const char* destination;
	unsigned short destPort;

	int createUDPSocket();
public:
	UDP_Sender(std::string dest, unsigned short port);

	void sendUDPData(std::string data);
};

#endif
