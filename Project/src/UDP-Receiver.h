#ifndef UDP_H
#define UDP_H

#include <WPILib.h>
#include <iostream>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include "Utility.h"
#include "Constants.h"

#define BUFSIZE      2048
#define SERVICE_PORT 8890	/* hard-coded port number */

class UDP_Receiver {
private:
	int ourSocket;
	struct sockaddr_in myAddress;
	struct sockaddr_in remoteAddress;
	socklen_t addressLength = sizeof(remoteAddress);
	int bytesRecievedCount;
	char buffer[BUFSIZE];

	bool isRealData = false;
	float newestUDPData[UDP::DataCount] = {-1, -1, -1, -1, -1};

	Timer udpAgeTimer;

	int createUDPSocket();
	//void getNumsFromString(unsigned char str[], int length, float nums[]);
public:
	UDP_Receiver();

	void checkUDP();
	void getUDPData(float data[]);
	double getUDPDataAge();
	bool getUDPDataIsReal();
};

#endif
