#include"bank.h"

// global value so that threads can know whether or not to send
// message to client
int shutDown = 0;

// global linked list of all accounts
account* head;

/*
// this is gonna be the SIGINT handler
void handle_sigint(int sig){
    shutDown = 1;
    kill(getpid(),1); 
}
*/

// struct to pass the port to the client acceptor function
typedef struct _acceptorP{
    int port;
} acceptorP;

typedef struct _clientP{
    int connfd;
} clientP;

// this function takes in a string that the user writes to the server
// and returns:
// 0 -> Create
// 1 -> Serve
// 2 -> Deposit
// 3 -> Withdraw
// 4 -> Query
// 5 -> End
// 6 -> Quit
// -1 -> if not a command
int commandCheck(char* command){
    int counter=0;
    
    char buff[MAX];
    bzero(buff,MAX);

    for(counter=0;counter<strlen(command);counter++){
        buff[counter] = command[counter];

        if(strcmp(buff,"create") == 0){
            return 0;
        }else if(strcmp(buff,"serve") == 0){
                return 1;
            }else if(strcmp(buff,"deposit") == 0){
                    return 2;
                }else if(strcmp(buff,"withdraw") == 0){
                        return 3;
                    }else if(strcmp(buff,"query") == 0){
                            return 4;
                        }else if(strcmp(buff,"end") == 0){
                                return 5;
                            }else if(strcmp(buff,"quit") == 0){
                                    return 6;
                                }
    }
    

    return -1;
}



// whenever a client connects, a thread should be spawned
// and this function will deal with the commands input from the client
void* client_service(void* params){

    // for checking to see if a client is in service because if they are
    // they cannot create accounts
    int inService = 0;    
    //account* acc = (account*)malloc(sizeof(account));


    clientP* test = (clientP*)params;
    int connfd = test->connfd;

    char buff[MAX];
    bzero(buff,sizeof(buff));  


    // constantly waiting to hear from a client
    for(;;){
        // read what the client is saying
        bzero(buff,sizeof(buff));
        read(connfd,buff,MAX);
        printf("Client %d says: %s",connfd,buff);

        // manipulate the buffer in order to figure out
        // what command they want to do
        int commandNum = commandCheck(buff);

        if(commandNum == 0){
            write(connfd,"\nCreate command received\n",26);
            // have to mutex account creation
            // initial balance is 0
            // A client who is in service cannot create an account
            if(inService == 1){
                write(connfd,"\nCannot create. In service\n",28);
            }else{
                
            }
        }else if(commandNum == 1){
                write(connfd,"\nServe command received\n",25);
                // run through second command checker
                
            }else if(commandNum == 2){
                    write(connfd,"\nDeposit command received\n",27);
                    // run through second command checker

                }else if(commandNum == 3){
                        write(connfd,"\nWithdraw command received\n",28);
                        // run through second command checker

                    }else if(commandNum == 4){
                            write(connfd,"\nQuery command received\n",25);
                            // run through second command checker

                        }else if(commandNum == 5){
                                write(connfd,"\nEnd command received\n",23);
                                // run through second command checker
                                
                                if(inService == 1){
                                    // set this client to not in service
                                    inService = 0;
                                    // set the account that was previously being serviced to not in service

                                }else{
                                    write(connfd,"\nNo account in service\n",24);
                                }                                

                            }else if(commandNum == 6){
                                    //write(connfd,"Quit command received\n",23);
                                    // run through second command checker
                                    
                                    if(inService == 1){
                                        // turn account to not in service
                                        write(connfd,"Shutting down",14);
                                        break;
                                    }else{
                                        write(connfd,"Shutting down",14);
                                        break;
                                    }                        
                                }else if(commandNum == -1){
                                        write(connfd,"\nPlease input a proper command\n",32);
                                    }

        // This is a check to shut down or not
        if(shutDown == 1){
            write(connfd,"Server shutting down",21);
            break;
        }


    }


    return NULL;
}


// function for the thread to listen and connect to clients that
// are trying to connect
void* session_acceptor(void* params){

    // cast void* param to actual param
    acceptorP* test = (acceptorP*)params;

    int sockfd, connfd; 
    struct sockaddr_in servaddr; 
    pthread_t thread_id;  
    

    // socket create and verification 
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(test->port); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
       printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 10)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
  
    // loop waiting for a client to connect
    for(;;){

        // Accept the data packet from client and verification 
        // blocks until a client connects
        connfd = accept(sockfd, (struct sockaddr*)NULL, NULL); 
    

        if (connfd < 0) {
            // ****************************************************************         MAKE SURE I SHOULDNT JUST CONTINUE 
            printf("server acccept failed...\n"); 
            exit(0); 
        } 
        else{
            printf("server acccept the client...\n"); 
            clientP* client_param = (clientP*)malloc(sizeof(clientP));
            client_param->connfd = connfd;
            // changing this so it can multi-thread?
            pthread_create(&thread_id,NULL,client_service,(void*)client_param);
        }
    }
    
    // ***********************************************************************          DO I HAVE TO JOIN ALL OF THE THREADS HERE?

   
    return NULL;
}

  
// Driver function 
int main(int argc,char** argv) { 
    int counter=0;

    if(argc!=2){
        printf("Please input the correct number arguments\n");
        return 1;
    }

    // check to see if the parameter is a digit
    for(counter=0;counter<strlen(argv[1]);counter++){
        char test = argv[1][counter];
        if(isdigit(test) == 0){
            printf("Please input a proper port\n");
            return 1;
        }
    }

    // setting the sigINT handler
    //signal(SIGINT,handle_sigint);


    acceptorP* acceptor_param = (acceptorP*)malloc(sizeof(acceptorP));
    acceptor_param->port = atoi(argv[1]);
    
    // create the session_acceptor thread
    pthread_t acceptor_thread;
    pthread_create(&acceptor_thread,NULL,session_acceptor,(void*)acceptor_param);
    pthread_join(acceptor_thread,NULL);

    return 0;
} 
