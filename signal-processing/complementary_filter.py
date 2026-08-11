import numpy as np
import matplotlib.pyplot as plt
noisy_signal = np.loadtxt("synthetic_breathing_data.csv", delimiter=",")

sample_rate = 100  # must match what was used to generate the data
num_samples = len(noisy_signal)
t = np.linspace(0, num_samples / sample_rate, num_samples)

alpha = 0.90 # will re-tune once real data is available 
filtered_signal = np.zeros(num_samples)
filtered_signal[0] = noisy_signal[0]  

for i in range(1, num_samples):
    filtered_signal[i] = alpha * filtered_signal[i - 1] + (1 - alpha) * noisy_signal[i]

# plot. 
plt.figure(figsize=(10, 4))
plt.plot(t, noisy_signal, label="raw noisy signal", alpha=0.5, color="hotpink")
plt.plot(t, filtered_signal, label=f"filtered (alpha={alpha})", linewidth=2, color="deeppink")
plt.xlabel("Time (seconds)")
plt.ylabel("Signal value")
plt.title("Complementary Filter — Raw vs Filtered")
plt.legend()
plt.show()