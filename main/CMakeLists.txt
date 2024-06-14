idf_component_register(SRCS "src/wireguard_setup.c" "src/mqtt_setup.c" "src/wifi_setup.c" "src/ping_functions.c" "main.c" "src/sync_time.c"
                    INCLUDE_DIRS "."
                                "src"
                    REQUIRES esp_wifi lwip esp_wireguard nvs_flash mqtt)
