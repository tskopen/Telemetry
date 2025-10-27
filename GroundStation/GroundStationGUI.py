import dearpygui.dearpygui as dpg
import serial
import collections
import time

ser = serial.Serial(
    port='/dev/cu.usbserial-0001', 
    baudrate=115200,                
    timeout=1                       
)
data_buffer = collections.deque(maxlen=100)
time_buffer = collections.deque(maxlen=100)
start_time = time.time()

latest_value = [0.0]
def read_serial_data():
    line = ser.readline().decode().strip()
    value = float(line)
    current_time = time.time() - start_time  # Time since start
                    
    time_buffer.append(current_time)
    data_buffer.append(value)

dpg.create_context()

with dpg.window(label="Live Serial Plot", width=800, height=600):
    dpg.add_text("Reading serial data...")
    dpg.add_separator()
    
    with dpg.plot(label="Altitude Data", height=300, width=-1):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="Time", tag="x_axis")
        dpg.add_plot_axis(dpg.mvYAxis, label="Value", tag="y_axis")
        line_series = dpg.add_line_series([], [], label="Sensor", parent="y_axis", tag="data_series")        

def update_display():
    read_serial_data()
    dpg.set_value("data_series", [list(time_buffer), list(data_buffer)])

dpg.create_viewport(title='Experimental groaund station', width=800, height=600)
dpg.setup_dearpygui()
dpg.show_viewport()


while dpg.is_dearpygui_running():
    update_display()
    dpg.render_dearpygui_frame()
    time.sleep(0.01)
    
dpg.start_dearpygui()
dpg.destroy_context()


    
