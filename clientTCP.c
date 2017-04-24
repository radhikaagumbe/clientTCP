#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void inputCharacter(char *name, int len);
void kvsPut(char *key, char *value);
void kvsGet(char *key2);
void kvsDelete(char *key1);
int sockfd;
char buffer1[256];
time_t mytime;

int main(int argc, char *argv[])
{
    int portno, n, option;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int flag = 1;

    if (argc < 3)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        error("Error opening socket");
    }

    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr,"Error! No such host exist\n");
        exit(1);
    }

    bzero((char *) &serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }

    kvsPut("Washington", "A state in the Pacific Northwest in Puget Sound");
    kvsPut("New York", "A state");
    kvsPut("Seattle", "Washington");
    kvsPut("DS", "Collection of independent computers appears as a single coherent systems");
    kvsPut("Tacoma", "A place in the state of Washington");
    kvsPut("UW", "A research university");
    kvsPut("Mascot", "Harry the husky");
    kvsPut("Spring", "Begins on March 1st");
    kvsPut("Summer", "Begins on June 1st");
    kvsPut("Autumn","Begins on September 1st");
    kvsPut("Winter","Begins on December 1st");
    kvsPut("Rainier","A mountain in Washington state");

    printf("key-value pairs are populated\n");
    printf("Set of keys: Washington, New York, Seattle, DS, Tacoma, UW, Mascot, Spring, Summer, Autumn, Winter, Rainier\n");

    printf("Get values for keys\n");
    kvsGet("Tacoma");
    kvsGet("Spring");
    kvsGet("DS");
    kvsGet("Summer");
    kvsGet("Seattle");
    kvsGet("Mascot");
    kvsGet("Autumn");

    printf("Delete key value pairs\n");
    kvsDelete("Tacoma");
    kvsDelete("Spring");
    kvsDelete("DS");
    kvsDelete("Rainier");
    kvsDelete("Summer");

    while(flag == 1){
        char k[256],v[256],m[256];
        printf("Enter the operation\n");
        printf("1: PUT, 2: GET, 3:DELETE, 4:QUIT\n");
        inputCharacter(k, 256);
        option = atoi(k);
        printf("Option: %d\n", option);
        bzero(k,256);
        bzero(v,256);
        bzero(m,256);
        int l;
        switch(option)
        {
            case 1:
                printf("Enter the key\n");
                inputCharacter(k, 256);
                printf("Key entered: %s\n", k);
                printf("Enter the value\n");
                inputCharacter(v, 256);
                printf("Value is %s\n",v);
                kvsPut(k,v);
                break;

            case 2:
                printf("Enter the key\n");
                inputCharacter(m, 256);
                kvsGet(m);
                break;

            case 3:
                printf("Enter the key\n");
                inputCharacter(m, 256);
                kvsDelete(m);
                break;

            case 4:
                flag = 0;
                mytime = time(NULL);
                printf("QUIT operation is requested at: %s\n",ctime(&mytime));
                break;
            default:
                printf("Invalid option entered\n");
        }
    }

    return 0;
}

void inputCharacter(char *name, int len){
    char ch;
    int i = 0;
    bzero(name, len);
    while (1) {
        ch = getchar();
        if (ch == '\n' || i >= len) {
            break;
        }
        name[i++] = ch;
    }
    name[i] = '\0';
}

void kvsPut(char *key, char *value){
    int c = 1,l, p;
    bzero(buffer1, 256);
    mytime = time(NULL);
    printf("PUT service is requested at: %s",ctime(&mytime));
    sprintf(buffer1,"%d:%s;%s",c,key,value);
    l = write(sockfd,buffer1,strlen(buffer1));
    if (l < 0)
    {
        error("ERROR writing to socket");
    }
      bzero(buffer1, 256);
      read(sockfd,buffer1,255);
      if (p < 0)
      {
          error("ERROR reading from socket");
      }
      printf("%s\n",buffer1);
        mytime = time(NULL);
      printf("PUT service response is received at: %s",ctime(&mytime));
}

void kvsGet(char *key2){
  int c = 2, p, l;
  printf("Key: %s\n",key2);
  char buffer2[256];
  char buffer4[256];
  bzero(buffer2, 256);
  mytime = time(NULL);
  printf("GET service is requested at: %s",ctime(&mytime));
  sprintf(buffer2,"%d:%s",c,key2);
  l = write(sockfd,buffer2,strlen(buffer2));
  if (l < 0)
  {
      error("ERROR writing to socket");
  }
  bzero(buffer4,256);
  p = read(sockfd,buffer4,255);
  if (p < 0)
  {
      error("ERROR reading from socket");
  }
  printf("Value: %s\n", buffer4);
  mytime = time(NULL);
  printf("GET service response is received at: %s",ctime(&mytime));
}

void kvsDelete(char *key1){
  int c = 3, l, p;
  char buffer3[256];
  char buffer5[256];
  bzero(buffer3,256);
    mytime = time(NULL);
  printf("DELETE service is requested at: %s",ctime(&mytime));
  sprintf(buffer3,"%d:%s",c,key1);
  l = write(sockfd,buffer3,strlen(buffer3));
  printf("Key: %s\n", key1);
  if (l < 0)
  {
      error("ERROR writing to socket");
  }
  bzero(buffer5,256);
  p = read(sockfd,buffer5,255);
  if (p < 0)
  {
      error("ERROR reading from socket");
  }
  printf("%s\n",buffer5);
    mytime = time(NULL);
  printf("DELETE service response is received at: %s",ctime(&mytime));
}
