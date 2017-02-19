#include "UDP-Sender.h"

UDP_Sender::UDP_Sender(std::string dest, unsigned short port)
{
	destination = dest.c_str();
	destPort = port;
	createUDPSocket();
}

int UDP_Sender::createUDPSocket()
{
	if ((ourSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Cannot create socket\n");
		return 1;
	}

	struct hostent *hp;

	memset((char *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(destPort);

	hp = gethostbyname(destination);
	if (!hp)
	{
		fprintf(stderr, "Could not obtain address of %s\n", destination);
		return 1;
	}

	memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);

	return 0;
}

void UDP_Sender::sendUDPData(char data[])
{
	if (sendto(ourSocket, data, strlen(data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) > 0)
	{
		perror("Send failed\n");
		return;
	}
}
