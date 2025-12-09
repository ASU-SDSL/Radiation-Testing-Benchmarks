# serial monitor and saver
import serial
import os
import sys
from datetime import datetime
import serial.tools.list_ports as sp
import multiprocessing 

# create data folder if doesn't exist
folder_path = "data"
os.makedirs(folder_path, exist_ok=True)

# User enter serial port
ports = list(sp.comports())
for i in ports:
    print(i)
print("Enter the COM Port (COM4, COM5, COM9, COM12, etc.) ")
comport = str(input())
print()

# Open Serial Port,
# usb so no UART params
ser = serial.Serial(port=comport, timeout=None)

now = datetime.now()
fileName = f"rad_data_{now.strftime('%Y_%m_%d_%H_%M_%S')}.txt"

if len(sys.argv) > 1:
    fileName = f"{sys.argv[1]}_{now.strftime('%Y_%m_%d_%H_%M_%S')}.txt"

# create file
file = open(os.path.join(folder_path, fileName), "a")
file.close()

error = False 
while 1:
    try:
        if error: 
            error = False
            ser.open()
        with open(os.path.join(folder_path, fileName), "a") as f:
            data = ser.read(64).decode(encoding="utf-8", errors="replace")
            
            print(data, end="")
            f.write(data)
            
    except KeyboardInterrupt:
        break
    except (serial.SerialException, BrokenPipeError, OSError):
        ser.close()
        error = True
