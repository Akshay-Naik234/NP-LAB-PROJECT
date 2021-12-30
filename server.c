#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

void write_file(int sockfd,char *outputFile){
  int n;
  FILE *fp;
  char *filename = outputFile;
  char buffer[SIZE];

  fp = fopen(filename, "a");
  printf("\n Data sent to created output file is: ");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    printf("%s",buffer);
    if (n <= 0){
      break;
    }
    
    fprintf(fp, "%s", buffer);
    
    bzero(buffer, SIZE);
  }
  fclose(fp);
  return;
}

int main(int argc, char **argv){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;
  int listenfd, connfd, n;
  pid_t childpid;
  socklen_t clilen;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("Error in socket");
    exit(1);
  }
  printf("Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("Error in bind");
    exit(1);
  }
  printf("Binding successfull.\n");

  if(listen(sockfd, 10) == 0){
		printf("Listening....\n");
	}else{
		perror("Error in listening");
    exit(1);
	}
	int k=0;
   for(;;)
   {
   	  k++;
   	  addr_size = sizeof(new_addr);
	new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
	  if ( (childpid = fork ()) == 0 ) {
	  	printf ("\n\nChild created for dealing with client %d request",k);

	  	//close listening socket
		close (listenfd);
		write_file(new_sock,argv[1]);
		printf("\nData written in the file successfully.\n");
   
   	  }
   }
  return 0;
}
