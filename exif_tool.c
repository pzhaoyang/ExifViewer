#include "exif_tool.h"


FILE* LoadImage(char* file){
    FILE *fp = NULL;
    
    fp = fopen(file, "rb");
    if(fp == NULL){
       EXIF_ERROR("Cannot Load File!");
    }

    return fp;
}