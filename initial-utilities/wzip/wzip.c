#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define errorhandler(message)  \
do{                         \
    printf("%s\n",message); \
    exit(EXIT_FAILURE);     \
}while(0)

void writeToFile(int count, int last)
{
    fwrite(&count, sizeof(int), 1, stdout);
    fprintf(stdout,"%c",last);
}

int main(int argc,char* argv[]){
    if (argc < 2){
        errorhandler("wzip: file1 [file2 ...]");
    }else{
        int last= 0;
        int count=1;
        for(int i = 1;i < argc;i++){
            FILE* fp = fopen(argv[i],"r");
            int temp;
            while((temp=fgetc(fp)) != EOF){
                if (temp ==  last){
                    count++;
                }else{
                    if(last != 0 ){
                        writeToFile(count,last);
                    }
                    last=temp;
                    count=1;
                }
            }
        }
        writeToFile(count,last);
    }
    return 0;
}
    