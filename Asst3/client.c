#include"bank.h"

int hostname_to_ip(char*,char*);

typedef struct _ioP{
    int sockfd;
} ioP;

// input thread
// What the user will send to the system
void* input_func(void* params){
    
    ioP* test = (ioP*)params;
    int sockfd = test->sockfd;
    char buff[MAX];

    // *************************************************************************        have to make it so that they can only input a command every two seconds

    for(;;){
        // prompt user to input a command
        // flush it bc sometimes it doesnt work
        //printf("Please input a command: ");
        //fflush(0);
    
        // read what the user inputs
        // read blocks?
        read(0,buff,MAX);
        write(sockfd,buff,sizeof(buff));

        // sleep for 2 seconds to throttle the client input
        sleep(2);
    }

    return NULL;
}

// output thread
// what the user will receive from the server
void* output_func(void* params){

    ioP* test = (ioP*)params;
    int sockfd = test->sockfd;
    char buff[MAX];    
    bzero(buff,sizeof(buff));


    // continuously loop until a certain command is received
    for(;;){
        bzero(buff,sizeof(buff));
        
        // read from the server
        read(sockfd,buff,sizeof(buff));

 
        // different outputs
        if(strcmp(buff,"Server shutting down") == 0){
            printf("SERVER SHUTTING DOWN\n");
            write(sockfd,"shutdown recieved\n",19);
            break;
        }else if(strcmp(buff,"Shutting down") == 0){
                printf("Shutting down\n");
                break;
            }else{
                printf(buff);
            }

    }

    return NULL;
}
  
int main(int argc,char** argv) 
{

    int counter=0;

    if(argc!=3){
        printf("Input the correct number of arguments\n");
        return 1;
    }

    // figure out the ip from the provided string
    char* hostname = argv[1];
    char ip[100];
   if(hostname_to_ip(hostname,ip) == 1){
        printf("Please input a proper IP\n");
        return 1;
    }



    // check to see if port is okay
    for(counter=0;counter<strlen(argv[2]);counter++){    
        char test = argv[2][counter];
        if(isdigit(test) == 0){
            printf("Please input a proper port\n");
            return 1;
        }
    }
    int port = atoi(argv[2]);

 
    int sockfd;//, connfd; 
    struct sockaddr_in servaddr; //, cli; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(ip); 
    servaddr.sin_port = htons(port); 
  

    // continually try to connect to the server
    for(;;){
    
        // connect the client socket to server socket 
        if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
            //printf("connection with the server failed...\n"); 
            //exit(0);
            continue; 
        } 
        else{
            printf("connected to the server..\n"); 
            break;
        }
    }

/*              START ACTUALLY LISTENING FOR INPUT AND OUTPUT               */
// 

    // create the struct to pass to the thread functions
    ioP* param = (ioP*)malloc(sizeof(ioP));
    param->sockfd = sockfd;

    // create the I/O threads
    pthread_t outputT,inputT;
    pthread_create(&inputT,NULL,input_func,(void*)param);
    pthread_create(&outputT,NULL,output_func,(void*)param);

    // join the output thread so when server sends shutdown message
    // the function will close and then the client should close as well
    pthread_join(outputT,NULL);
  
    close(sockfd); 

    return 0;
}

int hostname_to_ip(char* hostname,char* ip){
    struct hostent* he;
    struct in_addr** addr_list;
    int i;

    if( (he = gethostbyname(hostname)) == NULL ){
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr**) he->h_addr_list;

    for(i=0;addr_list[i]!=NULL;i++){
        strcpy(ip,inet_ntoa(*addr_list[i]));
        return 0;
    }

    return 1;
}

