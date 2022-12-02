#define LGFX_USE_V1
#include <M5Unified.h>

#include "wav_unsigned_8bit_click.cpp"

class LGFX : public lgfx::LGFX_Device {
   public:
    lgfx::Panel_CVBS _panel_instance;

    LGFX(void) {
        {
            auto cfg = _panel_instance.config();

            cfg.memory_width  = 240;
            cfg.memory_height = 180;
            cfg.panel_width   = 240;
            cfg.panel_height  = 180;
            cfg.offset_x      = 0;
            cfg.offset_y      = 0;
            _panel_instance.config(cfg);
        }
        {
            auto cfg = _panel_instance.config_detail();

            // cfg.signal_type = cfg.signal_type_t::NTSC_J;
            // cfg.signal_type = cfg.signal_type_t::PAL;
            // cfg.signal_type = cfg.signal_type_t::PAL_M;
            cfg.signal_type = cfg.signal_type_t::PAL_N;

            cfg.pin_dac      = 25;
            cfg.use_psram    = 0;
            cfg.output_level = 128;
            cfg.chroma_level = 128;
            _panel_instance.config_detail(cfg);
        }

        setPanel(&_panel_instance);
    }
};

LGFX gfx_rca;

static constexpr int color_list[3]   = {TFT_RED, TFT_GREEN, TFT_BLUE};
static int color_index               = 0;
static unsigned long last_clear_time = 0;
static bool is_pin_25                = true;

void play_wav(void*) {
    while (true) {
        M5.Speaker.playWav(sunrise_anna_single_left, sizeof(sunrise_anna_single_left), ~0u, 0,
                           true);
        delay(3000);
        M5.Speaker.stop(0);
        delay(500);

        M5.Speaker.playWav(sunrise_anna_single_right, sizeof(sunrise_anna_single_right), ~0u, 1,
                           true);
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

    M5.Display.drawCenterString("Using Pin 25", 160, 120);
    M5.Display.drawString("Pin 25", 25, 210);
    M5.Display.drawString("Pin 26", 220, 210);

//    if (M5.getBoard() == m5gfx::boards::board_M5StackCore2) {
//        {
//            auto spk_cfg        = M5.Speaker.config();
//            spk_cfg.i2s_port    = I2S_NUM_1;
//            spk_cfg.sample_rate = 96000;
//
//            spk_cfg.pin_data_out = 2;
//            spk_cfg.pin_bck      = 19;
//            spk_cfg.pin_ws       = 0;  // LRCK
//
//            spk_cfg.stereo        = true;
//            spk_cfg.buzzer        = false;
//            spk_cfg.use_dac       = false;
//            spk_cfg.magnification = 16;
//            M5.Speaker.config(spk_cfg);
//        }
//    } 
//    else 
//        {
//            auto spk_cfg        = M5.Speaker.config();
//            spk_cfg.i2s_port    = I2S_NUM_1;
//            spk_cfg.sample_rate = 96000;
//
//            spk_cfg.pin_data_out = 15;
//            spk_cfg.pin_bck      = 13;
//            spk_cfg.pin_ws       = 0;  // LRCK
//
//            spk_cfg.stereo        = true;
//            spk_cfg.buzzer        = false;
//            spk_cfg.use_dac       = false;
//            spk_cfg.magnification = 16;
//            M5.Speaker.config(spk_cfg);
//        }
    

//    M5.Speaker.begin();
//    M5.Speaker.setVolume(128);
//    M5.Speaker.setChannelVolume(0, 255);
//    M5.Speaker.setChannelVolume(1, 255);

    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);

//    xTaskCreatePinnedToCore(play_wav, "play_wav", 8192, NULL, 1, NULL, 1);

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

        lgfx::Panel_CVBS* p = (lgfx::Panel_CVBS*)gfx_rca.panel();
        p->deinit();
        auto cfg = p->config();
        cfg.memory_width  = 240;
        cfg.memory_height = 180;
        cfg.panel_width   = 240;
        cfg.panel_height  = 180;
        cfg.offset_x      = 0;
        cfg.offset_y      = 0;
        p->config(cfg);
        auto cfg_d = p->config_detail();
        cfg_d.pin_dac      = 25;
        cfg_d.use_psram    = 0;
        cfg_d.output_level = 128;
        cfg_d.chroma_level = 128;
        p->config_detail(cfg_d);

        gfx_rca.init();
        gfx_rca.setTextColor(TFT_WHITE, color_list[color_index]);
        gfx_rca.drawCenterString("Using Pin 25", 120, 110);
    }

    if (M5.BtnC.isPressed()) {
        if (!is_pin_25) {
            return;
        }
        is_pin_25 = false;
        M5.Display.drawCenterString("Using Pin 26", 160, 120);

        lgfx::Panel_CVBS* p = (lgfx::Panel_CVBS*)gfx_rca.panel();
        p->deinit();
        auto cfg = p->config();
        cfg.memory_width  = 240;
        cfg.memory_height = 180;
        cfg.panel_width   = 240;
        cfg.panel_height  = 180;
        cfg.offset_x      = 0;
        cfg.offset_y      = 0;
        p->config(cfg);
        auto cfg_d = p->config_detail();
        cfg_d.pin_dac      = 26;
        cfg_d.use_psram    = 0;
        cfg_d.output_level = 128;
        cfg_d.chroma_level = 128;
        p->config_detail(cfg_d);

        gfx_rca.init();
        gfx_rca.setTextColor(TFT_WHITE, color_list[color_index]);
        gfx_rca.drawCenterString("Using Pin 26", 120, 110);
    }
}
