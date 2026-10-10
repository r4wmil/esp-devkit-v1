/* WS2812 Solid Color Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "ws2812_solid";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 23

static led_strip_handle_t led_strip;

static void set_led_color(uint8_t red, uint8_t green, uint8_t blue)
{
	led_strip_set_pixel(led_strip, 0, red, green, blue);
	led_strip_set_pixel(led_strip, 1, red, green, blue);
	led_strip_set_pixel(led_strip, 2, red, green, blue);
	led_strip_refresh(led_strip);
}

static void configure_led(void)
{
	ESP_LOGI(TAG, "Configured to drive WS2812 on GPIO %d", BLINK_GPIO);
	led_strip_config_t strip_config = {
		.strip_gpio_num = BLINK_GPIO,
		.max_leds = 3,
	};
#if CONFIG_BLINK_LED_STRIP_BACKEND_RMT
	led_strip_rmt_config_t rmt_config = {
		.resolution_hz = 10 * 1000 * 1000, // 10MHz
		.flags.with_dma = false,
	};
	ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
#elif CONFIG_BLINK_LED_STRIP_BACKEND_SPI
	led_strip_spi_config_t spi_config = {
		.spi_bus = SPI2_HOST,
		.flags.with_dma = true,
	};
	ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
#else
#error "unsupported LED strip backend"
#endif
	led_strip_clear(led_strip);
}

void app_main(void)
{
	configure_led();

	while (1) {
		ESP_LOGI(TAG, "LED RED");
		set_led_color(32, 0, 0);
		vTaskDelay(pdMS_TO_TICKS(1000));

		ESP_LOGI(TAG, "LED OFF");
		led_strip_clear(led_strip);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
