#include "DRFZigbee.h"

void DRFZigbee::sendData(uint8_t cmd, const std::initializer_list<uint8_t> args)
{
    byteArray sendArray;
    sendArray += cmd;
    sendArray += args.size();
    sendArray += byteArray(args.begin(),args.size());


    uint8_t sum = 0;
    for (int i = 0; i < sendArray.size(); i++) sum += sendArray.at(i);
    sendArray += sum;
#ifdef ZIGBEE_DEBUG
    Serial.print("[SEND]");
    for (int i = 0; i < sendArray.size(); i++)
    {
        Serial.printf("%02X ", sendArray.at(i));
    }
    Serial.println(" ");
#endif
    _uartp->write(sendArray.dataptr(),sendArray.size());
}

int DRFZigbee::sendCMDAndWaitRevice(uint8_t cmd, const std::initializer_list<uint8_t> args, byteArray *reviceArray, size_t timeout)
{
    byteArray sendArray(args.begin(),args.size());
    return sendCMDAndWaitRevice(cmd,sendArray,reviceArray,timeout);
}

int DRFZigbee::sendCMDAndWaitRevice(uint8_t cmd, byteArray &array, byteArray *reviceArray, size_t timeout)
{
    byteArray sendArray;
    sendArray += cmd;
    sendArray += array.size();
    sendArray += array;

    uint8_t sum = 0x00;
    for (int i = 0; i < sendArray.size(); i++) sum += sendArray.at(i);
    sendArray += sum;

#ifdef ZIGBEE_DEBUG
    Serial.print("[SEND]");sendArray.printfArray<HardwareSerial>(&Serial);
#endif
    _uartp->flush();
    _uartp->write(sendArray.dataptr(),sendArray.size());
    
    uint8_t  reviceBuff[256];

    _uartp->setTimeout(timeout);

    if(!_uartp->find(0xfa)){ return lastErrorcode = kTimeoutError; }

    size_t length = _uartp->available();
    _uartp->readBytes(reviceBuff,length);

#ifdef ZIGBEE_DEBUG
    Serial.print("[REVC]");byteArray(reviceBuff,length).printfArray<HardwareSerial>(&Serial);
#endif

    if( reviceBuff[0] != ( length - 3 )){ return lastErrorcode = kLengthError; }

    uint8_t checkSum = 0xfa;
    for (int i = 0; i < length - 1; i++)  checkSum += reviceBuff[i];

    if( checkSum != reviceBuff[length - 1]){ return lastErrorcode = kCheckSumError; }
    
    if( reviceArray != nullptr ) *reviceArray = byteArray(&reviceBuff[1],length - 2);

    return kReviceOK;
}

int DRFZigbee::sendDataP2P(uint8_t mode,uint16_t addr,uint8_t *dataptr,size_t length)
{
    byteArray sendArray(dataptr,length);
    return sendDataP2P(mode,addr,sendArray);
}

int DRFZigbee::sendDataP2P(uint8_t mode,uint16_t addr,byteArray &array)
{
    byteArray sendArray;
    sendArray += mode;
    sendArray += array.size();
    sendArray += addr >> 8;
    sendArray += addr & 0x00ff;
    sendArray += array;

#ifdef ZIGBEE_DEBUG
    Serial.print("[SEND]");sendArray.printfArray<HardwareSerial>(&Serial);
#endif
    _uartp->flush();
    _uartp->write(sendArray.dataptr(),sendArray.size());

    return 0;
}
int DRFZigbee::sendDataP2P(uint8_t mode,uint16_t addr,const std::initializer_list<uint8_t> args)
{
    byteArray sendArray(args.begin(),args.size());
    sendDataP2P(mode,addr,sendArray);
    return 0;
}

