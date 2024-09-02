import paho.mqtt.client as mqtt
import random
import string
import time
import json
import os
import threading
CONNECTION_TYPES = {1: "TLS", 2: "WireGuard", 3: "Unencrypted"}
DEVICES = {1: "ESP32", 2: "ESP8266", 3: "Raspi"}
conn_type = int(input('\nConnection type?\n1: TLS\n2: WireGuard\n3: Unencrypted\nSelect type: '))
CONNECTION_TYPE = CONNECTION_TYPES.get(conn_type, None)
if not CONNECTION_TYPE:
    print("Wrong input")
    exit()
print(f'Test connection type set to {CONNECTION_TYPE}')
device = int(input('\nWhat is the device being tested?\n1: ESP32\n2: ESP8266\n3: Raspberry Pi\nSelect type: '))
dev_name = DEVICES.get(device)
PAYLOAD_SIZES = [2**i for i in range(1, 13)]
clientid = input('\nClient ID? (Must match the one set in menuconfig of device being tested.)\n')
BROKER_ADDRESS = "192.168.0.199"
PORT = 1883
USERNAME = "raspi"
PASSWORD = "gc5459"
REQUEST_TOPIC = f'request{clientid}'
RESPONSE_TOPIC = f'response{clientid}'
NUM_ITERATIONS = 1000
DELAY_BETWEEN_ITERATIONS = 0.4
received_payload = None
def generate_random_payload(size):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=size))
def on_message(client, userdata, message):
    global received_payload
    if message.topic == RESPONSE_TOPIC:
        received_payload = message.payload.decode()
def run_client():
    client.loop_forever()
def publish_and_wait(client, payload, timeout=5):
    global received_payload
    received_payload = None
    start_time = time.perf_counter_ns()
    client.publish(REQUEST_TOPIC, payload, qos=1)
    while received_payload is None:
        if (time.perf_counter_ns() - start_time) > timeout * 1e9:
            raise TimeoutError("Response timed out")
        time.sleep(0.001)
    return (time.perf_counter_ns() - start_time) / 1e6
results_file = f"{dev_name}_results.json"
if os.path.exists(results_file):
    with open(results_file, "r") as f:
        results = json.load(f)
else:
    results = {"TLS": {}, "WireGuard": {}, "Unencrypted": {}}
client = mqtt.Client()
client.username_pw_set(USERNAME, PASSWORD)
client.on_message = on_message
client.connect(BROKER_ADDRESS, PORT, 60)
client.subscribe(RESPONSE_TOPIC, qos=1)
thread = threading.Thread(target=run_client, daemon=True)
thread.start()
for payload_size in PAYLOAD_SIZES:
    results[CONNECTION_TYPE].setdefault(payload_size, [])
    print(f"\nTesting payload size: {payload_size} bytes")
    for i in range(NUM_ITERATIONS):
        random_payload = generate_random_payload(payload_size)
        try:
            rtt = round(publish_and_wait(client, random_payload), 3)
            results[CONNECTION_TYPE][payload_size].append(rtt)
        except TimeoutError as e:
            print(f"Iteration {i + 1}/{NUM_ITERATIONS}: {str(e)}")
        time.sleep(DELAY_BETWEEN_ITERATIONS)
with open(results_file, "w") as f:
    json.dump(results, f)
client.disconnect()
