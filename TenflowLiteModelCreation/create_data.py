import pandas as pd
import numpy as np
import os

np.random.seed(42)
samples = 1000

data = {
    "temperature": np.random.uniform(10, 50, samples).round(2),
    "humidity": np.random.uniform(30, 80, samples).round(2),
    "pm25": np.random.exponential(100, samples).clip(0, 500).round(),
    "voc": np.random.normal(1.5, 1.0, samples).clip(0, 5).round(2),
}

df = pd.DataFrame(data)


def ventilation_need(row):
    if row["pm25"] > 300 or row["voc"] > 5.0:
        return 4  # Urgent ventilation
    elif row["pm25"] > 200 or row["voc"] > 4.0:
        return 3  # High ventilation
    elif row["pm25"] > 100 or row["voc"] > 3.0:
        return 2  # Moderate ventilation
    elif row["pm25"] > 50 or row["voc"] > 2.5:
        return 1  # Low ventilation
    else:
        return 0  # No ventilation needed


df["ventilation"] = df.apply(ventilation_need, axis=1)

# Save the data explicitly to the current directory
file_path = os.path.join(os.getcwd(), "karachi_ventilation_data_5_levels.csv")
df.to_csv(file_path, index=False)

print(f"Generated file at: {file_path}")
