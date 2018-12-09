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

// this function receives the output from commandCheck and the string
// it then figures out if the trailing part is correct.
// Returns the string after the command
char* command_param(int commandNum,char* command){
    int counter=0;
    char* buff = (char*)malloc(sizeof(char)*MAX);
    bzero(buff,MAX);

    if(commandNum == 0){
        // create
        // returns "" if there is no word after create
        // or there is no space after create
        if(strlen(command)<7 || command[6] != ' '){
            // only have create in command. No name
            // NO SPACE AFTER COMMAND NAME WHAT TO DO
            return "";
        }
        for(counter=7;counter<strlen(command);counter++){
            buff[counter-7] = command[counter];
        }

        // *********************************************************** do i have to check if the characters are not alphanumeric??
        return buff;

    }else if(commandNum == 1){
        // serve
        if(strlen(command) < 6 || command[5] != ' '){
            // only have the serve command no name
            // no space after command name
            return "";
        }
        for(counter=6;counter<strlen(command);counter++){
            buff[counter-6] = command[counter];
        }
        return buff;

    }else if(commandNum == 2){
        // deposit
    }else if(commandNum == 3){
        // withdraw
    }else if(commandNum == 4){
        // query
        if(strlen(command) < 6)
            return "";
        else
            return "error";
    }else if(commandNum == 5){
        // end
        if(strlen(command) < 4){
            return "";
        }else{
            return "error";
        }
    }else if(commandNum == 6){
        // quit

        if(strlen(command) < 5){
            return "";
        }else
            return "error";
    }

    return "";
}

// this function searches the list for the given name and returns the
// ptr to the account
// returns null if it does not exist
account* find_account(char* name){
    account* ptr = head;

    while(ptr!=NULL){
        if(strcmp(ptr->name,name) == 0)
            return ptr;
        ptr = ptr->next;
    }

    return NULL;
}

