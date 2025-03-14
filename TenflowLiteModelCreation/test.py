import numpy as np
import tensorflow as tf
from sklearn.preprocessing import StandardScaler

# Initialize scaler with known dataset parameters (from training data)
scaler = StandardScaler()
scaler.mean_ = np.array([30, 55, 100, 1.5])
scaler.scale_ = np.array([10, 20, 100, 1])

# Load TFLite model
interpreter = tf.lite.Interpreter(model_path="ventilation_5_levels.tflite")
interpreter.allocate_tensors()

input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

ventilation_labels = ["No ventilation", "Low", "Moderate", "High", "Urgent"]

# Interactive loop
while True:
    try:
        temp = float(input("Enter temperature (°C): "))
        hum = float(input("Enter humidity (%): "))
        pm25 = float(input("Enter PM2.5 level: "))
        voc = float(input("Enter VOC level: "))

        user_input = np.array([[temp, hum, pm25, voc]])
        user_input_scaled = scaler.transform(user_input)

        interpreter.set_tensor(
            input_details[0]["index"], user_input_scaled.astype(np.float32)
        )
        interpreter.invoke()

        prediction = interpreter.get_tensor(output_details[0]["index"])
        predicted_class = np.argmax(prediction)

        print(f"Predicted ventilation need: {ventilation_labels[predicted_class]}\n")

    except Exception as e:
        print(f"Error: {e}\nPlease enter valid numeric inputs.\n")

    continue_prompt = input("Do you want to check another reading? (y/n): ").lower()
    if continue_prompt != "y":
        print("Exiting...")
        break
