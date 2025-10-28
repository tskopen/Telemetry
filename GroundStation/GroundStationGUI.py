import dearpygui.dearpygui as dpg
import serial
import collections
import time

ser = serial.Serial(
    port='/dev/cu.usbserial-0001', 
    baudrate=115200,                
    timeout=1
)

data_buffers = {
    "MX": collections.deque(maxlen=500),
    "MY": collections.deque(maxlen=500),
    "MZ": collections.deque(maxlen=500),
    
    "GX": collections.deque(maxlen=500),
    "GY": collections.deque(maxlen=500),
    "GZ": collections.deque(maxlen=500)
}



time_buffer = collections.deque(maxlen=500)
start_time = time.time()

latest_value = [0.0]

def read_serial_data():
    while ser.in_waiting > 0:
        line = ser.readline().decode(errors='ignore').strip()
        
        current_time = time.time() - start_time  # Time since start
                        
        time_buffer.append(current_time)
        
        if ':' in line:
            #Base setup
            ID, val_str = line.split(':', 1)
            ID = ID.strip()
            val_str = val_str.strip()
            try:
                value = float(val_str)
            except ValueError:
                print(f"Ignored non-numeric line: {line}")
                return

            #Data type split
            if ID in data_buffers:
                data_buffers[ID].append(value)

            
dpg.create_context()

with dpg.window(label="Mango Serial Plot", width=450, height=450, pos=(0,0)):
    dpg.add_text("Reading serial data...")
    dpg.add_separator()
    
    with dpg.plot(label="Axis Data (MX, MY, MZ)", height=400, width=-1):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="Time", tag="x_axis_M")
        dpg.add_plot_axis(dpg.mvYAxis, label="Value", tag="y_axis_M")
        
        line_series = dpg.add_line_series([], [], label="MX", parent="y_axis_M", tag="MX_data_series")        
        line_series = dpg.add_line_series([], [], label="MZ", parent="y_axis_M", tag="MZ_data_series")        
        line_series = dpg.add_line_series([], [], label="MY", parent="y_axis_M", tag="MY_data_series")        

        dpg.set_axis_limits("y_axis_M", -180, 180)


with dpg.window(label="Gyro Serial Plot", width=450, height=450, pos=(450,0)):
    dpg.add_text("Reading serial data...")
    dpg.add_separator()
    
    with dpg.plot(label="Axis Data (GX, GY, GZ)", height=400, width=-1):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="Time", tag="x_axis_G")
        dpg.add_plot_axis(dpg.mvYAxis, label="Value", tag="y_axis_G")
        
        line_series = dpg.add_line_series([], [], label="GX", parent="y_axis_G", tag="GX_data_series")        
        line_series = dpg.add_line_series([], [], label="GZ", parent="y_axis_G", tag="GZ_data_series")        
        line_series = dpg.add_line_series([], [], label="GY", parent="y_axis_G", tag="GY_data_series")

        dpg.set_axis_limits("y_axis_G", -180, 180)



def update_display():
    read_serial_data()
    if len(time_buffer) > 0:
        for axis in ["MX", "MY", "MZ", "GX", "GY", "GZ"]:
            if len(data_buffers[axis]) > 0:
                dpg.set_value(f"{axis}_data_series", [list(time_buffer), list(data_buffers[axis])])

    dpg.fit_axis_data("x_axis_M")
    dpg.fit_axis_data("x_axis_G")


dpg.create_viewport(title='Experimental ground station', width=1200, height=1200)
dpg.setup_dearpygui()
dpg.show_viewport()


while dpg.is_dearpygui_running():
    update_display()
    dpg.render_dearpygui_frame()
    time.sleep(0.02)
    
dpg.destroy_context()


    
