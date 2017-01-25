#ifndef UDP_H
#define UDP_H

#include <iostream>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>

const int  BUFSIZE      = 2048;
const int  SERVICE_PORT = 8890;	/* hard-coded port number */

class UDP_Receiver {
public:
	UDP_Receiver();
	void RobotInit();
	void AutoInit();
	void AutoPeriodic();
	void TeleopInit();
	void TeleopPeriodic();

	int* getUDPData();
private:
	int ourSocket;
	struct sockaddr_in myAddress;
	struct sockaddr_in remoteAddress;
	socklen_t addressLength = sizeof(remoteAddress);
	int bytesReceivedCount;
	unsigned char buffer[BUFSIZE];
	int newestUDPData[];

	int createUDPSocket();
	void checkUDP();
	void getNumsFromString(unsigned char str[], int length, int nums[]);
};

#endif
