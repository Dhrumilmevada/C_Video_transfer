#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>

#define PORT 5000

int main()
{
	int cli_soc=0;
	struct sockaddr_in ser_addr;
	long size = 0;
	char c_size[10];
	char str[20];
	int ret=0;
	int i=0;
	
	memset(&ser_addr,0,sizeof(ser_addr));

	cli_soc = socket(AF_INET,SOCK_STREAM,0);
	printf("Socket Created......................\n");
	if(cli_soc==0){
		perror("\nsocket fail.");
		exit(EXIT_FAILURE);
	}

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(PORT);
	
	if(inet_pton(AF_INET,"10.115.2.243",&ser_addr.sin_addr)<=0)
	{
		perror("\nInvalid address or Address nor suppored.");
		exit(EXIT_FAILURE);
	}
	printf("request to  connect servere......................\n");
	if((ret = connect(cli_soc,(struct sockaddr *)&ser_addr,sizeof(ser_addr)))<0)
	{
		perror("\nConnect not established");
		//exit(EXIT_FAILURE);
	}
	printf("\nConnection established.......................\n");
	
	/*ret = recv(cli_soc,str,sizeof(str),0);
	str[ret] = '\0';
	printf("%s   %d",str,ret);*/

	ret = recv(cli_soc,c_size,sizeof(c_size),0);
	c_size[ret] = '\0';
	printf("\n%s   %d\n",c_size,ret);
	
	sscanf(c_size,"%ld",&size);
	printf("\nsize recv:-%ld\n",size);

	//char c_buffer[size] = {'0'};
	int buffer[size];
	memset(&buffer,0,sizeof(buffer));
	printf("\nSize of Buffer:-%d",(int)sizeof(buffer));	
	
	ret=0;
	int total_byte_rec = 0;

	while(total_byte_rec < sizeof(buffer)){
		//if((size-total_byte_rec) < 32768)
			//	break;		
		printf("\n1");
		ret = read(cli_soc,&buffer[total_byte_rec],sizeof(buffer)-total_byte_rec/*32768*/);
		printf("\n.............%d",ret);
		if(ret == -1)
			break;
			
		total_byte_rec += ret;
		printf("\n-------------------%d",total_byte_rec);
	}
	//ret = read(cli_soc,&buffer[total_byte_rec],/*sizeof(buffer)-total_byte_rec*/size - total_byte_rec);
	//printf("\n.............%d",ret);
	//total_byte_rec += ret;
	//printf("\n-------------------%d",total_byte_rec);
	//printf("\n%d\n",ret);

	/*for(int i=0;i<size;i++)
	{
		printf("\n%d",buffer[i]);
	}*/
	
	FILE *fp;

	fp = fopen("output.h264","wb");

	for(i=0;i<size;i++)
	{
		//printf("***************************************************%d\n",i);
		fputc(buffer[i],fp);
	}

	fclose(fp);
	
	/*char *args = "ffplay output.h264";	
	system(args);*/
	
	close(cli_soc);
return 0;
}

