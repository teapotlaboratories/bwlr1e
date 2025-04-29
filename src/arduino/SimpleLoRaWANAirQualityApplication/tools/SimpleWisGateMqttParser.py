#!/usr/bin/env python3

import random
import json
import base64
import packet

from paho.mqtt import client as mqtt_client

broker = '192.168.224.19'
port = 1883
topic = "application/+/device/+/rx"

# generate client ID with pub prefix randomly
client_id = f'SimpleWisGateMqttParser-{random.randint(0, 65535)}'

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        try:
            sensor_report_json = json.loads(msg.payload.decode())
            sensor_data_bytes = base64.b64decode(sensor_report_json["data"].encode('ascii'))
            parser = packet.Parser.Parse( sensor_data_bytes )
            print("Receive {} report".format(len(parser)))
            for pckt in parser:
                print("Packet type: {}".format(pckt))
                print(json.dumps(pckt.GetAsDict(), indent=4, default=lambda o: '<not serializable>'))
       
        except Exception as e:
            print("exception occurs: {}".format(e))
    
    client.subscribe(topic)
    client.on_message = on_message


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()
