import serial
import time

def read_weight_data(port='/dev/ttyUSB0', baudrate=9600):
    ser = serial.Serial(port, baudrate)
    weights = []

    for _ in range(100):  # Collect 100 readings
        line = ser.readline().decode('utf-8').strip()
        weights.append(float(line))
        time.sleep(1)  # Wait for 1 second between readings

    ser.close()
    return weights

if __name__ == "__main__":
    weights = read_weight_data()
    # Save weights for further processing
    with open('weights.txt', 'w') as f:
        for weight in weights:
            f.write(f"{weight}\n")