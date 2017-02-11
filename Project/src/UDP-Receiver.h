#ifndef UDP_H
#define UDP_H

#include <WPILib.h>
#include <iostream>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>

#define BUFSIZE      2048
#define SERVICE_PORT 8890	/* hard-coded port number */

class UDP_Receiver {
private:
	int ourSocket;
	struct sockaddr_in myAddress;
	struct sockaddr_in remoteAddress;
	socklen_t addressLength = sizeof(remoteAddress);
	int bytesRecievedCount;
	unsigned char buffer[BUFSIZE];
	float newestUDPData[4] = {-1, -1, -1};

	Timer udpAgeTimer;

	int createUDPSocket();
	void getNumsFromString(unsigned char str[], int length, float nums[]);
public:
	UDP_Receiver();

	void checkUDP();
	float* getUDPData();
	double getUDPDataAge();
};

#endif
