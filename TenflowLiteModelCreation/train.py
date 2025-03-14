import numpy as np
import pandas as pd
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

# Load dataset
df = pd.read_csv("karachi_ventilation_data_5_levels.csv")

# Split features and labels
X = df[["temperature", "humidity", "pm25", "voc"]].values
y = df["ventilation"].values

# Standardize the data
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# Split into train and test
X_train, X_test, y_train, y_test = train_test_split(
    X_scaled, y, test_size=0.2, random_state=42
)

# Define the TensorFlow model
model = tf.keras.Sequential(
    [
        tf.keras.layers.Dense(32, activation="relu", input_shape=(4,)),
        tf.keras.layers.Dense(32, activation="relu"),
        tf.keras.layers.Dense(5, activation="softmax"),  # 5 ventilation categories
    ]
)

model.compile(
    optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"]
)

# Train the model
model.fit(X_train, y_train, epochs=25, validation_data=(X_test, y_test))

# Convert to TensorFlow Lite
converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

# Save the TensorFlow Lite model
with open("ventilation_5_levels.tflite", "wb") as f:
    f.write(tflite_model)

# Testing TensorFlow Lite model
interpreter = tf.lite.Interpreter(model_path="ventilation_5_levels.tflite")
interpreter.allocate_tensors()
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# Sample data for testing (random realistic values)
sample_input = np.array([[30, 60, 120, 2.0]])  # moderate ventilation expected
sample_input_scaled = scaler.transform(sample_input)

interpreter.set_tensor(
    input_details[0]["index"], sample_input_scaled.astype(np.float32)
)
interpreter.invoke()
prediction = interpreter.get_tensor(output_details[0]["index"])

predicted_class = np.argmax(prediction)
ventilation_labels = ["No need", "Low", "Moderate", "High", "Urgent"]

print(f"Sample input: {sample_input}")
print(f"Predicted ventilation need: {ventilation_labels[predicted_class]}")
