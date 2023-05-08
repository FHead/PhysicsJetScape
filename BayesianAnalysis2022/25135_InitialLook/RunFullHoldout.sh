#!/bin/bash

Config=yaml/Exponential20221012RBF.yaml

python3 SetupAnalysis.py --Config $Config --CentralityMax 10
python3 MakeCovarianceMatrixPlot.py
python3 MakeDesignObservablePlot.py
python3 MakeDesignSpacePlot.py

for i in `python3 DumpAllData.py --Key designindex | tr -d '[]'`
do
   echo Doing holdout $i...
   python3 SetupAnalysis.py --Config $Config --CentralityMax 10 --Holdout $i
   python3 CleanPastFiles.py
   python3 RunEmulator.py
   python3 MakeHoldoutCheckPlot.py
   python3 WriteToText.py
done
