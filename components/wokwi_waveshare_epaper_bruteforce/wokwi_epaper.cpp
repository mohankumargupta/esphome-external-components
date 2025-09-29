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
            // ESP_LOGCONFIG(TAG, "  Full Update Every: %" PRIu32, this->full_update_every_);
            // LOG_PIN("  Reset Pin: ", this->reset_pin_);
            // LOG_PIN("  DC Pin: ", this->dc_pin_);
            // LOG_PIN("  Busy Pin: ", this->busy_pin_);
            // LOG_UPDATE_INTERVAL(this);
        }

        void WaveshareEPaperTypeA::setup()
        {
            ESP_LOGD(TAG, "Setup.");
            this->disable();

            //Setup display buffer
            //this->init_internal_(this->get_buffer_length_());



            // Setup pins
            this->dc_pin_->setup(); // OUTPUT
            this->dc_pin_->digital_write(false);
            if (this->reset_pin_ != nullptr)
            {
                this->reset_pin_->setup(); // OUTPUT
                this->reset_pin_->digital_write(true);
            }
            if (this->busy_pin_ != nullptr)
            {
                this->busy_pin_->setup(); // INPUT
            }

            this->spi_setup();
            this->reset();

            ESP_LOGD(TAG, "Software reset sent.");

            //Init sequence
            //0x01, 0x27, 0x01, 0x00
            const uint8_t data1[] = {0x01, 0x27, 0x01, 0x00};
            this->cmd_data(data1, 3); 
            //this->wait_until_idle_();

            //0x3C, 0x05
            const uint8_t data2[] = {0x3C, 0x05};
            this->cmd_data(data2, 1); 

            //0x21,0x00,0x80
            const uint8_t data3[] = {0x21,0x00,0x80};
            this->cmd_data(data3, 2);
            
            //0x18,0x80
            const uint8_t data4[] = {0x18,0x80};
            this->cmd_data(data4, 1);    
            
            //0x11,0x03
            const uint8_t data5[] = {0x11,0x03};
            this->cmd_data(data5, 1);   

            //0x44,0x00,0x0F
            const uint8_t data6[] = {0x44,0x00,0x0F};
            this->cmd_data(data6, 2);             


            //0x45, 0x00,0x00, 0x27, 0x01
            const uint8_t data7[] = {0x45,0x00,0x00,0x27,0x01};
            this->cmd_data(data7, 4); 

            //0x4E, 0x00
            const uint8_t data8[] = {0x4E, 0x00};
            this->cmd_data(data8, 1);
                      
            //0x4F, 0x00, 0x00
            const uint8_t data9[] = {0x4F, 0x00, 0x00};
            this->cmd_data(data9, 2);
              
            //0x22, 0xC0
            const uint8_t data10[] = {0x22, 0xC0};
            this->cmd_data(data10, 1);
            //this->wait_until_idle_();   

            //0x20
            this->command(0x20);
            delay(15000);


            //0x24
            this->command(0x24);
            this->data_x_times(0xFB, 200);
            this->data_x_times(0xFF, 4736 -200);
            //0x26
            delay(100);
            this->command(0x26);
            this->data_x_times(0xFB, 200);
            this->data_x_times(0xFF, 4736 -200);

            //0x22,0xF4,0x20
            const uint8_t data11[] = {0x22, 0xF4};
            this->cmd_data(data11, 1);
            this->command(0x20);


            //this->wait_until_idle_();

            /*

            //0x11, 0x03
            const uint8_t data2[] = {0x11, 0x03};
            this->cmd_data(data2, 1); 
            this->wait_until_idle_();
            delay(20);

            //0x44, 0x00, 0x0F
            const uint8_t data3[] = {0x44, 0x00, 0x0F};
            this->cmd_data(data3, 2);
            this->wait_until_idle_();

            //0x45, 0x00,0x00, 0x27, 0x01
            const uint8_t data4[] = {0x45, 0x00,0x00, 0x27, 0x01};
            this->cmd_data(data4, 4);
            this->wait_until_idle_();

            //0x21, 0x00, 0x80,
            const uint8_t data5[] = {0x21, 0x00, 0x80};
            this->cmd_data(data5, 2);
            this->wait_until_idle_();


         

            //0x24
            this->command(0x24);
            this->data_x_times(0xF8, 4736);
            // this->data_x_times(0xFF, 4700);
            // this->data_x_times(0x00, 36);
            this->wait_until_idle_();   

            //0x22, 0xF7
            const uint8_t data8[] = {0x22, 0xF7};
            this->cmd_data(data8, 1);
            this->wait_until_idle_();   

            //0x20
            this->command(0x20);
            */


            ESP_LOGD(TAG, "Init sequence sent.");
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
            return 128;
        }

        void WaveshareEPaperTypeA::reset()
        {
            if (this->reset_pin_ == nullptr)
                return;

            // Start reset sequence (step 1: RST high)
            this->reset_pin_->digital_write(true);
            delay(200);
            this->reset_pin_->digital_write(false);
            delay(20);
            this->reset_pin_->digital_write(true);
            this->reset_pin_->digital_write(true);
            delay(200);
            this->command(0x12);  // SWRESET
            //this->wait_until_idle_();
            delay(10);
            ESP_LOGD(TAG, "software reset sent.");


            /*

            // After 200ms, pull low
            this->set_timeout("epaper_reset_low", 200, [this]()
                              {
                this->reset_pin_->digital_write(false);

                // After 5ms, pull high again
                this->set_timeout("epaper_reset_high", 5, [this]() {
                    this->reset_pin_->digital_write(true);

                    // After another 200ms, reset sequence is done
                    this->set_timeout("epaper_reset_done", 200, [this]() {
                        ESP_LOGD("epaper", "Reset sequence complete");

                        //Init sequence
                        //0x12
                        this->command(0x12);  // SWRESET
                        this->wait_until_idle_();

                        //0x01, 0x27, 0x01, 0x00
                        const uint8_t data1[] = {0x01, 0x27, 0x01, 0x00};
                        this->cmd_data(data1); 
   
                        //0x11, 0x03
                        const uint8_t data2[] = {0x11, 0x03};
                        this->cmd_data(data2); 

                        //0x44, 0x00, 0x0F
                        const uint8_t data3[] = {0x44, 0x00, 0x0F};
                        this->cmd_data(data3);

                        //0x45, 0x00,0x00, 0x27, 0x01
                        const uint8_t data4[] = {0x45, 0x00,0x00, 0x27, 0x01};
                        this->cmd_data(data4);

                        //0x21, 0x00, 0x80,
                        const uint8_t data5[] = {0x21, 0x00, 0x80};
                        this->cmd_data(data5);

                    });
                }); });

            */
        }

        bool WaveshareEPaperTypeA::wait_until_idle_()
        {
            if (this->busy_pin_ == nullptr || !this->busy_pin_->digital_read())
            {
                return true;
            }

            const uint32_t start = millis();
            while (this->busy_pin_->digital_read())
            {
                if (millis() - start > this->idle_timeout_())
                {
                    ESP_LOGE(TAG, "Timeout while displaying image!");
                    return false;
                }
                delay(1);
            }
            return true;
        }

        void WaveshareEPaperTypeA::cmd_data(const uint8_t *c_data, size_t length)
        {
            this->dc_pin_->digital_write(false);
            this->enable();
            this->write_byte(c_data[0]);
            this->dc_pin_->digital_write(true);
            this->write_array(c_data + 1, length);
            this->disable();
        }

        void WaveshareEPaperTypeA::data_x_times(uint8_t data, uint16_t count) {
            for (int i=0; i<count; i++) {
                this->write_byte(data);
            }
        }

        void WaveshareEPaperTypeA::command(uint8_t cmd)
        {
            this->dc_pin_->digital_write(false);
            this->enable();
            this->write_byte(cmd);
            this->disable();
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