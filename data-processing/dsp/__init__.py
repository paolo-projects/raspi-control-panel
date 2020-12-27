from display import Plot
from scipy.signal import savgol_filter, lfilter, correlate, convolve
from scipy.fft import fft, ifft
import numpy as np
from sys import stderr
import os
from ccd import request_data

dataset = []
smooth_dataset = []
derivative = []
smooth_derivative = []
sigmoid_function = []
sigmoid_derivative = []
dx_convolution: np.ndarray = np.ndarray([])
dx_correlation: np.ndarray = np.ndarray([])
dx_fft: np.ndarray = np.ndarray([]) # just for noise pattern recognition

plot = None

options = {
    "daf": "None", # data filter: savgol, FIR
    "daf_n": 20, # FIR filter N
    "derf": "None", # derivative filter: savgol, FIR
    "derf_n": 50, # FIR filter N
    "sh": 200,
    "icg": 100000
}

datafiles = []


# Data processing is done through the following procedure:

# 1 -> Clean the data by removing meaningless points (i.e. the first 32 and the last 14 points)
# 2 -> Normalize the data -> fit the y values into a [0 - 1] range where 1.0 is the maximum value of the dataset
# 3 -> Apply Savitzky-Golay smoothing to the data. The sigmoid-like curve should be preserved
# 4 -> Compute the first derivative from the data -> yields a Lorentzian or Gaussian-like curve
# 5 -> Apply IIR filter to the first derivative -> more smoothing, needed for a clean correlation
# 6 -> Generate a narrow test sigmoid curve, centered at PIXELS / 2
# 7 -> Compute its first derivative
# 8 -> Find the cross-correlation between the first-derivative of the measured data and the sigmoid derivative
# 9 -> The data point is the pixel corresponding to the maximum of the correlation curve


# derive sigmoid from logistic curve
def logistic(l, k, x0): return lambda x: l/(1+np.exp(-k*(x-x0)))


def load_dataset(filename: str):
    global dataset
    dataset = []
    # Import dataset
    with open(filename, "r") as dsfile:
        first_line = True
        for line in dsfile.readlines():
            if first_line is True:
                first_line = False
                continue
            try:
                comma_pos = line.index(',')
                value = float(line[comma_pos+1:-1])
                dataset.append(value)
            except ValueError:
                continue


def process_dataset():
    global dataset
    # Normalize values
    max_val = max(dataset)
    for i in range(0, len(dataset)):
        dataset[i] /= max_val


def smooth_the_dataset():
    global smooth_dataset
    smooth_dataset = []
    if options["daf"] == "savgol":
        smooth_dataset = savgol_filter(dataset, 7, 3)
    elif options["daf"] == "FIR":
        n = options["daf_n"]
        smooth_dataset = lfilter([1.0/n]*n, 1, dataset)
    else:
        smooth_dataset = dataset.copy()


def compute_derivative():
    global derivative
    derivative = []
    for i in range(1, len(smooth_dataset)):
        derivative.append(smooth_dataset[i] - smooth_dataset[i-1])


def smooth_the_derivative():
    global smooth_derivative
    smooth_derivative = []
    if options["derf"] == "savgol":
        smooth_derivative = savgol_filter(derivative, 7, 3)
    elif options["derf"] == "FIR":
        n = options["derf_n"]
        smooth_derivative = lfilter([1.0 / n]*n, 1, derivative)
    else:
        smooth_derivative = derivative.copy()

    adjust_derivative()


def adjust_derivative():
    integration = 0.0
    for val in smooth_derivative:
        integration += val

    if integration < 0:
        for i in range(0, len(smooth_derivative)):
            smooth_derivative[i] *= -1


def create_sigmoid():
    global smooth_derivative, sigmoid_function, sigmoid_derivative
    sigmoid_function = []
    sigmoid_derivative = []
    sigmoid = logistic(1.0, 0.05, len(dataset)/2)

    for i in range(0, len(dataset)):
        sigmoid_function.append(sigmoid(i))

    # Compute the derivative
    for i in range(1, len(sigmoid_function)):
        sigmoid_derivative.append(sigmoid_function[i] - sigmoid_function[i-1])


