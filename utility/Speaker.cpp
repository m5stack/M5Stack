#include "Speaker.h"

SPEAKER::SPEAKER(void) {
    _volume = 8;
}

void SPEAKER::begin() {
    ledcSetup(TONE_PIN_CHANNEL, 0, 13);
    ledcAttachPin(SPEAKER_PIN, TONE_PIN_CHANNEL);
}

void SPEAKER::beep() {
    tone(1000, 200);
}

void SPEAKER::tone(uint16_t frequency) {
    ledcWriteTone(TONE_PIN_CHANNEL, frequency);
}

void SPEAKER::tone(uint16_t frequency, uint32_t duration) {
    tone(frequency);
    _count = millis() + duration;
    speaker_on = 1;
}

void SPEAKER::setVolume(uint8_t volume) {
    // uint32_t valueMax = 255;
    // // calculate duty, 8191 from 2 ^ 13 - 1
    // uint32_t duty = (8191 / valueMax) * min(volume, valueMax);

    // // write duty to LEDC
    // ledcWrite(TONE_PIN_CHANNEL, duty);
    _volume = 10 - volume;
}

void SPEAKER::mute() {
    ledcWriteTone(TONE_PIN_CHANNEL, 0);
    digitalWrite(SPEAKER_PIN, 0);
}

void SPEAKER::update() {
    if(speaker_on) {
        if(millis() > _count) {
            speaker_on = 0;
            mute();
        }
    }
}

void SPEAKER::write(uint8_t value) {
    dacWrite(SPEAKER_PIN, value);
}

void SPEAKER::playMusic(const uint8_t* music_data, uint16_t sample_rate) {
    uint32_t length = strlen((char*)music_data);
    uint16_t delay_interval = ((uint32_t)1000000/sample_rate);
    if(_volume != 0) {
        for(int i=0; i<length; i++) {
            dacWrite(SPEAKER_PIN, music_data[i]/_volume);
            delayMicroseconds(delay_interval);
        }
    }

}