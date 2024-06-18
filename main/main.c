#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include "wifi_setup.h"
#include "wireguard_setup.h"
#include "mqtt_setup.h"
#include "sync_time.h"
#include "ping_functions.h"

static const char *TAG = "main_app";

// MQTT Configuration
static const char *MQTT_BROKER_URI = "mqtt://172.17.0.1";

// Emulation Mode: 0 for Periodic, 1 for Bursty
static int emulation_mode = 0;  // Set to 0 for Periodic, 1 for Bursty

void app_main(void) {
    esp_err_t err;
    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];
    wireguard_ctx_t ctx = {0};

    esp_log_level_set("esp_wireguard", ESP_LOG_DEBUG);
    esp_log_level_set("wireguardif", ESP_LOG_DEBUG);
    esp_log_level_set("wireguard", ESP_LOG_DEBUG);
    err = nvs_flash_init();
#if defined(CONFIG_IDF_TARGET_ESP8266) && ESP_IDF_VERSION <= ESP_IDF_VERSION_VAL(3, 4, 0)
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
#else
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
#endif
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = wifi_init_sta();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "wifi_init_sta: %s", esp_err_to_name(err));
        goto fail;
    }

    obtain_time();
    time(&now);

    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in New York is: %s", strftime_buf);

    err = wireguard_setup(&ctx);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "wireguard_setup: %s", esp_err_to_name(err));
        goto fail;
    }

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        err = esp_wireguardif_peer_is_up(&ctx);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Peer is up");
            break;
        } else {
            ESP_LOGI(TAG, "Peer is down");
        }
    }

    mqtt_set_mode(emulation_mode);

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
    };

    ESP_LOGI(TAG, "MQTT Broker URI: %s", MQTT_BROKER_URI);  // Output the broker URI

    mqtt_app_start(&mqtt_cfg);  // Start the MQTT client after WireGuard is connected

    // Continuous loop to keep the main task alive
    while (1) {
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "Main task alive.");
    }

fail:
    ESP_LOGE(TAG, "Halting due to error");
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
