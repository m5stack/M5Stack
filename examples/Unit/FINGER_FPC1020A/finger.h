#ifndef __TFS_M64_H
#define __TFS_M64_H
 
#define TRUE  1
#define FALSE 0
 
#define ACK_SUCCESS    0x00
#define ACK_FAIL       0x01
#define ACK_FULL       0x04
#define ACK_NOUSER     0x05
#define ACK_USER_EXIST 0x07
#define ACK_TIMEOUT    0x08
 
#define ACK_GO_OUT		 0x0F

#define ACK_ALL_USER     0x00
#define ACK_GUEST_USER 	 0x01
#define ACK_NORMAL_USER  0x02
#define ACK_MASTER_USER  0x03
 
#define USER_MAX_CNT	 50
 
#define HEAD 0
#define CMD  1
#define CHK  6
#define TAIL 7
 
#define P1   2
#define P2   3
#define P3   4
#define Q1   2
#define Q2   3
#define Q3   4

#define CMD_HEAD		  0xF5
#define CMD_TAIL		  0xF5
#define CMD_ADD_1  	      0x01
#define CMD_ADD_2 		  0x02
#define CMD_ADD_3	  	  0x03
#define CMD_MATCH		  0x0C
#define CMD_DEL			  0x04
#define CMD_DEL_ALL		  0x05
#define CMD_USER_CNT      0x09
#define CMD_SLEEP_MODE	  0x2C
#define CMD_ADD_MODE      0x2D
 
#define CMD_FINGER_DETECTED 0x14
 

class FingerPrint {

  public:
        FingerPrint(void);
        uint8_t fpm_sendAndReceive(uint16_t delayMs);
        uint8_t fpm_sleep(void);
        uint8_t fpm_setAddMode(uint8_t fpm_mode);
        uint8_t fpm_readAddMode(void);
        uint16_t fpm_getUserNum(void);
        uint8_t fpm_deleteAllUser(void);
        uint8_t fpm_deleteUser(uint8_t userNum);
        uint8_t fpm_addUser(uint8_t userNum, uint8_t userPermission);
        uint8_t fpm_compareFinger(void);
  public:
        uint8_t TxBuf[9];
        uint8_t RxBuf[9];
        uint8_t RxCnt;
  private:
    
  private:

};


#endif /* __TFS-M64_H */
 
 
 
