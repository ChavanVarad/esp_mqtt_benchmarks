import paho.mqtt.client as mqtt
import random
import string
import time
import json
import os
import threading
import numpy as np
CONNECTION_TYPE = None
conn_type = int(input(f'\nConnection type?\n1: TLS\n2: WireGuard\n3: Unencrypted\nSelect type: '))
match conn_type:
    case 1:
        CONNECTION_TYPE = "TLS"
    case 2:
        CONNECTION_TYPE = "WireGuard"
    case 3:
        CONNECTION_TYPE = "Unencrypted"
    case _:
        print("Wrong input")
        exit()
print(f'Test connection type set to {CONNECTION_TYPE}')
device = int(input(f'\nWhat is the device being tested?\n1: ESP32\n2: ESP8266\n3: Raspberry Pi\nSelect type: '))
match device:
    case 1:
        dev_name = "ESP32"
    case 2:
        dev_name = "ESP8266"
    case 3:
        dev_name = "Raspi"
PAYLOAD_SIZE = 128
clientid = input(f'\nClient ID? (Must match the one set in menuconfig of device being tested.)\n')
BROKER_ADDRESS = "192.168.0.199"
PORT = 1883
USERNAME = "raspi"
PASSWORD = "gc5459"
REQUEST_TOPIC = f'request{clientid}'
RESPONSE_TOPIC = f'response{clientid}'
NUM_ITERATIONS = 50
DELAY_BETWEEN_ITERATIONS = [round(i, 2) for i in np.arange(0, 0.55, 0.01)]
received_payload = None
def generate_random_payload(size):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=size))
def on_message(client, userdata, message):
    global received_payload
    if message.topic == RESPONSE_TOPIC:
        received_payload = message.payload.decode()
client = mqtt.Client()
client.username_pw_set(USERNAME, PASSWORD)
client.on_message = on_message
client.connect(BROKER_ADDRESS, PORT, 60)
client.subscribe(RESPONSE_TOPIC, qos=1)
def run_client():
    client.loop_forever()
thread = threading.Thread(target=run_client)
thread.daemon = True
thread.start()
def publish_and_wait(client, payload, timeout=5):
    global received_payload
    received_payload = None
    start_time = time.perf_counter_ns()
    client.publish(REQUEST_TOPIC, payload, qos=1)
    while received_payload is None:
        elapsed_time = time.perf_counter_ns() - start_time
        if elapsed_time > (timeout * 1e9):
            raise TimeoutError("Response timed out")
    elapsed_time_ms = (time.perf_counter_ns() - start_time) / 1e6
    return round(elapsed_time_ms, 3)
if os.path.exists(f"{dev_name}_results.json"):
    with open(f"{dev_name}_results.json", "r") as f:
        individual_results = json.load(f)
else:
    individual_results = {"TLS": {}, "WireGuard": {}, "Unencrypted": {}}
for delay in DELAY_BETWEEN_ITERATIONS:
    if delay not in individual_results[CONNECTION_TYPE]:
        individual_results[CONNECTION_TYPE][delay] = []
    print(f"\nTesting delay: {delay} seconds")
    for i in range(NUM_ITERATIONS):
        random_payload = generate_random_payload(PAYLOAD_SIZE)
        try:
            rtt = publish_and_wait(client, random_payload)
            individual_results[CONNECTION_TYPE][delay].append(rtt)
            print(f"Iteration {i + 1}/{NUM_ITERATIONS}: Success, RTT = {rtt} milliseconds")
        except TimeoutError as e:
            print(f"Iteration {i + 1}/{NUM_ITERATIONS}: {str(e)}")
        time.sleep(delay)
with open(f'{dev_name}_results.json', "w") as f:
    json.dump(individual_results, f)
client.disconnect()
