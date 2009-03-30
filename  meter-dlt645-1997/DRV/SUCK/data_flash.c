#include "Pub_PUCK.h"

//INT8U DataflashModelFlag = 0;
#pragma optimize=no_code_motion

//PUCK ADD:09-02-03
void Init_Bus_PUCK(void)
{
  DIR_FLASH_SI=0;
  DIR_FLASH_SCL=0;
  DIR_FLASH_CS=0;
  
  DIR_FLASH_SO=1;
  DIR_FLASH_RDY=1;
  
  DATA_FLASH_SI=1;
  DATA_FLASH_SCL=1;
  DATA_FLASH_CS=1;
}


wp_enable(void)
{
  ;
}
wp_disable(void)
{
  ;
}

void DFClock( INT8U cnt )
{
    DATA_FLASH_SI_OUT &= ((INT8U)(~DATA_FLASH_SI ));//volatile
     // DATA_FLASH_SI_OUT = (~(DATA_FLASH_SI ));
    do{
        DATA_FLASH_SCL_OUT = ~( DATA_FLASH_SCL );
        DATA_FLASH_SCL_OUT = ~( DATA_FLASH_SCL );
    }while( --cnt );
}

void DataflashWriteByte( INT8U wrbyte )
{
    INT8U BitCounter = 8;

    do{
        DATA_FLASH_SCL_OUT =~ ( DATA_FLASH_SCL );
        
        if( wrbyte & 0x80 ) 
        {   
            DATA_FLASH_SI_OUT = 1;
        }
        else 
        {
            DATA_FLASH_SI_OUT = 0;
        }
        
        DATA_FLASH_SCL_OUT =~ ( DATA_FLASH_SCL );
        wrbyte <<= 1;
    }while( --BitCounter );
}

INT8U DataflashReadByte(void)
{
    INT8U rdbyte = 0, BitCounter = 8;

    do{
        DATA_FLASH_SCL_OUT =~ ( DATA_FLASH_SCL );
        rdbyte <<= 1;        
        
        
        if( DATA_FLASH_SO_IN & DATA_FLASH_SO ) 
        {
            rdbyte++;
        }
        
        DATA_FLASH_SCL_OUT =~ ( DATA_FLASH_SCL );
    }while( --BitCounter );
    
    return( rdbyte );
}

const INT8U _df_cmd_mode[] =
{
    DF_ENTER_SLEEP,
    DF_ENTER_ACTIVE
};

void DataflashEnterMode(INT8U sel)
{
    DATA_FLASH_CS_OUT =0;
    DataflashWriteByte( _df_cmd_mode[sel] );
    DATA_FLASH_CS_OUT =1;
    WAITFOR_DRV_CYCLE_TIMEOUT(200);	
}

const INT8U _df_cmd_sta[]=
{
    DF_READ_STATUS1,
    DF_READ_STATUS2
};

/**********************************************************************************
PUCK
函数功能：通过读flash 的寄存器，获取Ready状态
入口：无
出口：1-------ready；0----------busy
注意：超限100次
**********************************************************************************/	
INT8U DataflashStatusRead( void )//F=1MHz;第5位到2位永远是1011
{
    INT8U sta,times;
    sta=times=0;
    WAITFOR_DRV_CYCLE_TIMEOUT(200);
    do{
        DATA_FLASH_CS_OUT =0;
        DataflashWriteByte( _df_cmd_sta[0] );
        
        sta = DataflashReadByte();
        DATA_FLASH_CS_OUT |= DATA_FLASH_CS;
        DATA_FLASH_SI_OUT |= DATA_FLASH_SI;
        DATA_FLASH_SCL_OUT |= DATA_FLASH_SCL;
       
        if( sta & 0x80 ) 
        {
                DATA_FLASH_CS_OUT =~ ( DATA_FLASH_CS );
                return 1;		  		
         }
        
    }while( times++ < 100 );
	
    //AlarmCtlWord1 |= ALARM_OF_DATA_FLASH;
    //DataflashModelFlag |= DATA_FLASH_MODEL_ERR;	
    DATA_FLASH_CS_OUT =1;
    return 0;
      
}

const INT8U _df_countinus_read[]=
{
    DF_READ_CMD_ARRAY
  //DF_ARRAY_READ
};

