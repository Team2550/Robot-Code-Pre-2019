#ifndef UDP_R_H
#define UDP_R_H

#include <WPILib.h>
#include <iostream>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include "Utility.h"
#include "Constants.h"

//these should be changed to constants instead of #defines
#define BUFSIZE      2048
#define SERVICE_PORT 8890	/* hard-coded port number */

class UDP_Receiver {
private:
	int ourSocket;
	struct sockaddr_in myAddress;
	struct sockaddr_in remoteAddress;
	socklen_t addressLength = sizeof(remoteAddress);
	char buffer[BUFSIZE];
	float newestUDPData[UDP::DataCount] = {-1, -1, -1, -1, -1, -1};
	bool isRealData = false;

	Timer udpAgeTimer;

	int createUDPSocket();
public:
	UDP_Receiver();

	bool checkUDP();
	void getUDPData(float data[]);
	double getUDPDataAge();
	bool getUDPDataIsReal();
	void clearUDPSocket();
};


#endif
