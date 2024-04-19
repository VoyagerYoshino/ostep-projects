#include <stdlib.h>
#include <stdio.h>
#define BUFFERSIZE 16

#define errorhandler()   \
do{                 \
    printf("wcat: cannot open file\n"); \
    exit(EXIT_FAILURE); \
}while(0)   

int main(int argc,char* argv[]){
    if (argc > 1){
        for(int i = 1;i < argc;i++){
            FILE* fp=fopen(argv[i],"r");
            if (fp == NULL){
                errorhandler();
            }
            char buffer[BUFFERSIZE];
            while(fgets(buffer,BUFFERSIZE,fp) != NULL){
                printf("%s",buffer);
            }
        }
    }
    return 0;
}