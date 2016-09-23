#ifndef __EXIF__TOOL_H__
#define __EXIF__TOOL_H__

#include "stdio.h"
#include "stdlib.h"

#define PRINT_MSG(fmt, ...) printf(fmt, ##__VA_ARGS__);

#define EXIF_ERROR(str) do{\
      printf("***EXIF ERROR***: %s\n", str);\
      exit(1);\
      }while(0);
#define EXIF_INFO(str) do{\
      printf("===EXIF INFO===: %s\n", str);\
      }while(0);

extern FILE* LoadImage(char* file);
#endif