int DRFZigbee::getNetworksTopology()
{
    uint8_t senddata[] = {0xfc,0x04,0x0a,0x22,0x33,0x44,0xa3};
    byteArray sendArray(senddata,7);
#ifdef ZIGBEE_DEBUG
    Serial.print("[SEND]");sendArray.printfArray<HardwareSerial>(&Serial);
#endif
    _uartp->flush();
    _uartp->write(sendArray.dataptr(),sendArray.size());

    uint8_t  reviceBuff[256];

    _uartp->setTimeout(5000);

    if(!_uartp->find(0xed)){ Serial.print("[TIMEOUT]\r\n"); return lastErrorcode = kTimeoutError;}

    size_t length = _uartp->available();
    _uartp->readBytes(reviceBuff,length);

#ifdef ZIGBEE_DEBUG
    Serial.print("[REVC]");byteArray(reviceBuff,length).printfArray<HardwareSerial>(&Serial);
#endif
    byteArray array(reviceBuff,length);
    int pos = array.indexof(0xed);
    Serial.printf("pos:%d\r\n",pos);
    //for( int i = 0; i < )
    return kReviceOK;
}

int DRFZigbee::linkMoudle()
{
    return sendCMDAndWaitRevice(0xfc,ZIGBEE_CMD_LINKMODULE);
}

int DRFZigbee::rebootModule()
{
    return sendCMDAndWaitRevice(0xfc,{0x06,0x44,0x54,0x4b,0xaa,0xbb});
}

int DRFZigbee::readModuleparm(zigbee_arg_t *parm)
{
    byteArray array;
    if( sendCMDAndWaitRevice(0xfc,ZIGBEE_CMD_READPARM,&array) != DRFZigbee::kReviceOK ) return lastErrorcode;
    if(( array.at(0) != 0x0A )||( array.at(1) != 0x0E ))  return kPramFormatError;
    array = array.mid(2);
#ifdef ZIGBEE_DEBUG
    array.printfArray<HardwareSerial>(&Serial);
#endif
    if( parm == nullptr ) return kPointerisnullptr;
    memcpy(parm->Wave,array.dataptr(),sizeof(DRFZigbee::zigbee_arg));
    return kReviceOK;
}

int DRFZigbee::setModuleparm(zigbee_arg_t &parm)
{
    byteArray sendArray;
    sendArray += 0x07;
    sendArray += byteArray(&parm.Wave[0],16);
    sendArray += byteArray(&parm.Wave[24],16);
    sendArray += byteArray(&parm.Wave[42],6);

    sendArray.printfArray<HardwareSerial>(&Serial);

    return sendCMDAndWaitRevice(0xfc,sendArray);
}

int8_t DRFZigbee::getModuleRSSI(nodeRSSI_t *nodeRSSIPtr)
{
    byteArray reviceArray;
    if( sendCMDAndWaitRevice(0xfc,ZIGBEE_CMD_GETNODERSSI,&reviceArray,5000) != kReviceOK ) return -1;

    if( reviceArray.at(0) != 0x0a ) return -1;

    if( nodeRSSIPtr != nullptr )
    {
        nodeRSSIPtr->routerLevel = reviceArray.at(1);
        nodeRSSIPtr->shortAddr = ( reviceArray.at(2) << 8 ) | reviceArray.at(3);
        nodeRSSIPtr->rssi = reviceArray.at(4);
    }

    return int8_t(reviceArray.at(4));
}

int DRFZigbee::reviceData(reviceData_t *revice,uint8_t type,size_t timeout)
{
    uint8_t  reviceBuff[300];

    _uartp->setTimeout(timeout);

    if(!_uartp->find(type)){ lastErrorcode = kTimeoutError; return kTimeoutError; }

    size_t length = _uartp->available();
    _uartp->readBytes(reviceBuff,length);

    revice->length = reviceBuff[0];
    revice->addr = (reviceBuff[1] << 8) + reviceBuff[2];
    revice->array = new byteArray(&reviceBuff[3],revice->length);
    revice->fromAddr = (reviceBuff[3 + revice->length] << 8) + reviceBuff[4 + revice->length];

    return kReviceOK;
}