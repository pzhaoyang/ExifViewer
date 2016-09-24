#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "exif_info.h"
#include "exif_tool.h"
#include "exif_tagdb.h"
 
typedef struct{
    unsigned int num;
    unsigned int denom;
} FRACT;

typedef struct{
    signed int num;
    signed int denom;
} SFRACT;

static ImageHeader gExifInfo;
static char* ExposureProgram[]={
    "Not Defined",
    "Manual",
    "Normal",
    "Av",
    "Tv",
    "Creative",
    "Action",
    "Portrait",
    "Landscape"
};
static char* SensitivityType[]={
     "Unknow",
     "Standard out sensitivity",
     "Recommended exposure index",
     "ISO speed",
     "Standard output sensitivity and Recommended exposure index",
     "Standard output sensitivity and ISO speed",
     "Recommended exposure index and ISO speed",
     "Standard output sensitivity and recommended exposure index and ISO speed"
};

static char* ComponentsConfiguration[]={
    "",
    "Y",
    "Cb",
    "Cr",
    "R",
    "G",
    "B"
};



void printExifs_new(TIFF_IFD_ENTRY *ifd_entry, FILE* fp);


void checkImageHeader(){
    if(gExifInfo.Exif.Marker != 0xE1FF){
        EXIF_ERROR("This program just support Exif image File Marker");
    }
    
    if(gExifInfo.Exif.ifh.ByteOrder[0] != 0x49 && gExifInfo.Exif.ifh.ByteOrder[1] != 0x49){
        EXIF_ERROR("This program just support Little Endian file.");
    }
    
    if(gExifInfo.Exif.ifh.FurtherID != 0x2A){
        EXIF_ERROR("This is a not vaild tiff tag.");
    }

    if( gExifInfo.Exif.ExifStr[0] != 0x45 
      &&gExifInfo.Exif.ExifStr[1] != 0x78 
      &&gExifInfo.Exif.ExifStr[2] != 0x69 
      &&gExifInfo.Exif.ExifStr[3] != 0x66){
        EXIF_ERROR("This is not a valid exif file");
    }
}

void printExifTags(FILE* fp){
		TIFF_IFD_ENTRY ifd_entry;

    fread(&ifd_entry.Count, 2, 1, fp);
    ifd_entry.pIFD = (TIFF_IFD*)malloc(sizeof(TIFF_IFD)*ifd_entry.Count);
    memset(ifd_entry.pIFD, 0x00, sizeof(TIFF_IFD)*ifd_entry.Count);
    fread(ifd_entry.pIFD, sizeof(TIFF_IFD)*ifd_entry.Count, 1, fp);
    printExifs_new(&ifd_entry, fp);
    
    free(ifd_entry.pIFD);
}


void printExifs_new(TIFF_IFD_ENTRY *ifd_entry, FILE* fp){
    int index = 0;
    
    printf("----exif counts =%d-----\n", ifd_entry->Count);
    for(index = 0; index < ifd_entry->Count; index++){
        char ValueString[1024] = {0};
        int strlen = ifd_entry->pIFD[index].CountItems;
        TIFF_IFD_TYPE type = ifd_entry->pIFD[index].Type;
        int bytes = strlen * type;
        
        if(ifd_entry->pIFD[index].TagID == 0x927c){
        	  continue;
        }
        
        if(bytes > sizeof(ifd_entry->pIFD[index].valueOffset)){
            fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
            fread(ValueString, strlen-1,1,fp);
            ValueString[strlen] = '\0';
            printf("%s: %s\n", getTagName(ifd_entry->pIFD[index].TagID), ValueString);
        }else{
		      	if(ifd_entry->pIFD[index].valueOffset == 0x00){
		      		 printf("TagID=0x%04d,TagName=%s, TagType=%d, CountItems=%d\n", 
		      		 ifd_entry->pIFD[index].TagID, 
		      		 getTagName(ifd_entry->pIFD[index].TagID),
		      		 ifd_entry->pIFD[index].Type,
		      		 ifd_entry->pIFD[index].CountItems
		      		 );
		      		 continue;
		        }
        	//printf("%s: %d\n", getTagName(ifd_entry->pIFD[index].TagID), ifd_entry->pIFD[index].valueOffset);
        }
        //printf("index = %d tagid = 0x%04x\n",index, ifd_entry->pIFD[index].TagID);
        if(ifd_entry->pIFD[index].TagID == 0x8769){
            fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
            
						printExifTags(fp);
        }
    }
}

