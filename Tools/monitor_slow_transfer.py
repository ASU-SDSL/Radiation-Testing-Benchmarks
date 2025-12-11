# serial monitor and saver
import serial
import os
import sys
from datetime import datetime, timedelta
import serial.tools.list_ports as sp

METADATA_MARK_TIME_PERIOD_S = 5

# create data folder if doesn't exist
folder_path = "data"
os.makedirs(folder_path, exist_ok=True)

# User enter serial port
ports = list(sp.comports())
for i in ports:
    if "n/a" not in i:
        print(i)
print("Enter the port ID ")
comport = str(input())
print()

# Open Serial Port,
# usb so no UART params
ser = serial.Serial(port=comport, timeout=0)

now = datetime.now()
filename = f"rad_data_{now.strftime('%Y_%m_%d_%H_%M_%S')}.txt"

if len(sys.argv) > 1:
    filename = f"{sys.argv[1]}_{now.strftime('%Y_%m_%d_%H_%M_%S')}.txt"

# create file
file = open(os.path.join(folder_path, filename), "a")
file.close()

error = False

while 1:
    try:
        if error:
            error = False
            ser.open()
        with open(os.path.join(folder_path, filename), "a") as f:
            data = ser.readline().decode(encoding="utf-8", errors="replace")

            # save
            print(data, end="")
            f.write(data)

    except KeyboardInterrupt:
        break
    except (serial.SerialException, BrokenPipeError, OSError):
        ser.close()
        error = True
