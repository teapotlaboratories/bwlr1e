#!/usr/bin/env python3

import random
import json
import base64
import packet
import time
import os
import os.path
import sys
import argparse
import uuid
import csv
import flatdict

from paho.mqtt import client as mqtt_client

kCsvDirectory = None

# application/1 uses the simple payload encoding
kTopic = "application/{}/device/+/rx"

# generate client ID with pub prefix randomly
kClientId = "SLAWGCL_{}".format( str(uuid.uuid4()) )

def LogReportToCSV( timestamp: int, dev_eui: str, pckt, csv_dir ):
    if not isinstance( pckt, packet.Packet ):
        return false

    write_header = False

    ## prepare data
    # flatten the dictionary and get header
    data = dict(flatdict.FlatDict(pckt.GetAsDict(), delimiter='.'))
    csv_header = list(data.keys())
    # add timestamp
    timestamp_key = "timestamp"
    data[timestamp_key] = timestamp
    csv_header = [timestamp_key] + csv_header
    
    # generate filename
    filename = "{}_{}.csv".format(dev_eui, pckt.GetName())
    filepath = os.path.join(csv_dir, filename)

    # if file exist don't write header
    write_header = not os.path.exists(filepath)
    with open(filepath, 'a', encoding='UTF8', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=csv_header)
        if write_header:
            writer.writeheader()
        writer.writerows([data])
    return True        

def str2bool(v):
    if isinstance(v, bool):
        return v
    if v.lower() in ('yes', 'true', 't', 'y', '1'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected.')

if __name__ == '__main__':  
    parser = argparse.ArgumentParser()
    parser.add_argument("--debug", type=str2bool, nargs='?',
                        const=True, default=False,
                        help="Set debug on/off")
    parser.add_argument('--mqtt-host', type=str, help="WisGate MQTT Broker Host Address", required=True)
    parser.add_argument('--mqtt-port', type=int, help="WisGate MQTT Broker Port Address", required=True)
    parser.add_argument('--csv-directory', type=str, help="Path to CSV Log Output Directory", required=True)
    parser.add_argument('--application-id', type=str, help="WisGate Application ID of the sensors", required=True)
    args = parser.parse_args()

    if not os.path.exists(args.csv_directory):
        print("csv directory not exist. exitting...")
        exit(1)

    kCsvDirectory = args.csv_directory    
    kTopic = kTopic.format(args.application_id)

    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(kClientId)
    client.on_connect = on_connect
    client.connect(args.mqtt_host, args.mqtt_port)

    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        try:
            sensor_report_json = json.loads(msg.payload.decode())
            device_eui = sensor_report_json["devEUI"]
            timestamp  = sensor_report_json["timestamp"]
            sensor_data_bytes = base64.b64decode(sensor_report_json["data"].encode('ascii'))
            parser = packet.Parser.Parse( sensor_data_bytes )
            print("Receive {} report".format(len(parser)))
            for pckt in parser:
                print("Packet type: {}".format(pckt))
                print(json.dumps(pckt.GetAsDict(), indent=4, default=lambda o: '<not serializable>'))
                LogReportToCSV( timestamp, device_eui, pckt, kCsvDirectory )
       
        except Exception as e:
            print("exception occurs: {}".format(e))
    
    client.subscribe(kTopic)
    client.on_message = on_message

    client.loop_forever()