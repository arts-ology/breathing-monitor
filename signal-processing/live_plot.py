import serial
import matplotlib.pyplot as plt
from collections import deque

PORT = "/dev/cu.usbserial-0001"
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)

max_points = 200
ax_data = deque(maxlen=max_points)
ay_data = deque(maxlen=max_points)
az_data = deque(maxlen=max_points)

plt.ion()
fig, ax = plt.subplots()
line_x, = ax.plot([], [], label="accelX")
line_y, = ax.plot([], [], label="accelY")
line_z, = ax.plot([], [], label="accelZ")
ax.legend()

while True:
    raw = ser.readline().decode(errors="ignore").strip()
    if not raw:
        continue

    parts = raw.split(",")
    if len(parts) != 6:
        continue

    try:
        ax_val, ay_val, az_val = float(parts[0]), float(parts[1]), float(parts[2])
    except ValueError:
        continue

    ax_data.append(ax_val)
    ay_data.append(ay_val)
    az_data.append(az_val)

    x_range = range(len(ax_data))
    line_x.set_data(x_range, ax_data)
    line_y.set_data(x_range, ay_data)
    line_z.set_data(x_range, az_data)

    ax.relim()
    ax.autoscale_view()
    plt.pause(0.001)