void Dataflash_continus_Read(INT32U addr, INT8U *data, INT16U len )
{   
  INT32U page, AdrInPage,temp_addr;
  DATA_FLASH_CS_OUT =0;
  //处理地址
  page = (addr & 0x00ffffff) / PAGE_SIZE;
  AdrInPage = (addr & 0x00ffffff) % PAGE_SIZE;
  temp_addr=((INT32U)((page<<10)+AdrInPage));
  
  DataflashWriteByte( _df_countinus_read[0]);    
  DataflashWriteByte(( INT8U )(( temp_addr&0x00ff0000) >> 16 ));
  DataflashWriteByte(( INT8U )(( temp_addr&0x0000ff00) >> 8 ));
  DataflashWriteByte(( INT8U )(( temp_addr&0x000000ff) >> 0 ));
  DFClock(8);
  DFClock(8);
  DFClock(8);   
  DFClock(8);
  do{
      *data = DataflashReadByte();
      data++;
  }while( --len );
  
  DATA_FLASH_CS_OUT |= DATA_FLASH_CS;
  DATA_FLASH_SI_OUT |= DATA_FLASH_SI;
  DATA_FLASH_SCL_OUT = 1;
  DATA_FLASH_CS_OUT =1;
}

const INT8U _df_cmd_rbuff[]=
{
    DF_READ_BUFFER1,
    DF_READ_BUFFER2
};

void DataflashBufferRead( INT8U buff, INT16U addr, INT8U *data, INT16U len )
{
    DATA_FLASH_CS_OUT =0;
    DataflashWriteByte( _df_cmd_rbuff[0] );
    DFClock(8);
    DataflashWriteByte( ( INT8U )( addr >> 8 ) );
    DataflashWriteByte( ( INT8U )addr );
    DFClock(8);
    do{
        *data = DataflashReadByte();
        data++;
    }while( --len );
    
    DATA_FLASH_CS_OUT = 1;
    DATA_FLASH_SI_OUT |= DATA_FLASH_SI;
    DATA_FLASH_SCL_OUT |=DATA_FLASH_SCL_OUT;
}


const INT8U _df_cmd_wbuff[]=
{
    DF_WRITE_BUFFER1,
    DF_WRITE_BUFFER2
};



INT8U DataflashBufferWrite(  INT16U addr, INT8U *data, INT16U len )
{
    DATA_FLASH_CS_OUT &=~ ( DATA_FLASH_CS );
    DataflashWriteByte(_df_cmd_wbuff[0]);
    DFClock(8);
    DataflashWriteByte( ( INT8U )( (addr&0x0300) >> 8 ) );
    DataflashWriteByte( ( INT8U )(addr&0x00ff ));
    do{
        DataflashWriteByte( *data );
        data++;
    }while( --len );
    //DATA_FLASH_CS_OUT |= DATA_FLASH_CS;
    DATA_FLASH_CS_OUT=1;
    DATA_FLASH_SI_OUT |= DATA_FLASH_SI;
    DATA_FLASH_SCL_OUT |= DATA_FLASH_SCL;
    WAITFOR_DRV_CYCLE_TIMEOUT(200);
    return DataflashStatusRead( );
     
    
}

const INT8U _df_cmd_btm[]=
{
    DF_WMM_WBIE_BUF1,
    DF_WMM_WBIE_BUF2
};

INT8U DataflashBufferToMainProg( INT16U page )
{
    INT8U times = 0;
    
    page=(page<<2);
    wp_enable();
    DATA_FLASH_CS_OUT =~ ( DATA_FLASH_CS );
    DataflashWriteByte( _df_cmd_btm[0] );
   
    
    DataflashWriteByte(( INT8U )(( page&0x3f00) >> 8 ));
    DataflashWriteByte(( INT8U )(( page&0x00ff) >> 0 ));
    
    
    
    DFClock(8);
    
    DATA_FLASH_CS_OUT=1;
    while(DataflashStatusRead()==0)
    {times++;
      if(times>=100) 
      {
       wp_disable();
       return 0;
      }
    }
    
     wp_disable();
     return 1;
 
}

const INT8U _df_cmd_mtb[]=
{
    DF_MAIN_TO_BUFFER1,
    DF_MAIN_TO_BUFFER2
};

INT8U DataflashMainToBuffer( INT16U page )
{
    INT8U times = 0;
    page <<= 2;
    DATA_FLASH_CS_OUT &=~ ( DATA_FLASH_CS );
    DataflashWriteByte( _df_cmd_mtb[0] );
    DataflashWriteByte( (INT8U)( page >> 8 ) );
    DataflashWriteByte( (INT8U)page );
    DFClock(8);
    DATA_FLASH_CS_OUT |= DATA_FLASH_CS;
    DATA_FLASH_SI_OUT |= DATA_FLASH_SI;
    DATA_FLASH_SCL_OUT |= DATA_FLASH_SCL;
    
    while(DataflashStatusRead()==0)
    {
      times++;
      if(times>=100) 
      {
       
       return 0;     
      }
    } 
     
     return 1;
}    


