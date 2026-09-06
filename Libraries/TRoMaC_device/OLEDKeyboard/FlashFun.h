#ifndef __FLASH_FUN_H
#define __FLASH_FUN_H

#include "stm32f4xx_hal.h"
#include "OLEDkeyboard_Config.h"


/*===============宏定义===============*/

/*取一整个页(2KByte)作为键显的存储区域*/
/*将整个页分为16个逻辑扇区，每个扇区包含32个页，每个页的大小为4个字节（uint32_t）*/
/* 空间使用逻辑
    在键显进入页面后首先对备份扇区执行擦除
    之后将起始扇区的所有内容复制到备份扇区
    复制完成后擦除键显使用的页
    选择进入的逻辑扇区之后先将除目标扇区之外的逻辑扇区复制回键显扇区
    在输入参数的过程中将参数直接写入目标扇区
    
    若执行返回上一级的指令则从第一步开始重新执行一遍（改变目标扇区）
    
    最后键显输入程序执行完成后将备份扇区进行擦除

*/

/* Base address of the Flash sectors */ 
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base address of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base address of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base address of Sector 10, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base address of Sector 11, 128 Kbytes  */
#define ADDR_FLASH_MAX          ((uint32_t)0x08100000)

//键显及其备份扇区
#define FLASH_OLEDKBD_SECTOR            FLASH_SECTOR_11
#define FLASH_BACKUP_SECTOR             FLASH_OLEDKBD_SECTOR-1
#define FLASH_BASE_ADDR                 ADDR_FLASH_SECTOR_0

#define FLASH_Logical_Page_SIZE         ((uint32_t)(sizeof(uint32_t)))
#define FLASH_Logical_Page_NUM          64    
#define FLASH_Logical_Sector_SIZE       ((uint32_t)(FLASH_Logical_Page_SIZE * FLASH_Logical_Page_NUM))
#define FLASH_Logical_Sector_NUM        16    
#define FLASH_Logical_USED_SIZE         ((uint32_t)FLASH_Logical_Sector_SIZE * FLASH_Logical_Sector_NUM)

//#define Get_Flash_ADDR(SECTOR, sector, page)    ((uint32_t)(FLASH_BASE_ADDR + (FLASH_PAGE_SIZE * PAGE) + (FLASH_Logical_Sector_SIZE * sector) + (FLASH_Logical_Page_SIZE * page)))

#define flash_read(SECTOR, sector, page, type)    ((type)(Flash_Read_Word(SECTOR, sector, page)))

typedef enum 
{
    FAILED = 0, 
    PASSED = !FAILED
} TestStatus;
/*===============外部变量===============*/

/*===============函数声明===============*/
uint32_t Get_Flash_ADDR(uint32_t SECTOR, uint16_t sector, uint16_t page);
uint8_t  Flash_write_Word(uint32_t SECTOR, uint32_t sector, uint32_t page, __IO uint32_t Data);
uint32_t Flash_Read_Word(uint32_t SECTOR, uint32_t sector, uint32_t page);
uint8_t  Flash_CleanSECTOR(uint32_t SECTOR_begin, uint32_t SECTOR_end);
uint8_t  Flash_Sectorcpy(uint32_t Source_SECTOR, uint32_t Target_SECTOR, uint32_t sector);
uint8_t  Flash_SECTORcpy(uint32_t Source_PAGE, uint32_t Target_PAGE);
void     Flash_Data_Recover(void);





#endif /*__FLASH_FUN_H*/
