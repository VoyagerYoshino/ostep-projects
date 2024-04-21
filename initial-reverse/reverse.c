#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "vector.h"
#include <string.h>

#define errorhandler(message)       \
do{                                 \
    fprintf(stderr,"%s\n",message); \
    exit(EXIT_FAILURE);             \
} while(0)


_Bool samefile(FILE* fp1,FILE* fp2){
    int fn1=fileno(fp1),fn2=fileno(fp2);
    struct stat stat1,stat2;
    if (fstat(fn1,&stat1) < 0 || fstat(fn2,&stat2)) errorhandler("reverse: fstat error");
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

int main(int argc,char* argv[]){
    FILE *fpin = NULL,*fpout = NULL;
    if (argc == 1){
        fpin=stdin;
        fpout=stdout;
    }else if (argc == 2){
        if ((fpin = fopen(argv[1],"r")) == NULL){
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }else if (argc == 3){
        if ((fpin = fopen(argv[1],"r")) == NULL){
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(EXIT_FAILURE);
        }        
        if ((fpout = fopen(argv[2],"w")) == NULL){
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            exit(EXIT_FAILURE);
        }   
        if (samefile(fpin,fpout)){
            errorhandler("reverse: input and output file must differ");
        }     
    }else{
        errorhandler("usage: reverse <input> <output>");
    }

    vector* v = vector_init();
    size_t linesize = BUFFERSIZE;
    char* line = malloc(linesize*sizeof(char));
    if (v->string[BUFFERSIZE] == NULL){
        errorhandler("reverse: malloc failed");
    }
    while(getline(&line,&linesize,fpin) != EOF){
        push_back(v,line);
    }
    for(int i = v->size-1;i >= 0;i--){
        fwrite(v->string[i],sizeof(char),strlen(v->string[i]),fpout);
    }
    free(line);
    free_all(v);
    fclose(fpin);
    fclose(fpout);
    return 0;
}