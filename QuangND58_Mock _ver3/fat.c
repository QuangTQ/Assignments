#include<stdio.h>
#include<stdint.h>
#include <stdlib.h>
#include <stdbool.h>
/*******************************************************************************
 * Include
 ******************************************************************************/
#include <string.h>
#include <math.h>
#include "HAL.h"
#include "fat.h"

/*******************************************************************************
 * Define
 ******************************************************************************/
/*convert 2 bytes to little endian*/
#define FATFS_CONVERT_2_BYTES(buff) (uint16_t)(((buff)[1] << 8) | (buff)[0])
/*convert 4 bytes to little endian*/
#define FATFS_CONVERT_4_BYTES(buff) (uint32_t)(((buff)[3] << 24) | (buff)[2]<<16 | (buff)[1]<<8 | (buff)[0])

/*FAT type size*/
#define FAT12_MAX_CLUSTER                                   (4085U)
#define FAT16_MAX_CLUSTER                                   (65526U)

/*Boot sector offset*/
#define BOOTREGION_BYTEPERSECTOR_OFFSET                     (0x0BU)
#define BOOTREGION_SECTORPERCLUSTER_OFFSET                  (0x0DU)
#define BOOTREGION_REVERSECTOR_OFFSET                       (0x0EU)
#define BOOTREGION_NUMBEROFFAT_OFFSET                       (0x10U)
#define BOOTREGION_NUMBERENTRYOFROOTDIREC_OFFSET            (0x11U)
#define BOOTREGION_TOTALSECTOR_OFFSET                       (0x13U)
#define BOOTREGION_NUMBERSECTOROFFAT_OFFSET                 (0x16U)
#define BOOTREGION_TYPEOFFATOFFSER                          (0x36U)

/*size entry default*/
#define FATFS_BYTES_PER_ENTRY                               (32U)

/*Root directory offset*/
#define FATFS_ENTRY_ATTRIBUTE_OFFSET                        (0X0BU)
#define FATFS_ENTRY_DELETE_OFFSET                           (0xE5)
#define FATFS_ENTRY_EMPTY                                   (0X00U)
#define FATFS_ENTRY_SFN_SIZE                                (8U)
#define FATFS_ENTRY_EXTENTION_SIZE                          (3U)
#define FATFS_ENTRY_LASTDATEMODIFY                          (0X18U)
#define FATFS_ENTRY_LASTTIMEMODIFY                          (0X16U)
#define FATFS_ENTRY_SIZE_OFFSET                             (0X1CU)
#define FATFS_ENTRY_FISRTCLUSTER                            (0x1AU)

/*Attributes long file name*/
#define FATFS_ENTRY_ATTRIBUTE_LONGFILENAME_OFFSET            (0X0FU)
#define FATFS_ENTRY_LFN_FISRT                                (0x01U)
#define FATFS_ENTRY_LFN_FISRT_SIZE                           (5U)
#define FATFS_ENTRY_LFN_NEXT                                 (0x0EU)
#define FATFS_ENTRY_LFN_NEXT_SIZE                            (6U)
#define FATFS_ENTRY_LFN_END                                  (0x1CU)
#define FATFS_ENTRY_LFN_END_SIZE                             (2U)

/*end of cluster */
#define FATFS_ENDOFCLUSTER_FAT12                              (0XFFFU)
#define FATFS_ENDOFCLUSTER_FAT16                              (0XFFFFU)
#define FATFS_ENDOFCLUSTER_FAT32                              (0X0FFFFFFFU)
/*******************************************************************************
* Prototypes
******************************************************************************/

/**
 * @brief: handle and save Boot sector infor
 * @return: None.
 */
static void FATFS_BootSector();

/**
 * @brief: read and convert Fat table
 * @return: None.
 */
static void FATFS_readFatTable_convert();

