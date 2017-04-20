/** TCP Server in C
 *  
 *  1. Create TCP socket using socket()
 *  2. Assign a port number to the socket with bind()
 *  3. Tell the system to allow connections to be made to that port, using listen()
 *  4. Repeatedly do the following:
 *     - Call accept() to get a new socket for each client connection
 *     - Communicate with the client via the new socket using send() and recv()
 *     - Close the client connection using close()
 *
 */


#include "includes\Main.h"

// Maximum outstanding connection requests
static const int MAXPENDING = 5;


int main(int argc, char *argv[])
{
	// Test for correct number of arguments
	if (argc != 2)
		DieWithUserMessage("Parameter(s)", "<Server Port>");
	
	// First arg: local port
	in_port_t servPort = atoi(argv[1]);
	
	// Create socket for incoming connections
	int servSock;
	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		DieWithSystemMessage("socket() failed");
	
	// Construct local address structure
	struct sockaddr_in servAddr; 					// Local address
	memset(&servAddr, 0, sizeof(servAddr)); 		// Zero out struct
	servAddr.sin_family = AF_INET;					// IPv4 Address family
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// Any incoming interface
	servAddr.sin_port = htons(servPort); 			// Local port
	
	// Bind to the local address
	if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) == -1)
		DieWithSystemMessage("bind() failed");
	
	// Mark the socket so it will listen for incoming connections
	if (listen(servSock, MAXPENDING) == -1)
		DieWithSystemMessage("listen() failed");
	
	/** The client address struct should be declared outside of the infinite loop. If we declared it inside the loop,
	 *  the program would have to deallocate and reallocate the struct's memory every time it ran the loop, pushing and
	 *  popping the stack frames. Rather than do all that, we just declare it preemptively here and just let the program
	 *  set it's parameters whenever it needs to.
	 */
	
	struct sockaddr_in clntAddr;
	
	// Run loop forever
	for (;;)
	{
		// Set length of client address struct (in-out parameter)
		socklen_t clntAddrLen = sizeof(clntAddr);
		
		// Wait for a client to connect
		int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
		if (clntSock == -1)
			DieWithSystemMessage("accept() failed");
		
		// clntSock is connected to a client
		
		// String to contain client address
		char clntName[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
			printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
		else
			puts("Unable to get client address");
		
		/** HandleTCPClient receives data on the given socket and sends it back on the same socket,
		 *  iterating as long as recv() returns a positive value (indicating that something was
		 *  received.) When the client closes the connection normally, recv() returns 0.
		 */
		
		HandleTCPClient(clntSock);
	}
	
	// NOT REACHED
	return EXIT_SUCCESS;
}