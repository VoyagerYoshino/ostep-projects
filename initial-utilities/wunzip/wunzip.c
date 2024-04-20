#include <stdio.h>
#include <stdlib.h>
#define nmemb 1

#define errorhandler(message)   \
do{                         \
    printf("%s\n",message); \
    exit(EXIT_FAILURE);     \
}while(0)


int main(int argc,char* argv[]){
    if (argc < 2){
        errorhandler("wunzip: file1 [file2 ...]");
    }else{
        for(int i = 1;i < argc;i++){
            FILE* fp = fopen(argv[i],"r");
            int count;
            while(fread(&count,sizeof(int),nmemb,fp) == nmemb){
                char ch;
                fread(&ch,sizeof(char),nmemb,fp);
                for(int i = 0;i < count;i++){
                    fprintf(stdout,"%c",ch);
                }
            }
            fclose(fp);
        }
    }
    return 0;
}