/*******************************************************************************
* Variables
******************************************************************************/
static FATFS_BPB_Struct s_bootData;                         /*Boot struct*/
static FATFS_DateModifine dateModifine;                     /*struct date modify*/
static FATFS_TimeModifine_struct timModifine;               /*struct time modify*/
static uint32_t startDataRegion=0U;                         /*sector start Data region*/
static uint16_t *s_fatTable=NULL;                           /*Fat table after convert*/
static uint8_t s_LongFileName[FATFS_LFNMAX_SIZE];           /*long file name*/
static bool s_fatfsTypesFAT12=false;                        /*use FAT 12*/
static bool s_fatfsTypesFAT16=false;                        /*use FAT 16*/
static bool s_fatfsTypesFAT32=false;                        /*use FAT 32*/
static uint32_t s_endOfCluster=0U;
/*******************************************************************************
* Code
******************************************************************************/
static void FATFS_BootSector()
{
    uint16_t BootSectorSizeDefault=512U;
    uint8_t count=0U;
    /*Allocate a buffer 512 bytes*/
    uint8_t *buff=(uint8_t*)malloc(BootSectorSizeDefault);
    /*read sector 0 save to buff*/
    HAL_ReadSector(0,buff);
    /*save information to s_bootData*/
    s_bootData.BPB_BytesPerSec=FATFS_CONVERT_2_BYTES(buff+BOOTREGION_BYTEPERSECTOR_OFFSET);
    s_bootData.BPB_SectorPerClus=buff[BOOTREGION_SECTORPERCLUSTER_OFFSET];
    s_bootData.BPB_RsvSector=FATFS_CONVERT_2_BYTES(buff+BOOTREGION_REVERSECTOR_OFFSET);
    s_bootData.BPB_NumberOfFAT=buff[BOOTREGION_NUMBEROFFAT_OFFSET];
    s_bootData.BPB_RootEntcnt=FATFS_CONVERT_2_BYTES(buff+BOOTREGION_NUMBERENTRYOFROOTDIREC_OFFSET);
    s_bootData.BPB_TotalSector=FATFS_CONVERT_2_BYTES(buff+BOOTREGION_TOTALSECTOR_OFFSET);
    s_bootData.BPB_SecPerFAT=FATFS_CONVERT_2_BYTES(buff+BOOTREGION_NUMBERSECTOROFFAT_OFFSET);
    s_bootData.BPB_StartRootSec=s_bootData.BPB_RsvSector+s_bootData.BPB_NumberOfFAT*s_bootData.BPB_SecPerFAT;
    
    for(count=0;count<FATFS_BPB_TYPEFAT_SIZE;count++)
    {
        s_bootData.BPB_TypeFAT[count]=buff[BOOTREGION_TYPEOFFATOFFSER+count];
    }
    /*check type FAT*/
     /*FAT12*/
    if(s_bootData.BPB_TotalSector<FAT12_MAX_CLUSTER)  
    {
        s_endOfCluster=FATFS_ENDOFCLUSTER_FAT12;
        s_fatfsTypesFAT12=true;
    }
     /*FAT16*/
    else if(s_bootData.BPB_TotalSector>FAT12_MAX_CLUSTER && s_bootData.BPB_TotalSector<FAT16_MAX_CLUSTER)
    {
        s_endOfCluster=FATFS_ENDOFCLUSTER_FAT16;
        s_fatfsTypesFAT16=true;
    }
    /*FAT32*/
    else
    {
        s_endOfCluster=FATFS_ENDOFCLUSTER_FAT32;
        s_fatfsTypesFAT32=true;
    }
    free(buff);
}

static void FATFS_readFatTable_convert()
{
    uint16_t offset = 0U;
    uint16_t fatValue=0U;
    uint8_t *buff=NULL;
    uint32_t sizeOfFat=0U;
    uint16_t index=0U;
    uint16_t indexCluster=0U;
    
    sizeOfFat=s_bootData.BPB_SecPerFAT*s_bootData.BPB_BytesPerSec;
    /*store FAT table */
    buff=(uint8_t*)malloc(sizeOfFat);
    HAL_ReadMultiSector(s_bootData.BPB_RsvSector,s_bootData.BPB_SecPerFAT,buff);
    if(s_fatfsTypesFAT12==true)
    {
        /*allocate FAT table*/
        s_fatTable=(uint16_t*)malloc(sizeOfFat*3/2);
        /*read 3 bytes once*/
        for(index=0;index<sizeOfFat;index+=3)
        {
            /*even*/
            s_fatTable[indexCluster]=(uint16_t)(buff[index] + (buff[index + 1] << 8) & 0xFFF);
            /*odd*/
            s_fatTable[indexCluster+1]=(uint16_t)((buff[index + 1] >> 4) + (buff[index + 2] << 4));
            indexCluster=indexCluster+2;
        }
    }
    else if(s_fatfsTypesFAT16==true)
    {
        /*allocate FAT table*/
        s_fatTable=(uint16_t*)malloc(sizeOfFat/2);
        /*read 2 bytes once*/
        for(index=0;index<sizeOfFat;index+=2)
        {
            s_fatTable[indexCluster]=FATFS_CONVERT_2_BYTES(buff+index);
            indexCluster++;
        }
    }
    else
    {
        /*FAT32*/
    }
}


