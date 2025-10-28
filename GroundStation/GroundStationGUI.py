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
    "MX": collections.deque(maxlen=300),
    "MY": collections.deque(maxlen=300),
    "MZ": collections.deque(maxlen=300)
}



time_buffer = collections.deque(maxlen=100)
start_time = time.time()

latest_value = [0.0]
def read_serial_data():
    line = ser.readline().decode().strip()

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

with dpg.window(label="Live Serial Plot", width=800, height=600):
    dpg.add_text("Reading serial data...")
    dpg.add_separator()
    
    with dpg.plot(label="Axis Data (MX, MY, MZ)", height=400, width=-1):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="Time", tag="x_axis")
        dpg.add_plot_axis(dpg.mvYAxis, label="Value", tag="y_axis")
        
        line_series = dpg.add_line_series([], [], label="MX", parent="y_axis", tag="MX_data_series")        
        line_series = dpg.add_line_series([], [], label="MZ", parent="y_axis", tag="MZ_data_series")        
        line_series = dpg.add_line_series([], [], label="MY", parent="y_axis", tag="MY_data_series")        

def update_display():
    read_serial_data()
    if len(time_buffer) > 0:
        for axis in ["MX", "MY", "MZ"]:
            if len(data_buffers[axis]) > 0:
                dpg.set_value(f"{axis}_data_series", [list(time_buffer), list(data_buffers[axis])])


dpg.create_viewport(title='Experimental ground station', width=800, height=600)
dpg.setup_dearpygui()
dpg.show_viewport()


while dpg.is_dearpygui_running():
    update_display()
    dpg.render_dearpygui_frame()
    time.sleep(0.02)
    
dpg.destroy_context()


    
