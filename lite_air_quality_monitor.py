import numpy as np
import json
import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient
from datetime import datetime
from sklearn.preprocessing import StandardScaler
from tflite_runtime.interpreter import Interpreter
import pandas as pd
import subprocess
import threading
import time

# InfluxDB client setup
influx_client = InfluxDBClient(host='localhost', port=8086, database='airquality')

# Setup scaler (use your actual training dataset statistics)
scaler = StandardScaler()
scaler.mean_ = np.array([30, 55, 100, 1.5])
scaler.scale_ = np.array([10, 20, 100, 1])

# Load TensorFlow Lite model
interpreter = Interpreter(model_path="ventilation_5_levels.tflite")
interpreter.allocate_tensors()
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

ventilation_labels = ["No ventilation", "Low", "Moderate", "High", "Urgent"]

# MQTT callback
def on_message(client, userdata, msg):
    payload = json.loads(msg.payload.decode())
    temp, hum, pm25, voc = payload['temperature'], payload['humidity'], payload['pm25'], payload['voc']

    # TFLite Prediction
    user_input = np.array([[temp, hum, pm25, voc]])
    user_input_scaled = scaler.transform(user_input)

    interpreter.set_tensor(input_details[0]['index'], user_input_scaled.astype(np.float32))
    interpreter.invoke()
    prediction = interpreter.get_tensor(output_details[0]['index'])
    predicted_class = np.argmax(prediction)
    ventilation_needed = ventilation_labels[predicted_class]

    # Prepare InfluxDB data
    json_body = [
        {
            "measurement": "ventilation_predictions",
            "time": datetime.utcnow().isoformat(),
            "fields": {
                "temperature": payload['temperature'],
                "humidity": payload['humidity'],
                "pm25": payload['pm25'],
                "voc": payload['voc'],
                "ventilation_level": predicted_class,
                "ventilation_label": ventilation_needed
            }
        }
    ]

    print(f"Writing to InfluxDB: {json_body[0]['fields']}")
    influx_client.write_points(json_body)

# MQTT setup
mqtt_client = mqtt.Client()
mqtt_client.connect("localhost", 1883, 60)
mqtt_client.subscribe("kammaaz/airquality/data")
mqtt_client.on_message = on_message
mqtt_client.loop_forever()
