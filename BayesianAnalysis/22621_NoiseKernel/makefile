default:
	echo Hello world

all: RunDataReproduce RunDataAlphaScan RunDataKernelScan RunDataFluctuationTest RunHoldout RunClosureExample

JustPlot:
	python3 MakeBaselinePlots.py --N -1 --Nu 2.5 --Model Matter --KernelChoice RBF --Alpha 0.0 --Noise 0.0 --CovType Separated --Data All

RunMatter:
	python3 RunOne.py --N -1 --Model Matter     --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Separated

RunLBT:
	python3 RunOne.py --N -1 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Separated

RunLBTOldTreatment:
	python3 RunOne.py --N -1 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.20 --CovType Length0.2

RunMatterLBT1:
	python3 RunOne.py --N -1 --Model MatterLBT1 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.30 --CovType Separated
	python3 RunOne.py --N -1 --Model MatterLBT1 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.30 --CovType Separated --Data LHC
	python3 RunOne.py --N -1 --Model MatterLBT1 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.30 --CovType Separated --Data RHIC

RunMatterLBT2Test:
	python3 RunOne.py --N -1 --Model MatterLBT2 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.50 --CovType Separated

RunMatterLBT2:
	python3 RunOne.py --N -1 --Model MatterLBT2 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.50 --CovType Separated
	python3 RunOne.py --N -1 --Model MatterLBT2 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.50 --CovType Separated --Data LHC
	python3 RunOne.py --N -1 --Model MatterLBT2 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.50 --CovType Separated --Data RHIC

RunDataTest:
	python3 RunOne.py --N -1 --Model Matter     --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Separated

RunDataSeparated:
	python3 RunOne.py --N -1 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise1 --Alpha 0.10 --CovType Separated

RunDataDataType:
	python3 RunOne.py --N -1 --Model MatterLBT1 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Separated --Data LHC
	python3 RunOne.py --N -1 --Model MatterLBT1 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Separated --Data RHIC

RunDataReproduce:
	python3 RunOne.py --N -1 --Model Matter     --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Length0.2
	python3 RunOne.py --N -1 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise1 --Alpha 0.10 --CovType Length0.2
	python3 RunOne.py --N -1 --Model MatterLBT1 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Length0.2
	python3 RunOne.py --N -1 --Model MatterLBT2 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Length0.2

RunDataReproduceType:
	python3 RunOne.py --N -1 --Model MatterLBT1 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Length0.2 --Data LHC
	python3 RunOne.py --N -1 --Model MatterLBT1 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Length0.2 --Data RHIC
	python3 RunOne.py --N -1 --Model MatterLBT2 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Length0.2 --Data LHC
	python3 RunOne.py --N -1 --Model MatterLBT2 --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.10 --CovType Length0.2 --Data RHIC

RunDataFlucutationTest:
	python3 RunOne.py --N -1 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1 --DoBigEarthquake
	python3 RunOne.py --N -1 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1 --DoBigEarthquake
	python3 RunOne.py --N -1 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1 --DoBigEarthquake
	python3 RunOne.py --N -1 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1 --DoBigEarthquake
	python3 RunOne.py --N -1 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1 --DoBigEarthquake

RunClosureExample:
	python3 RunOne.py --N 10 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1
	python3 RunOne.py --N 14 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1
	python3 RunOne.py --N 24 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1
	python3 RunOne.py --N 30 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1
	python3 RunOne.py --N 34 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1
	python3 RunOne.py --N 49 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1
	python3 RunOne.py --N 55 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1
	python3 RunOne.py --N 63 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1
	python3 RunOne.py --N 78 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1
	python3 RunOne.py --N 96 --Model LBT        --Nu 2.5 --KernelChoice MaternNoise --Alpha 0.1

CopyAway:
	cp plots/*pdf ~/WindowsHome/Downloads/