def compute_correlation():
    global dx_convolution, dx_correlation
    dx_convolution = []
    dx_correlation = []
    dx_convolution = convolve(smooth_derivative, sigmoid_derivative, 'same', 'fft')
    dx_correlation = correlate(smooth_derivative, sigmoid_derivative, 'full', 'fft')


def compute_fft():
    global dx_fft
    dx_fft = fft(smooth_derivative)


def print_point():
    # print the position of the maximum in the correlation data
    max_px = dx_correlation.argmax() - 1
    print(f"The shadow edge is at: {max_px}")


def calculate():
    # apply savitzky-golay smoothing to the light intensity values
    # window size 7 (can't use more as it gives an error), 3rd degree polynomial
    smooth_the_dataset()
    # compute the derivative from the smoothed curve
    compute_derivative()
    # smooth again the derivative through IIR filter
    smooth_the_derivative()
    # create the test sigmoid function (and derivative)
    create_sigmoid()
    # compute the correlation
    compute_correlation()
    # fft
    compute_fft()
    # print the max of the correlation
    print_point()


def update():
    plot.update_data(0, 0, smooth_dataset)
    plot.update_data(0, 1, smooth_derivative)
    plot.update_data(1, 0, sigmoid_function)
    plot.update_data(1, 1, sigmoid_derivative)
    plot.update_data(2, 0, dx_convolution)
    plot.update_data(2, 1, dx_fft)
    plot.update()


def plot_data():
    plot.set_title(0, 0, "Measured data")
    plot.set_xlabel(0, 0, "px")
    plot.set_ylabel(0, 0, "Intensity")
    plot.set_data(0, 0, smooth_dataset)

    plot.set_title(0, 1, "Measured data - d/dx")
    plot.set_xlabel(0, 1, "px")
    plot.set_ylabel(0, 1, "d/dx")
    plot.set_data(0, 1, smooth_derivative)

    plot.set_title(1, 0, "Sample sigmoid function")
    plot.set_data(1, 0, sigmoid_function)

    plot.set_title(1, 1, "Sample sigmoid - d/dx")
    plot.set_data(1, 1, sigmoid_derivative)

    plot.set_title(2, 0, "Convolution on derivative")
    plot.set_xlabel(2, 0, "px")
    plot.set_ylabel(2, 0, "Convolution")
    plot.set_data(2, 0, dx_convolution)

    plot.set_title(2, 1, "Fourier Transform")
    plot.set_xlabel(2, 1, "f")
    plot.set_ylabel(2, 1, "I")
    plot.set_data(2, 1, dx_fft)

    plot.show()


def data_filter_change(data_filter):
    options["daf"] = data_filter
    calculate()
    update()


def der_filter_change(der_filter):
    options["derf"] = der_filter
    calculate()
    update()


def data_fir_n_change(data_fir_n):
    options["daf_n"] = data_fir_n
    calculate()
    update()


def der_fir_n_change(der_fir_n):
    options["derf_n"] = der_fir_n
    calculate()
    update()


def refresh_button_click(_):
    global dataset
    dataset = request_data("com8", options["sh"], options["icg"])[33:-14]
    #for i in range(0, len(dataset)):
    #    dataset[i] = 4095 - dataset[i]
    process_dataset()
    calculate()
    update()


def sh_click(value):
    options["sh"] = value


def icg_click(value):
    options["icg"] += value


def loaded_file_change(filename: str):
    load_dataset(filename)
    process_dataset()
    calculate()
    update()


def show_dsp():
    global plot

    for df in os.listdir("."):
        if df.endswith(".csv"):
            datafiles.append(df)

    if len(datafiles) == 0:
        print("No .csv files found in current directory", file=stderr)
        exit(1)

    plot = Plot(data_filter_change, data_fir_n_change, der_filter_change, der_fir_n_change, loaded_file_change,
                refresh_button_click, sh_click, icg_click, datafiles)
    # load sample dataset (made from real measured values)
    load_dataset(datafiles[0])
    # normalize values
    process_dataset()
    # DSP
    calculate()
    # plot
    plot_data()


if __name__ == '__main__':
    show_dsp()