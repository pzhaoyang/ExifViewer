#include "exif_tagdb.h"


char* getTagName(unsigned short TagID){
   int index;
   int count;
   
   count = sizeof(gTagDB)/sizeof(TAG_ITEM);
   for(index=0; index<count; index++ ){
      if(TagID == gTagDB[index].TagID){
          return gTagDB[index].TagName;
      }
   }
}

char* traverseTagDB(){
   int index;
   int count;
   
   count = sizeof(gTagDB)/sizeof(TAG_ITEM);
   printf("TagID\tTagName\n");
   for(index=0; index<count; index++ ){
       printf("0x%04x\t%s\n",gTagDB[index].TagID, gTagDB[index].TagName);
   }
}