/** TCP Server in C */

#include "includes\Main.h"

int main()
{
	int sock,
		connected,
		bytes_received,
		sin_size,
		true = 1;
	
	char 	send_data[1024],
			recv_data[1024];
			
	struct sockaddr_in 	server_addr,
						client_addr;
			
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}
	
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(EXIT_FAILURE);
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero), 8);
	
	if (bind(sock, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1) {
		perror("Unable to bind");
		exit(EXIT_FAILURE);
	}
	
	printf("\nTCP Server waiting for client on port 5000");
	fflush(stdout);
	
	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);
		
		connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
		printf("\nI got a connection from (%s, %d)", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		
		while (1)
		{
			printf("\nSEND (q or Q to quit) : ");
			gets(send_data);
			
			if (strcmp(send_data, "q") == 0 || strcmp(send_data, "Q") == 0) {
				send(connected, send_data, strlen(send_data), 0);
				close(connected);
				break;
			} else {
				send (connected, send_data, strlen(send_data), 0);
			}
			
			bytes_received = recv(connected, recv_data, 1024, 0);
			recv_data[bytes_received] = '\0';
			
			if (strcmp(recv_data, "q") == 0 || strcmp(recv_data, "Q") == 0) {
				close(connected);
				break;
			} else {
				printf("\nRECEIVED DATA: %s", recv_data);
				fflush(stdout);
			}
		}
	}
	
	close(sock);
	return EXIT_SUCCESS;
}