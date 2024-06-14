#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define errorhandler                \
do{                                 \
    char error_message[30] = "An error has occurred\n";         \
    write(STDERR_FILENO, error_message, strlen(error_message)); \
} while(0)

#define OPTNUM  5
#define FALSE 0
#define TRUE 1

char* OPT[OPTNUM] = {"p","g","d","c","a"};

int arguments_sepration(char* command,char* arguments[]){
    int index = 0;
    char* token;
    const char* delim = ",";
    while(((token = strsep(&command,delim)) != NULL) && index < 3){
        if((arguments[index] = malloc((strlen(token)+1)*sizeof(char))) == NULL){
            errorhandler;
            exit(EXIT_FAILURE);
        }
        strcpy(arguments[index],token);
        index++;
    }
    return index;
}

void put(int key,char* value){
    FILE *fp;
    if((fp = fopen("data.txt","a"))==NULL){
        errorhandler;
        exit(EXIT_FAILURE);
    }
    if (fprintf(fp,"%d,%s\n",key,value) < 0 ){
        errorhandler;
        exit(EXIT_FAILURE);
    }       
    fclose(fp);
}

void get(char* key){
    FILE* fp;
    _Bool flag = FALSE;
    if ((fp = fopen("data.txt","r")) == NULL){
        errorhandler;
        exit(EXIT_FAILURE);
    }
    char* line;
    size_t len = 0;
    while(getline(&line,&len,fp) != EOF){
        char* line_args[2] = {NULL,NULL};
        arguments_sepration(line,line_args);
        if (strcmp(line_args[0],key) == 0){
            flag =TRUE;
            fprintf(stdout,"%s,%s",line_args[0],line_args[1]);
            break;
        }
    }
    fclose(fp);
    if (flag == FALSE){
        fprintf(stdout,"%s not found\n",key);
    }
}



int singleCommand_execution(char* arguments[],int index){
    _Bool flag = FALSE;
    for(int i = 0;i < OPTNUM;i++){
        if (strcmp(arguments[0],OPT[i]) == 0){
            if (i == 0 && index ==3){
                flag =TRUE;
                put(atoi(arguments[1]),arguments[2]);
                break;
            }
            if (i == 1 && index ==2){
                flag =TRUE;
                get(arguments[1]);
                break;
            }

        }
    }
}


int main(int argc,char* argv[]){
    char* arguments[3] = {NULL,NULL,NULL};
    if (argc > 1){
        for (int i = 1;i < argc;i++){
            int index = arguments_sepration(argv[i],arguments);
            singleCommand_execution(arguments,index);
        }
    }

    return 0;
}