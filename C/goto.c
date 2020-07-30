#include <stdio.h>

int main(){
    int a=10;
    LOOP:do{
        if(a==15){
            a=a+1;
            goto LOOP;
        }
        printf("a value:%d",a);
        a++;
    }while(a<20);
    return
}