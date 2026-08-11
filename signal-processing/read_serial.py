import serial

PORT = "/dev/cu.usbserial-0001"
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)

while True:
    line = ser.readline().decode(errors="ignore").strip()
    if line:
        print(line)