// whenever a client connects, a thread should be spawned
// and this function will deal with the commands input from the client
void* client_service(void* params){

    // for checking to see if a client is in service because if they are
    // they cannot create accounts
    int inService = 0;    
    account* acc = (account*)malloc(sizeof(account));


    clientP* test = (clientP*)params;
    int connfd = test->connfd;

    char buff[MAX];
    bzero(buff,sizeof(buff));  

    int newLineC =0;

    // constantly waiting to hear from a client
    for(;;){
        // read what the client is saying
        bzero(buff,sizeof(buff));
        read(connfd,buff,MAX);
        // loop through getting rid of newlines
        for(newLineC=0;newLineC<strlen(buff);newLineC++)
            if(buff[newLineC] == '\n')
                buff[newLineC] = '\0';

        // manipulate the buffer in order to figure out
        // what command they want to do
        int commandNum = commandCheck(buff);

        if(commandNum == 0){
            //write(connfd,"\nCreate command received\n",26);
            // have to mutex account creation
            // initial balance is 0

            // A client who is in service cannot create an account
            if(inService == 1){
                write(connfd,"Cannot create. In service\n",27);
            }else{
                char* commandArg = (char*)malloc(sizeof(char)*MAX);

                if(strlen(buff) < 8 || buff[6] != ' '){
                    // either command had no space after create
                    // or the command was only create and had no name
                    write(connfd,"Please use create correctly\n",29);                    
                }else{ 
                    // change the value of command to just be the name in this case
                    strcpy(commandArg, command_param(commandNum,buff));
                    account* tempAcc = find_account(commandArg);
                    if(tempAcc == NULL){
                        // name is not in use
                        tempAcc = (account*)malloc(sizeof(account));
                        strcpy(tempAcc->name, commandArg);
                        tempAcc->balance = 0;
                        tempAcc->session =0;
                        tempAcc->next = head;
                        head = tempAcc;
                        write(connfd,"Account created\n",17);
                    }else{
                        // name is already used
                        write(connfd,"Name is already in use\n",24);
                    }

                }
                
            }
        }else if(commandNum == 1){
                //write(connfd,"\nServe command received\n",25); // this needs to be commented out
                // run through second command checker
                if(strlen(buff) < 7){
                    write(connfd,"Please use serve correctly\n",28);
                }else{
                    if(inService == 0){
                        char* commandArg = (char*)malloc(sizeof(char)*MAX);
                        // get account name from command
                        strcpy(commandArg, command_param(commandNum,buff));
                        account* tempAcc = find_account(commandArg);
                        // check to see if the account exists
                        // check to see if the service is in session from another user
                        if(tempAcc == NULL){ 
                            // account doesnt exist
                            write(connfd,"Account does not exist\n",24);
                        }else if(tempAcc->session == 1){
                            // account currently in use
                            write(connfd,"Account is in use\n",19);
                        }else{
                            // set local variable equal to the account in use
                            acc = tempAcc;
                            acc->session = 1;
                            inService = 1;
                            write(connfd,"Account now in session\n",24);
                        }                        

                    }else{
                        write(connfd,"Please end current session to serve another account\n",53);
                    }                
                }

            }else if(commandNum == 2){
                    //write(connfd,"\nDeposit command received\n",27);
                    // run through second command checker
                    if(strlen(buff) < 9){
                        write(connfd,"Please use deposit correctly\n",30);
                    }else{
                        if( inService == 1){

                        }else{
                            // not in session so cant do command
                            write(connfd,"No account in service\n",23);
                        }
                    }

                }else if(commandNum == 3){
                        //write(connfd,"\nWithdraw command received\n",28);
                        // run through second command checker
                        if(strlen(buff) < 10){
                            write(connfd,"Please use withdraw correctly\n",31);
                        }else{
                            if(inService == 1){

                            }else{
                                // not in session so cant do command
                                write(connfd,"No account in service\n",23);
                            }
                        }

                    }else if(commandNum == 4){
                            //write(connfd,"\nQuery command received\n",25);
                            // run through second command checker
                            if(strlen(buff) < 6){        
                                if(inService == 1){
                                    // initialize the string to 1000 bytes
                                    char stroutput[1000];
                                    bzero(stroutput,1000);

                                    // convert the double to a string
                                    sprintf(stroutput,"Balance: %lf\n",acc->balance);

                                    write(connfd,stroutput,1000);
      
                                }else{
                                    write(connfd,"No account in service\n",23);
                                }
                            }else{
                                write(connfd,"Please use query correctly\n",28);
                            }

                        }else if(commandNum == 5){
                                //write(connfd,"\nEnd command received\n",23); // write below isnt working if this write exists
                                // run through second command checker
                                if(strlen(buff) < 4){        
                                    if(inService == 1){
                                        // set this client to not in service
                                        inService = 0;
                                        // set the account that was previously being serviced to not in service
                                        acc->session = 0;
                                        //acc = NULL;
                                        write(connfd,"Service session ended\n",23);
                                    }else{
                                        write(connfd,"No account in service\n",23);
                                    }           
                                }else{
                                    write(connfd,"Please use end correctly\n",26);
                                }                     

                            }else if(commandNum == 6){
                                    //write(connfd,"Quit command received\n",23); // write below isnt oworking if this write exists
                                    // run through second command checker
                                    

                                    if(strlen(buff) < 5){

                                        if(inService == 1){
                                            // turn account to not in service
                                            acc->session  = 0;    
                                            inService = 0;                   
                                        }

                                        // write to client and then break out of this loop so this thread goes away                        
                                        write(connfd,"Shutting down",14);
                                        break;
                                    }else{
                                        write(connfd,"Please use quit correctly\n",27);
                                    }
                                }else if(commandNum == -1){
                                        write(connfd,"Please input a proper command\n",31);
                                    }

        // This is a check to shut down or not
        if(shutDown == 1){
            // have to turn all accounts to not in session

            write(connfd,"Server shutting down",21);
            break;
        }


    }


    // *************************************************************************************            DO I HAVE TO CLOSE THE SOCKET???

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
