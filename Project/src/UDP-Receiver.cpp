#include "UDP-Receiver.h"

UDP_Receiver::UDP_Receiver()
{
	createUDPSocket();
}

void UDP_Receiver::RobotInit()
{

}

void UDP_Receiver::AutoInit()
{

}

void UDP_Receiver::AutoPeriodic()
{
	checkUDP();

	std::cout << "As numbers:";

	for (int i = 0; i < 4; i++)
		std::cout << std::to_string(newestUDPData[i]);

	std::cout << std::endl;
}

void UDP_Receiver::TeleopInit()
{

}

void UDP_Receiver::TeleopPeriodic()
{
	checkUDP();

	std::cout << "As numbers:";

	for (int i = 0; i < 4; i++)
		std::cout << std::to_string(newestUDPData[i]);

	std::cout << std::endl;
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
		std::cerr << "cannot create socket" << std::endl
		          << strerror(errno) << std::endl;
		return 1;
	}

	/* bind the socket to any valid IP address and a specific port */

	memset((char *) &myAddress, 0, sizeof(myAddress));
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddress.sin_port = htons(SERVICE_PORT);

	if (bind(ourSocket, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0) {
		std::cerr << "bind failed" << strerror(errno) << std::endl;
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
	std::cout << "waiting on port " << SERVICE_PORT << std::endl;

	bytesReceivedCount =
			recvfrom(ourSocket, buffer, BUFSIZE, 0, (struct sockaddr *)&remoteAddress, &addressLength);

	std::cout << "received " << bytesReceivedCount << " bytes" << std::endl;

	if (bytesReceivedCount > 0) {
		buffer[bytesReceivedCount] = 0;
		std::cout << "received message: \"" << buffer << '\"' << std::endl;

		getNumsFromString(buffer, bytesReceivedCount, newestUDPData);
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

	std::vector<char> currentNum;

	while(start < length)
	{
		currentNum.clear();

		end = start;

		while( str[end] != ' ' && end >= length )
			end++;

		int j;
		for (j = 0; start < end; start++, j++)
		{
			currentNum.push_back(str[start]);
		}

		currentNum.push_back(0);

		char* currentNumArray = &currentNum[0];
		nums[i] = atoi(currentNumArray);

		start = end + 1;
	}
}
