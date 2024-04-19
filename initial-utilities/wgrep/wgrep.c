#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define errorhandler(message)   \
    do{                         \
        printf("%s\n",message); \
        exit(EXIT_FAILURE);     \
    }while(0)


void singleFile(FILE* fp,char* st){
    size_t buffersize=16;
    char* buffer=malloc(buffersize*sizeof(char));
    while(getline(&buffer,&buffersize,fp) != -1){
        if (strstr(buffer,st) != NULL){
            printf("%s",buffer);
        }
    }
    free(buffer);
}

int main(int argc,char* argv[]){
    if (argc == 1){
        errorhandler("wgrep: searchterm [file ...]");
    }else if (argc == 2){
        FILE* fp=stdin;
        singleFile(fp,argv[1]);
    }else{
        for(int i = 2;i < argc;i++){
            FILE* fp=fopen(argv[i],"r");
            if (fp == NULL){
                errorhandler("wgrep: cannot open file");
            }
            singleFile(fp,argv[1]);
        }
    }
    return 0;
}
