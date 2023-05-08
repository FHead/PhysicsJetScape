import numpy as np
from pathlib import Path

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)
DataList = AllData['observables'][0][1]

from src import lazydict, emulator
Emulator = emulator.Emulator.from_cache('HeavyIon')

tag = AllData['tag']

# Write data points out for beautiful plots
for Item in DataList:
    with Path(f'result/{tag}/txt/' + Item + "_X.txt").open('w') as f:
        np.savetxt(f, [AllData['data']["HeavyIon"]["R_AA"][Item]['x']])
    with Path(f'result/{tag}/txt/' + Item + "_Y.txt").open('w') as f:
        np.savetxt(f, [AllData['data']["HeavyIon"]["R_AA"][Item]['y']])
    with Path(f"result/{tag}/txt/" + Item + "_E.txt").open('w') as f:
        np.savetxt(f, [np.sqrt(np.diag(AllData['cov']["HeavyIon"][("R_AA",Item)][("R_AA",Item)]))])
    with Path(f"result/{tag}/txt/" + Item + "_S.txt").open('w') as f:
        np.savetxt(f, [AllData['data']["HeavyIon"]["R_AA"][Item]['yerr']['stat'][:,0]])

# Write design out for beautiful plots
Design = Emulator.predict(AllData['design'])
for Item in DataList:
    np.savetxt(f'result/{tag}/txt/' + Item + '_PredictedDesign.txt.gz', Design['R_AA'][Item])

for Item in DataList:
    np.savetxt(f'result/{tag}/txt/' + Item + '_Design.txt.gz', AllData['model']['HeavyIon']['R_AA'][Item]['Y'])

# Write holdout out if applicable
if 'holdout' in AllData and AllData['holdout'] >= 0:
    HoldoutPrediction = Emulator.predict(AllData['holdoutdesign'])
    for Item in DataList:
        np.savetxt(f'result/{tag}/txt/' + Item + '_HoldoutPredictedDesign.txt.gz', HoldoutPrediction['R_AA'][Item])



