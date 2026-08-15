import serial

PORT = "/dev/cu.usbserial-0001"
BAUD = 115200

print("Step 1: Attempting to open serial port...")
ser = serial.Serial(PORT, BAUD, timeout=1)
print("Step 2: Serial port opened successfully")

print("Step 3: Entering read loop, waiting for data...")
while True:
    line = ser.readline().decode(errors="ignore").strip()
    if line:
        print("Received:", line)
    else:
        print("Step 4: No data received in this read window (timeout)")