// HAVE TO PUT ALL OF MY INCLUDES IN HERE TO MAKE SHIT NOT MESSY
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<ctype.h>
#include<pthread.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/time.h>

#define INTERVAL 15000
#define MAX 300
#define PORT 9153
#define SA struct sockaddr


typedef struct _account{
    char name[256];
    double balance;
    int session; // 0 means not in session 1 means in session
    struct _account* next;
}account;

typedef struct _threadID{
    pthread_t id;
    struct _threadID* next;
}threadID;
