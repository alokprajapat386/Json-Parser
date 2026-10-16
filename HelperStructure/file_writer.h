#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include<stdio.h>
#include<stdlib.h>

typedef struct {
    FILE *fp;
} FileWriter;

FileWriter* createFileWriter(char* file_name){
    FileWriter* fw=(FileWriter*)malloc(sizeof(FileWriter*));
    fw->fp=fopen(file_name, "w");
    return fw;
}

void printInFile(FileWriter* fw, char* s){
    fprintf(fw->fp, s);
}



#endif