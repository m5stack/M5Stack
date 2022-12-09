#include <M5Unified.h>
#include <M5UnitRCA.h>

#include "wav_unsigned_8bit_click.cpp"

M5UnitRCA gfx_rca;

// M5UnitRCA gfx_rca ( 216                            // logical_width
//                   , 144                            // logical_height
//                   , 256                            // output_width
//                   , 160                            // output_height
//                   , M5UnitRCA::signal_type_t::PAL  // signal_type
//                   , 26                             // GPIO pin
//                   );
// signal_type: can be selected from NTSC / NTSC_J / PAL / PAL_M / PAL_N.
// GPIO pin: can be selected from 25 / 26

static constexpr int color_list[3]   = {TFT_RED, TFT_GREEN, TFT_BLUE};
static int color_index               = 0;
static unsigned long last_clear_time = 0;
static bool is_pin_25                = true;

void play_wav(void*) {
    while (true) {
        M5.Speaker.playWav(sunrise_anna_single_left,
                           sizeof(sunrise_anna_single_left), ~0u, 0, true);
        delay(3000);
        M5.Speaker.stop(0);
        delay(500);

        M5.Speaker.playWav(sunrise_anna_single_right,
                           sizeof(sunrise_anna_single_right), ~0u, 1, true);
        delay(3000);
        M5.Speaker.stop(1);
        delay(500);
    }
}

void setup(void) {
    auto cfg         = M5.config();
    cfg.external_spk = true;
    cfg.internal_spk = false;

    M5.begin();

    M5.Display.clear(TFT_BLACK);
    M5.Display.setFont(&DejaVu24);
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Display.drawCenterString("M5 RCA Module :)", 160, 20);

    M5.Display.drawCenterString("Using Pin 26", 160, 120);
    M5.Display.drawString("Pin 25", 25, 210);
    M5.Display.drawString("Pin 26", 220, 210);

    // if (M5.getBoard() == m5gfx::boards::board_M5StackCore2) {
    //     {
    //         auto spk_cfg        = M5.Speaker.config();
    //         spk_cfg.i2s_port    = I2S_NUM_1;
    //         spk_cfg.sample_rate = 96000;

    //         spk_cfg.pin_data_out = 2;
    //         spk_cfg.pin_bck      = 19;
    //         spk_cfg.pin_ws       = 0;  // LRCK

    //         spk_cfg.stereo        = true;
    //         spk_cfg.buzzer        = false;
    //         spk_cfg.use_dac       = false;
    //         spk_cfg.magnification = 16;
    //         M5.Speaker.config(spk_cfg);
    //     }
    // } else {
    //     auto spk_cfg        = M5.Speaker.config();
    //     spk_cfg.i2s_port    = I2S_NUM_1;
    //     spk_cfg.sample_rate = 96000;

    //     spk_cfg.pin_data_out = 15;
    //     spk_cfg.pin_bck      = 13;
    //     spk_cfg.pin_ws       = 0;  // LRCK

    //     spk_cfg.stereo        = true;
    //     spk_cfg.buzzer        = false;
    //     spk_cfg.use_dac       = false;
    //     spk_cfg.magnification = 16;
    //     M5.Speaker.config(spk_cfg);
    // }

    // M5.Speaker.begin();
    // M5.Speaker.setVolume(128);
    // M5.Speaker.setChannelVolume(0, 255);
    // M5.Speaker.setChannelVolume(1, 255);

    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);

    // xTaskCreatePinnedToCore(play_wav, "play_wav", 8192, NULL, 1, NULL, 1);

    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
}

int blink_flag = 0;
void loop(void) {
    M5.update();

    if ((millis() - last_clear_time) > 1000) {
        last_clear_time = millis();

        color_index++;
        if (color_index > 2) {
            color_index = 0;
        }

        gfx_rca.clear(color_list[color_index]);
        gfx_rca.setTextColor(TFT_WHITE, color_list[color_index]);
        gfx_rca.drawCenterString("M5 RCA Module :)", 120, 50);

        if (is_pin_25) {
            gfx_rca.drawCenterString("Using Pin 25", 120, 110);
        } else {
            gfx_rca.drawCenterString("Using Pin 26", 120, 110);
        }

        blink_flag = !blink_flag;
        digitalWrite(16, blink_flag);
        digitalWrite(17, blink_flag);
    }

    if (M5.BtnA.isPressed()) {
        if (is_pin_25) {
            return;
        }
        is_pin_25 = true;
        M5.Display.drawCenterString("Using Pin 25", 160, 120);

        gfx_rca.setOutputPin(25);
        gfx_rca.setTextColor(TFT_WHITE, color_list[color_index]);
        gfx_rca.drawCenterString("Using Pin 25", 120, 110);
    }

    if (M5.BtnC.isPressed()) {
        if (!is_pin_25) {
            return;
        }
        is_pin_25 = false;
        M5.Display.drawCenterString("Using Pin 26", 160, 120);

        gfx_rca.setOutputPin(26);
        gfx_rca.setTextColor(TFT_WHITE, color_list[color_index]);
        gfx_rca.drawCenterString("Using Pin 26", 120, 110);
    }
}
