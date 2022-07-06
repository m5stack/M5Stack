#pragma once
#ifndef __MODBUS__H_
#define __MODBUS__H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Arduino.h"

#define MB_ID 0xAA

/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN 4   /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX 256 /*!< Maximum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC 2   /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF 0   /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF  1   /*!< Offset of Modbus-PDU in Ser-PDU. */

#define MB_ADDRESS_BROADCAST (0)   /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN       (1)   /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX       (247) /*! Biggest possible slave address. */

#define MB_FUNC_NONE                         (0)
#define MB_FUNC_READ_COILS                   (1)
#define MB_FUNC_READ_DISCRETE_INPUTS         (2)
#define MB_FUNC_WRITE_SINGLE_COIL            (5)
#define MB_FUNC_WRITE_MULTIPLE_COILS         (15)
#define MB_FUNC_READ_HOLDING_REGISTER        (3)
#define MB_FUNC_READ_INPUT_REGISTER          (4)
#define MB_FUNC_WRITE_REGISTER               (6)
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS     (16)
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS (23)
#define MB_FUNC_DIAG_READ_EXCEPTION          (7)
#define MB_FUNC_DIAG_DIAGNOSTIC              (8)
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT       (11)
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG       (12)
#define MB_FUNC_OTHER_REPORT_SLAVEID         (17)
#define MB_FUNC_ERROR                        (128)

enum MB_FC {
    MB_FC_NONE       = 0, /*!< null operator */
    MB_FC_READ_COILS = 1, /*!< FCT=1 -> read coils or digital outputs */
    MB_FC_READ_DISCRETE_INPUT = 2, /*!< FCT=2 -> read digital inputs */
    MB_FC_READ_REGISTERS = 3, /*!< FCT=3 -> read registers or analog outputs */
    MB_FC_READ_INPUT_REGISTER = 4, /*!< FCT=4 -> read analog inputs */
    MB_FC_WRITE_COIL          = 5, /*!< FCT=5 -> write single coil or output */
    MB_FC_WRITE_REGISTER      = 6, /*!< FCT=6 -> write single register */
    MB_FC_WRITE_MULTIPLE_COILS =
        15, /*!< FCT=15 -> write multiple coils or outputs */
    MB_FC_WRITE_MULTIPLE_REGISTERS =
        16 /*!< FCT=16 -> write multiple registers */
};

enum MB_STATUS {
    MB_OK            = 0,
    MB_FRAME         = -1,
    MB_BUFF_OVERFLOW = -2,
    MB_CRC_ERROR     = -3,
};

typedef enum {
    MB_ENOERR,    /*!< no error. */
    MB_ENOREG,    /*!< illegal register address. */
    MB_EINVAL,    /*!< illegal argument. */
    MB_EPORTERR,  /*!< porting layer error. */
    MB_ENORES,    /*!< insufficient resources. */
    MB_EIO,       /*!< I/O error. */
    MB_EILLSTATE, /*!< protocol stack in illegal state. */
    MB_ETIMEDOUT  /*!< timeout error occurred. */
} eMBErrorCode;

void mb_put_rec_data(uint8_t data);
void mb_init(uint8_t id, uint32_t boudrate, volatile uint32_t *time_base);
void mb_poll();
void mb_send_frame(uint8_t *frame, uint16_t length);

__attribute__((weak)) void mb_send_one_byte(uint8_t data);
__attribute__((weak)) void mb_get_frame_cb(uint8_t rcvAddress, uint8_t *frame,
                                           uint16_t length);
__attribute__((weak)) void mb_get_frame_error_cb(eMBErrorCode);

#ifdef __cplusplus
}
#endif

#endif