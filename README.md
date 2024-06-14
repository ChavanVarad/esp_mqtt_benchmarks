
## Getting Started

### Prerequisites

- [ESP-IDF](https://github.com/espressif/esp-idf) version 5.3 or later.
- A configured ESP32 development environment.

### Installation

1. **Clone the repository**:
    ```sh
    git clone https://github.com/yourusername/espidf-wireguard_benchmark.git
    cd espidf-wireguard_benchmark
    ```

2. **Set up the ESP-IDF environment**:
    Follow the [Getting Started Guide for ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html).

### Configuration

1. **WiFi Configuration**:
   Configure the WiFi settings in `sdkconfig`:
    ```sh
    idf.py menuconfig
    ```

2. **WireGuard Configuration**:
   Add your WireGuard configuration details to `sdkconfig`.

3. **MQTT Configuration**:
   Set the MQTT broker URI in `main.c`:
    ```c
    static const char *MQTT_BROKER_URI = "mqtt://your_broker_uri";
    ```

4. **Emulation Mode**:
   Set the emulation mode in `main.c`:
    ```c
    static int emulation_mode = PERIODIC_MODE;  // Set to BURSTY_MODE for bursty data
    ```

### Build and Flash

1. **Build the project**:
    ```sh
    idf.py build
    ```

2. **Flash the project to your ESP32**:
    ```sh
    idf.py -p /dev/ttyUSB0 flash monitor
    ```

## Usage

- The application will connect to the specified WiFi network.
- It will establish a WireGuard VPN connection.
- SNTP will synchronize the system time.
- Depending on the configured mode, it will start publishing sensor data to the MQTT broker either periodically or in bursty mode.

## Configuration Options

- **WiFi Settings**: Configure SSID, password, and maximum retry count in `sdkconfig`.
- **WireGuard Settings**: Configure WireGuard keys, ports, and peer settings in `sdkconfig`.
- **MQTT Settings**: Configure the MQTT broker URI in `main.c`.
- **Emulation Mode**: Switch between `PERIODIC_MODE` and `BURSTY_MODE` in `main.c`.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
