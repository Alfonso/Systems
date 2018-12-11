#include"bank.h"

// global value so that threads can know whether or not to send
// message to client
int shutDown = 0;

// global linked list of all accounts
account* head;

// global acceptor thread id
pthread_t acceptor_thread;

// global linked list of all client threads
threadID* threadHead;

// global mutexes
pthread_mutex_t mutex;


// this is gonna be the SIG handler
void handle_sig(int sig){
    
    // SIGINT
    if(sig == SIGINT){
        shutDown = 1;

        // kill the acceptor thread
        pthread_cancel(acceptor_thread);
        
        // join the client threads
        threadID* ptr = threadHead;
        while(ptr != NULL){
            pthread_join(ptr->id,NULL);
            ptr = ptr->next;
        }

        // make every account not in session
        // instead, should I free this memory?
        account* accptr = head;
        while(accptr != NULL){
            accptr->session = 0;
            accptr = accptr->next;
        }    

    }else if(sig == SIGALRM){
        account* ptr = head;
        printf("ACCOUNTS:\n");
        while(ptr != NULL){
            printf("%s\t%lf",ptr->name,ptr->balance);
            if(ptr->session == 1){
                printf("\tIN SERVICE\n");
            }else{
                printf("\n");
            }
            ptr = ptr->next;
        }
    }
}


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
        if(strlen(command) < 8 || command[7] != ' '){
            // command is not formatted correct
            // no space after the command name
            return "";
        }

        // used to keep track of if there is more than one decimal point
        int hasDecimal = 0;

        // check if what is after the space is a digit. if it is return it
        for(counter=8;counter<strlen(command);counter++){
            // looking at this current char
            char temp = command[counter];
            // check to see if the character is either a digit or is '.'
            if(isdigit(temp) != 0 || temp == '.'){
                // character is either a digit or is a '.'
                if(temp == '.'){
                    // character is a '.'
                    if(hasDecimal == 0){
                        // this is the first decimal point
                        hasDecimal = 1;
                        buff[counter-8] = command[counter];
                    }else if(hasDecimal == 1){
                        // there has already been a decimal point
                        // command is invalid
                        return "";
                    }
                }else{
                    // character is a digit        
                    buff[counter-8] = command[counter];
                }
            }else{
                return "";
            }
        }

        // this theoretically should return the string version of the double 
        return buff;

    }else if(commandNum == 3){
        // withdraw
        // basically deposit but removing instead of adding
        // the check for if they can remove that amount is going to be done in the client_thread
        if(strlen(command) < 9 || command[8] != ' '){
            // command is not formatted correct
            // no space after the command name
            return "";
        }

        // used to keep track of if there is more than one decimal point
        int hasDecimal = 0;

        // check if what is after the space is a digit. if it is return it
        for(counter=9;counter<strlen(command);counter++){
            // looking at this current char
            char temp = command[counter];
            // check to see if the character is either a digit or is '.'
            if(isdigit(temp) != 0 || temp == '.'){
                // character is either a digit or is a '.'
                if(temp == '.'){
                    // character is a '.'
                    if(hasDecimal == 0){
                        // this is the first decimal point
                        hasDecimal = 1;
                        buff[counter-9] = command[counter];
                    }else if(hasDecimal == 1){
                        // there has already been a decimal point
                        // command is invalid
                        return "";
                    }
                }else{
                    // character is a digit        
                    buff[counter-9] = command[counter];
                }
            }else{
                return "";
            }
        }

        // this theoretically should return the string version of the double 
        return buff;

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

// this thread will check if the shutdown value has changed
// if it has changed, it will send a shut down message to the client
// and then the client will write back to the server telling it to shut down
void* shutdown_check(void* params){
    
    clientP* test = (clientP*)params;
    int connfd = test->connfd;

    // should I sleep so it doesnt take up too much memory?

    for(;;){
        if(shutDown == 1){
            write(connfd,"Server shutting down",21);
            break;
        }
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

    // create thread that checks for shutdown
    pthread_t shutdownID;
    pthread_create(&shutdownID,NULL,shutdown_check,(void*)params);

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
            //              CREATE
            // *******************************************have to mutex account creation
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
                    // create temp accoun t to see if it exists
                    account* tempAcc = find_account(commandArg);
                    if(tempAcc == NULL){
                        // name is not in use
    
                        // lock the accounts here so you can add a new one
                        pthread_mutex_lock(&mutex);

                        tempAcc = (account*)malloc(sizeof(account));
                        strcpy(tempAcc->name, commandArg);
                        tempAcc->balance = 0;
                        tempAcc->session =0;
                        tempAcc->next = head;
                        head = tempAcc;
                        write(connfd,"Account created\n",17);

                        //unlock the accounts here
                        pthread_mutex_unlock(&mutex);

                    }else{
                        // name is already used
                        write(connfd,"Name is already in use\n",24);
                    }

                }
                
            }
        }else if(commandNum == 1){
                //                  SERVE
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
                    //                  DEPOSIT
                    // run through second command checker
                    if(strlen(buff) < 9){
                        write(connfd,"Please use deposit correctly\n",30);
                    }else{
                        if( inService == 1){
                           // check to see if they are actually sending over a number to deposit from
                            char* commandArg = (char*)malloc(sizeof(char)*MAX);
                            bzero(commandArg,MAX);
                            
                            strcpy(commandArg,command_param(commandNum,buff));


                            if(strcmp(commandArg,"") == 0){
                                write(connfd,"Please use deposit correctly\n",30);
                            }else{
                                // there is nothing wrong about the input so convert the string to a double
                                acc->balance = acc-> balance + atof(commandArg);
                                
                                write(connfd,"Successfully deposited\n",24);
                            }
                        }else{
                            // not in session so cant do command
                            write(connfd,"No account in service\n",23);
                        }
                    }

                }else if(commandNum == 3){
                        //                      WITHDRAW
                        // run through second command checker
                        if(strlen(buff) < 10){
                            write(connfd,"Please use withdraw correctly\n",31);
                        }else{
                            if(inService == 1){
                                // check so ee if they are actually sending over a number to withdraw from
                                char* commandArg = (char*)malloc(sizeof(char)*MAX);
                                bzero(commandArg,MAX);

                                strcpy(commandArg,command_param(commandNum,buff));

                                if(strcmp(commandArg,"") == 0){
                                    write(connfd,"Please use withdraw correctly\n",31);
                                }else{
                                    // there is nothing wrong with the input so convert the string to a double
                                    double tempWith = atof(commandArg);
                                    // check to see if tempWith is greater than the current balance. If so, then only
                                    // remove to 0
                                    if(tempWith > acc->balance){
                                        if(acc->balance == 0){
                                            write(connfd,"Current balance is 0. Cannot withdraw\n",39);
                                        }else{
                                            acc->balance = 0;
                                            write(connfd,"Withdrew maximum amount possible (account at 0)\n",49);
                                        }
                                    }else if(tempWith <= acc->balance){
                                        acc->balance = acc->balance - tempWith;
                                        write(connfd,"Successfully withdrawn\n",24);
                                    }
                                }            
                            }else{
                                // not in session so cant do command
                                write(connfd,"No account in service\n",23);
                            }
                        }

                    }else if(commandNum == 4){
                            //                  QUERY
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
                                //                  END
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
                                    //                  QUIT
                                    // run through second command checker
                                    

                                    if(strlen(buff) < 5){

                                        if(inService == 1){
                                            // turn account to not in service
                                            acc->session  = 0;    
                                            inService = 0;                   
                                        }

                                        // write to client and then break out of this loop so this thread goes away                        
                                        write(connfd,"Shutting down",14);
                                        // print to server that a client disconnected
                                        printf("Client disconnected...\n");
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
            
            //write(connfd,"Server shutting down",21);
            break;
        }


    }


    // *************************************************************************************            DO I HAVE TO CLOSE THE SOCKET???
    close(connfd);    

    // join shutdown thread
    pthread_join(shutdownID,NULL);

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
    if ((listen(sockfd, 100)) != 0) { 
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
            printf("Client connected...\n"); 
            clientP* client_param = (clientP*)malloc(sizeof(clientP));
            client_param->connfd = connfd;
            // changing this so it can multi-thread?
            pthread_create(&thread_id,NULL,client_service,(void*)client_param);
            // add the thread ID to the global list to join later
            threadID* temp = (threadID*)malloc(sizeof(threadID));
            temp->next = threadHead;
            temp->id = thread_id;
        }

