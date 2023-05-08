import numpy as np
from pathlib import Path

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)
DataList = AllData['observables'][0][1]

import src
src.Initialize()
from src import mcmc
chain = mcmc.Chain()
MCMCSamples = chain.load()

from src import lazydict, emulator
Emulator = emulator.Emulator.from_cache('HeavyIon')

tag = AllData['tag']

# Write posterior out for beautiful plots
Posterior = Emulator.predict(MCMCSamples)

for Item in DataList:
    np.savetxt(f'result/{tag}/txt/' + Item + '_Posterior.txt.gz', Posterior['R_AA'][Item])



