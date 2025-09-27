#include "wokwi_epaper.h"
#include <bitset>
#include <cinttypes>
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace wokwi_waveshare_epaper_bruteforce
    {
        static const char *const TAG = "wokwi_waveshare_epaper_bruteforce";

        void WaveshareEPaperTypeA::dump_config()
        {
            ESP_LOGCONFIG(TAG, "  Full Update Every: %" PRIu32, this->full_update_every_);
            LOG_PIN("  Reset Pin: ", this->reset_pin_);
            LOG_PIN("  DC Pin: ", this->dc_pin_);
            LOG_PIN("  Busy Pin: ", this->busy_pin_);
            LOG_UPDATE_INTERVAL(this);
        }

        void WaveshareEPaperTypeA::setup()
        {
            ESP_LOGD(TAG, "Setup.");

            // Setup pins
        }

        void WaveshareEPaperTypeA::update()
        {
            ESP_LOGD(TAG, "Update.");
        }

        int WaveshareEPaperTypeA::get_height_internal()
        {
            return 296;
        }

        int WaveshareEPaperTypeA::get_width_internal()
        {
            return 176;
        }

        void WaveshareEPaperTypeA::reset()
        {
            if (this->reset_pin_ == nullptr)
                return;

            // Start reset sequence (step 1: RST high)
            this->reset_pin_->digital_write(true);

            // After 200ms, pull low
            this->set_timeout("epaper_reset_low", 200, [this]() {
                this->reset_pin_->digital_write(false);

                // After 5ms, pull high again
                this->set_timeout("epaper_reset_high", 5, [this]() {
                    this->reset_pin_->digital_write(true);

                    // After another 200ms, reset sequence is done
                    this->set_timeout("epaper_reset_done", 200, [this]() {
                        ESP_LOGD("epaper", "Reset sequence complete");
                    });
                }); 
            });
        }

        void HOT WaveshareEPaperTypeA::draw_absolute_pixel_internal(int x, int y, Color color)
        {
            if (x >= this->get_width_internal() || y >= this->get_height_internal() || x < 0 || y < 0)
                return;

            const uint32_t pos = (x + y * this->get_width_controller()) / 8u;
            const uint8_t subpos = x & 0x07;
            // flip logic
            if (!color.is_on())
            {
                this->buffer_[pos] |= 0x80 >> subpos;
            }
            else
            {
                this->buffer_[pos] &= ~(0x80 >> subpos);
            }
        }

        float WaveshareEPaperTypeA::get_setup_priority() const { return setup_priority::PROCESSOR; }
    }
}