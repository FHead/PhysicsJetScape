import numpy as np
import pickle
from pathlib import Path
import src.reader as Reader
import yaml
import sys
import argparse

# Initialize temporary containers
DataList = []
Setup = {}
RawData = {}
RawPrediction = {}
RawPredictionError = {}
RawDesign = {}

parser = argparse.ArgumentParser()
parser.add_argument("--Config", help = "Config yaml file", default = "yaml/TestAnalysis.yaml")
parser.add_argument("--Holdout", help = "Holdout test", default = -1, type = int)
parser.add_argument("--CentralityMin", help = "Centrality range", default = -1, type = int)
parser.add_argument("--CentralityMax", help = "Centrality range", default = -1, type = int)
parser.add_argument("--DoSmoothing", help = "Switch to smooth predictions", action = "store_true")
args = parser.parse_args()

###################################################
# Part 1: read raw data and prediction and design #
###################################################

ConfigFile = args.Config

with open(ConfigFile, "r") as stream:
    try:
        Setup = yaml.safe_load(stream)

    except yaml.YAMLError as exc:
        print(exc)
        exit()

Tag = Setup["Tag"]

if args.CentralityMin >= 0:
    Setup['Data'] = {k: v for k, v in Setup['Data'].items() if v['Attribute']['CentralityMin'] >= args.CentralityMin}
    Tag = Tag + '_CentralityMin' + str(args.CentralityMin)
if args.CentralityMax >= 0:
    Setup['Data'] = {k: v for k, v in Setup['Data'].items() if v['Attribute']['CentralityMax'] <= args.CentralityMax}
    Tag = Tag + '_CentralityMax' + str(args.CentralityMax)

DataList = list(Setup['Data'].keys())
for Item in DataList:
    RawData[Item] = Reader.ReadData(Setup['BaseDirectory'] + Setup['Data'][Item]['Data'])
    RawPrediction[Item] = Reader.ReadPrediction(Setup['BaseDirectory'] + Setup['Data'][Item]['Prediction'])
    RawPredictionError[Item] = Reader.ReadPrediction(Setup['BaseDirectory'] + Setup['Data'][Item]['PredictionError'])

RawDesign = Reader.ReadDesign(Setup['BaseDirectory'] + Setup['Design']['File'])
for I in Setup['Design']['LogScale']:
    RawDesign['Design'][:, I] = np.log(RawDesign['Design'][:, I])
    RawDesign['Parameter'][I] = 'log(' + RawDesign['Parameter'][I] + ')'

    Setup['Design']['Range'][I][0] = np.log(Setup['Design']['Range'][I][0])
    Setup['Design']['Range'][I][1] = np.log(Setup['Design']['Range'][I][1])



#########################################################
# Part 2: sort out missing points and extra data points #
#########################################################

def InsertZeroDesign(Raw, Locations):
    for Location in Locations:
        Raw = np.insert(Raw, Location, np.zeros(Raw.shape[1]), axis = 0)
    return Raw

def DeleteRawData(Raw, Items):
    Raw["Data"]["x"]    = np.delete(Raw["Data"]["x"], Items, axis = 0)
    if 'xerr' in Raw['Data']:
        Raw["Data"]["xerr"] = np.delete(Raw["Data"]["xerr"], Items, axis = 0)
    Raw["Data"]["y"]    = np.delete(Raw["Data"]["y"], Items, axis = 0)
    for item in Raw["Data"]["yerr"]:
        Raw["Data"]["yerr"][item] = np.delete(Raw["Data"]["yerr"][item], Items, axis = 0)

# Clean up points
for Item in DataList:
    if 'DataExclude' in Setup['Data'][Item]:
        DeleteRawData(RawData[Item], Setup['Data'][Item]['DataExclude'])
    if 'PredictionExclude' in Setup['Data'][Item]:
        RawPrediction[Item]["Prediction"] = np.delete(RawPrediction[Item]["Prediction"], Setup['Data'][Item]['PredictionExclude'], axis = 1)
        RawPredictionError[Item]["Prediction"] = np.delete(RawPredictionError[Item]["Prediction"], Setup['Data'][Item]['PredictionExclude'], axis = 1)

# Clean up low energy points
if 'DataCut' in Setup and 'MinPT' in Setup['DataCut']:
    for Item in DataList:
        DeleteCount = sum(i < Setup['DataCut']['MinPT'] for i in RawData[Item]["Data"]["x"])
        DeleteRawData(RawData[Item], range(0, DeleteCount))
        RawPrediction[Item]['Prediction'] = np.delete(RawPrediction[Item]["Prediction"], range(0, DeleteCount), axis = 1)
        RawPredictionError[Item]['Prediction'] = np.delete(RawPredictionError[Item]["Prediction"], range(0, DeleteCount), axis = 1)


