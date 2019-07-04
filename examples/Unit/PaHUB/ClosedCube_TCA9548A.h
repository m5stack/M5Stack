#ifndef CLOSEDCUBE_TCA9548A_H
#define CLOSEDCUBE_TCA9548A_H

#include <Arduino.h>

#define TCA9548A_MAX_CHANNELS 6

namespace ClosedCube 
{

    namespace Wired 
    {

        class TCA9548A 
        {

            public:
                TCA9548A();
                TCA9548A(uint8_t address);

                void address(uint8_t address);

                uint8_t getChannel();

                uint8_t selectChannel(uint8_t channel);
                uint8_t nextChannel();

            private:
                uint8_t _address;
                uint8_t _currentChannel;

        };
        
    }
}

#endif //CLOSEDCUBE_TCA9548A_H
