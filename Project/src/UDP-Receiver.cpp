#include "UDP-Receiver.h"

UDP_Receiver::UDP_Receiver()
{
	createUDPSocket();
}

/*================================================
Name: getUDPData
Desc: Returns the most recent UDP data
Arguments:
	none
Return:
	Array of ints from UDP
================================================*/
int* UDP_Receiver::getUDPData()
{
	return newestUDPData;
}

/*================================================
Name: createUDPSocket
Desc: Creates UDP socket
Arguments:
	none
Return:
	0 if success, 1 if failed
================================================*/
int UDP_Receiver::createUDPSocket()
{
	if ((ourSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Cannot create socket\n");
		return 1;
	}

	/* bind the socket to any valid IP address and a specific port */

	memset((char *) &myAddress, 0, sizeof(myAddress));
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddress.sin_port = htons(SERVICE_PORT);

	if (bind(ourSocket, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0) {
		perror("bind failed");
		return 1;
	}

	return 0;
}

/*================================================
Name: checkUDP
Desc: Checks UDP socket for new data
Arguments:
	none
Return:
	none
================================================*/
void UDP_Receiver::checkUDP()
{
	printf("Waiting on port %d\n", SERVICE_PORT);

	int bytesRecievedCount = 0;

	try
	{
		bytesRecievedCount = recvfrom(ourSocket, buffer, BUFSIZE, MSG_DONTWAIT, (struct sockaddr *)&remoteAddress, &addressLength);
	}
	catch (const int e)
	{
		bytesRecievedCount = 0;
	}

	if (bytesRecievedCount > 0) {
		printf("received %d bytes\n", bytesRecievedCount);

		buffer[bytesRecievedCount] = 0;
		printf("received message: \"%s\"\n", buffer);

		getNumsFromString(buffer, bytesRecievedCount, newestUDPData);
	}
}

/*================================================
Name: getNums
Desc: Converts UDP string to C++ array
Arguments:
	str (I)    : String to split
	length (I) : Length of string
	nums (O)   : Numbers from string
Return:
	none
================================================*/
void UDP_Receiver::getNumsFromString(unsigned char str[], int length, int nums[])
{
	int start = 0;
	int end = 0;
	int i = 0;

	char currentNum[BUFSIZE] = {};

	while(start < length)
	{
		while( str[end] != ' ' && end < length )
			end++;

		int j = 0;
		for (; start < end; start++, j++)
		{
			currentNum[j] = str[start];
		}

		currentNum[j] = '\0';

		nums[i] = atoi(currentNum);

		start = ++end;
	}
}
