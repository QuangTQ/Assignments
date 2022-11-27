#ifndef H_HAL
#define H_HAL
/*******************************************************************************
 * Define
 ******************************************************************************/
#define DEFAULT_SECTOR_SIZE 512U

typedef enum {
    HAL_success=0x00,   
    HAL_initFail=0x01,
    HAL_deinitFail=0x02
}HAL_errorType;
/*******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief: Open file
 * @param fPath: path to file 
 * @return: status
 */
HAL_errorType HAL_init(const uint8_t *fPath);

/**
 * @brief: Read a sector
 * @param index: Position read
 * @param buff: array to save data after read 
 * @return: return number of bytes readed.
 */
int32_t HAL_ReadSector(uint32_t index,uint8_t *buff);
/**
 * @brief: Read multi sector
 * @param index: Position read
 * @param num: number of sector read
 * @param buff: array to save data after read 
 * @return: return number of bytes readed.
 */
int32_t HAL_ReadMultiSector(uint32_t index,uint32_t num,uint8_t *buff);

/**
 * @brief: Close file
 * @param fPath: path to file 
 * @return: status
 */
HAL_errorType HAL_deinit(const uint8_t*fPath);
#endif /*H_HAL*/
/*******************************************************************************
 * End of file
 ******************************************************************************/
