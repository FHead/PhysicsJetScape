import matplotlib.pyplot as plt
import numpy as np

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

# Taken from https://dsp.stackexchange.com/questions/1676/savitzky-golay-smoothing-filter-for-not-equally-spaced-data
def non_uniform_savgol(x, y, window, polynom):
    """
    Applies a Savitzky-Golay filter to y with non-uniform spacing
    as defined in x

    This is based on https://dsp.stackexchange.com/questions/1676/savitzky-golay-smoothing-filter-for-not-equally-spaced-data
    The borders are interpolated like scipy.signal.savgol_filter would do

    Parameters
    ----------
    x : array_like
        List of floats representing the x values of the data
    y : array_like
        List of floats representing the y values. Must have same length
        as x
    window : int (odd)
        Window length of datapoints. Must be odd and smaller than x
    polynom : int
        The order of polynom used. Must be smaller than the window size

    Returns
    -------
    np.array of float
        The smoothed y values
    """
    if len(x) != len(y):
        raise ValueError('"x" and "y" must be of the same size')

    if len(x) < window:
        raise ValueError('The data size must be larger than the window size')

    if type(window) is not int:
        raise TypeError('"window" must be an integer')

    if window % 2 == 0:
        raise ValueError('The "window" must be an odd integer')

    if type(polynom) is not int:
        raise TypeError('"polynom" must be an integer')

    if polynom >= window:
        raise ValueError('"polynom" must be less than "window"')

    half_window = window // 2
    polynom += 1

    # Initialize variables
    A = np.empty((window, polynom))     # Matrix
    tA = np.empty((polynom, window))    # Transposed matrix
    t = np.empty(window)                # Local x variables
    y_smoothed = np.full(len(y), np.nan)

    # Start smoothing
    for i in range(half_window, len(x) - half_window, 1):
        # Center a window of x values on x[i]
        for j in range(0, window, 1):
            t[j] = x[i + j - half_window] - x[i]

        # Create the initial matrix A and its transposed form tA
        for j in range(0, window, 1):
            r = 1.0
            for k in range(0, polynom, 1):
                A[j, k] = r
                tA[k, j] = r
                r *= t[j]

        # Multiply the two matrices
        tAA = np.matmul(tA, A)

        # Invert the product of the matrices
        tAA = np.linalg.inv(tAA)

        # Calculate the pseudoinverse of the design matrix
        coeffs = np.matmul(tAA, tA)

        # Calculate c0 which is also the y value for y[i]
        y_smoothed[i] = 0
        for j in range(0, window, 1):
            y_smoothed[i] += coeffs[0, j] * y[i + j - half_window]

        # If at the end or beginning, store all coefficients for the polynom
        if i == half_window:
            first_coeffs = np.zeros(polynom)
            for j in range(0, window, 1):
                for k in range(polynom):
                    first_coeffs[k] += coeffs[k, j] * y[j]
        elif i == len(x) - half_window - 1:
            last_coeffs = np.zeros(polynom)
            for j in range(0, window, 1):
                for k in range(polynom):
                    last_coeffs[k] += coeffs[k, j] * y[len(y) - window + j]

    # Interpolate the result at the left border
    for i in range(0, half_window, 1):
        y_smoothed[i] = 0
        x_i = 1
        for j in range(0, polynom, 1):
            y_smoothed[i] += first_coeffs[j] * x_i
            x_i *= x[i] - x[half_window]

    # Interpolate the result at the right border
    for i in range(len(x) - half_window, len(x), 1):
        y_smoothed[i] = 0
        x_i = 1
        for j in range(0, polynom, 1):
            y_smoothed[i] += last_coeffs[j] * x_i
            x_i *= x[i] - x[-half_window - 1]

    return y_smoothed

SystemCount = len(AllData["systems"])
BinCount = len(AllData['observables'][0][1])

RC = int(np.sqrt(BinCount))
CC = int(np.ceil(BinCount / RC))

figure, axes = plt.subplots(figsize = (3 * CC, 3 * RC), nrows = RC, ncols = CC)

for s2 in range(0, BinCount):
    ax = s2 % RC
    ay = int(np.floor(s2 / RC))
    axes[ax][ay].set_xlabel(r"$p_{T}$")
    axes[ax][ay].set_ylabel(r"$R_{AA}$")

    S1 = AllData["systems"][0]
    O  = AllData["observables"][0][0]
    S2 = AllData["observables"][0][1][s2]

    DX = AllData["data"][S1][O][S2]['x']
    DY = AllData["data"][S1][O][S2]['y']
    DE = np.sqrt(AllData["data"][S1][O][S2]['yerr']['stat'][:,0]**2 + AllData["data"][S1][O][S2]['yerr']['sys'][:,0]**2)

    axes[ax][ay].set_title(AllData["observables"][0][1][s2])
    axes[ax][ay].set_xscale('log')

    for i, y in enumerate(AllData["model"][S1][O][S2]['Y']):
        if len(DX) > 7:
            Smoothed = non_uniform_savgol(DX, y, 7, 2)
            axes[ax][ay].plot(DX, Smoothed, 'g-', alpha=0.2, label="Smoothed" if i == 0 else '')
        elif len(DX) > 5:
            Smoothed = non_uniform_savgol(DX, y, 5, 2)
            axes[ax][ay].plot(DX, Smoothed, 'g-', alpha=0.2, label="Smoothed" if i == 0 else '')
        elif len(DX) > 3:
            Smoothed = non_uniform_savgol(DX, y, 3, 1)
            axes[ax][ay].plot(DX, Smoothed, 'g-', alpha=0.2, label="Smoothed" if i == 0 else '')
        else:
            axes[ax][ay].plot(DX, y, 'b-', alpha=0.2, label="Posterior" if i==0 else '')

    axes[ax][ay].errorbar(DX, DY, yerr = DE, fmt='ro', label="Measurements")

plt.tight_layout()
tag = AllData['tag']
figure.savefig(f'result/{tag}/plots/PredictionSmoothing.pdf', dpi = 192)

