#include<stdio.h>
#include<stdlib.h>

typedef struct _unknown {
    int val1;
    int val2;
    int val3;

} unknown;

int main(int argc,char** argv){

    unknown* thingy = (unknown*)malloc(4*sizeof(unknown));

    int mystery = 0;
    int mystery2 = 0;
    mystery = (thingy + 1);
    
    mystery2 = thingy;
    int mystery3 = 0;
    mystery3 = (thingy+1) - thingy;
    int mystery4 = 0;
    mystery4 = ((unknown *) thingy+1) - thingy;

    printf("Thingy: %d\n",thingy);
    printf("m1: %d\n",mystery);
    printf("m2: %d\n",mystery2);    
    printf("m1-m2: %d\n",mystery-mystery2);
    printf("m3: %d\n",mystery3);
    printf("m4: %d\n",mystery4);

    return 0;
}
