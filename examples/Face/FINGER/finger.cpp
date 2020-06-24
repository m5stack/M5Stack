#include <M5Stack.h>
#include "finger.h"


FingerPrint::FingerPrint(void){

}

FingerPrint FP;

#define F_Time 10000

uint8_t FingerPrint::fpm_sendAndReceive(uint16_t timeout)
{
    uint8_t  i, j;
    uint8_t checkSum = 0;
	 
    FP.RxCnt = 0;
	FP.TxBuf[5] = 0;
  
    Serial2.write(CMD_HEAD);	   
    for (i = 1; i < 6; i++)       
    {
        Serial2.write(FP.TxBuf[i]);		 
        checkSum ^= FP.TxBuf[i];
    }
    Serial2.write(checkSum);    
    Serial2.write(CMD_TAIL);        

    while ((!Serial2.available()) && timeout > 0)         
    {
        delay(1);
        timeout--;		   
    }
    
    uint8_t ch;
    for(i=0;i<8;i++)
    {
        if(Serial2.available()){
            ch = Serial2.read();
            FP.RxCnt++;
            FP.RxBuf[i] = ch;
        }
      }

    if (FP.RxCnt != 8) {FP.RxCnt = 0;return ACK_TIMEOUT;}
    if (FP.RxBuf[HEAD] != CMD_HEAD) return ACK_FAIL;
    if (FP.RxBuf[TAIL] != CMD_TAIL) return ACK_FAIL;
    if (FP.RxBuf[CMD] != (FP.TxBuf[CMD])) return ACK_FAIL;
    
    checkSum = 0;
    for (j = 1; j < CHK; j++) {   
        checkSum ^= FP.RxBuf[j];
    }
        if (checkSum != FP.RxBuf[CHK]) {
        return ACK_FAIL;
    }
        return ACK_SUCCESS;
}	 

uint8_t FingerPrint::fpm_sleep(void)
{
    uint8_t res;
    
    FP.TxBuf[CMD] = CMD_SLEEP_MODE;
    FP.TxBuf[P1] = 0;
    FP.TxBuf[P2] = 0;
    FP.TxBuf[P3] = 0;
    
    res = fpm_sendAndReceive(500);
    
    if(res == ACK_SUCCESS) {
        return ACK_SUCCESS;
    }
    else {
        return ACK_FAIL;
    }
  
}

uint8_t FingerPrint::fpm_setAddMode(uint8_t fpm_mode)
{
    uint8_t res;
    
    FP.TxBuf[CMD] = CMD_ADD_MODE;
    FP.TxBuf[P1] = 0;
    FP.TxBuf[P2] = fpm_mode;
    FP.TxBuf[P3] = 0;
    
    res = fpm_sendAndReceive(200);
    
    if(res == ACK_SUCCESS && RxBuf[Q3] == ACK_SUCCESS) {
        return ACK_SUCCESS;
    }
    else {
        return ACK_FAIL;
    }
}

uint8_t FingerPrint::fpm_readAddMode(void)
{ 
    FP.TxBuf[CMD] = CMD_ADD_MODE;
    FP.TxBuf[P1] = 0;
    FP.TxBuf[P2] = 0;
    FP.TxBuf[P3] = 0X01;
    
    fpm_sendAndReceive(200);
    
    return RxBuf[Q2];
}

uint16_t FingerPrint::fpm_getUserNum(void)
{
    uint8_t res;
    
    FP.TxBuf[CMD] = CMD_USER_CNT;    
    FP.TxBuf[P1] = 0;
    FP.TxBuf[P2] = 0;
    FP.TxBuf[P3] = 0;
    
    res = fpm_sendAndReceive(200);
    
    if(res == ACK_SUCCESS && FP.RxBuf[Q3] == ACK_SUCCESS) {                   
        return FP.RxBuf[Q2];
    }
    else {
        return 0XFF;
    }
  
}

uint8_t FingerPrint::fpm_deleteAllUser(void)
{
    uint8_t res;
    
    FP.TxBuf[CMD] = CMD_DEL_ALL;
    FP.TxBuf[P1] = 0;
    FP.TxBuf[P2] = 0;
    FP.TxBuf[P3] = 0;
    
    res = fpm_sendAndReceive(200);
    
    if(res == ACK_SUCCESS && RxBuf[Q3] == ACK_SUCCESS) {
        return ACK_SUCCESS;
    }
    else {
        return ACK_FAIL;
    }
}

uint8_t FingerPrint::fpm_deleteUser(uint8_t userNum)
{
    uint8_t res;
    
    FP.TxBuf[CMD] = CMD_DEL;
    FP.TxBuf[P1] = 0;
    FP.TxBuf[P2] = userNum;
    FP.TxBuf[P3] = 0;
    
    res = fpm_sendAndReceive(200);
    
    if(res == ACK_SUCCESS && RxBuf[Q3] == ACK_SUCCESS) {
        return ACK_SUCCESS;
    }
    else {
        return ACK_FAIL;
    }
}

uint8_t FingerPrint::fpm_addUser(uint8_t userNum, uint8_t userPermission)
{
    uint8_t res;
    
    FP.TxBuf[CMD] = CMD_ADD_1;
    FP.TxBuf[P1] = 0;
    FP.TxBuf[P2] = userNum;
    FP.TxBuf[P3] = userPermission;
    
    res = fpm_sendAndReceive(F_Time);
    
    if(res == ACK_SUCCESS) {
        if(FP.RxBuf[Q3] == ACK_SUCCESS) {
        FP.TxBuf[CMD] = CMD_ADD_2;
        
        res = fpm_sendAndReceive(F_Time);
        
        if(res == ACK_SUCCESS) {
            if(FP.RxBuf[Q3] == ACK_SUCCESS) {
            FP.TxBuf[CMD] = CMD_ADD_3;
            
            res = fpm_sendAndReceive(F_Time);
            
            if(res == ACK_SUCCESS) {
                return FP.RxBuf[Q3];
            }  
            }
        }
        }
    }
    return res;
 
}

uint8_t FingerPrint::fpm_compareFinger(void)
{
    uint8_t res;
    
    FP.TxBuf[CMD] = CMD_MATCH;
    FP.TxBuf[P1] = 0;
    FP.TxBuf[P2] = 0;
    FP.TxBuf[P3] = 0;
    
    res = fpm_sendAndReceive(8000);
    
    if(res == ACK_SUCCESS) 
    {
        if(FP.RxBuf[Q3] == ACK_NOUSER) {
        return ACK_NOUSER;
        }
        if(FP.RxBuf[Q3] == ACK_TIMEOUT) {
        return ACK_TIMEOUT;
        }
        if((FP.RxBuf[Q2] != 0) && (FP.RxBuf[Q3] == 1 || FP.RxBuf[Q3] == 2 || FP.RxBuf[Q3] == 3)) {
        return ACK_SUCCESS;
        }
    }
    return res;
}
 
 
