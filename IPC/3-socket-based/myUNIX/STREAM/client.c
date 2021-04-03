#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <string.h>

#define SOCKET_NAME "./my.socket"

int main(int argc, char *argv[]) {
   int sockfd, n;
   char* socketId ; // maps to the SOCKET_NAME in server
   struct sockaddr_un serv_addr;
   
   char buffer[256];
   

   if (argc < 2) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }

   socketId = argv[1];

   /* Create a socket point */
   sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sun_family = AF_UNIX;
   strncpy(serv_addr.sun_path, socketId, sizeof(serv_addr.sun_path) - 1);
   
   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }
   
   /* Now ask for a message from the user, this message
      * will be read by server
   */
	
   printf("Please enter the message: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);
   
   /* Send message to the server */
   n = write(sockfd, buffer, strlen(buffer));
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   
   /* Now read server response */
   bzero(buffer,256);
   n = read(sockfd, buffer, 255);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
	
   printf("%s\n",buffer);
   return 0;
}
