idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
			  REQUIRES esp_wifi lwip nvs_flash mqtt driver)