# Delete prediction that are longer than available design points
NDesign = RawDesign['Design'].shape[0]
for Item in DataList:
    if RawPrediction[Item]['Prediction'].shape[0] > NDesign:
        ToDelete = RawPrediction[Item]['Prediction'].shape[0] - NDesign
        RawPrediction[Item]['Prediction'] = np.delete(RawPrediction[Item]['Prediction'], range(NDesign, RawPrediction[Item]['Prediction'].shape[0]), axis = 0)
    if RawPredictionError[Item]['Prediction'].shape[0] > NDesign:
        ToDelete = RawPredictionError[Item]['Prediction'].shape[0] - NDesign
        RawPredictionError[Item]['Prediction'] = np.delete(RawPredictionError[Item]['Prediction'], range(NDesign, RawPredictionError[Item]['Prediction'].shape[0]), axis = 0)

# Remove bad design points
InputDesignCount = RawDesign['Design'].shape[0]
ListToDelete = [x for x in Setup['Design']['ListToDelete'] if x < InputDesignCount]
DesignIndex = np.delete(range(0, InputDesignCount), ListToDelete)

if 'PoorManQALimit' in Setup['Design']:
    for I in DesignIndex:
        if sum(any(RawPrediction[Item]['Prediction'][I,:] > Setup['Design']['PoorManQALimit']) for Item in DataList) > 0:
            ListToDelete.append(I)
    ListToDelete.sort()

RawDesign['Design'] = np.delete(RawDesign['Design'], ListToDelete, axis = 0)
DesignIndex = np.delete(range(0, InputDesignCount), ListToDelete)

for Item in DataList:
    RawPrediction[Item]['Prediction'] = np.delete(RawPrediction[Item]['Prediction'], ListToDelete, axis = 0)
    RawPredictionError[Item]['Prediction'] = np.delete(RawPredictionError[Item]['Prediction'], ListToDelete, axis = 0)

# If prediction error is exactly 0, throw a warning and set it to some huge number
for Item in DataList:
    for i, I in enumerate(DesignIndex):
        if any(RawPredictionError[Item]['Prediction'][i,:] == 0):
            print(f"Warning: prediction error for {Item} and design point {I} contains 0.000's");
            RawPredictionError[Item]['Prediction'][i,:] = [(9999 if x == 0 else x) for x in RawPredictionError[Item]['Prediction'][i,:]]


# Add NPC to check in the check list if it is enabled
if 'PCACheck' in Setup['Emulator']:
    if Setup['Emulator']['NPC'] not in Setup['Emulator']['PCACheck']:
        Setup['Emulator']['PCACheck'].append(Setup['Emulator']['NPC'])
    if len(DesignIndex) not in Setup['Emulator']['PCACheck']:
        Setup['Emulator']['PCACheck'].append(len(DesignIndex))

# Add smoothing
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

if args.DoSmoothing == True:
    for Item in DataList:
        x = RawData[Item]["Data"]['x']
        for i, I in enumerate(DesignIndex):
            y = RawPrediction[Item]['Prediction'][i,:]
            if len(x) > 7:
                RawPrediction[Item]['Prediction'][i,:] = non_uniform_savgol(x, y, 7, 1)
            elif len(x) > 5:
                RawPrediction[Item]['Prediction'][i,:] = non_uniform_savgol(x, y, 5, 1)
            elif len(x) > 3:
                RawPrediction[Item]['Prediction'][i,:] = non_uniform_savgol(x, y, 3, 1)
    Tag = Tag + "_Smoothed"



#############################################
# Part 3: Deal with holdout and other tests #
#############################################

HoldoutDesign = {}
HoldoutPrediction = {}
HoldoutPredictionError = {}

if args.Holdout in DesignIndex:
    I = np.where(DesignIndex == args.Holdout)

    HoldoutDesign = RawDesign['Design'][I]

    for Item in DataList:
        HoldoutPrediction[Item] = RawPrediction[Item]['Prediction'][I]
        HoldoutPredictionError[Item] = RawPredictionError[Item]['Prediction'][I]
        RawData[Item]["Data"]["y"] = HoldoutPrediction[Item][0]
        RawData[Item]["Data"]["yerr"]["stat"][:,0] =HoldoutPredictionError[Item]
        RawData[Item]["Data"]["yerr"]["stat"][:,1] =HoldoutPredictionError[Item]
        RawData[Item]["Data"]["yerr"]["sys"] = RawData[Item]["Data"]["yerr"]["sys"] * 0

    RawDesign['Design'] = np.delete(RawDesign['Design'], I, axis = 0)
    for Item in DataList:
        RawPrediction[Item]['Prediction'] = np.delete(RawPrediction[Item]['Prediction'], I, axis = 0)
        RawPredictionError[Item]['Prediction'] = np.delete(RawPredictionError[Item]['Prediction'], I, axis = 0)

    Tag = Tag + "_Holdout" + str(args.Holdout)
