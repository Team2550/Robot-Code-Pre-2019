/*
        demo-udp-03: udp-recv: a simple udp server
	receive udp messages

        usage:  udp-recv

        Paul Krzyzanowski
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 2048
#define SERVICE_PORT	8890	/* hard-coded port number */

int* dataFromPi;

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
void getNums(unsigned char str[], int length, int nums[])
{
	int start = 0;
	int end = 0;
	char dataPoint[10];
	int i = 0;

	while(start < length)
	{
		while( str[end] != ' ' && end >= length )
			end++;

		int j = 0;
		for (; start < end; j++)
		{
			dataPoint[j] = str[start];
			start++;
		}

		start++;
		dataPoint[j] = 0;
		nums[i] = atoi(dataPoint);
		start = end;
	}
}

/*================================================
Name: udpReceive
Desc: Receives UDP data from raspberry pi
Arguments:
	none
Return:
	0 if fails
================================================*/
int udpReceive()
{
	struct sockaddr_in myAddress;	                  /* our address */
	struct sockaddr_in remoteAddress;	              /* remote address */
	socklen_t addressLength = sizeof(remoteAddress);  /* length of addresses */
	int bytesRecievedCount;	                          /* # bytes received */
	int ourSocket;                                    /* our socket */
	unsigned char buffer[BUFSIZE];	                  /* receive buffer */
	int openCVData[10];

	/* create a UDP socket */

	if ((ourSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */

	memset((char *) &myAddress, 0, sizeof(myAddress));
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddress.sin_port = htons(SERVICE_PORT);

	if (bind(ourSocket, (struct sockaddr *) &myAddress, sizeof(myAddress)) < 0) {
		perror("bind failed");
		return 0;
	}

	/* now loop, receiving data and printing what we received */
	while(1)
	{
		printf("waiting on port %d\n", SERVICE_PORT);
		bytesRecievedCount =
				recvfrom(ourSocket, buffer, BUFSIZE, 0, (struct sockaddr *)&remoteAddress, &addressLength);
		printf("received %d bytes\n", bytesRecievedCount);
		if (bytesRecievedCount > 0) {
			buffer[bytesRecievedCount] = 0;
			printf("received message: \"%s\"\n", buffer);

			getNums(buffer, bytesRecievedCount, openCVData);

			dataFromPi = openCVData;
			/*printf("As numbers:");

			int i = 0;
			for (; i < bytesRecievedCount; i++)
				printf(" %i", openCVData[i]);

			printf("\n");*/
		}
	}
	/* never exits */

	return 0;
}
