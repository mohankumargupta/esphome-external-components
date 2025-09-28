#pragma once

#include "esphome/core/component.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome
{
    namespace wokwi_waveshare_epaper_bruteforce
    {
        enum WaveshareEPaperTypeAModel
        {
            WAVESHARE_EPAPER_1_54_IN = 0,
            WAVESHARE_EPAPER_1_54_IN_V2,
            WAVESHARE_EPAPER_2_13_IN,
            WAVESHARE_EPAPER_2_13_IN_V2,
            WAVESHARE_EPAPER_2_9_IN,
            WAVESHARE_EPAPER_2_9_IN_V2,
            TTGO_EPAPER_2_13_IN,
            TTGO_EPAPER_2_13_IN_B73,
            TTGO_EPAPER_2_13_IN_B1,
            TTGO_EPAPER_2_13_IN_B74,
        };

        class WaveshareEPaperTypeA : public display::DisplayBuffer,
                                     public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST,
                                                           spi::CLOCK_POLARITY_LOW,
                                                           spi::CLOCK_PHASE_LEADING,
                                                           spi::DATA_RATE_75KHZ>
        {

              uint32_t reset_duration_{200};

        public:
            void set_dc_pin(GPIOPin *dc_pin) { dc_pin_ = dc_pin; }
            float get_setup_priority() const override;
            void set_reset_pin(GPIOPin *reset) { this->reset_pin_ = reset; }
            void set_busy_pin(GPIOPin *busy) { this->busy_pin_ = busy; }
            void set_reset_duration(uint32_t reset_duration) { this->reset_duration_ = reset_duration; }

            void setup() override;
            void update() override;
            void dump_config() override;

            void reset();
            void command(uint8_t cmd);
            void cmd_data(const uint8_t *data, size_t length);

            int get_width_internal() override;
            int get_height_internal() override;
            int get_width_controller() { return this->get_width_internal(); }
            display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_BINARY; }
            void draw_absolute_pixel_internal(int x, int y, Color color) override;

        protected:
            uint32_t full_update_every_{30};
            bool wait_until_idle_();



            // int get_width_controller() override;

            GPIOPin *reset_pin_{nullptr};
            GPIOPin *dc_pin_;
            GPIOPin *busy_pin_{nullptr};
            uint32_t idle_timeout_() { return 1000u; }
        };

    }
}
