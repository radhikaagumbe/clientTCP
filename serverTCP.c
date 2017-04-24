#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilength;
    char buffer[256];
    char cont[256],file_name[256], c;
    struct sockaddr_in serv_addr, cli_addr;
    int n = 0,rm;
    char num[256];
    time_t mytime;
    char w[256];
    int yes=1;

    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
    perror("setsockopt");
    exit(1);
    }

    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }

    listen(sockfd,5);
    clilength = sizeof(cli_addr);
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilength);

    if (newsockfd < 0)
    {
        error("ERROR on accept");
    }

    while(1){
        char src[] = "/tmp/";
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        mytime = time(NULL);
        printf("Service requested at: %s\n",ctime(&mytime));
        if (n <= 0)
        {
            error("ERROR reading from socket");
        }

        bzero(file_name,256);
        bzero(cont,256);
        int i,j=0,k,l=0;

        k = buffer[0] - '0';

        for(i=2;i<strlen(buffer);i++){
            if(buffer[i] == ';'){
                break;
            }
            file_name[j] = buffer[i];
            j++;
        }

        i=i+1;
        for(;i<strlen(buffer);i++){
            if(buffer[i] == '\0'){
                break;
            }
            cont[l] = buffer[i];
            l++;
        }

        int q;
        strcat(src, file_name);
        strcat(src,".txt");
        FILE *fptr;
        switch(k){
            case 1:
                fptr = fopen(src,"w");
                if(fptr == NULL)
                {
                    printf("Error!");
                    exit(1);
                }
                fprintf(fptr,"%s",cont);
                bzero(num, 256);
                sprintf(num,"Key Value Added");
                q = write(newsockfd,num,strlen(num));
                mytime = time(NULL);
                printf("Service response sent to client at: %s\n",ctime(&mytime));
                if (q < 0)
                {
                    error("ERROR writing to socket");
                }
                fclose(fptr);
                break;

            case 2:
                  if ((fptr = fopen(src,"r")) == NULL){
                    printf("File not found\n");
                    char *r = "KEY NOT FOUND";
                    write(newsockfd, r, strlen(r));
                    break;
                }
                bzero(num, 256);
                fgets(num, 256, fptr);
                q = write(newsockfd,num,strlen(num));
                mytime = time(NULL);
                printf("Service response sent to client at: %s\n",ctime(&mytime));
                if (q < 0)
                {
                    error("ERROR writing to socket");
                }
                fclose(fptr);
                break;

            case 3:
                bzero(w, 255);
                rm = remove(src);
                if(rm==0){
                    strcpy(w, "KEY IS DELETED SUCCESSFULLY");
                }
                else{
                    printf("File not found\n");
                    strcpy(w, "KEY NOT FOUND AND IS NOT DELETED");
                }
                q =  write(newsockfd, w, strlen(w));
                mytime = time(NULL);
                printf("Service response sent to client at: %s\n",ctime(&mytime));
                if (q < 0)
                {
                    error("ERROR writing to socket");
                }
                  printf("%s\n",w);
                break;
        }
    }
    return 0;
}
