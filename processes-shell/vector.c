
#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*  Very Small BUFFERSIZE JUST FOR TEST.
*   Normally use INITCAPACITY = 16,BUFFERSIZE = 256.
*/

#define INITCAPACITY 4      
#define BUFFERSIZE 8        

#define errorhandler                \
do{                                 \
    char error_message[30] = "An error has occurred\n";         \
    write(STDERR_FILENO, error_message, strlen(error_message)); \
} while(0)

#endif

typedef struct dynamic_array_struct{
    char**   string;
    size_t  capacity;
    size_t  size;
}vector;

vector* vector_init(){
    vector*  v = (vector*)malloc(sizeof(vector));
    v->string=(char**)malloc(INITCAPACITY*sizeof(char*));
    if (v->string == NULL){
        errorhandler;
    }
    v->size = 0;
    v->capacity = INITCAPACITY;
    return v;
}

void vector_resize(vector* v){
    v->string=(char**)realloc(v->string,2*v->capacity*sizeof(char*));
    if (v->string == NULL){
        errorhandler;
    }
    v->capacity*=2;
}

void push_back(vector* v,char s[]){
    int index=v->size;
    v->string[index]=(char*)malloc((strlen(s)+1)*sizeof(char));
    if (v->string[index] == NULL){
        errorhandler;
    }
    strcpy(v->string[index],s);
    v->size++;
    if (v->size >= 0.75 * v->capacity){
        vector_resize(v);
    }
}

char* get_element(vector* v,int i){
    return v->string[i];
}

void free_all(vector* v) {
    for (size_t i = 0; i < v->size; i++) {
        free(v->string[i]); 
    }
    free(v->string); 
    free(v); 
}
