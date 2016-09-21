#include "stdio.h"
#include "string.h"
#include "stdlib.h"
 /*
  *
    typedef enum{
     BYTE = 1,         //BYTE An 8-bit unsigned integer.
     ASCII = 2,        //ASCII An 8-bit byte containing one 7-bit ASCII code. The final byte is terminated with NULL.
     SHORT = 3,        //SHORT A 16-bit (2-byte) unsigned integer
     LONG = 4,         //LONG A 32-bit (4-byte) unsigned integer
     RATIONAL = 5,     //RATIONAL Two LONGs. The first LONG is the numerator and the second LONG expresses the denominator.
     SBYTE = 6,        //An 8-bit signed(two complement) integer.
     UNDEFINED = 7,    //UNDEFINED An 8-bit byte that can take any value depending on the field definition
     SSHORT = 8,       //A 16-bit(2 byte) signed (twos-complement) integer.
     SLONG = 9,        //SLONG A 32-bit (4-byte) signed integer (2's complement notation)
     SRATIONAL = 10,   //SRATIONAL Two SLONGs. The first SLONG is the numerator and the second SLONG is the denominator
     FLOAT = 11,       //Signle precision(4-byte) IEEE format.
     DOUBLE = 12,      //Double precision(8-byte) IEEE format.
    } TIFF_IFD_TYPE;
  *
*/

typedef struct{
  char ByteOrder[2];
  unsigned short FurtherID; //42
  unsigned int offset;// 1stIFD Location
} TIFF_IFH;

typedef struct{
    unsigned short TagID;
    unsigned short Type;
    unsigned int CountItems; //In the case of one value of SHORT (16 bits), for example, the count is '1' even though it is 2 bytes
    unsigned int valueOffset; //This tag records the offset from the start of the TIFF header to the position where the value itself is recorded
} TIFF_IFD;

typedef struct{
    short Count;
    TIFF_IFD *pIFD;
}TIFF_IFD_ENTRY;

#pragma pack (2)
typedef struct{
    unsigned short Marker;
    unsigned short Length;
    char ExifStr[4];
    unsigned short Reserve;
    TIFF_IFH  ifh;
} APP1_SEG;

typedef struct{
  unsigned short SOI;
  APP1_SEG Exif;
} ImageHeader;
#pragma pack ()


static ImageHeader gExifInfo;

FILE* openfile(char* file){
    FILE *fp = NULL;
    fp = fopen(file, "rb");
    
    if(fp == NULL){
       printf("Cannot open the file!\n");
       exit(1);
    }

    return fp;
}

void checkImageHeader(){
    if(gExifInfo.Exif.Marker != 0xE1FF){
        printf("This program just support Exif image File Marker=0x%04x.\n", gExifInfo.Exif.Marker);
        exit(1);
    }
    
    if(gExifInfo.Exif.ifh.ByteOrder[0] != 0x49 && gExifInfo.Exif.ifh.ByteOrder[1] != 0x49){
        printf("This program just support Big Endian file.\n");
        exit(1);
    }
    
    if(gExifInfo.Exif.ifh.FurtherID != 0x2A){
        printf("This is a not vaild tiff tag.\n");
        exit(1);
    }

    if( gExifInfo.Exif.ExifStr[0] != 0x45 
      &&gExifInfo.Exif.ExifStr[1] != 0x78 
      &&gExifInfo.Exif.ExifStr[2] != 0x69 
      &&gExifInfo.Exif.ExifStr[3] != 0x66){
        printf("This is not a valid exif file\n");
        exit(1);
    }
}


void printExifs(TIFF_IFD_ENTRY *ifd_entry){
    int index = 0;
    
    for(index = 0; index < ifd_entry->Count; index++){
        
        switch(ifd_entry->pIFD[index].TagID){
          default:
             printf("The tagid is: 0x%04x\n", ifd_entry->pIFD[index].TagID);
        }
    }
    
}
void ReadExifData(char* imagefile){
    FILE *fp = NULL;
    TIFF_IFD_ENTRY ifd_entry;
    
    fp = openfile(imagefile);
    memset(&gExifInfo, 0, sizeof(gExifInfo));
    fread(&gExifInfo, sizeof(gExifInfo), 1, fp);
    checkImageHeader();
    
    if(gExifInfo.Exif.ifh.offset != 0x00000008){
       printf("The 0th IFD is not followed immediately.\n");
       exit(1);
    }
    
    fread(&ifd_entry.Count, 2, 1, fp);
    ifd_entry.pIFD = (TIFF_IFD*)malloc(sizeof(TIFF_IFD)*ifd_entry.Count);
    memset(ifd_entry.pIFD, 0x00, sizeof(TIFF_IFD)*ifd_entry.Count);
    fread(ifd_entry.pIFD, sizeof(TIFF_IFD)*ifd_entry.Count, 1, fp);
    printExifs(&ifd_entry);
}

void PrintExifInfo(ImageHeader *exif){
    printf("\n========pic Info============\n");
    printf("APP1:       %x\n", exif->SOI);
    printf("\n============================\n");

}
int main(int argc, char * argv[]){

    if(argc != 2) {
        printf("Invalid input.\n");
        exit(0);
    }
    
    ReadExifData(argv[1]);
    PrintExifInfo(&gExifInfo);
}