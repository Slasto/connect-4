#include <stdio.h>
#include <stdlib.h>

void ciao(int* c);

int main (){
    int c=10;
        ciao(&c);
    printf("%d",c);
}

void ciao(int* c){

*c = *c + 3;

}