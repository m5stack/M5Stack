#include "Arduino.h"
#include "config.h"
#include "esp32-hal-dac.h"

class SPEAKER {
public:
    SPEAKER(void);
    
    void begin();
    void beep();
    void mute();
    void tone(uint16_t frequency);
    void tone(uint16_t frequency, uint32_t duration);
    void update();
    void write(uint8_t value);
    void setVolume(uint8_t volume);
    void playMusic(const uint8_t* music_data, uint16_t sample_rate);
    
private:
    uint32_t _count;
    uint8_t _volume;
    bool speaker_on;
};
