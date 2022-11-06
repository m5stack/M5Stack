#ifndef __PROTOCOL__H
#define __PROTOCOL__H

#include "Arduino.h"

/*֡ͷ����*/
#define FRAME_HEADER_LENGTH 1U

/*֡β����(��У���)*/
#define FRAME_TAIL_LENGTH 1U

/*֡ͷ��ͬ�ֽ�*/
#define FRAME_HEAD_SAME_FA 0xFA

/*���ջ���������*/
#define RX_BUF_LENGTH 64U
#define RX_END_LENGTH 1U

typedef struct {
    uint8_t cmd_id;
    uint8_t ack_flag;
    uint8_t *cmd_value;
    uint16_t cmd_length;
} PayloadData;

typedef struct {
    uint8_t src;
    uint8_t dst;
    uint8_t seq;
    uint8_t res;
    PayloadData payload;
} CmdData;

/*����Э�鹫������*/
typedef struct {
    volatile uint8_t step;  /*switch �����ת����*/
    volatile int8_t tmpCnt; /*���ڼ�������ʱ����*/
    volatile uint8_t aRxBufIndex; /*�������ݻ���������*/
    volatile uint8_t aRxCrcIndex; /*��β���ݻ���������*/
    uint8_t aRxCrc[RX_END_LENGTH];
    uint8_t aRxBuf[RX_BUF_LENGTH];
} protocolComType_t;

extern void protocol_rec_put(uint8_t data);
extern void protocol_send_data(CmdData dat);

#endif