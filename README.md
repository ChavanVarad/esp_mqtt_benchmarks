# Introduction

This is the public GitHub repo for Varad Chavan's Cyber Security Master's Dissertation at Lancaster University.

This guide assumes that the local network tested on is in the IP range `192.168.0.0/24`, and the Raspberry Pi server has been assigned a static IP address `192.168.0.199`. Setup for all three ESP chips when testing threat model haven't been provided here, just the test metrics.

#### Note: There are public/private keypairs provided here that were used in a local isolated environment, hence aren't considered "unsafe" for tets purposes. This is purely for convenience. Use newly generated secrets for other deployments.

# ESP32 and ESP8266 setup

## Building
To set up and run the ESP MQTT Benchmarks project, follow these instructions:

### Step 1: Clone the Repository

Start by cloning the repository to your local machine using the following command:

```bash
git clone https://github.com/ChavanVarad/esp_mqtt_benchmarks
```

### Step 2: Navigate to the Project Directory

Change into the project directory:

```bash
cd esp_mqtt_benchmarks
```

### Step 3: Configuration for Unencrypted MQTT

1. Navigate to the unencrypted MQTT configuration directory:

    ```bash
    cd ue
    ```

2. Run the menu configuration utility:

`make menuconfig` in the case of ESP8266_RTOS_SDK (for ESP8266). `idf.py menuconfig` in the case of ESP-IDF (for ESP32).

3. **WiFi Configuration**:

    - **WiFi SSID**: Enter the SSID of your border router.
    - **WiFi Password**: Enter the password of your border router.
    - **Use TCPIP Adapter**: Set to `true` if compiling for ESP8266.
    - **Use ESP Netif**: Set to `true` if compiling for ESP32.

4. **MQTT Broker Configuration**:

    - **Broker URL**: Set to the actual URL or IP address of your MQTT broker, prepended with `mqtt://`.
    - **Client Username**: Set to either `esp32-1` or `esp8266-1` depending on the chip you are compiling for.
    - **Client Password**: Set to the broker password. In this case, use `"gc5459"`.

5. Exit the configuration menu and save your settings.


### Step 4: Configuration for TLS MQTT

1. Navigate to the unencrypted MQTT configuration directory:

    ```bash
    cd tls
    ```

2. Run the menu configuration utility:

`make menuconfig` in the case of ESP8266_RTOS_SDK (for ESP8266). `idf.py menuconfig` in the case of ESP-IDF (for ESP32).

3. **WiFi Configuration**:

    - **WiFi SSID**: Enter the SSID of your border router.
    - **WiFi Password**: Enter the password of your border router.
    - **Use TCPIP Adapter**: Set to `true` if compiling for ESP8266.
    - **Use ESP Netif**: Set to `true` if compiling for ESP32.

4. **MQTT Broker Configuration**:

    - **Broker URL**: Set to the actual URL or IP address of your MQTT broker, prepended with `mqtts://`.
    - **Client Username**: Set to either `esp32-1` or `esp8266-1` depending on the chip you are compiling for.
    - **Client Password**: Set to the broker password. In this case, use `"gc5459"`.

5. Exit the configuration menu and save your settings.

### Step 6: Configuration for WireGuard MQTT

1. Navigate to the unencrypted MQTT configuration directory:

    ```bash
    cd wg
    ```

2. Run the menu configuration utility:

`make menuconfig` in the case of ESP8266_RTOS_SDK (for ESP8266). `idf.py menuconfig` in the case of ESP-IDF (for ESP32).

3. **WiFi Configuration**:

    - **WiFi SSID**: Enter the SSID of your border router.
    - **WiFi Password**: Enter the password of your border router.
    - **Use TCPIP Adapter**: Set to `true` if compiling for ESP8266.
    - **Use ESP Netif**: Set to `true` if compiling for ESP32.

4. **MQTT Broker Configuration**:

    - **Broker URL**: Set to the actual URL or IP address of your MQTT broker, prepended with `mqtt://{wireguard_local_ip}`.
    - **Client Username**: Set to either `esp32-1` or `esp8266-1` depending on the chip you are compiling for.
    - **Client Password**: Set to the broker password. In this case, use `"gc5459"`.

5. **WireGuard peer configuration and component setting**:
    - **Wireguard Private Key**: Set to private key generated for the ESP32 or ESP8266. In this case, `qCEY2fFpeBmCvlAyvwVL15z9qxbeItNx7dakCI8hn0I=`.
    - **Wireguard local IP address**: Set to WireGuard IP to be used for the ESP32 or ESP8266. In this case, `10.8.0.2`.
    - **Wireguard local netmask**: Subnet mask used by WireGuard virtual IP range. Since its `\24` here, use `255.255.255.0`.
    - **Wireguard local port**: Port to be used for ingress and egress WireGuard traffic on the ESP chips. Here, `51820`.
    - **Wireguard remote peer public key**: Public key of the Raspberry Pi WireGuard server. Here, `4IVGEBmd+HqfDH28X7P8wtXKUPlcLm3l5Cb05b4i7XA=`.
    - **Wireguard pre-shared symmetric key**: Pre-shared key set for the specific device. Here, `+Centm7O8bJfRMD+0/eUD6hhnl1W8f11KtGnTebGbg4=`.
    - **Wireguard remote peer address**: Local IP address of the Raspberry Pi. For our setup, `192.168.0.199`.
    - **Wireguard remote peer port**: Port used at Raspberry Pi for WireGuard connection. In this case, `51820`.

5. Exit the configuration menu and save your settings.

## Flashing

For flashing, `make flash` in the case of ESP8266_RTOS_SDK (for ESP8266). `idf.py flash` in the case of ESP-IDF (for ESP32).

# Raspberry Pi setup

## Configurations

1. **Operating System**: Flash the Raspberry Pi with `Raspbian (Bookworm)` using the Raspberry Pi imager.
2. **Update repositories**: `sudo apt update` and `sudo apt upgrade` before installing any necessary packages.
3. **Required packages**:
   ```bash
   sudo apt install wireguard-tools chrony mosquitto mosquitto-clients
   ```
4. **Clone repository configurations**:
   ```bash
   cd /etc/

   sudo git clone https://github.com/ChavanVarad/esp_mqtt_benchmarks/tree/main/raspberry_pi_configurations
   ```
6. **Permissions for mosquitto broker**:
   ```bash
   sudo chown -R mosquitto:mosquitto /etc/mosquitto/
   ```
7. **Start services**:
   ```bash
   sudo systemctl enable mosquitto chrony
   sudo systemctl enable wg-quick@wg0.service
   sudo systemctl daemon-reload
   sudo systemctl start mosquitto chrony wg-quick@wg0.service
   ```