/*              INSTEAD OF THIS, SIG HANDLER JUST KILLS THIS THREAD BECAUSE I COULDNT FIGURE OUT HOW TO HANDLE ACCEPT BLOCKING
        if(shutDown == 1){
            printf("SHUT DOWN RECEIVED IN ACCEPTOR THREAD\n");
            break;
        }
*/

    }
    
    // ***********************************************************************          DO I HAVE TO JOIN ALL OF THE THREADS HERE?
    // I think so

/*              INSTEAD OF JOINING ALL OF HTE THREADS HERE, THEY ARE JOINED IN THE SIG HANDLER
    threadID* ptr = threadHead;
    while(ptr != NULL){
        pthread_join(ptr->id,NULL);
        ptr = ptr->next;
    }
*/

   
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
    //signal(SIGINT,handle_sig);

    // do something to prevent the acceptor thread from not joining
    // because accept is blocking
    struct sigaction a;
    a.sa_handler = handle_sig;
    a.sa_flags = 0;
    sigemptyset(&a.sa_mask);
    sigaction(SIGINT,&a,NULL);

    // setting the sigalarm handler
    signal(SIGALRM,handle_sig);

    // setting the timer for SIGALRM
    struct itimerval it_val;
    it_val.it_value.tv_sec = INTERVAL/1000;
    it_val.it_value.tv_usec = (INTERVAL*1000) % 1000000;
    it_val.it_interval = it_val.it_value;
    setitimer(ITIMER_REAL,&it_val,NULL);
    
    // struct to pass port to acceptor thread
    acceptorP* acceptor_param = (acceptorP*)malloc(sizeof(acceptorP));
    acceptor_param->port = atoi(argv[1]);
    
    // create the session_acceptor thread
    pthread_create(&acceptor_thread,NULL,session_acceptor,(void*)acceptor_param);
    pthread_join(acceptor_thread,NULL);

    // have to stop timer
    it_val.it_value.tv_sec = 0;
    it_val.it_value.tv_usec = 0;
    it_val.it_interval = it_val.it_value;
    setitimer(ITIMER_REAL,&it_val,NULL);

    printf("\b\bSERVER SHUT DOWN\n");

    return 0; 


}
