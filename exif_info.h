#ifndef __EXIF_INFO_H__
#define __EXIF_INFO_H__

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
}ImageHeader;
#pragma pack ()


int TIFF_IFD_TYPE_MEMBER_BYTES[]={1, 1, 2, 4, 8, 1, 1, 2, 4, 8, 4, 8};

#endif