#include "mqtt_setup.h"
#include <esp_log.h>
#include <mqtt_client.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdlib.h>
#include <time.h>

static const char *TAG = "mqtt_setup";
static esp_mqtt_client_handle_t mqtt_client = NULL;
static int emulation_mode = 0;

#define PERIODIC_MODE 0
#define BURSTY_MODE 1

static void mqtt_event_handler(void *handler_args, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            esp_mqtt_client_subscribe(mqtt_client, "/topic/qos0", 0);
            if (emulation_mode == PERIODIC_MODE) {
                ESP_LOGI(TAG, "Starting periodic sensor data publishing...");
            } else if (emulation_mode == BURSTY_MODE) {
                ESP_LOGI(TAG, "Starting bursty event data publishing...");
            }
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%" PRIi32, event_id);
            break;
    }
}

static void publish_periodic_data(void *pvParameters) {
    char payload[100];
    while (1) {
        float temperature = 20.0 + (rand() % 1000) / 100.0;
        float humidity = 50.0 + (rand() % 1000) / 100.0;
        float pressure = 1000.0 + (rand() % 1000) / 10.0;
        snprintf(payload, sizeof(payload), "{\"temperature\": %.2f, \"humidity\": %.2f, \"pressure\": %.2f}", temperature, humidity, pressure);
        esp_mqtt_client_publish(mqtt_client, "/sensors/data", payload, 0, 0, 0);
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Publish every 5 seconds
    }
}

static void publish_bursty_data(void *pvParameters) {
    char payload[100];
    while (1) {
        if (rand() % 10 < 3) { // 30% chance to send data in a burst
            float gyroscope = (rand() % 2000) / 100.0 - 10.0;
            float accelerometer = (rand() % 2000) / 100.0 - 10.0;
            snprintf(payload, sizeof(payload), "{\"gyroscope\": %.2f, \"accelerometer\": %.2f}", gyroscope, accelerometer);
            esp_mqtt_client_publish(mqtt_client, "/sensors/event", payload, 0, 0, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS); // Burst interval
        } else {
            vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait 1 second before next possible burst
        }
    }
}

void mqtt_set_mode(int mode) {
    emulation_mode = mode;
}

void mqtt_app_start(esp_mqtt_client_config_t *mqtt_cfg) {
    mqtt_client = esp_mqtt_client_init(mqtt_cfg);
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return;
    }

    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, mqtt_client);
    esp_mqtt_client_start(mqtt_client);

    // Create tasks for periodic or bursty data publishing based on the mode
    if (emulation_mode == PERIODIC_MODE) {
        xTaskCreate(publish_periodic_data, "publish_periodic_data", 4096, NULL, 5, NULL);
    } else if (emulation_mode == BURSTY_MODE) {
        xTaskCreate(publish_bursty_data, "publish_bursty_data", 4096, NULL, 5, NULL);
    }
}
