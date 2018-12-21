#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>


#define PORT 5000

/*typedef struct{
	int data[2000000];
}shmem;*/

void *read_data(void *argv)
{
	FILE *fp,*fp1;
	
	fp = fopen("demo.h264","rb");
	fp1 = fopen("demo.txt","wb");

	fseek(fp,0,SEEK_END);
	long size = ftell(fp);
	rewind(fp);
	
	int buffer[size];
	memset(&buffer,0,sizeof(buffer));		

	int c,bf_cnt=0;

	/*long i=0;
	shmem *shptr;

	key_t key = ftok("demo.h264",85);
	int shm_id = shmget(key,sizeof(shmem),IPC_CREAT | 0666);
	shptr = (shmem *)shmat(shm_id,NULL,0);*/
	 

	while((c=fgetc(fp))!=EOF)
	{
		printf("....................................................%d\n",c);
		buffer[bf_cnt++] = c;
		fputc(c,fp1);
		/*shptr->data[bf_cnt] = c;*/
		
	}

	sleep(3);
	/*for( i=0; i<size ; i++)
		printf("%d\n",shptr->data[i]);
	
	shmdt((void *)shptr);*/

	fclose(fp);
	fclose(fp1);

	/*for(long i=0;i<size;i++)	
		printf("\n%d",buffer[i]);*/
}

int main()
{

	pthread_t tid;

	pthread_create(&tid,NULL,read_data,NULL);
	pthread_join(tid,NULL);	

	FILE *fp;

	fp = fopen("demo.txt","rb");

	fseek(fp,0,SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	int c=0,buffer[size],bf_cnt=0;
	int i=0,ret;
	
	memset(&buffer,0,sizeof(buffer));

	while((c=fgetc(fp)) !=EOF)
	{
		printf("%d\n",c);
		buffer[bf_cnt++] = c;
	}

	fclose(fp);	

	int ser_soc,cli_soc;
	struct sockaddr_in ser_addr,cli_addr;
	int addrlen = sizeof(cli_addr);

	printf("\n\tStarted creating socket.....");	

	ser_soc = socket(AF_INET, SOCK_STREAM, 0);
	
	printf("\n%d",ser_soc);

	if(ser_soc==0)
	{
		perror("\nSocket failed.");
		exit(EXIT_FAILURE);
	}
	printf("\n\nSocket created........................\n");
	memset(&ser_addr,0, sizeof(ser_addr));
	memset(&ser_addr, 0 , sizeof(cli_addr));

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(PORT);
	ser_addr.sin_addr.s_addr = INADDR_ANY;

	/*if(inet_pton(AF_INET,"10.115.2.243",&ser_addr.sin_addr)<=0)
	{
		perror("\nInvalid address or Address nor suppored.");
		exit(EXIT_FAILURE);
	}*/

	printf("\n binding socket........................\n");
	if(bind(ser_soc , (struct sockaddr * )&ser_addr , sizeof(ser_addr)) < 0 )
	{
		perror("\nBind failed.");
		exit(EXIT_FAILURE);	
	}
	printf("\n binding successfully completed......................\n");
	

	printf("\nlistening for client socket ..............\n");
	if(listen(ser_soc,5) < 0)
	{
		perror("\nlisten failed.");
		exit(EXIT_FAILURE);	
	}
	printf("\nlisten completed..................\n");

	char c_size[10] = {'0'};
	sprintf(c_size,"%ld",size);
	printf("%d -- %s",(int )strlen(c_size),c_size);
	
	printf("\nwaiting for client................\n");
	while(1)
	{
		cli_soc = accept(ser_soc, (struct sockaddr *)&cli_addr, (socklen_t *)&addrlen);
		
		if(cli_soc < 0)
		{
			perror("\naccept failed.");
			exit(EXIT_FAILURE);	
		}
		
		printf("\n one client received...................\n");
		
		/*ret = send(cli_soc,"Hello From Server",strlen("Hello From Server")+1,0);
		printf("\n%d",ret);*/
	
		ret =0;
		ret = send(cli_soc,c_size,strlen(c_size),0);
		printf("\nSize send:-%d",ret);	

		printf("\nSize of Buffer:-%d",(int)sizeof(buffer));
		
		ret=0;
		int total_byte_write = 0;
//		while(total_byte_write < sizeof(buffer))
		{
			printf("\nserver");
			//if((size-total_byte_write) < 32768)
			{
				//break;
			}
			ret = write(cli_soc,&buffer[total_byte_write],sizeof(buffer)/*32768*/);
			printf("\n.....................%d",ret);
			if(ret == -1)
				break;
			total_byte_write += ret;
			printf("\n......................%d",total_byte_write);
		}
		//ret = write(cli_soc,&buffer[total_byte_write],/*size-total_byte_write*/(size-total_byte_write));
		//printf("\n.....................%d",ret);
		//total_byte_write += ret;
		//printf("\n......................%d",total_byte_write);
		printf("\nBuffer send:-%d",ret);
	}
	

	/*fp = fopen("write.h264","wb");
	
	printf("writing .h264 file");

	
	
	for(i=0;i<size;i++)
	{
		printf("***************************************************%d\n",i);
		fputc(buffer[i],fp);
	}

	fclose(fp);
	
	char *args = "ffplay write.h264";	
	system(args);*/
return 0;
}
