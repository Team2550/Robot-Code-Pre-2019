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
void UDP_Receiver::getUDPData(float data[])
{
	memcpy(data, newestUDPData, sizeof(newestUDPData));
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
	if ((ourSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
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
		buffer[bytesRecievedCount] = '\0';
		std::string dataAsString(buffer, buffer + sizeof(buffer) / sizeof(buffer[0]));

		std::vector<std::string> dataPoints = Utility::splitString(dataAsString, ',');

		std::vector<std::vector<float>> data;

		for (unsigned int i = 0; i < dataPoints.size(); i++)
			data.push_back(Utility::strVectorToFloatVector(Utility::splitString(dataPoints[i], ' ')));

		for (int i = 0; i < UDP::DataCount; i++)
		{
			newestUDPData[i] = 0;
			for (unsigned int j = 0; j < data.size(); j++)
				newestUDPData[i] += data[j][i] / data.size();
		}

		udpAgeTimer.Reset();
		isRealData = true;
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
/*void UDP_Receiver::getNumsFromString(std::string str, float nums[])
{
	int start = 0;
	int end = 0;
	int i = 0;

	std::string currentNum;

	while(i < str.length())
	{
		currentNum = "";

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
}*/
