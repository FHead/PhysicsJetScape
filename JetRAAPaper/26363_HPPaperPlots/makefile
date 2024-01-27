

default: Figure2

RunFigure1: MakeFigure1.cpp
	CompileRootMacro MakeFigure1.cpp RunFigure1

RunFigure2: MakeFigure2.cpp
	CompileRootMacro MakeFigure2.cpp RunFigure2

Figure1: RunFigure1
	./RunFigure1 --Suffix "_N3_Hadron" --MaxSample -1 \
		--Index 0,1,4 \
		--NBin 50,50,50 \
		--Min 0.1,1,0 \
		--Max 0.49,9.9,1.49 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Nominal,Hadron
	./RunFigure1 --Suffix "_N4_Hadron" --MaxSample -1 \
		--Index 0,1,4,5 \
		--NBin 50,50,50,50 \
		--Min 0.1,1,0,0 \
		--Max 0.49,9.9,1.49,4.6 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}","ln c_{3}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Nominal,Hadron
	./RunFigure1 --Suffix "_N6_Hadron" --MaxSample -1 \
		--Index 0,1,4,2,3,5 \
		--NBin 50,50,50,50,50,50 \
		--Min 0.1,1,0,0,0,0 \
		--Max 0.49,9.9,1.49,2.3,2.3,4.6 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}","ln c_{1}","ln c_{2}","ln c_{3}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Nominal,Hadron
	./RunFigure1 --Suffix "_N3_HadronPT30" --MaxSample -1 \
		--Index 0,1,4 \
		--NBin 50,50,50 \
		--Min 0.1,1,0 \
		--Max 0.49,9.9,1.49 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_PTCut30_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Nominal,"Hadron p_{T} > 30 GeV"
	./RunFigure1 --Suffix "_N6_HadronPT30" --MaxSample -1 \
		--Index 0,1,4,2,3,5 \
		--NBin 50,50,50,50,50,50 \
		--Min 0.1,1,0,0,0,0 \
		--Max 0.49,9.9,1.49,2.3,2.3,4.6 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}","ln c_{1}","ln c_{2}","ln c_{3}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_PTCut30_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Nominal,"Hadron p_{T} > 30 GeV"
	./RunFigure1 --Suffix "_N3" --MaxSample -1 \
		--Index 0,1,4 \
		--NBin 50,50,50 \
		--Min 0.1,1,0 \
		--Max 0.49,9.9,1.49 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 NONE \
		--Legend Nominal,Hadron
	./RunFigure1 --Suffix "_N6" --MaxSample -1 \
		--Index 0,1,4,2,3,5 \
		--NBin 50,50,50,50,50,50 \
		--Min 0.1,1,0,0,0,0 \
		--Max 0.49,9.9,1.49,2.3,2.3,4.6 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}","ln c_{1}","ln c_{2}","ln c_{3}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 NONE \
		--Legend Nominal,Hadron

Figure2: RunFigure2
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt --Output Figure2Raw.pdf --Label "Nominal" --DoJet true --DoOld true
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_ObservableJet/txt/MCMCSamples.txt \
		--Label "Hadron","Jet" --DoJet false \
		--Output Figure3LRaw.pdf


