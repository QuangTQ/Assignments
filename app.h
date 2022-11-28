/*******************************************************************************
 * Define
 ******************************************************************************/
#ifndef H_APP
#define H_APP

/*******************************************************************************
 * API
 ******************************************************************************/
 /**
 * @brief: Main function mange application
 * @return: none
 */
void mange_app();

/**
 * @brief: User menu option  
 * @return: return user select.
 */
uint8_t user_menu();

/**
 * @brief: Show entry list
 * @param head: head of entry list 
 * @return: return number of entry
 */
uint8_t show_entry_list(FATFS_entryList_struct *head);

/**
 * @brief: Show file
 * @param buff: buffer store file
 * @param sizeBuff: size of buff  
 * @return: none
 */
void show_file(uint8_t *buff,uint32_t sizeBuff);

#endif/*H_APP*/
/*******************************************************************************
 * End of file
 ******************************************************************************/
 
