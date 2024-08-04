# Define variables
CHIP := esp32-1 # Set to "esp32", "esp8266", or "raspi"

COMPONENT_EMBED_TXTFILES := certs/$(CLIENTID).crt certs/$(CLIENTID).key ca.crt
