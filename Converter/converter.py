import tkinter as tk
from tkinter import filedialog, messagebox
from tkinter import ttk
from PIL import Image, ImageTk, ImageOps
import numpy as np
from to_height import convert_normal_to_height_with_individual_maps
from to_height_advanced import convert_normal_to_height_advanced
from tkinterdnd2 import DND_FILES, TkinterDnD

class ImageConverterUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Normal to Height map Converter")

        self.original_image = None
        self.converted_image_array = None

        self.setup_frames()
        self.add_placeholder_texts()
        self.setup_buttons()
        self.setup_drop_area()

    def setup_frames(self):
        self.left_frame = tk.Frame(self.root, width=700, height=700, bg='white')
        self.left_frame.grid(row=0, column=0, padx=10, pady=10)
        self.left_frame.grid_propagate(False)
        
        self.middle_frame = tk.Frame(self.root, width=100, height=700)
        self.middle_frame.grid(row=0, column=1, padx=10, pady=10)
        
        self.right_frame = tk.Frame(self.root, width=700, height=700, bg='white')
        self.right_frame.grid(row=0, column=2, padx=10, pady=10)
        self.right_frame.grid_propagate(False)

        self.add_placeholder_texts()

    def add_placeholder_texts(self):
        self.left_placeholder = tk.Label(self.left_frame, text="Normal Map Placeholder", bg='white', fg='grey', font=('Arial', 30))
        self.left_placeholder.place(relx=0.5, rely=0.5, anchor='center')

        self.right_placeholder = tk.Label(self.right_frame, text="Generated Height Map Placeholder", bg='white', fg='grey', font=('Arial', 30))
        self.right_placeholder.place(relx=0.5, rely=0.5, anchor='center')

    def setup_buttons(self):
        self.button_open_file = ttk.Button(self.middle_frame, text="Open in File Browser", command=self.open_file_browser)
        self.button_open_file.pack(pady=10)
        
        self.separator = ttk.Separator(self.middle_frame, orient='horizontal')
        self.separator.pack(fill='x', pady=10)

        self.button_convert_to_height = ttk.Button(self.middle_frame, text="Convert to Height map", command=self.convert_to_height)
        self.button_convert_to_height.pack(pady=20)

        self.button_convert_to_height_advanced = ttk.Button(self.middle_frame, text="Convert to Height map Advanced", command=self.convert_to_height_advanced)
        self.button_convert_to_height_advanced.pack(pady=20)

        self.separator = ttk.Separator(self.middle_frame, orient='horizontal')
        self.separator.pack(fill='x', pady=10)

        self.save_label = ttk.Label(self.middle_frame, text="File name:")
        self.save_label.pack(pady=5)

        self.save_entry = ttk.Entry(self.middle_frame)
        self.save_entry.pack(pady=5)

        self.save_button = ttk.Button(self.middle_frame, text="Save Image", command=self.save_image)
        self.save_button.pack(pady=20)

    def open_file_browser(self):
        file_path = filedialog.askopenfilename()
        if file_path:
            self.load_image(file_path)


    def setup_drop_area(self):
        self.root.drop_target_register(DND_FILES)
        self.root.dnd_bind('<<Drop>>', self.on_drop)

    def on_click(self, event):
        file_path = filedialog.askopenfilename()
        if file_path:
            self.load_image(file_path)
        return 'break'

    def on_drop(self, event):
        file_path = event.data
        if file_path:
            self.load_image(file_path)
        return 'break'

    def load_image(self, file_path):
        self.original_image = Image.open(file_path)
        self.display_image(self.original_image, self.left_frame)

    def display_image(self, image, frame):
        for widget in frame.winfo_children():
            widget.destroy()
        image = ImageOps.fit(image, (700, 700), Image.LANCZOS)
        photo = ImageTk.PhotoImage(image)
        
        label = tk.Label(frame, image=photo)
        label.image = photo
        label.pack()

    def convert_to_height(self):
        if self.original_image:
            normal_map = np.array(self.original_image)
            _, height_map = convert_normal_to_height_with_individual_maps(normal_map)
            self.converted_image_array = height_map
            self.display_converted_image(height_map)

    def convert_to_height_advanced(self):
        if self.original_image:
            normal_map = np.array(self.original_image)
            height_map_advanced = convert_normal_to_height_advanced(normal_map)
            self.converted_image_array = height_map_advanced
            self.display_converted_image(height_map_advanced)

    def display_converted_image(self, image_array):
        converted_image = Image.fromarray(image_array)
        self.display_image(converted_image, self.right_frame)

    def save_image(self):
        if self.converted_image_array is not None:
            file_name = self.save_entry.get()
            save_image = Image.fromarray(self.converted_image_array)
            if not file_name:
                file_name = "generated_height"
            save_image.save(file_name + ".png")
        else:
            messagebox.showwarning("No Image", "There is no image to save.")

if __name__ == "__main__":
    root = TkinterDnD.Tk()
    app = ImageConverterUI(root)
    root.mainloop()
