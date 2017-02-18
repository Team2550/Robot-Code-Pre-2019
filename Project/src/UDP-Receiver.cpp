#include "UDP-Receiver.h"

UDP_Receiver::UDP_Receiver()
{
	createUDPSocket();

	udpAgeTimer.Start();
}

/*================================================
Name: getUDPData
Desc: Returns the most recent UDP data
Arguments:
	none
Return:
	Array of ints from UDP
================================================*/
float* UDP_Receiver::getUDPData()
{
	return newestUDPData;
}

/*================================================
Name: getUDPDataAge
Desc: Returns the age of the UDP data
Arguments:
	none
Return:
	Seconds since data was last received
================================================*/
double UDP_Receiver::getUDPDataAge()
{
	return udpAgeTimer.Get();
}

/*================================================
Name: getUDPDataIsReal
Desc: Returns whether or not the current data is real or a placeholder
Arguments:
	none
Return:
	True if real, false if placeholder
================================================*/
bool UDP_Receiver::getUDPDataIsReal()
{
	return isRealData;
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
		printf("Received %d bytes\n", bytesRecievedCount);

		buffer[bytesRecievedCount] = 0;
		printf("Received message: \"%s\"\n", buffer);

		float newUDPData[3] = {};
		getNumsFromString(buffer, bytesRecievedCount, newUDPData);

		if (newUDPData[0] != -1)
		{
			memcpy(newestUDPData, newUDPData, sizeof(newUDPData));
			udpAgeTimer.Reset();
			isRealData = true;
		}
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
void UDP_Receiver::getNumsFromString(unsigned char str[], int length, float nums[])
{
	int start = 0;
	int end = 0;
	int i = 0;

	char currentNum[BUFSIZE] = {};

	while(i < length && start < BUFSIZE)
	{
		while(str[end] != ' ' && end < BUFSIZE)
			end++;

		int j = 0;
		for (; start < end; start++, j++)
		{
			currentNum[j] = str[start];
		}

		currentNum[j] = '\0';

		nums[i++] = atof(currentNum);

		start = ++end;
	}
}