void FATFS_read_file(uint32_t fisrtCluster,uint8_t *buff)
{
    uint32_t clusterPosition=0U;
    uint32_t sizeOfCluster=0U;
    uint16_t nextCluster=0U;
    uint32_t offset = 0U;
    
    sizeOfCluster=s_bootData.BPB_BytesPerSec *s_bootData.BPB_SectorPerClus;
    nextCluster=fisrtCluster;
       do{
           clusterPosition=(nextCluster-2)*s_bootData.BPB_SectorPerClus + startDataRegion;
           HAL_ReadMultiSector(clusterPosition,s_bootData.BPB_SectorPerClus,&buff[sizeOfCluster*offset]);
           nextCluster = s_fatTable[nextCluster];
           offset+=1;
        }while(nextCluster!=s_endOfCluster);
}

uint16_t FATFS_readDirectory(uint32_t fisrtCluster,FATFS_Entry_struct *entryList)
{
    FATFS_Entry_struct entryData;     /*struct store entry data*/
    uint8_t *buff=NULL;               /*buffer store Root Directory data and Sub Directory data*/
    uint16_t sectorPosition=0U;       /*sector position read */
    uint32_t sizeRoot=0U;             /*number sector of Root */
    uint16_t indexEntry=0U;           
    uint8_t index=0U;
    uint16_t entryNum=0U;             /*number of entry of Direct*/
    uint32_t dateValue=0U;            /*date value*/
    uint32_t timeValue=0U;            /*time value*/
    uint32_t sizeCluster=0U;          /*cluster size (bytes)*/
    uint16_t indexLFN=0U;             /*index use long file name*/
    
    sizeRoot=s_bootData.BPB_RootEntcnt*FATFS_BYTES_PER_ENTRY/s_bootData.BPB_BytesPerSec;
    sizeCluster=s_bootData.BPB_SectorPerClus*s_bootData.BPB_BytesPerSec;
    startDataRegion=s_bootData.BPB_StartRootSec+sizeRoot;
    
    if(0U==fisrtCluster)
    {
        /* jum to Root sector */
        sectorPosition=s_bootData.BPB_StartRootSec;

    }
//    else if(1U==fisrtCluster)
//    {
//       // fisrt cluster fail 
//    }
    else
    {
        sectorPosition=(fisrtCluster-2)*s_bootData.BPB_SectorPerClus + startDataRegion;
    }
    buff=(uint8_t*)malloc(sizeCluster);
    if(buff!=NULL)
    {
        if(sizeCluster==HAL_ReadMultiSector(sectorPosition,s_bootData.BPB_SectorPerClus,buff))
        {
            // check empty entry
            while(buff[indexEntry]!=FATFS_ENTRY_EMPTY)
            {
                /*get long file name*/
                if(buff[indexEntry+FATFS_ENTRY_ATTRIBUTE_OFFSET]==FATFS_ENTRY_ATTRIBUTE_LONGFILENAME_OFFSET)
                {
                    /*add 10bytes fisrt - store 5 character*/
                    for(index=0;index<FATFS_ENTRY_LFN_FISRT_SIZE;index++)
                    {
                       s_LongFileName[indexLFN]= buff[FATFS_ENTRY_LFN_FISRT+(index*2)];
                       indexLFN++;
                    }
                    /*add 11bytes next- store 6 character*/
                    for(index=0;index<FATFS_ENTRY_LFN_NEXT_SIZE;index++)
                    {
                        s_LongFileName[indexLFN]= buff[FATFS_ENTRY_LFN_NEXT+(index*2)];
                       indexLFN++;
                    }
                    /*add 4bytes end - store 2 charater*/
                    for(index=0;index<FATFS_ENTRY_LFN_END_SIZE;index++)
                    {
                        s_LongFileName[indexLFN]= buff[FATFS_ENTRY_LFN_END+(index*2)];
                       indexLFN++;
                    }
                }
                
                /* check attribute skip long file name(0x0F) and file or folder error(0xE5)*/
                else if((buff[indexEntry+FATFS_ENTRY_ATTRIBUTE_OFFSET]!=FATFS_ENTRY_ATTRIBUTE_LONGFILENAME_OFFSET)&&(buff[indexEntry]!=FATFS_ENTRY_DELETE_OFFSET))  
                {
                    if(indexLFN!=0U)                  /*get long file name*/
                    {
                        //strcpy(entryData.FATFS_EntryLongFileName,s_LongFileName);
                        for(index=0;index<indexLFN;index++)
                        {
                            entryData.FATFS_EntryLongFileName[index]=s_LongFileName[index];
                        }  
                        indexLFN=0U;
                    }
                    else            /*have not long file name*/
                    {
                        entryData.FATFS_EntryLongFileName[0]='\0';
                    }
                    for(index=0U;index<FATFS_ENTRY_SFN_SIZE;index++)
                    {
                        entryData.FATFS_EntryName[index]=buff[indexEntry+index];
                    }
                    /*get extention of entry*/
                    for(index=0u;index<FATFS_ENTRY_EXTENTION_SIZE;index++)
                    {
                        entryData.FATFS_EntryExtension[index]=buff[indexEntry+index+8];
                    }
                    /*get attribute */
                    entryData.FATFS_EntryAttribute=buff[indexEntry+FATFS_ENTRY_ATTRIBUTE_OFFSET];
            
                    /* get date modifine of entry */
                    dateValue = FATFS_CONVERT_2_BYTES(buff + indexEntry + FATFS_ENTRY_LASTDATEMODIFY);    
                    dateModifine.DateModifine_Year = (dateValue >> 9) + 1980;       /*so nam tinh tu 1980 chiem 6 bit cao cua dateValue*/
                    dateModifine.DateModifine_Month = (dateValue >> 5) & 0x0f;      /*so thang bit 8- bit 5*/
                    dateModifine.DateModifine_Date = dateValue & 0x1f;              /*so ngay bit 4 - bit 0*/
                    entryData.FATFS_EntryDateModifine = dateModifine;
                    
                    /* get time modify of entry */
                    timeValue = FATFS_CONVERT_2_BYTES(buff + indexEntry + FATFS_ENTRY_LASTTIMEMODIFY);    
                    timModifine.TimeModifine_Hour= timeValue >> 11;                 /*bit 15-bit 11*/  
                    timModifine.TimeModifine_Minute = (timeValue >> 5) & 0x3f;      /*bit 10 -bit 5*/
                    timModifine.TimeModifine_Second = timeValue & 0x1f;             /*bit 4- bit 0*/
                    entryData.FATFS_EntryTimeModifine = timModifine;
                    
                    /* get size of entry */
                    entryData.FATFS_EntrySize = FATFS_CONVERT_4_BYTES(buff + indexEntry + FATFS_ENTRY_SIZE_OFFSET);
    
                    /* get first cluster to entry */
                    entryData.FATFS_EntryFisrtCluster = FATFS_CONVERT_2_BYTES(buff + indexEntry + FATFS_ENTRY_FISRTCLUSTER);
                    entryData.FATFS_EntryUpDateSize=ceil(entryData.FATFS_EntrySize/sizeCluster+1) *sizeCluster;
                    
                    entryNum++;
                    /*push entrydata to list*/
                    entryList[entryNum]=entryData;
                }
                indexEntry+=FATFS_BYTES_PER_ENTRY;
            }
        }
    }
    free(buff);
    return entryNum;
}


FATFS_Error_type_t FATFS_init(const uint8_t *filePath)
{
    FATFS_Error_type_t status=0U;
    FATFS_Error_type_t retHAL=0U;
    /* */
    retHAL=HAL_init(filePath);
    if(retHAL==HAL_success)
    {
        /* take information from Boot*/
        FATFS_BootSector();
        FATFS_readFatTable_convert();
        status=FATFS_success;
    }
    else
    {
        status=FATFS_initFail;
    } 
    return status;
}

FATFS_Error_type_t FATFS_deinit(const uint8_t *filePath)
{
    FATFS_Error_type_t status=0U;
    FATFS_Error_type_t retHAL=0U;
    
    retHAL=HAL_deinit(filePath);
    if(retHAL==HAL_deinitFail)
    {
        status=FATFS_deinitFail;
    }
    else
    {
        status=FATFS_success;
    }
    return status;
}
/*******************************************************************************
 * End of file
 ******************************************************************************/
