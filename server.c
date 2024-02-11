#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>



void main()
{
	int numberOfListen = 100,connectionClient = 0;
	int sock = socket(AF_INET,SOCK_STREAM,0);
	char buffer[1024] = {0};
	char sendBuffer[1024] = {0};	

	struct sockaddr_in serv_addr = 
	{
		AF_INET,
		htons(5000),
		0
		
	}; 

	bind(sock,&serv_addr,sizeof(serv_addr));

	listen(sock,numberOfListen);

	while (1)
	{
		connectionClient = accept(sock,0,0);
		recv(connectionClient,buffer,sizeof(buffer),0);

                // GET / index.html .....

		char* f = buffer + 5;
		*strchr(f,' ') = 0;
	
		int opened_fp = open(f,O_RDONLY);
		char* headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
		
		write(connectionClient,headers,strlen(headers));

		sendfile(connectionClient, opened_fp,0,sendBuffer);

		close(opened_fp);
		close(connectionClient);
		sleep(1);
	}
} 
