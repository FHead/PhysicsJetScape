import matplotlib.pyplot as plt
import numpy as np

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

import src
src.Initialize()
from src import mcmc
chain = mcmc.Chain()
MCMCSamples = chain.load()

from src import lazydict, emulator
Emulator = emulator.Emulator.from_cache('HeavyIon')


Examples = MCMCSamples[ np.random.choice(range(len(MCMCSamples)), 500), :]

TempPrediction = {"HeavyIon": Emulator.predict(Examples)}

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

    for i, y in enumerate(TempPrediction[S1][O][S2]):
        axes[ax][ay].plot(DX, y, 'b-', alpha=0.05, label="Posterior" if i==0 else '')
    axes[ax][ay].errorbar(DX, DY, yerr = DE, fmt='ro', label="Measurements")

plt.tight_layout()
tag = AllData['tag']
figure.savefig(f'result/{tag}/plots/ObservablePosterior.pdf', dpi = 192)
# figure.savefig(f'result/{tag}/plots/ObservablePosterior.png', dpi = 192)
