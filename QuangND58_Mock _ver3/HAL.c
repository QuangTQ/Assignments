/*******************************************************************************
 * Include
 ******************************************************************************/
#include<stdio.h>
#include<stdint.h>
#include "HAL.h" 
/*******************************************************************************
 * Variables
 ******************************************************************************/
static FILE *filePointer=NULL;
static sizeOfSec=DEFAULT_SECTOR_SIZE;
/*******************************************************************************
 * Code
 ******************************************************************************/
HAL_errorType HAL_init(const uint8_t *fPath)
{
    HAL_errorType status=0U;
    filePointer=fopen(fPath,"rb");
    
    if(filePointer==NULL)
    {
        status=HAL_initFail;
    }
    else
    {
        status=HAL_success;
    }
    
    return status;
}

int32_t HAL_ReadSector(uint32_t index,uint8_t *buff)
{
    uint32_t bytesRead=0;
    
    if(filePointer!=NULL)
    {
        if(0U==fseek(filePointer,index*sizeOfSec,SEEK_SET))
        {
            bytesRead=fread(buff,sizeof(uint8_t),sizeOfSec,filePointer);
        }
    }
    
    return bytesRead;/*return number of bytes read*/
}

int32_t HAL_ReadMultiSector(uint32_t index, uint32_t num,uint8_t *buff)
{
    uint32_t bytesRead=0;

    if(filePointer!=NULL)
    {
        if(0U==fseek(filePointer,index*sizeOfSec,SEEK_SET))
        {
            bytesRead=fread(buff,sizeof(uint8_t),num*sizeOfSec,filePointer);
        }
    }
    return bytesRead;
}

HAL_errorType HAL_deinit(const uint8_t *fPath)
{
    HAL_errorType status=0U;
    if(fPath==NULL)
    {
        status=HAL_deinitFail;
    }
    else
    {
        fclose(filePointer);
    }
    return status;
}
/*******************************************************************************
 * End of file
 ******************************************************************************/
 
