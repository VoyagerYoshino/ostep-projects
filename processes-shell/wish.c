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

int phasespace(char* inputline,vector* arguments){      /* PHASE SPACE HELP FUNCTION*/
    char* token;
    char* delim = " \t\n";
    while((token = strsep(&inputline,delim)) != NULL){
        if (*token != '\0'){
            push_back(arguments,token);
        }
    }
    return arguments->size;
}

int phaseinput(char* inputline,vector* arguments,vector* output){  
    int argumentSize;
    if (strstr(inputline,">") == NULL){
        argumentSize = phasespace(inputline,arguments);
    }else{
        char* token;
        char* delim = ">";
        token = strsep(&inputline,delim);
        argumentSize = phasespace(token,arguments);
        phasespace(inputline,output);
        if (output->size != 1 || arguments->size == 0){
            return -1;
        }
    }
    return argumentSize;
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

void otherCommand(vector* arguments,vector* output){
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
                if (output->size == 1){                         /*REDIRECTION PART*/
                    FILE* fp = fopen(get_element(output,0),"w");
                    dup2(fileno(fp),STDOUT_FILENO);
                    dup2(fileno(fp),STDERR_FILENO);
                    fclose(fp);
                }
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
        vector* output = vector_init();
        char* inputLine;
        size_t lineSize = 0;
        int rc;
        if((rc = getline(&inputLine,&lineSize,fp)) != EOF){
            rc = phaseinput(inputLine,arguments,output);
            if(rc == 0) continue;                   /*NO ARGUMENTS AT ALL*/
            if(rc == -1){                           /*REDIRECTION ERROR*/
                errorhandler;
                continue;                           
            }
            if(buildinidentifer(get_element(arguments,0))){
                buildInCommand(arguments);
             }else{
                otherCommand(arguments,output);
             }
        }
        free_all(arguments);
        free_all(output);
        if (rc == EOF){
            exit(EXIT_SUCCESS);
        }
    }while(1);
}