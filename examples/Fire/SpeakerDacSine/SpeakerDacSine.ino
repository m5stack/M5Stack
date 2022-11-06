/*
  I2S sine wave
  This example produces a digital sine wave on the build in DAC1
  which is connected to the speaker

  Hardware:
   microcontroller board: M5StackFire
                          speaker amplifier internaly connected to DAC pin 25

  September 2018 ChrisMicro
*/

#define DACPIN 25  // speaker DAC, only 8 Bit

#define SAMPLINGFREQUENCY 44100
#define NUMBEROFSAMPLES   SAMPLINGFREQUENCY * 1  // paly 1 seconds

#define DAC_MAX_AMPLITUDE 127 / 4  // max value is 127, but it is too loud

#define AUDIOBUFFERLENGTH NUMBEROFSAMPLES

uint8_t AudioBuffer[AUDIOBUFFERLENGTH];

void setup() {
    const float frequency = 440;
    const float amplitude = DAC_MAX_AMPLITUDE;

    // store sine wave in buffer
    for (int n = 0; n < NUMBEROFSAMPLES; n++) {
        int16_t sineWaveSignal =
            (sin(2 * PI * frequency / SAMPLINGFREQUENCY * n)) * amplitude;
        AudioBuffer[n] = sineWaveSignal + 128;
    }
}

void loop() {
    uint32_t start = micros();

    for (int n = 0; n < NUMBEROFSAMPLES; n++) {
        // wait for next sample
        while (start + (1000000UL / SAMPLINGFREQUENCY) > micros())
            ;
        start = micros();

        dacWrite(DACPIN, AudioBuffer[n]);
    }

    delay(3000);
}
