##Python conversion
import time
import board
import busio
import adafruit_lsm6ds.lsm6dsox
import adafruit_lis3mdl

# Initialize I2C bus
i2c = busio.I2C(board.SCL, board.SDA, frequency=400000)

# Initialize LSM6DSOX accelerometer/gyroscope
try:
    lsm6dsox = adafruit_lsm6ds.lsm6dsox.LSM6DSOX(i2c)
    print("LSM6DSOX found!")
except ValueError as e:
    print("Error initializing LSM6DSOX:", e)
    exit(1)

# Configure LSM6DSOX settings
lsm6dsox.accelerometer_range = adafruit_lsm6ds.ACCEL_RANGE_2G
lsm6dsox.gyro_range = adafruit_lsm6ds.GYRO_RANGE_125_DPS
lsm6dsox.accelerometer_data_rate = 208
lsm6dsox.gyro_data_rate = 208

# Initialize LIS3MDL magnetometer
try:
    lis3mdl = adafruit_lis3mdl.LIS3MDL(i2c)
    print("LIS3MDL found!")
except ValueError as e:
    print("Error initializing LIS3MDL:", e)
    exit(1)

# Configure LIS3MDL settings
lis3mdl.performance_mode = adafruit_lis3mdl.MediumMode
lis3mdl.operation_mode = adafruit_lis3mdl.CONTINUOUSMODE
lis3mdl.data_rate = 155  # Hz
lis3mdl.range = adafruit_lis3mdl.RANGE_4GAUSS

# Print configuration
print("\nSensor Configuration:")
print(f"Accelerometer: {lsm6dsox.accelerometer_range}g range, {lsm6dsox.accelerometer_data_rate}Hz")
print(f"Gyroscope: {lsm6dsox.gyro_range}dps range, {lsm6dsox.gyro_data_rate}Hz")
print(f"Magnetometer: {lis3mdl.range} range, {lis3mdl.data_rate}Hz, {lis3mdl.performance_mode} mode\n")

def main():
    while True:
        # Read and print accelerometer data (in m/s^2)
        accel_x, accel_y, accel_z = lsm6dsox.acceleration
        print(f"Acceleration X: {accel_x:.2f} m/s², Y: {accel_y:.2f} m/s², Z: {accel_z:.2f} m/s²")
        
        # Read and print gyroscope data (in rad/s)
        gyro_x, gyro_y, gyro_z = lsm6dsox.gyro
        print(f"Rotation X: {gyro_x:.2f} rad/s, Y: {gyro_y:.2f} rad/s, Z: {gyro_z:.2f} rad/s")
        
        # Read and print magnetometer data (in uTesla)
        mag_x, mag_y, mag_z = lis3mdl.magnetic
        print(f"Magnetic X: {mag_x:.2f} uT, Y: {mag_y:.2f} uT, Z: {mag_z:.2f} uT")
        print()
        
        time.sleep(0.1)  # Small delay similar to your Arduino code

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nProgram stopped by user")
