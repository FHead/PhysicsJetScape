import numpy as np
from pathlib import Path

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)
DataList = AllData['observables'][0][1]

from src import lazydict, emulator
Emulator = emulator.Emulator.from_cache('HeavyIon')

import matplotlib.pyplot as plt

N = len(Emulator.pca.explained_variance_ratio_)

figure, axes = plt.subplots()

axes.plot(np.arange(1, N), Emulator.pca.explained_variance_ratio_[:-1], 'o-b')
axes.axvline(x = Emulator.npc, ymin = 0, color='red', linestyle='dotted', linewidth=2)

axes.set_xlabel('PCA Index')
axes.set_ylabel('Percentage variance')
axes.set_yscale('log')

plt.tight_layout()
tag = AllData['tag']

plt.grid('both')
plt.savefig(f'result/{tag}/plots/PCAIndividualPlot.pdf', dpi = 192)

figure, axes = plt.subplots()

axes.plot(np.arange(1, N + 1), np.cumsum(Emulator.pca.explained_variance_ratio_), 'o-b')
axes.axvline(x = Emulator.npc, ymin = 0, color='red', linestyle='dotted', linewidth=2)

axes.set_xlabel('PCA Count')
axes.set_ylabel('Percentage variance explained')

plt.tight_layout()
tag = AllData['tag']

plt.grid('both')
plt.savefig(f'result/{tag}/plots/PCAPlot.pdf', dpi = 192)

if N > 15:
    figure, axes = plt.subplots()

    axes.plot(np.arange(1, 15 + 1), np.cumsum(Emulator.pca.explained_variance_ratio_[0:15]), 'o-b')
    axes.set_xlabel('PCA Count')
    axes.set_ylabel('Percentage variance explained')

    axes.axvline(x = Emulator.npc, ymin = 0, color='red', linestyle='dotted', linewidth=2)

    plt.grid('both')

    plt.tight_layout()
    tag = AllData['tag']

    plt.savefig(f'result/{tag}/plots/PCAPlotZoomIn.pdf', dpi = 192)


