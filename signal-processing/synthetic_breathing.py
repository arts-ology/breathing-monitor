#not using ANY data from the sensor because theres no way to put it on the chest and measure yet 

import numpy as np
import matplotlib.pyplot as plt
duration_seconds = 30       # how long the fake recording is
sample_rate = 100           # samples per second (matches your real sensor's ~100Hz rate)
breath_cycle_seconds = 4    # one full inhale+exhale every 4 seconds
noise_level = 0.05          # how much random jitter to mix in (small = subtle noise)

num_samples = duration_seconds * sample_rate
t = np.linspace(0, duration_seconds, num_samples)

#sine wave for breathing signal
breathing_freq_hz = 1 / breath_cycle_seconds
clean_signal = np.sin(2 * np.pi * breathing_freq_hz * t)

#adding random variation \
noise = np.random.normal(0, noise_level, num_samples)
noisy_signal = clean_signal + noise
#plot
plt.plot(t, noisy_signal, label="synthetic signal (with noise)", alpha=0.7, color="hotpink")
plt.plot(t, clean_signal, label="underlying clean wave", linewidth=2, color="blue")
plt.xlabel("Time (seconds)")
plt.ylabel("Simulated tilt / motion")
plt.title("Synthetic Breathing Signal")
plt.legend()
plt.show()

np.savetxt("synthetic_breathing_data.csv", noisy_signal, delimiter=",")
print("Saved synthetic_breathing_data.csv —", num_samples, "samples") 

