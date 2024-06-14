#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <mqtt_client.h>

void mqtt_app_start(esp_mqtt_client_config_t *mqtt_cfg);
void mqtt_set_mode(int mode);

#endif // MQTT_SETUP_H
