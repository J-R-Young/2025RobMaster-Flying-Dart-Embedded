#include "stm32f4xx_hal.h"
#include "OLEDkeyboard_Config.h"
#include "FlashFun.h"

uint32_t Get_Flash_ADDR(uint32_t SECTOR, uint16_t sector, uint16_t page)
{
    __IO uint32_t Address;
    
    if(  (SECTOR != FLASH_OLEDKBD_SECTOR && SECTOR != FLASH_BACKUP_SECTOR)
       || sector >= FLASH_Logical_Sector_NUM
       || page   >= FLASH_Logical_Page_NUM
      )
    {
        while(1)
        {
            OLED_Show_Numbers(0, 5, SECTOR, TextSize_F6x8);
            OLED_Show_Numbers(45, 5, sector, TextSize_F6x8);
            OLED_Show_Numbers(85, 5, page, TextSize_F6x8);
            
            OLED_Show_Str(0, 0, "FLASH_ERR", TextSize_F8x16);
            /*异常处理*/
//            Address = NULL;
        }
    }
    else
    {
        if(SECTOR == FLASH_OLEDKBD_SECTOR)
            Address = ((uint32_t)(ADDR_FLASH_SECTOR_11 + FLASH_Logical_Sector_SIZE * sector +  FLASH_Logical_Page_SIZE * page));
        else
            Address = ((uint32_t)(ADDR_FLASH_SECTOR_10 + FLASH_Logical_Sector_SIZE * sector +  FLASH_Logical_Page_SIZE * page));
    }
    
    return Address;
}


/**
  * @brief  根据输入的地址给出它所在的sector
  *					例如：
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address：地址
  * @retval 地址所在的sector
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t SECTOR = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    SECTOR = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    SECTOR = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    SECTOR = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    SECTOR = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    SECTOR = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    SECTOR = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    SECTOR = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    SECTOR = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    SECTOR = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    SECTOR = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    SECTOR = FLASH_SECTOR_10;  
  }
  else if((Address < ADDR_FLASH_MAX) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    SECTOR = FLASH_SECTOR_11;  
  }
  
  return SECTOR;
}


uint8_t Flash_write_Word(uint32_t SECTOR, uint32_t sector, uint32_t page, __IO uint32_t Data)
{
    uint32_t Address;		                    //记录写入的地址
	TestStatus MemoryProgramStatus = PASSED;    //记录测试结果
    
    /* 计算出要写入数据的地址 */
    Address = Get_Flash_ADDR(SECTOR, sector, page);
    
    /* 解锁 */
    HAL_FLASH_Unlock();
    
    /* 向内部FLASH写入数据 */
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data);
    
    /* 上锁 */
    HAL_FLASH_Lock();
  
    /* 检查写入的数据是否正确 */
    if((*(__IO uint32_t*) Address) != Data)
    {
        MemoryProgramStatus = FAILED;
        while(1)
        {
            OLED_Show_Str(0, 0, "Flash_write_Err", TextSize_F8x16);
        }
    }
    
    /* 返回写FLASH结果 */
	return MemoryProgramStatus;
}

uint32_t Flash_Read_Word(uint32_t SECTOR, uint32_t sector, uint32_t page)
{
    __IO uint32_t Address;		                    //记录要读取的地址
    
    /* 计算出要读取数据的地址 */
    Address = Get_Flash_ADDR(SECTOR, sector, page);
    
    return (*(__IO uint32_t*) Address);
}

///*************************************************************
//**Function: Flash_CleanSECTOR
//**Description: 内部FLASH擦除指定的页
//**Others: 正常擦除结束返回HAL_OK   擦除出现问题返回HAL_ERROR
//************************************************************/
uint8_t Flash_CleanSECTOR(uint32_t SECTOR_begin, uint32_t SECTOR_end)
{
    __IO uint32_t Address;
    
    HAL_StatusTypeDef FLASHStatus = HAL_OK;  //记录擦除的结果	
    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SECTOR_Error;

    if(SECTOR_begin > SECTOR_end)
    {
        return HAL_ERROR;
    }
    if(SECTOR_end > FLASH_SECTOR_11)
    {
        return HAL_ERROR;
    }
    
    /* 解锁 */
    HAL_FLASH_Unlock();
    
    /* Fill EraseInit structure */
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector = SECTOR_begin;
    EraseInitStruct.NbSectors = SECTOR_end - SECTOR_begin +1;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_4;
    
	/* 开始擦除操作 */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTOR_Error) != HAL_OK)
	{
		/*擦除出错，返回，实际应用中可加入处理 */
		return HAL_ERROR;
	}
    
    /* 上锁 */
    HAL_FLASH_Lock();
    
    return HAL_OK;
}