else:
    args.Holdout = -1



##########################
# Part 4: Pickle Dumping #
##########################

# Initialize empty dictionary
AllData = {}

# Basic information
AllData["systems"] = ["HeavyIon"]
AllData["keys"] = RawDesign["Parameter"]
AllData["labels"] = RawDesign["Parameter"]
AllData["ranges"] = [tuple(x) for x in Setup['Design']['Range']]
AllData["observables"] = [('R_AA', DataList)]

# Data points
Data = {"HeavyIon": {"R_AA": {}}}
for Item in DataList:
    Data["HeavyIon"]["R_AA"][Item] = RawData[Item]["Data"]

# Model predictions
Prediction = {"HeavyIon": {"R_AA": {}}}
for Item in DataList:
    Prediction["HeavyIon"]["R_AA"][Item] = {}
    Prediction["HeavyIon"]["R_AA"][Item]["Y"] = RawPrediction[Item]["Prediction"]
    Prediction["HeavyIon"]["R_AA"][Item]["YError"] = RawPredictionError[Item]["Prediction"]
    Prediction["HeavyIon"]["R_AA"][Item]["x"] = RawData[Item]["Data"]['x']

# Covariance matrices - the indices are [system][measurement1][measurement2], each one is a block of matrix
Covariance = Reader.InitializeCovariance(Data)
for Item in DataList:
    SysLength = {"default": 0.10}
    if "Correlation" in Setup["Data"][Item]:
        for key, value in Setup["Data"][Item]["Correlation"].items():
            SysLength[key+",high"] = value
    Covariance["HeavyIon"][("R_AA", Item)][("R_AA", Item)] = Reader.EstimateCovariance(RawData[Item], RawData[Item], SysLength = SysLength)

# Assign data to the dictionary
AllData["design"] = RawDesign["Design"]
AllData['designindex'] = DesignIndex
AllData['designexcluded'] = ListToDelete
AllData["model"] = Prediction
AllData["data"] = Data
AllData["cov"] = Covariance

# Emulator settings
AllData['emulator'] = {}
AllData['emulator']['npc'] = Setup['Emulator']['NPC']
AllData['emulator']['kernel'] = Setup['Emulator']['Kernel']
if 'MaternNu' in Setup['Emulator']:
    AllData['emulator']['nu'] = Setup['Emulator']['MaternNu']
AllData['emulator']['retrain'] = Setup['Emulator']['NRetrain']
if 'PCACheck' in Setup['Emulator']:
    AllData['emulator']['pcacheck'] = Setup['Emulator']['PCACheck']

# MCMC settings
AllData['mcmc'] = {}
AllData['mcmc']['walker'] = Setup['MCMC']['NWalker']
AllData['mcmc']['burn'] = Setup['MCMC']['NBurn']
AllData['mcmc']['step'] = Setup['MCMC']['NStep']

# Other stuff
AllData["tag"] = Tag

# Holdout stuff
AllData["holdout"] = args.Holdout
if args.Holdout in DesignIndex:
    AllData["holdoutdesign"] = HoldoutDesign
    AllData["holdoutmodel"] = {"HeavyIon": {"R_AA": {}}}
    for Item in DataList:
        AllData["holdoutmodel"]["HeavyIon"]["R_AA"][Item] = {}
        AllData["holdoutmodel"]["HeavyIon"]["R_AA"][Item]["Y"] = HoldoutPrediction[Item]
        AllData["holdoutmodel"]["HeavyIon"]["R_AA"][Item]["x"] = RawData[Item]["Data"]['x']

# Save to the desired pickle file
with open('input/default.p', 'wb') as handle:
    pickle.dump(AllData, handle, protocol = pickle.HIGHEST_PROTOCOL)


#######################
# Make result folders #
#######################

Path('result/' + AllData["tag"] + '/txt').mkdir(parents = True, exist_ok = True)
Path('result/' + AllData["tag"] + '/plots').mkdir(parents = True, exist_ok = True)


############################################
# Finally, some print out and sanity check #
############################################

print('Design shape: ' + str(RawDesign['Design'].shape))
NDesign = RawDesign['Design'].shape[0]
for Item in DataList:
    PredictionDimension = RawPrediction[Item]['Prediction'].shape
    DataCount = RawData[Item]['Data']['y'].shape[0]
    if PredictionDimension[0] != NDesign:
        print('Number of design points in ' + Item + ' is ' + str(PredictionDimension[0]))
    if PredictionDimension[1] != DataCount:
        print('Number of data points in ' + Item + ' is ' + str(DataCount) + ', but prediction has ' + str(PredictionDimension[1]))


