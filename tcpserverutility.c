#include "includes\Main.h"


void HandleTCPClient(int clntSocket) {
	char buffer[BUFSIZE];
	
	// Receive message from client
	ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
	if (numBytesRcvd == -1)
		DieWithSystemMessage("recv() failed");
	
	// Send received string and receive again until end of stream
	while (numBytesRcvd > 0)
	{
		// Echo message back to the client
		ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
		if (numBytesSent < 0)
			DieWithSystemMessage("send() failed");
		else if (numBytesSent != numBytesRcvd)
			DieWithUserMessage("send()", "sent unexpected number of bytes");
		
		// See if there is more data to receive
		numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
		if (numBytesRcvd == -1)
			DieWithSystemMessage("recv() failed");
	}
	
	// Close client socket
	close(clntSocket);
}