#include "protocol.h"

CmdData cmd_save;

static protocolComType_t pUartHandle;

__attribute__((weak)) void protocol_callback(CmdData cmd);
__attribute__((weak)) void protocol_data_write(uint8_t data);

static uint8_t CheckSumCal(uint8_t *pData, uint32_t num) {
  if (pData == NULL) { return 0x00; }
  if (num == 0) { return 0x00; }
  uint8_t crc_data = 0x00;

  for (uint32_t i = 0; i < num; i++) {
    /*仅保留低位*/
    crc_data += 0xff & pData[i];
  }
  return crc_data;
}

static void InitPtcStruct() {
  pUartHandle.step = 0;
  pUartHandle.tmpCnt = 0;
  pUartHandle.aRxBufIndex = 0;
  pUartHandle.aRxCrcIndex = 0;
}

void protocol_rec_put(uint8_t data) {
  switch (pUartHandle.step) {
  case 0:
    if (data == FRAME_HEAD_SAME_FA) { /*帧头正确*/
      pUartHandle.step++;/*跳转下一步骤*/
      pUartHandle.aRxBuf[pUartHandle.aRxBufIndex++] = data;
    }
    break;
  case 1:
    if (data == FRAME_HEAD_SAME_FA) {
      pUartHandle.step = 1;/*第一帧头重复，回到第二帧头判断处,AA 55 AA 55的情况*/
      pUartHandle.aRxBufIndex = 1;/*更新第二帧头*/
    }
    else {
      pUartHandle.tmpCnt = data + 1;/*临时计数值*/
      pUartHandle.step++;/*跳转下一步骤*/
      pUartHandle.aRxBuf[pUartHandle.aRxBufIndex++] = data;/*压入缓冲区*/
      if (((RX_BUF_LENGTH - pUartHandle.aRxBufIndex) < data) || (data == 0)) {/*缓冲区溢出或数据长度为 0*/
        InitPtcStruct();/*初始化结构体值,准备下一次接收*/
      }
    }
    break;
  case 2:
    if ((--pUartHandle.tmpCnt) > 0) {/*接收数据到缓冲区*/
      pUartHandle.aRxBuf[pUartHandle.aRxBufIndex++] = data;
      if (pUartHandle.aRxBufIndex >= RX_BUF_LENGTH) {/*长度被意外修改，导致缓冲区溢出*/
        InitPtcStruct();/*初始化结构体值,准备下一次接收*/
      }
    }
    else {
      if (pUartHandle.aRxBufIndex > 7 && CheckSumCal(&pUartHandle.aRxBuf[0], pUartHandle.aRxBufIndex + 1) == data) {
        cmd_save.src = pUartHandle.aRxBuf[2];
        cmd_save.dst = pUartHandle.aRxBuf[3];
        cmd_save.seq = pUartHandle.aRxBuf[4];
        cmd_save.res = pUartHandle.aRxBuf[5];
        cmd_save.payload.cmd_id = pUartHandle.aRxBuf[6];
        cmd_save.payload.ack_flag = pUartHandle.aRxBuf[7];
        cmd_save.payload.cmd_length = pUartHandle.aRxBufIndex - 8;
        cmd_save.payload.cmd_value = (uint8_t *)malloc(sizeof(uint8_t) * cmd_save.payload.cmd_length);
        for (int i = 0; i < cmd_save.payload.cmd_length; i++) {
          cmd_save.payload.cmd_value[i] = pUartHandle.aRxBuf[8 + i];
        }
        protocol_callback(cmd_save);
        free(cmd_save.payload.cmd_value);
      }
      InitPtcStruct(); /*初始化结构体值,准备下一次接收*/
    }
    break;

  default:
    InitPtcStruct();/*初始化结构体值,准备下一次接收*/
    break;
  }
}

void protocol_send_data(CmdData data) {
  protocolComType_t pUartHandle;
  pUartHandle.aRxBufIndex = 0;
  pUartHandle.aRxBuf[pUartHandle.aRxBufIndex++] = FRAME_HEAD_SAME_FA;

  pUartHandle.aRxBuf[pUartHandle.aRxBufIndex++] = data.payload.cmd_length + 6;
  uint8_t *ptx = (uint8_t *)&data;
  for (int i = 0; i < 6; i++) {
    pUartHandle.aRxBuf[pUartHandle.aRxBufIndex++] = ptx[i];
  }

  if (data.payload.cmd_length) {
    ptx = data.payload.cmd_value;
    for (int i = 0; i < data.payload.cmd_length; i++) {
      pUartHandle.aRxBuf[pUartHandle.aRxBufIndex++] = ptx[i];
    }
  }

  uint8_t check = CheckSumCal(&pUartHandle.aRxBuf[0], 8 + data.payload.cmd_length);
  pUartHandle.aRxBuf[pUartHandle.aRxBufIndex++] = check;

  for (int i = 0; i < pUartHandle.aRxBufIndex; i++) {
    protocol_data_write(pUartHandle.aRxBuf[i]);
  }
  InitPtcStruct();
}