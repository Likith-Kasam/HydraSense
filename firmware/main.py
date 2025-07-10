import serial
import time
import csv

# Replace 'COM6' with your ESP32 port (check Arduino > Tools > Port)
SERIAL_PORT = 'COM6'
BAUD_RATE = 115200
CSV_FILE = 'sensor_data_again.csv'

def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
    except:
        print(f"❌ Could not connect to {SERIAL_PORT}")
        return

    with open(CSV_FILE, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Timestamp', 'DHT22_Temp', 'DHT22_Humidity', 'DS18B20_Temp', 'BME680_Temp', 'BME680_Humidity', 'BME680_Pressure', 'BME680_Gas'])

        while True:
            try:
                line = ser.readline().decode('utf-8').strip()
                print(line)

                # Parse data only if all sensor readings are present
                if "DHT22" in line:
                    dht_temp = dht_hum = ds_temp = bme_temp = bme_hum = bme_press = bme_gas = ""

                if "DHT22 -" in line:
                    parts = line.split()
                    dht_temp = parts[3]
                    dht_hum = parts[7]

                elif "DS18B20 -" in line:
                    parts = line.split()
                    ds_temp = parts[3]

                elif "BME680 -" in line:
                    parts = line.split()
                    bme_temp = parts[3]
                    bme_hum = parts[7]
                    bme_press = parts[11]
                    bme_gas = parts[15]

                    timestamp = time.strftime('%Y-%m-%d %H:%M:%S')
                    writer.writerow([timestamp, dht_temp, dht_hum, ds_temp, bme_temp, bme_hum, bme_press, bme_gas])
                    file.flush()

            except KeyboardInterrupt:
                print("Logging stopped by user.")
                break

if __name__ == '__main__':
    main()
