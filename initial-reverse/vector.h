#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define errorhandler(message)       \
do{                                 \
    fprintf(stderr,"%s\n",message); \
    exit(EXIT_FAILURE);             \
} while(0)

/*  Very Small BUFFERSIZE JUST FOR TEST.
*   Normally use INITCAPACITY = 16,BUFFERSIZE = 1024.
*/

#define INITCAPACITY 4      
#define BUFFERSIZE 8        

#endif

typedef struct dynamic_array_struct{
    char*   string[BUFFERSIZE];
    size_t  capacity;
    size_t  size;
}vector;

vector* vector_init(){
    vector*  v = (vector*)malloc(sizeof(vector));
    v->string[BUFFERSIZE]=(char*)malloc(INITCAPACITY*sizeof(char[BUFFERSIZE]));
    if (v->string[BUFFERSIZE] == NULL){
        errorhandler("reverse: malloc failed");
    }
    v->size = 0;
    v->capacity = INITCAPACITY;
    return v;
}

void vector_resize(vector* v){
    v->string[BUFFERSIZE]=(char*)realloc(v->string,2*v->capacity);
    if (v->string[BUFFERSIZE] == NULL){
        errorhandler("reverse: realloc failed");
    }
    v->capacity*=2;
}

void push_back(vector* v,char s[]){
    int index=v->size;
    v->string[index]=(char*)realloc(v->string[index],strlen(s)*sizeof(char));
    if (v->string[BUFFERSIZE] == NULL){
        errorhandler("reverse: realloc failed");
    }
    strcpy(v->string[index],s);
    v->size++;
    if (v->size >= 0.75 * v->capacity){
        vector_resize(v);
    }
}

void free_all(vector* v){
    free(v);
}