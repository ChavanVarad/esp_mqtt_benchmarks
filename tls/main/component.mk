# Define variables
CHIP := esp32 # Set to "esp32" or "esp8266"
CLIENTID := 1 # Set to "1", "2", or "3"

COMPONENT_EMBED_TXTFILES := $(CHIP)-certs/$(CHIP)-$(CLIENTID).crt $(CHIP)-certs/$(CHIP)-$(CLIENTID).key ca.crt

all:
    @echo $(COMPONENT_EMBED_TXTFILES)