///*************************************************************
//**Function: Flash_Sectorcpy
//**Description: 将内部FLASH中指定的页一个逻辑扇区中的数据复制到另一个指定页的相同编号的逻辑扇区内
//**Others: 正常完成返回 PASSED (1)   异常返回 FAILED (0)
//************************************************************/
uint8_t Flash_Sectorcpy(uint32_t Source_SECTOR, uint32_t Target_SECTOR, uint32_t sector)
{
    
    uint32_t SourceAddress, TargetAddress, EndAddress;
	TestStatus MemoryProgramStatus = PASSED;    //记录测试结果
    HAL_StatusTypeDef FLASHStatus = HAL_OK; //记录每次写入的结果
    
    /* 计算出要读写数据的地址 */
    SourceAddress = Get_Flash_ADDR(Source_SECTOR, sector, 0);
    TargetAddress = Get_Flash_ADDR(Target_SECTOR, sector, 0);
    EndAddress    = SourceAddress + FLASH_Logical_Sector_SIZE;
    
    /* 解锁 */
    HAL_FLASH_Unlock();
    
    /* 向内部FLASH写入数据 */
    while((SourceAddress < EndAddress) && (FLASHStatus == HAL_OK))
    {   
        FLASHStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, TargetAddress, (*(__IO uint32_t*) SourceAddress));
        SourceAddress += sizeof(uint32_t);
        TargetAddress += sizeof(uint32_t);
    }
    
    /* 上锁 */
    HAL_FLASH_Lock();
  
    /* 检查写入的数据是否正确 */
    SourceAddress = Get_Flash_ADDR(Source_SECTOR, sector, 0);
    TargetAddress = Get_Flash_ADDR(Target_SECTOR, sector, 0);
    EndAddress    = SourceAddress + FLASH_Logical_Sector_SIZE;
    
    while((SourceAddress < EndAddress) && (MemoryProgramStatus != FAILED))
    {
        if((*(__IO uint32_t*) TargetAddress) != (*(__IO uint32_t*) SourceAddress))
        {
            MemoryProgramStatus = FAILED;
        }
        SourceAddress += sizeof(uint32_t);
        TargetAddress += sizeof(uint32_t);
    }
    
    /* 返回写FLASH结果 */
	return MemoryProgramStatus;
}




///*************************************************************
//**Function: FLASH_SECTORcpy
//**Description: 将内部FLASH中指定的页中的数据复制到另一个指定页内
//**Others: 正常完成返回 PASSED (1)   异常返回 FAILED (0)
//************************************************************/
uint8_t Flash_SECTORcpy(uint32_t Source_SECTOR, uint32_t Target_SECTOR)
{
    uint8_t i;
    
    for(i = 0; i < FLASH_Logical_Sector_NUM ; i++)
    {
        if(Flash_Sectorcpy(Source_SECTOR, Target_SECTOR, i) == FAILED)
        {
            break;
        }
    }
    
    if(i < 16)  
    {
        return FAILED;
    }
    else
    {
        return PASSED;
    }
}


/*************************************************************************
*  函数名称：void Flash_Data_Recover(unsigned char desSector, unsigned char backupSector)
*  功能说明：按复位键后将数据从备份扇区恢复
* @param   desSector     ：  目的扇区   范围  0-11
* @param   backupSector  ：  扇区   范围  0-11, 默认11
*  函数返回：TRUE:恢复成功   FLASE:扇区不匹配
*  修改时间：2020年6月6日
*           2022年11月24日     //在键显初始化完成后先进行数据救援，全盘复制
*************************************************************************/
void Flash_Data_Recover(void)
{
    uint32_t sectorFlag = flash_read(FLASH_BACKUP_SECTOR, 0, 0, uint32_t);
    uint32_t finishFlag = flash_read(FLASH_BACKUP_SECTOR, 0, 1, uint32_t);

    if(finishFlag != sectorFlag) //flash数据没有输入结束
    {
        Flash_SECTORcpy(FLASH_BACKUP_SECTOR, FLASH_OLEDKBD_SECTOR);                                     //将数据恢复到原扇区
    }
}














