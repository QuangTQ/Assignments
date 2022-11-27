/*******************************************************************************
 * Include
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include "fat.h"
#include "app.h"

/*******************************************************************************
 *Code
 ******************************************************************************/
void mange_app()
{
    const uint8_t *filePath = "floppy.img";
    uint8_t *buff=NULL;
    uint32_t fisrtCluster=0U;
    uint16_t entryNumber=0U;
    uint8_t userOption=0U;
    /*mang chua cac entry */
    FATFS_Entry_struct entryList[224];
    /* init file pointer */
    FATFS_init(filePath);
    while(1)
    {
        system("cls"); /* Clear cmd screen */
        //read and show directory
        entryNumber=FATFS_readDirectory(fisrtCluster,entryList);
        /*show direc*/
        show_entry_list(entryList,entryNumber);
        /*user choice */
        userOption=user_menu();
        if(0U==userOption)
        {
            exit(0);
        }
        else
        {
            /*entry is folder 0x10*/
            if(entryList[userOption].FATFS_EntryAttribute==0x10)
            {
                fisrtCluster=entryList[userOption].FATFS_EntryFisrtCluster;
            }
            /*read file*/
            else
            {
                buff=(uint8_t*)malloc(entryList[userOption].FATFS_EntryUpDateSize);

                if(buff!=NULL)
                {
                    FATFS_read_file(entryList[userOption].FATFS_EntryFisrtCluster,buff);
                    show_file(buff,entryList[userOption].FATFS_EntrySize);
                    free(buff);
                }
                else
                {
                    printf("\nAllocate memory Fail!!!");
                }
                buff=NULL;
            }
        }
        printf("\n\nPress to continue...");
        getch();
    }
    FATFS_deinit(filePath);
}

uint8_t user_menu()
{
    int userOption=0;
    
    printf("\n--------------------------------------------------------------------\n");
    printf("\nEnter 0 to exit");
    printf("\nSelect No. to access folder or file");
    printf("\nUser option:");
    scanf("%d",&userOption);
    fflush(stdin);
    printf("\n--------------------------------------------------------------------\n");
    return userOption;
}
void show_entry_list(FATFS_Entry_struct *entryList,uint16_t entryNum)
{
    uint16_t indexEntry=0U;
    uint8_t index=0U;
    printf("\n-----------------------------------------------------------------\n");
    printf("%-5s %-12s %-15s %-26s %-4s\n", "No.", "Name", "Type", "Date Modifine ", "Size");
    printf("-----------------------------------------------------------------\n");
    for(indexEntry=1U;indexEntry<=entryNum;indexEntry++)
    {
        printf("%-5d",indexEntry);
        if(entryList[indexEntry].FATFS_EntryLongFileName[0]!='\0')
        {
            printf("%s",entryList[indexEntry].FATFS_EntryLongFileName);
        }
        else
        {
            for(index=0;index<8;index++)
            {
                printf("%c",entryList[indexEntry].FATFS_EntryName[index]);
            }
            printf("%3s","   ");
        }
        
        printf("%5s","     ");
        switch (entryList[indexEntry].FATFS_EntryAttribute)
        {
        case 0x01:
            printf("%-12s    ", "Read Only");
            break;
        case 0x02:
            printf("%-12s    ", "Hidden");
            break;
        case 0x04:
            printf("%-12s    ", "System");
            break;
        case 0x08:
            printf("%-12s    ", "Volume Label");
            break;
        case 0x10:
            printf("%-12s    ", "Folder");
            break;
        case 0x00:           // file
            for(index=0;index<3;index++)
            {
                printf("%c",entryList[indexEntry].FATFS_EntryExtension[index]);
            }
            printf("             ");
            break;
        default:
            printf("%-12s    ", "Unknown");
            break;
        }
        printf("%-3d/%-2d/%-2d   ",entryList[indexEntry].FATFS_EntryDateModifine.DateModifine_Date,entryList[indexEntry].FATFS_EntryDateModifine.DateModifine_Month,entryList[indexEntry].FATFS_EntryDateModifine.DateModifine_Year);
        printf("%-3d:%-2d:%-5d   ",entryList[indexEntry].FATFS_EntryTimeModifine.TimeModifine_Hour,entryList[indexEntry].FATFS_EntryTimeModifine.TimeModifine_Minute,entryList[indexEntry].FATFS_EntryTimeModifine.TimeModifine_Second);
        printf("%d",entryList[indexEntry].FATFS_EntrySize);
        printf("\n");
    }
}

void show_file(uint8_t *buff,uint32_t sizeBuff)
{
    uint32_t index = 0U;
    for (index; index < sizeBuff; index++)
    {
        printf("%c", buff[index]);
    }
}
/*******************************************************************************
 * End of file
 ******************************************************************************/
 
