#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define errorhandler(message)  \
do{                         \
    printf("%s\n",message); \
    exit(EXIT_FAILURE);     \
}while(0)

int main(int argc,char* argv[]){
    if (argc < 2){
        errorhandler("wzip: file1 [file2 ...]");
    }else{
        int last= 0;
        int len=1;
        for(int i = 1;i < argc;i++){
            FILE* fp = fopen(argv[i],"r");
            if (last == 0 ){
                last = fgetc(fp);
            }
            int temp;
            while((temp=fgetc(fp)) != EOF){
                if (temp ==  last){
                    len++;
                }else{
                    fwrite(&len,sizeof(int),1,stdout);
                    fprintf(stdout,"%c",last);
                    last=temp;
                    len=1;
                }
            }
            if (i == argc -1){
                fwrite(&len,sizeof(int),1,stdout);
                fprintf(stdout,"%c",last);
            }
        }
    }
    return 0;
}
    