/*
*********************************************************************************************************
* Description: This function is read flash abs
*
* Arguments  : the absolute addr(0~)
*              the buffer for coming data
*              the size of data
*
* Note(s)    :
*********************************************************************************************************
*/

INT8U flash_read_abs(INT32U addr_abs,INT8U *buf,INT32U size)
{ INT8U times = 0;
  
    Init_Bus_PUCK();
   while(DataflashStatusRead()==0)    //获取flash的ready状态，1----ready，0---busy---PUCK
    {times++;
      if(times>=100) 
       return 0;
    }  
  Dataflash_continus_Read(addr_abs,buf,size );
  return 1;
}

PartialPageWrite(INT8U *src,INT32U dest,INT32U size)
{
	INT32U page, AdrInPage;
	INT8U times;
       // INT8U test_temp[PAGE_SIZE];
	
	page = (dest & 0x00ffffff) / PAGE_SIZE;
	AdrInPage = (dest & 0x00ffffff) % PAGE_SIZE;
        //total_size += size;

	 while(DataflashStatusRead()==0)
            {times++;
            if(times>=100) 
                {
       
                   return 0;     
                }
            }

	// Read the contents of the page in the Sram Buffer
        if(DataflashMainToBuffer(page )!=1)   
	return 0;

	//DataflashBufferRead( 0, 0, test_temp, PAGE_SIZE);
        
        if (DataflashBufferWrite(  AdrInPage, src, size )!=1)
	return 0;
        
	
        //DataflashBufferRead( 0, 0, test_temp, PAGE_SIZE);
          
          
	// Rewrite the modified Sram Buffer in the main memory
         if(DataflashBufferToMainProg(page )!=1)  
         return 0;
         
	return 1;
}

INT8U DataFlashWrite(INT32U dest,INT8U *src,INT32U size)
{
        INT32U length;
	INT32U page;
        INT32U all_space;
	
	
    dest &= 0x00ffffff;
    
		
    //if ( (dest + size) > (INT32U)(PAGE_SIZE * PAGE_NUMBER))
    //all_space=(INT32U)(PAGE_SIZE * PAGE_NUMBER);
    all_space=0x210000;
	if ( (dest + size) > all_space)
          return 0;

    	//start address 
    if ((dest % ((INT32U)(PAGE_SIZE)))  != 0 )
     {
		length = PAGE_SIZE - (dest % (PAGE_SIZE));

		if (size < length)
			length = size;

		if(PartialPageWrite(src, dest, length)!= 1)	
		
		{
			return 0;
		}		

		/* Update size, source and destination pointers */
        	size -= length;
        	dest += length;
        	src += length;
    }

    all_space=size - PAGE_SIZE;
    while ((INT32S)( size - PAGE_SIZE ) >= 0 )
    {
		/* program dataflash page */		
		page = (INT32U)dest / PAGE_SIZE;

                if (DataflashBufferWrite(  0, src, PAGE_SIZE )!=1)
		return 0;
                
                if(DataflashBufferToMainProg(page )!=1)  
                return 0;
                
		
		/* Update size, source and destination pointers */
	   	size -= PAGE_SIZE ;
	   	dest += PAGE_SIZE ;
	   	src  += PAGE_SIZE ;
    }

    /* If still some bytes to read */
    if ( size >0 ) {
		/* program dataflash page */
      
		if(PartialPageWrite(src, dest, size)!= 1)	
		
		{
			return 0;
		}	
	}
		//OSMutexPost(sem_spi_flash_swk);
    return 1;
}
  
  

/*
*********************************************************************************************************
* Description: This function is read flash abs
*
* Arguments  : the absolute addr(0~8M)
*              the buffer for sending data
*              the size of data
*
* Note(s)    :
*********************************************************************************************************
*/

INT8U flash_writ_abs_sub(INT32U addr_abs,INT8U *buf,INT32U size)
{
  
  return (DataFlashWrite(addr_abs,buf,size));

}

INT8U flash_writ_abs(INT32U addr_abs,INT8U *buf,INT32U size)
{  
  Init_Bus_PUCK();
  return flash_writ_abs_sub(addr_abs,buf,size);
}
