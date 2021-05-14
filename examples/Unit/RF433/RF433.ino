
/*
    Description: Define `RF433RX` by enabling or commenting the macro, switch the program function, 
    and burn the transmitting and receiving programs to the two M5Core masters respectively.
    Connect RF433R/T to PortB (G26, G36), the transmitter end presses button A to transmit, 
    and the receiver end can view the received data through Serial.
*/

#include "M5Stack.h"
#include <driver/rmt.h>

#define RF433RX

#define RMT_TX_CHANNEL     RMT_CHANNEL_0
#define RMT_RX_CHANNEL     RMT_CHANNEL_1
#define RTM_TX_GPIO_NUM    26
#define RTM_RX_GPIO_NUM    36
#define RTM_BLOCK_NUM      1

#define RMT_CLK_DIV         80    /*!< RMT counter clock divider */ 
#define RMT_1US_TICKS (80000000 / RMT_CLK_DIV / 1000000)
#define RMT_1MS_TICKS (RMT_1US_TICKS * 1000)

rmt_item32_t rmtbuff[2048];

#define T0H 	670
#define T1H 	320
#define T0L  	348
#define T1L  	642

#define RMT_CODE_H      {670,1,320,0}
#define RMT_CODE_L      {348,1,642,0}
#define RMT_START_CODE0 {4868,1,2469,0}
#define RMT_START_CODE1 {1647,1,315,0}


void initRMT()
{
    #ifndef RF433RX
    rmt_config_t txconfig;
    txconfig.rmt_mode = RMT_MODE_TX;
    txconfig.channel = RMT_TX_CHANNEL;
    txconfig.gpio_num = gpio_num_t(RTM_TX_GPIO_NUM);
    txconfig.mem_block_num = RTM_BLOCK_NUM;
    txconfig.tx_config.loop_en = false;
    txconfig.tx_config.carrier_en = false;
    txconfig.tx_config.idle_output_en = true;
    txconfig.tx_config.idle_level = rmt_idle_level_t(0);
    txconfig.clk_div = RMT_CLK_DIV;			//时钟分频

    ESP_ERROR_CHECK(rmt_config(&txconfig));
    ESP_ERROR_CHECK(rmt_driver_install(txconfig.channel, 0, 0));
    #else
    rmt_config_t rxconfig;
    rxconfig.rmt_mode = RMT_MODE_RX;
    rxconfig.channel = RMT_RX_CHANNEL;
    rxconfig.gpio_num = gpio_num_t(RTM_RX_GPIO_NUM);
    rxconfig.mem_block_num = 6;
    rxconfig.clk_div = RMT_CLK_DIV;			//时钟分频
    rxconfig.rx_config.filter_en = true;  //开启滤波器
    rxconfig.rx_config.filter_ticks_thresh = 200 * RMT_1US_TICKS; //滤波信号宽度100*1M = 100us
    rxconfig.rx_config.idle_threshold = 3 * RMT_1MS_TICKS;
    
    ESP_ERROR_CHECK(rmt_config(&rxconfig));
    ESP_ERROR_CHECK(rmt_driver_install(rxconfig.channel, 2048, 0));
    #endif
}
/*
uint8_t databuff1[5] = {0xdd,0x41,0x53,0x80,0x9f};
uint8_t databuff2[5] = {0xdd,0x41,0x5a,0x80,0x96};
uint8_t databuff3[5] = {0xdd,0x41,0x58,0x80,0x95};
*/

uint8_t data[6] = {0xAA,0x55,0x01,0x02,0x03,0x04};
void send(uint8_t* buff,size_t size)
{
    rmtbuff[0] = (rmt_item32_t){RMT_START_CODE0};
    rmtbuff[1] = (rmt_item32_t){RMT_START_CODE1};
    for (int i = 0; i < size; i++)
    {
        uint8_t mark = 0x80;
        for (int n = 0; n < 8; n++)
        {
            rmtbuff[2+i*8+n] = ((buff[i]&mark))?((rmt_item32_t){RMT_CODE_H}):((rmt_item32_t){RMT_CODE_L});
            mark >>= 1;
        }
    }
    for( int i = 0; i< 8;i++)
    {
        ESP_ERROR_CHECK(rmt_write_items(RMT_TX_CHANNEL, rmtbuff, 42, false));
        ESP_ERROR_CHECK(rmt_wait_tx_done(RMT_TX_CHANNEL, portMAX_DELAY));
    }
}

