#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "vector.c"

#define INTERMODE 1
#define BATCHMODE 2
#define CAPACITY 256 

char* PATH[CAPACITY] = {"/bin",NULL};


#define errorhandler                \
do{                                 \
    char error_message[30] = "An error has occurred\n";         \
    write(STDERR_FILENO, error_message, strlen(error_message)); \
} while(0)

#define buildinidentifer(co)   ((strcmp(co,"cd") == 0) || (strcmp(co,"exit") == 0) || (strcmp(co,"path") == 0)) ? 1 : 0

int phasearguments(char* inputline,vector* arguments){
    char* token;
    char* delim = " \t\n";
    int size = 0;
    while((token = strsep(&inputline,delim)) != NULL){
        if (*token != '\0'){
            push_back(arguments,token);
        }
    }
    return arguments->size;
}

void buildInCommand(vector* arguments){
    if(strcmp(get_element(arguments,0),"exit") == 0){
        if (arguments->size == 1){   
            exit(EXIT_SUCCESS);
        }else{
            errorhandler;
        }   
        return;
    }
    if(strcmp(get_element(arguments,0),"cd") == 0){
        if (arguments->size == 2){   
            chdir(get_element(arguments,1));
        }else{    
            errorhandler;
        }
        return;
    }
    if(strcmp(get_element(arguments,0),"path") == 0){
        if (arguments->size == 1){
            PATH[0] = NULL;
        }else{
            for(int i = 1;i < arguments->size;i++){
                char destination[CAPACITY] = {'\0'};
                strcpy(destination,get_element(arguments,i));
                PATH[i-1] = destination;  
            }
        }
        return;
    }
}

void otherCommand(vector* arguments){
    int i = 0;
    if (PATH[0] == NULL){
        errorhandler;
        return;
    }
    while(PATH[i] != NULL){
        char destination[CAPACITY];
        strcpy(destination,PATH[i]);
        strcat(destination,"/");
        strcat(destination,get_element(arguments,0));
        if (access(destination,X_OK) == 0){
            int rc = fork();
            if (rc < 0){
                errorhandler;
                return;
            }else if (rc == 0){
                execv(destination,arguments->string);
            }else{
                wait(NULL);
                break;
            }
        }else{
            errorhandler;
            return;
        }
        i++;
    }
}

int main(int argc,char* argv[]){
    FILE *fp = stdin;
    if (argc != INTERMODE && argc != BATCHMODE){
        errorhandler;
        exit(EXIT_FAILURE);
    }

    if (argc == BATCHMODE){
        if ((fp = fopen(argv[1],"r")) == NULL){
            errorhandler;
            exit(EXIT_FAILURE);
        }    
    }
    do{
        if (argc == INTERMODE)      fprintf(stdout,"wish> ");
        vector* arguments = vector_init();
        char* inputLine;
        size_t lineSize = 0;
        int rc;
        if((rc = getline(&inputLine,&lineSize,fp)) != EOF){
            if(phasearguments(inputLine,arguments) == 0) continue;
            if(buildinidentifer(get_element(arguments,0))){
                buildInCommand(arguments);
             }else{
                otherCommand(arguments);
             }
        }
        free_all(arguments);
        if (rc == EOF){
            exit(EXIT_SUCCESS);
        }
    }while(1);
}