#ifndef _DRFZIGBEE_H_
#define _DRFZIGBEE_H_

#include "Arduino.h"
#include "byteArray.h"
#include <initializer_list>
#include <map>

#define ZIGBEE_DEBUG

#define ZIGBEE_CMD_LINKMODULE \
    { 0x04, 0x44, 0x54, 0x4b, 0x52, 0x46 }
#define ZIGBEE_CMD_READPARM \
    { 0x0E, 0x44, 0x54, 0x4b, 0x52, 0x46 }
#define ZIGBEE_CMD_GETNODERSSI \
    { 0x0c, 0x44, 0x54, 0x4b, 0x52, 0x46 }

class DRFZigbee {
   public:
    typedef struct zigbee_arg {
        union {
            struct {
                uint8_t main_pointType;
                uint16_t main_PANID;
                uint8_t main_channel;
                uint8_t main_transmissionMode;
                uint16_t main_customID;
                uint16_t main_res0;
                uint8_t main_uartBaud;
                uint8_t main_uartBit;
                uint8_t main_uatrtStop;
                uint8_t main_uartCheck;
                uint16_t main_res1;
                uint8_t main_ATN;
                uint8_t main_mac[8];

                uint8_t preset_pointType;
                uint16_t preset_PANID;
                uint8_t preset_channel;
                uint8_t preset_transmissionMode;
                uint16_t preset_customID;
                uint16_t preset_res0;
                uint8_t preset_uartBaud;
                uint8_t preset_uartBit;
                uint8_t preset_uatrtStop;
                uint8_t preset_uartCheck;
                uint16_t preset_res1;
                uint8_t preset_ATN;

                uint16_t shortAddr;
                uint8_t res3;
                uint8_t encryption;
                uint8_t password[4];
            } __attribute__((packed));
            uint8_t Wave[48];
        };
        zigbee_arg()
            : main_res0(0xbbaa),
              main_res1(0xa605),
              preset_res0(0xddcc),
              preset_res1(0xa605),
              res3(0x01) {
        }
    } zigbee_arg_t;

    typedef struct reviceData {
        size_t length;
        uint16_t addr;
        byteArray *array;
        uint16_t fromAddr;

    } reviceData_t;

    typedef struct nodeRSSI {
        uint8_t routerLevel;
        uint16_t shortAddr;
        uint8_t rssi;
    } nodeRSSI_t;

    typedef struct node {
        union {
            struct {
                uint16_t shortAddr;
                uint8_t cnt;
                uint8_t routerLevel;
                uint8_t type;
                uint16_t parentAddr;
                uint8_t rssi;
            } __attribute__((packed));
            uint8_t Wave[11];
        };
        node(uint16_t __shortAddr, uint8_t __cnt, uint8_t __routerLevel,
             uint8_t __type, uint16_t __parentAddr, uint8_t __rssi)
            : shortAddr(__shortAddr),
              cnt(__cnt),
              routerLevel(__routerLevel),
              type{__type},
              parentAddr(__parentAddr),
              rssi(__rssi) {
        }
    } node_t;

   private:
    HardwareSerial *_uartp = nullptr;

   public:
    DRFZigbee(/* args */) {
    }
    ~DRFZigbee() {
    }
    void begin(HardwareSerial &uart) {
        _uartp = &uart;
    }

    void sendData(uint8_t cmd, const std::initializer_list<uint8_t> args);

    int sendCMDAndWaitRevice(uint8_t cmd, byteArray &array,
                             byteArray *reviceArray = nullptr,
                             size_t timeout         = 1000);
    int sendCMDAndWaitRevice(uint8_t cmd,
                             const std::initializer_list<uint8_t> args,
                             byteArray *reviceArray = nullptr,
                             size_t timeout         = 1000);

    int sendDataP2P(uint8_t mode, uint16_t addr, uint8_t *dataptr,
                    size_t length);
    int sendDataP2P(uint8_t mode, uint16_t addr, byteArray &array);
    int sendDataP2P(uint8_t mode, uint16_t addr,
                    const std::initializer_list<uint8_t> args);

    int getNetworksTopology();

    int linkMoudle();
    int rebootModule();
    int readModuleparm(zigbee_arg_t *parm);
    int setModuleparm(zigbee_arg_t &parm);

    int8_t getModuleRSSI(nodeRSSI_t *nodeRSSIPtr = nullptr);

    int reviceData(reviceData_t *revice, uint8_t type = kP2PCustomIDMode,
                   size_t timeout = 1000);

    template <typename T>
    static T swap(const T &arg) {
        T reArg;
        int size = sizeof(arg);
        if (size <= 1) {
            reArg = arg;
            return reArg;
        }
        uint8_t *cvi = (uint8_t *)&arg;
        uint8_t *cri = (uint8_t *)&reArg;
        cri          = cri + size - 1;
        for (int i = 0; i < size; i++) {
            *cri = *cvi;
            ++cvi;
            --cri;
        }
        return reArg;
    }

   public:
    int lastErrorcode = kReviceOK;

    enum errorlist {
        kReviceOK         = 0,
        kTimeoutError     = -1,
        kLengthError      = -2,
        kCheckSumError    = -3,
        kPramFormatError  = -4,
        kPointerisnullptr = -5,
    };

    enum {
        kCoordinator = 1,
        kRouter      = 2,
        kEndDevice   = 3,
    };

    enum {
        kTransparent     = 1,
        kCustom          = 2,
        kShortAddr       = 3,
        kMACAddr         = 4,
        kN2Ntransmission = 5,
    };

    enum {
        kBand_1200 = 1,
        kBand_2400,
        kBand_4800,
        kBand_9600,
        kBand_19200,
        kBand_38400,
        kBand_57600,
        kBand_115200,
    };

    enum {
        kANTOB = 0,
        kANTEXP,
    };

    std::map<int, node> nodeList;

    enum {
        kP2PShortAddrMode = 0xfd,
        kP2PCustomIDMode  = 0xed,
    };
};

#endif

/******
 * {
  "ProductKey": "a1vvwhzDkfc",
  "DeviceName": "LED_Device",
  "DeviceSecret": "df2eebb356c4ec0318260455892291d9"
}
**/
