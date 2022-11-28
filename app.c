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
    uint8_t *filePath = "file4FAT16.IMA";
    //uint8_t *filePath = "floppy1.img";
    uint8_t *buff=NULL;
    uint32_t fisrtCluster=0U;
    uint16_t entryNumber=0U;
    uint8_t userOption=0U;
    /*linked list entry */
    FATFS_entryList_struct *node=NULL;
    FATFS_entryList_struct *current=NULL;
    /* init file pointer, fat table, boot sector */
    FATFS_init(filePath);
    while(1)
    {
        system("cls");
        /*read directory*/
        FATFS_readDirectory(fisrtCluster,&node);
        /*show direc*/
        entryNumber=show_entry_list(node);
        //printf("\nentryNumber%d",entryNumber);
        /*user choice */
        userOption=user_menu();
        current=node;
        if(0U==userOption)
        {
            exit(0);
        }
        else
        {
            while(userOption>1)
            {
                userOption--;
                current=current->next;
            }
            /*entry is folder 0x10*/
            if(current->Data.FATFS_EntryAttribute==0x10)
            {
                fisrtCluster=current->Data.FATFS_EntryFisrtCluster;
            }
            /*read file*/
            else
            {
                buff=(uint8_t*)malloc(current->Data.FATFS_EntryUpDateSize);

                if(buff!=NULL)
                {
                    FATFS_read_file(current->Data.FATFS_EntryFisrtCluster,buff);
                    show_file(buff,current->Data.FATFS_EntrySize);
                    free(buff);
                }
                else
                {
                    printf("\nAllocate memory Fail!!!");
                }
            }
        }
        printf("\n\nPress to continue...");
        getch();
    }
    FATFS_deinit(filePath,&node);
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
uint8_t show_entry_list(FATFS_entryList_struct *head)
{
    uint8_t entryCnt=0U;
    FATFS_entryList_struct *current=NULL;
    uint8_t index=0U;
    
    current=head;
    printf("\n---------------------------------------------------------------------------------------\n");
    printf("%-5s %-12s %-15s %-26s %-7s %s\n", "No.", "Name", "Type", "Date Modifine ", "Size","Long file name");
    printf("-----------------------------------------------------------------------------------------\n");
    while(current!=NULL)
    {
        entryCnt++;
        printf("%-5d",entryCnt);
        
        /*short file name*/
        for(index=0;index<8;index++)
        {
            printf("%c",current->Data.FATFS_EntryName[index]);
        }
        printf("%3s","   ");
        printf("%5s","     ");
        switch(current->Data.FATFS_EntryAttribute)
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
                    printf("%c",current->Data.FATFS_EntryExtension[index]);
                }
                printf("             ");
                break;
            default:
                printf("%-12s    ", "Unknown");
                break;
        }
        printf("%-3d/%-2d/%-2d   ",current->Data.FATFS_EntryDateModifine.DateModifine_Date,current->Data.FATFS_EntryDateModifine.DateModifine_Month,current->Data.FATFS_EntryDateModifine.DateModifine_Year);
        printf("%-3d:%-2d:%-5d   ",current->Data.FATFS_EntryTimeModifine.TimeModifine_Hour,current->Data.FATFS_EntryTimeModifine.TimeModifine_Minute,current->Data.FATFS_EntryTimeModifine.TimeModifine_Second);
        printf("%-7d",current->Data.FATFS_EntrySize);
        /*long file name*/
        printf("%s",current->Data.FATFS_EntryLongFileName);
        printf("\n");
        current=current->next;
    }
    
    return entryCnt;
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
 
