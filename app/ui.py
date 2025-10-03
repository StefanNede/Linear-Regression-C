from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from main import run_regression, plot_3d

import customtkinter as ctk
import numpy as np
import time

DATAPOINTS_FILE = "../data/data.txt"
PLANE_FILE = "../data/plane.txt"

# --- MOCK BACKEND FUNCTIONS ---
# You can replace these with your actual Python functions.

def save_data_to_csv(csv_data):
    """ save data to DATAFILE_DIR """
    print("--- SAVING DATA ---")
    # print(f"Data received:\n{csv_data}")
    with open(DATAPOINTS_FILE, "w") as file:
        file.write(csv_data)
    print("--- SAVE COMPLETE ---")


def process_data_and_create_figure():
    """
    Placeholder function to simulate your data processing.
    In your real app, this function would read the CSV, run processing,
    and return the data points for the graph.
    """
    print("--- SIMULATING PROCESSING ---")
    # Simulating a delay for processing
    time.sleep(1.5)
    
    # Generate random 3D data for demonstration purposes
    # Replace this with loading and processing your actual data
    points = np.random.rand(50, 3) * 100
    print("--- PROCESSING COMPLETE ---")
    return points

# --- MAIN APPLICATION CLASS ---

class App(ctk.CTk):
    def __init__(self):
        super().__init__()

        # --- Window Setup ---
        self.title("Linear Regression Model")
        self.geometry("1100x600")
        ctk.set_appearance_mode("Dark")

        # --- Layout Configuration ---
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=2)
        self.grid_rowconfigure(0, weight=1)

        # --- Left Frame (Controls) ---
        self.controls_frame = ctk.CTkFrame(self, width=500)
        self.controls_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")
        self.controls_frame.grid_propagate(False) # Prevent frame from shrinking
        
        # Widgets for the control frame
        self.create_control_widgets()

        # --- Right Frame (Graph) ---
        self.graph_frame = ctk.CTkFrame(self)
        self.graph_frame.grid(row=0, column=1, padx=(0, 10), pady=10, sticky="nsew")
        
        # Matplotlib figure and canvas setup
        self.canvas_widget = None # Initialise canvas reference
        self.placeholder_label = None # Initialise placeholder reference
        self.create_graph_widget()

    def create_control_widgets(self):
        """Creates all the widgets for the left control panel."""
        self.controls_frame.grid_columnconfigure(0, weight=1)
        
        title_label = ctk.CTkLabel(self.controls_frame, text="Controls", font=ctk.CTkFont(size=20, weight="bold"))
        title_label.grid(row=0, column=0, padx=20, pady=(20, 10), sticky="ew")

        # Data Input Textbox
        input_label = ctk.CTkLabel(self.controls_frame, text="Input New CSV Data:", anchor="w")
        input_label.grid(row=1, column=0, padx=20, pady=(10, 5), sticky="ew")
        
        self.data_input = ctk.CTkTextbox(self.controls_frame, height=150)
        self.data_input.grid(row=2, column=0, padx=20, pady=5, sticky="ew")
        self.data_input.insert("1.0", "10.5,25.1,80.3\n12.8,28.4,85.7\n")

        # Buttons
        self.add_data_btn = ctk.CTkButton(self.controls_frame, text="Add Data (to CSV)", command=self.add_data_action)
        self.add_data_btn.grid(row=3, column=0, padx=20, pady=10, sticky="ew")
        
        self.process_btn = ctk.CTkButton(self.controls_frame, text="Start Processing & Graph", command=self.process_data_action)
        self.process_btn.grid(row=4, column=0, padx=20, pady=10, sticky="ew")
        
        # Status Box
        status_label = ctk.CTkLabel(self.controls_frame, text="Status:", anchor="w")
        status_label.grid(row=5, column=0, padx=20, pady=(10, 5), sticky="ew")
        
        self.status_box = ctk.CTkTextbox(self.controls_frame, height=100, state="disabled")
        self.status_box.grid(row=6, column=0, padx=20, pady=5, sticky="nsew")
        self.controls_frame.grid_rowconfigure(6, weight=1)

    def create_graph_widget(self):
        """Creates a placeholder for the graph. The actual canvas is added later."""
        # Create a placeholder label
        self.placeholder_label = ctk.CTkLabel(self.graph_frame, 
                                              text="Graph will appear here after processing",
                                              font=ctk.CTkFont(size=16),
                                              text_color="gray")
        self.placeholder_label.pack(pady=20, padx=20, expand=True)
        
    def log_status(self, message):
        """Adds a message to the status box."""
        self.status_box.configure(state="normal")
        self.status_box.insert(ctk.END, f"- {message}\n")
        self.status_box.configure(state="disabled")
        self.status_box.see(ctk.END) # Auto-scroll
        self.update_idletasks() # Force UI update
        
    # --- Button Actions ---
    def add_data_action(self):
        """Handles the 'Add Data' button click."""
        data = self.data_input.get("1.0", ctk.END).strip()
        if not data:
            self.log_status("Error: Input data is empty.")
            return

        self.log_status("Saving data...")
        save_data_to_csv(data) # Call your backend function
        self.log_status("Data saved successfully.")
        self.data_input.delete("1.0", ctk.END) # Clear the input box
    
    def process_data_action(self):
        """Handles the 'Start Processing' button click."""
        self.log_status("Starting data processing...")
        
        # Calling backend processing function to get the figure
        is_3d = run_regression()
        if is_3d:
            fig = plot_3d(DATAPOINTS_FILE, PLANE_FILE)
        else:
            self.log_status("Processing complete. Graph cannot be rendered.")
            return
        
        self.log_status("Processing complete. Rendering graph...")

        # --- Update Matplotlib Graph ---
        # If a canvas or placeholder already exists, destroy it
        if self.canvas_widget:
            self.canvas_widget.destroy()
        if self.placeholder_label:
            self.placeholder_label.destroy()
            self.placeholder_label = None

        # Create a new canvas with the figure returned from the function
        canvas = FigureCanvasTkAgg(fig, master=self.graph_frame)
        self.canvas_widget = canvas.get_tk_widget()
        self.canvas_widget.pack(side=ctk.TOP, fill=ctk.BOTH, expand=True)
        canvas.draw()
        
        self.log_status("Graph rendered.")


if __name__ == "__main__":
    app = App()
    app.mainloop()