void printExifs(TIFF_IFD_ENTRY *ifd_entry, FILE* fp){
    int index = 0;
    
    printf("TagID\tType\tName\tCount\n");
    for(index = 0; index < ifd_entry->Count; index++){
        char ValueString[1024] = {0};
        int strlen = ifd_entry->pIFD[index].CountItems;
        
        switch(ifd_entry->pIFD[index].TagID){
        /*
          case 0x010f:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                
                printf("Manufacturer: %x\n",ifd_entry->pIFD[index].valueOffset);
                printf("Manufacturer: %s\n",ValueString);
                break;
          case 0x0110:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("Modle: %s\n",ValueString);
                break;
          case 0x0112:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("Orientation NULL: %s\n",ValueString);
                break;
          case 0x011a:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("XResolution NULL: %s\n",ValueString);
                break;
          case 0x011b:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("YResolution  NULL: %s\n",ValueString);
                break;
          case 0x0128:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("ResolutionUnit   NULL: %s\n",ValueString);
                break;
          case 0x0131:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("Software: 0x%lx\n",ifd_entry->pIFD[index].valueOffset);
                printf("Software: %s\n",ValueString);
                break;
          case 0x0132:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("DateTime: %s\n",ValueString);
                break;
          case 0x013b:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("Artist NULL: %s\n",ValueString);
                break;
          case 0x0213:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("YCbCrPositioning NULL: %s\n",ValueString);
                break;
          case 0x8298:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("Copyright: %s\n",ValueString);
                break;
          case 0x8769:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                free(ifd_entry->pIFD);
                memset(ifd_entry, 0x00, sizeof(TIFF_IFD)*ifd_entry->Count);
                
                fread(&ifd_entry->Count, 2, 1, fp);
                ifd_entry->pIFD = (TIFF_IFD*)malloc(sizeof(TIFF_IFD)*ifd_entry->Count);
                memset(ifd_entry->pIFD, 0x00, sizeof(TIFF_IFD)*ifd_entry->Count);
                fread(ifd_entry->pIFD, sizeof(TIFF_IFD)*ifd_entry->Count, 1, fp);
                printExifs(ifd_entry, fp);
                break;
          case 0x829a:{
                FRACT exposureSpeed;
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(&exposureSpeed, sizeof(FRACT), 1, fp);
                printf("Exposure Speed: %d/%d second\n", exposureSpeed.num, exposureSpeed.denom);
          }
                break;
          case 0x829d:{
                double toDisplay;
                FRACT fStop;
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(&fStop, sizeof(FRACT), 1, fp);
                toDisplay = ((float)fStop.num)/((float)fStop.denom);
                printf("F-Stop: f/%.1f\n", toDisplay);
          }
                break;
          case 0x8822:{
                unsigned short exp;
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(&exp, 2, 1, fp);
                printf("ExposureProgram: %s\n", exp <= 9 ? ExposureProgram[exp] : "Unknow");
          }
                break;
          case 0x8827:{
                unsigned short iso;
                iso = ifd_entry->pIFD[index].valueOffset;
                printf("ISO: %d\n", iso);
          }
                break;
          case 0x8830:{
               unsigned short st;
               st = ifd_entry->pIFD[index].valueOffset;
               printf("SensitivityType: %s\n", st <= 7 ? SensitivityType[st] : "Unknow");
          }
               break;
          case 0x8832:{
               unsigned short iso;
               iso = ifd_entry->pIFD[index].valueOffset;
               printf("RecommendedExposureIndex: %d\n", iso);
          }
               break;
          case 0x9000:{
                unsigned char exif_version[5] = {0};
                
                memcpy(exif_version, &ifd_entry->pIFD[index].valueOffset, 
                    sizeof(ifd_entry->pIFD[index].valueOffset));
                printf("ExifVersion: %s\n", exif_version);
          }
                break;
          case 0x9003:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("DataTimeOriginal: %s\n",ValueString);
                break;
          case 0x9004:
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(ValueString, strlen-1,1,fp);
                ValueString[strlen] = '\0';
                printf("DataTimeDigitized: %s\n",ValueString);
                break;
          case 0x9101:{
                unsigned char cc[4] = {0};
              
                memcpy(cc, &ifd_entry->pIFD[index].valueOffset, 
                    sizeof(ifd_entry->pIFD[index].valueOffset));
                printf("ComponentsConfiguration: %s%s%s%s\n",
                  ComponentsConfiguration[cc[0]],
                  ComponentsConfiguration[cc[1]],
                  ComponentsConfiguration[cc[2]],
                  ComponentsConfiguration[cc[3]]);
          }
                break;
          case 0x9201:{
                SFRACT Tv = {0};
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(&Tv, sizeof(SFRACT), 1, fp);
                printf("ShutterSpeedValue: ((APEX))%d/%d sec\n", Tv.num, Tv.denom);
          }
                break;
          case 0x9202:{
                FRACT Av = {0};
                fseek(fp, ifd_entry->pIFD[index].valueOffset + 12, SEEK_SET);
                fread(&Av, sizeof(FRACT), 1, fp);
                printf("ApertureValue: (APEX)%.1f\n", ((float)Av.num)/((float)Av.denom));
          }
                break;
          */
          default:
             //printf("The tagid is: 0x%04x, offset= 0x%04x\n", ifd_entry->pIFD[index].TagID,ifd_entry->pIFD[index].valueOffset);
             printf("0x%04x\t%02d\t%s\t%02d\n", ifd_entry->pIFD[index].TagID,ifd_entry->pIFD[index].Type, "Name", ifd_entry->pIFD[index].CountItems);
             break;
        }
    }
    
}
void ReadExifData(char* imagefile){
    FILE *fp = NULL;
    TIFF_IFD_ENTRY ifd_entry;
    
    fp = LoadImage(imagefile);
    memset(&gExifInfo, 0, sizeof(gExifInfo));
    fread(&gExifInfo, sizeof(gExifInfo), 1, fp);
    checkImageHeader();
    
    if(gExifInfo.Exif.ifh.offset != 0x00000008){
       EXIF_ERROR("The 0th IFD is not followed immediately.");
    }
    
    printExifTags(fp);
    /*fread(&ifd_entry.Count, 2, 1, fp);
    ifd_entry.pIFD = (TIFF_IFD*)malloc(sizeof(TIFF_IFD)*ifd_entry.Count);
    memset(ifd_entry.pIFD, 0x00, sizeof(TIFF_IFD)*ifd_entry.Count);
    fread(ifd_entry.pIFD, sizeof(TIFF_IFD)*ifd_entry.Count, 1, fp);
    printExifs_new(&ifd_entry, fp);*/
}

int main(int argc, char * argv[]){
    if(argc != 2) {
        EXIF_ERROR("Invalid input.");
    }
    
    ReadExifData(argv[1]);
}