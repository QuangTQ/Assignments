/*******************************************************************************
 * Define
 ******************************************************************************/
#ifndef H_FAT
#define H_FAT

/*size element boot sector*/
#define FATFS_BPB_JUMBOOT_SIZE       (3U)
#define FATFS_BPB_OEMN_SIZE          (8U)
#define FATFS_BPB_TYPEFAT_SIZE       (8U)

/*size element entry */
#define FATFS_LFNMAX_SIZE            (255U)
#define FATFS_SFN_SIZE               (8U)
#define FATFS_EXTENTION_SIZE         (3U)

/*error return type */
typedef enum{
    FATFS_success=0x00,
    FATFS_initFail=0x01,
    FATFS_readFileFail=0x02,
    FATFS_deinitFail=0x03,
    FATFS_fisrtClusterFail=0x04
}FATFS_Error_type_t;

/*Boot Sector */
typedef struct {
    uint8_t BPB_JumBoot[FATFS_BPB_JUMBOOT_SIZE];/*Jum instruction*/ 
    uint8_t BPB_OEMN[FATFS_BPB_OEMN_SIZE];      /*OEM name*/                           
    uint16_t BPB_BytesPerSec;                   /*Bytes per logical Sector*/
    uint8_t BPB_SectorPerClus;                  /*Logical Sector per Cluster*/
    uint16_t BPB_RsvSector;                     /*Reserver logical Sector boot+resever */
    uint16_t BPB_SecPerTrack;                   /*Sector of Track*/
    uint16_t BPB_NumHeads;                      /*number of head read*/
    uint32_t BPB_HiddSec;                        /*hidden sector*/
    uint8_t BPB_DriNumber;                      /*physical drive number*/
    uint16_t BPB_Signature;                     /*Boot sector signature*/
    uint8_t BPB_NumberOfFAT;                    /*Number of FAT*/
    uint16_t BPB_RootEntcnt;                    /*entry number of Root*/
    uint16_t BPB_SecPerFAT;                     /*Sector per FAT*/
    uint8_t BPB_Media;                          /*Media description*/
    uint32_t BPB_StartRootSec;                  /*start Root Sector */ 
    uint8_t BPB_TypeFAT[FATFS_BPB_TYPEFAT_SIZE];/*Type of FAT*/
    uint16_t BPB_TotalSector;                   /*Total sector on imange*/
    uint32_t BPB_sizeVolume;                    /*Total sector if BPB_TotalSector=0*/
    uint16_t BPB_EndOfFile;                     /*End of boot sector*/
}FATFS_BPB_Struct;

/*struct of time modifine*/
typedef struct {
    uint16_t TimeModifine_Second; 
    uint16_t TimeModifine_Minute;
    uint16_t TimeModifine_Hour;
}FATFS_TimeModifine_struct;

/*struct of date modifine*/
typedef struct {
    uint16_t DateModifine_Date;
    uint16_t DateModifine_Month;
    uint16_t DateModifine_Year;
}FATFS_DateModifine;

/*struct of entry information data*/
typedef struct { 
    uint8_t FATFS_EntryName[FATFS_SFN_SIZE];            /*Short file name*/
    uint8_t FATFS_EntryAttribute;                       /*Status attributes*/
    uint8_t FATFS_EntryExtension[FATFS_EXTENTION_SIZE]; /*Extention name*/
    FATFS_TimeModifine_struct FATFS_EntryTimeModifine;  /*Time modifine*/
    FATFS_DateModifine FATFS_EntryDateModifine;         /*Date modifine*/
    uint8_t FATFS_EntryLongFileName[FATFS_LFNMAX_SIZE]; /*Long file name*/
    uint16_t FATFS_EntryFisrtCluster;                   /*Fisrt cluster*/
    uint32_t FATFS_EntrySize;                           /*Entry size*/
    uint32_t FATFS_EntryUpDateSize;                     
}FATFS_Entry_struct;

typedef struct FATFS_entryList_struct
{
    FATFS_Entry_struct Data;
    struct FATFS_entryList_struct *next;
}FATFS_entryList_struct;

/*******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief:  FAT init
 * @param filePath: file path 
 * @return: return error type
 */
FATFS_Error_type_t FATFS_init(uint8_t *filePath);
/**
 * @brief: read directory
 * @param fisrtCluster: fisrt Cluster
 * @param head: head of list
 * @return: return error type
 */
FATFS_Error_type_t FATFS_readDirectory(uint32_t fisrtCluster,FATFS_entryList_struct **head);

/**
 * @brief: read file
 * @param fisrtCluster: fisrt Cluster
 * @param buff: buff array
 * @return: none
 */
void FATFS_read_file(uint32_t fisrtCluster,uint8_t *buff);

/**
 * @brief: deinit FAT
 * @param filePath: file path 
 * @param head: head of list
 * @return: return error type
 */
FATFS_Error_type_t FATFS_deinit(uint8_t *filePath,FATFS_entryList_struct **head);
#endif /*H_FATDriver*/
/*******************************************************************************
 * End of file
 ******************************************************************************/
 
