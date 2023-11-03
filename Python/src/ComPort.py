import serial
import time
import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext  # Import the scrolledtext module for a scrolling console
from serial.tools import list_ports
import threading

class SerialCommunicator:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial Communicator")

        # Variables for selected COM port and baud rate
        self.selected_port = tk.StringVar()
        self.selected_baud_rate = tk.StringVar()

        # UI Components
        ttk.Label(root, text="COM Port:").grid(row=0, column=0, padx=5, pady=5, sticky=tk.E)
        self.combo_ports = ttk.Combobox(root, textvariable=self.selected_port, state="readonly")
        self.combo_ports.grid(row=0, column=1, padx=5, pady=5, sticky=tk.W)

        ttk.Label(root, text="Baud Rate:").grid(row=1, column=0, padx=5, pady=5, sticky=tk.E)
        self.combo_baud_rates = ttk.Combobox(root, textvariable=self.selected_baud_rate, state="readonly")
        # Add more baud rates if needed
        self.combo_baud_rates["values"] = ["300", "1200", "2400", "4800", "9600", "14400", "19200", "38400", "57600", "115200"]
        self.combo_baud_rates.grid(row=1, column=1, padx=5, pady=5, sticky=tk.W)
        self.combo_baud_rates.set("9600")  # Set a default baud rate

        self.connect_button = ttk.Button(root, text="Connect", command=self.connect_serial)
        self.connect_button.grid(row=2, column=0, columnspan=2, pady=10)
        # Entry widget to type data to send
        ttk.Label(root, text="Data to Send:").grid(row=3, column=0, padx=5, pady=5, sticky=tk.E)
        self.entry_data = ttk.Entry(root)
        self.entry_data.grid(row=3, column=1, padx=5, pady=5, sticky=tk.W)

        # Button to send data
        self.send_button = ttk.Button(root, text="Send", command=self.send_data)
        self.send_button.grid(row=4, column=0, columnspan=2, pady=10)

        # Console for live output
        ttk.Label(root, text="Logs:").grid(row=5, column=0, columnspan=2, padx=5, pady=5)
        self.console = scrolledtext.ScrolledText(root, wrap=tk.WORD, width=20, height=20)
        self.console.grid(row=6, column=0, columnspan=2, padx=5, pady=5)

        # Console for received data (Rx)
        ttk.Label(root, text="Rx/Tx:").grid(row=5, column=5, columnspan=2, padx=5, pady=5)
        self.console_rx = scrolledtext.ScrolledText(root, wrap=tk.WORD, width=50, height=20)
        self.console_rx.grid(row=6, column=5, columnspan=2, padx=5, pady=5)

        # Button to clear Tx console
        self.clear_tx_button = ttk.Button(root, text="Clear Logs", command=self.clear_tx)
        self.clear_tx_button.grid(row=5, column=0, pady=5)

        # Button to clear Rx console
        self.clear_rx_button = ttk.Button(root, text="Clear Rx/Tx", command=self.clear_rx)
        self.clear_rx_button.grid(row=5, column=5, pady=5)
        # Get available COM ports
        self.update_available_ports()

        # Serial object
        self.ser = None
        self.start_serial_read()

    def start_serial_read(self):
        # Function to continuously read from the serial port in a separate thread
        def serial_read():
            previous_data = b""
            while True:
                if self.ser and self.ser.is_open:
                    try:
                        received_data = self.ser.readline().decode('utf-8')
                        if received_data and received_data != previous_data:
                            print(f"Received data (raw): {received_data}")
                            self.log_rx(f"Received data (hex): {received_data}")
                            if received_data:
                                decimal_numbers = [int(num) for num in received_data.strip().split() if num.strip().isdigit()]
                                print(f"Decimal numbers: {decimal_numbers}")
                            hex_data = " ".join([f"{num:x}" for num in decimal_numbers])
                            print(f"Received data (hex): {hex_data}")
                            self.log_rx(f"Received (hex): {hex_data}")
                            # Check the first decimal number
                            if decimal_numbers:
                                first_number = decimal_numbers[0]
                                print(f"First number: {first_number}")
                                self.log_rx(f"First number: {first_number}")
                                if first_number == 0x4F:
                                    self.console_rx.tag_configure("red", foreground="red")
                                    self.console_rx.insert(tk.END, hex_data + "\n", "red")
                                else:
                                    self.console_rx.tag_configure("green", foreground="green")
                                    self.console_rx.insert(tk.END, hex_data + "\n", "green")
                            previous_data = received_data
                    except Exception as e:
                        self.log(f"Error reading from serial port: {str(e)}")

        # Start the serial reading thread
        self.read_thread = threading.Thread(target=serial_read, daemon=True)
        self.read_thread.start()

    def log_rx(self, message):
        self.console_rx.insert(tk.END, message + "\n")
        self.console_rx.see(tk.END)  # Auto-scroll to the bottom of the console_rx
    
    def update_available_ports(self):
        ports = [port.device for port in list_ports.comports()]
        self.combo_ports["values"] = ports

    def connect_serial(self):
        try:
            port = self.selected_port.get()
            baud_rate = int(self.selected_baud_rate.get())

            if self.ser and self.ser.is_open:
                self.ser.close()
                self.connect_button.config(text="Connect")
                self.log("Disconnected.")
            else:
                self.ser = serial.Serial(port, baud_rate, timeout=1)
                time.sleep(2)  # Wait for the serial connection to initialize
                self.connect_button.config(text="Disconnect")
                self.log(f"Connected to {port} with baud rate {baud_rate}.")

        except Exception as e:
            self.log(f"Error: {str(e)}")

    def send_data(self):
        try:
            data_to_send = self.entry_data.get() + '\r'
            num_pieces = len(data_to_send.split())
            if num_pieces < 10:
                num_pieces = "0" + str(num_pieces)
            formatted_data_to_send = f"{num_pieces} {data_to_send}"
            if self.ser and self.ser.is_open:
                self.ser.write(formatted_data_to_send.encode())
                self.log_rx(f"{data_to_send}")
            else:
                self.log("Serial port is not open.")

        except Exception as e:
            self.log(f"Error: {str(e)}")

    def log(self, message):
        self.console.insert(tk.END, message + "\n")
        self.console.see(tk.END)  # Auto-scroll to the bottom of the console\
    
    def clear_tx(self):
        self.console.configure(state="normal")
        self.console.delete(1.0, tk.END)

    def clear_rx(self):
        self.console_rx.configure(state="normal")
        self.console_rx.delete(1.0, tk.END)
if __name__ == "__main__":
    root = tk.Tk()
    app = SerialCommunicator(root)
    root.mainloop()