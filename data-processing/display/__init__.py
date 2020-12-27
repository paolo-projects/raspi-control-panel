import matplotlib.pyplot as plt
from matplotlib import widgets
import numpy
from typing import List

class Plot:
    def __init__(self, data_filter_change = None, data_fir_n_change = None, der_filter_change = None,
                 der_fir_n_change = None, file_change = None, refresh_button_click = None, sh_click = None,
                 icg_click = None, file_list: List[str] = None):

        self.fig, self.axs = plt.subplots(3, 2, constrained_layout=True)
        self.lines = numpy.ndarray(shape=(3, 2), dtype=plt.Line2D)
        plt.subplots_adjust(bottom=0.25)

        widgets.TextBox(plt.axes([0, 0.17, 0.12, 0.02]), "", initial="Data filter")
        self.data_filter_widget = widgets.RadioButtons(plt.axes([0, 0.01, 0.12, 0.15]), ["savgol", "FIR", "None"], 2)
        self.data_filter_widget.on_clicked(data_filter_change)

        self.data_fir_n_widget = widgets.Slider(plt.axes([0.4, 0.08, 0.25, 0.05]), "Data FIR N", 5, 100, 20, valstep=1)
        self.data_fir_n_widget.on_changed(data_fir_n_change)

        widgets.TextBox(plt.axes([0.12, 0.17, 0.12, 0.02]), "", initial="Derivative filter")
        self.der_filter_widget = widgets.RadioButtons(plt.axes([0.12, 0.01, 0.12, 0.15]), ["savgol", "FIR", "None"], 2)
        self.der_filter_widget.on_clicked(der_filter_change)

        self.der_fir_n_widget = widgets.Slider(plt.axes([0.4, 0.02, 0.25, 0.05]), "Deriv FIR N", 5, 100, 50, valstep=1)
        self.der_fir_n_widget.on_changed(der_fir_n_change)

        self.refresh_button = widgets.Button(plt.axes([0.40, 0.15, 0.08, 0.05]), "Measure")
        self.refresh_button.on_clicked(refresh_button_click)

        self.sh_widget = widgets.TextBox(plt.axes([0.5, 0.15, 0.05, 0.05]), "", "200")
        self.sh_plus_btn = widgets.Button(plt.axes([0.55, 0.175, 0.02, 0.025]), "+")
        self.sh_minus_btn = widgets.Button(plt.axes([0.55, 0.15, 0.02, 0.025]), "-")

        def sh_pl_cl(_):
            new_val = int(self.sh_widget.text) + 50
            self.sh_widget.set_val(str(new_val))
            sh_click(new_val)

        def sh_mn_cl(_):
            new_val = int(self.sh_widget.text) - 50
            self.sh_widget.set_val(str(new_val))
            sh_click(new_val)

        self.sh_plus_btn.on_clicked(sh_pl_cl)
        self.sh_minus_btn.on_clicked(sh_mn_cl)
        self.sh_widget.on_text_change(lambda _: sh_click(int(self.sh_widget.text)))

        self.icg_widget = widgets.TextBox(plt.axes([0.6, 0.15, 0.05, 0.05]), "", "100000")
        self.icg_plus_btn = widgets.Button(plt.axes([0.65, 0.175, 0.02, 0.025]), "+")
        self.icg_minus_btn = widgets.Button(plt.axes([0.65, 0.15, 0.02, 0.025]), "-")
        self.icg_widget.on_text_change(lambda _: icg_click(int(self.icg_widget.text)))

        def icg_pl_cl(_):
            new_val = int(self.icg_widget.text) + 1000
            self.icg_widget.set_val(str(new_val))
            sh_click(new_val)

        def icg_mn_cl(_):
            new_val = int(self.icg_widget.text) - 1000
            self.icg_widget.set_val(str(new_val))
            sh_click(new_val)

        self.icg_plus_btn.on_clicked(icg_pl_cl)
        self.icg_minus_btn.on_clicked(icg_mn_cl)

        if file_list is not None:
            self.file_list_widget = widgets.RadioButtons(plt.axes([0.7, 0.01, 0.28, 0.15]), file_list)
            self.file_list_widget.on_clicked(file_change)

    def set_title(self, row: int, col: int, title: str):
        self.axs[row][col].set_title(title)

    def set_ylabel(self, row: int, col: int, label: str):
        self.axs[row][col].set_ylabel(label)

    def set_xlabel(self, row: int, col: int, label: str):
        self.axs[row][col].set_xlabel(label)

    def set_data(self, row: int, col: int, data):
        self.lines[row][col], = self.axs[row][col].plot(data)

    def update_data(self, row: int, col: int, data):
        self.lines[row][col].set_ydata(data)
        self.axs[row][col].relim()
        self.axs[row][col].autoscale_view()

    def show(self):
        plt.show()

    def update(self):
        plt.draw()
