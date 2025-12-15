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
ser = serial.Serial(port=comport, timeout=None)

now = datetime.now()
filename = f"rad_data_{now.strftime('%Y_%m_%d_%H_%M_%S')}.txt"
metafilename = f"rad_data_{now.strftime('%Y_%m_%d_%H_%M_%S')}_metadata.csv"

if len(sys.argv) > 1:
    filename = f"{sys.argv[1]}_{now.strftime('%Y_%m_%d_%H_%M_%S')}.txt"
    metafilename = f"{sys.argv[1]}_{now.strftime('%Y_%m_%d_%H_%M_%S')}_metadata.csv"


# create file
file = open(os.path.join(folder_path, filename), "a")
file.close()

error = False

file = open(os.path.join(folder_path, metafilename), "a")
file.write("Timestamp, Bytes Received\n")
file.write(f"{datetime.now()}, 0\n")
file.close()

last_time = datetime.now()
bytes_received = 0
while 1:
    try:
        if error:
            error = False
            ser.open()
        with (
            open(os.path.join(folder_path, filename), "a") as f,
            open(os.path.join(folder_path, metafilename), "a") as fm,
        ):
            data = ser.read(64).decode(encoding="utf-8", errors="replace")
            bytes_received += 64  # mark reception

            if (datetime.now() - last_time) > timedelta(
                seconds=METADATA_MARK_TIME_PERIOD_S
            ):
                fm.write(f"{datetime.now()}, {bytes_received}\n")
                last_time = datetime.now()

            # save
            print(data, end="")
            f.write(data)

    except KeyboardInterrupt:
        break
    except (serial.SerialException, BrokenPipeError, OSError):
        ser.close()
        error = True