void setup() {

    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Lcd.fillRect(0,0,320,240,TFT_BLACK);
    M5.Lcd.fillRect(0,0,320,20,M5.Lcd.color565(38,38,38));
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.drawString("RF433 factory Test",20,2,2);
    pinMode(RTM_TX_GPIO_NUM,OUTPUT);
    pinMode(RTM_RX_GPIO_NUM,INPUT);
    initRMT();
    delay(100);
    M5.Lcd.setCursor(5,100);
    M5.Lcd.setTextSize(2);
    #ifndef RF433RX
    M5.Lcd.print("Click Btn A send data");
    #else
    M5.Lcd.print("Use Serial to view data"); 
    #endif

    //rf.begin<PT2262>(26,27).arg<int>(1000);
}

int parsedData(rmt_item32_t* item,size_t size,uint8_t* dataptr,size_t maxsize)
{
    //if((item == nullptr)||(size < 4))return -1;
    if(size < 4)return -1;
    int cnt = 0;
    uint8_t data = 0;
    uint8_t bitcnt = 0, hex_cnt = 0;
    if((( item[0].level0 == 0 ))&&(item[0].duration0 > 2300 )&&( item[0].duration0 < 2600 ))
    {
        //dataptr = (uint8_t*)malloc(size * sizeof(uint8_t));
        rmt_item32_t dataitem;
        dataitem.level0 = 1;
        dataitem.level1 = 0;
        dataitem.duration0 = item[0].duration1;
        do
        {
            cnt ++;
            dataitem.duration1 = item[cnt].duration0;
            if( cnt > 1 )
            {
                //Serial.printf("%d:%d,%d:%d ",dataitem.level0,dataitem.duration0,dataitem.level1,dataitem.duration1);
                if((( dataitem.duration0 + dataitem.duration1 ) < 1100 )&&(( dataitem.duration0 + dataitem.duration1 ) > 800 ))
                {
                    data <<= 1;
                    if( dataitem.duration0 > dataitem.duration1 )
                    {
                        data += 1;
                    }

                    bitcnt ++;
                    if( bitcnt >= 8 )
                    {
                        Serial.printf("%02x ",data);
                        if( hex_cnt >= maxsize )
                        {
                            return hex_cnt;
                        }
                        dataptr[hex_cnt] = data;
                        data = 0;
                        hex_cnt ++;
                        bitcnt = 0;
                    }
                }
                else
                {
                    Serial.printf("%d %d:%d,%d:%d ",hex_cnt,dataitem.level0,dataitem.duration0,dataitem.level1,dataitem.duration1);
                    return hex_cnt;
                }
            }
            dataitem.duration0 = item[cnt].duration1;
        }while (cnt < size);

        Serial.println("END");
    }
    return hex_cnt;
}

void loop() {

#ifndef RF433RX
    if(M5.BtnA.wasPressed())
    {
        Serial.println("SEND");
        send(data,6);
    }
#else
    int revicecnt = 0;

    RingbufHandle_t rb = nullptr;
    rmt_get_ringbuf_handle(RMT_RX_CHANNEL,&rb);
    rmt_rx_start(RMT_RX_CHANNEL,true);
    while (rb) 
    {
        size_t rx_size = 0;
        rmt_item32_t* item = (rmt_item32_t*) xRingbufferReceive(rb, &rx_size, 500);
        if( item != nullptr )
        {
            if( rx_size != 0 )
            {
                uint8_t databuff[256];
                Serial.printf("\r\nsize:%d\r\n",rx_size);
                int size = parsedData(item,rx_size,databuff,255);
                if(( size >= 5 )&&( databuff[0] == 0xaa )&&(databuff[1] == 0x55)&&(databuff[2] == 0x01)&&(databuff[3] == 0x02)&&(databuff[4] == 0x03))
                {
                    revicecnt ++;
                    Serial.printf("Revice %d\r\n",revicecnt);
                    if( revicecnt == 4 )
                    {
                        M5.Lcd.fillRect(0,20,320,220,TFT_GREEN);
                    }
                }
            }
            vRingbufferReturnItem(rb, (void*) item);
        }
        else 
        {
            //Serial.printf("mmp????\r\n");
            if( revicecnt != 0 )
            {
                Serial.printf("Revice %d\r\n",revicecnt);
                M5.Lcd.fillRect(0,20,320,220,TFT_RED);
            }
            revicecnt = 0;
        }
    }
    rmt_rx_stop(RMT_RX_CHANNEL);

#endif

    delay(10);
    M5.update();
}