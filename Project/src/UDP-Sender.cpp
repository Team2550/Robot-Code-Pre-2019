#include "UDP-Sender.h"

UDP_Sender::UDP_Sender(std::string dest, unsigned short port)
{
	destination = dest.c_str();
	destPort = port;
	createUDPSocket();
}

int UDP_Sender::createUDPSocket()
{
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(destPort);

	struct hostent* hp = gethostbyname(destination);
	if (!hp)
	{
		fprintf(stderr, "Could not obtain address of %s\n", destination);
		return 1;
	}

	if ((ourSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Failed to create socket");
		return 1;
	}

	if (bind(ourSocket, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0)
	{
		perror("Bind failed");
		return 1;
	}

	memcpy((void *)&myaddr.sin_addr, hp->h_addr_list[0], hp->h_length);

	return 0;
}

void UDP_Sender::sendUDPData(std::string data)
{
	if (sendto(ourSocket, data.c_str(), data.size(), 0, (struct sockaddr *)&myaddr, sizeof(myaddr)) != (int)data.size())
	{
		perror("Send failed: mismatch in number of bytes sent\n");
		return;
	}
}
