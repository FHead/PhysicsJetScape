

default: Figure2 Finalize

all: Figure1 Figure2 Figure5 FigureExtra Finalize

Finalize:
	bash AddAllLogo.sh

RunFigure1: MakeFigure1.cpp
	CompileRootMacro MakeFigure1.cpp RunFigure1

RunFigure2: MakeFigure2.cpp
	CompileRootMacro MakeFigure2.cpp RunFigure2

RunFigure5: MakeFigure5.cpp
	CompileRootMacro MakeFigure5.cpp RunFigure5

Figure1: RunFigure1
	./RunFigure1 --Suffix "_N3_Hadron" --MaxSample -1 \
		--Index 0,1,4 \
		--NBin 50,50,50 \
		--Min 0.1,1,0 \
		--Max 0.49,9.9,1.49 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Combined,Hadron
	./RunFigure1 --Suffix "_N4_Hadron" --MaxSample -1 \
		--Index 0,1,4,5 \
		--NBin 50,50,50,50 \
		--Min 0.1,1,0,0 \
		--Max 0.49,9.9,1.49,4.6 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}","ln c_{3}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Combined,Hadron
	./RunFigure1 --Suffix "_N6_Hadron" --MaxSample -1 \
		--Index 0,1,4,2,3,5 \
		--NBin 50,50,50,50,50,50 \
		--Min 0.1,1,0,0,0,0 \
		--Max 0.49,9.9,1.49,2.3,2.3,4.6 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}","ln c_{1}","ln c_{2}","ln c_{3}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Combined,Hadron
	./RunFigure1 --Suffix "_N3_HadronPT30" --MaxSample -1 \
		--Index 0,1,4 \
		--NBin 50,50,50 \
		--Min 0.1,1,0 \
		--Max 0.49,9.9,1.49 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_PTCut30_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Combined,"Hadron p_{T} > 30 GeV"
	./RunFigure1 --Suffix "_N6_HadronPT30" --MaxSample -1 \
		--Index 0,1,4,2,3,5 \
		--NBin 50,50,50,50,50,50 \
		--Min 0.1,1,0,0,0,0 \
		--Max 0.49,9.9,1.49,2.3,2.3,4.6 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}","ln c_{1}","ln c_{2}","ln c_{3}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_PTCut30_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Combined,"Hadron p_{T} > 30 GeV"
	./RunFigure1 --Suffix "_N3" --MaxSample -1 \
		--Index 0,1,4 \
		--NBin 50,50,50 \
		--Min 0.1,1,0 \
		--Max 0.49,9.9,1.49 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 NONE \
		--Legend Combined,Hadron
	./RunFigure1 --Suffix "_N6" --MaxSample -1 \
		--Index 0,1,4,2,3,5 \
		--NBin 50,50,50,50,50,50 \
		--Min 0.1,1,0,0,0,0 \
		--Max 0.49,9.9,1.49,2.3,2.3,4.6 \
		--Label "#alpha_{s}","Q_{0}","#tau_{0}","ln c_{1}","ln c_{2}","ln c_{3}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 NONE \
		--Legend Combined,Hadron
	./RunFigure1 --Suffix "_N2_c1c2_Hadron" --MaxSample -1 \
		--Index 2,3 \
		--NBin 50,50 \
		--Min 0,0 \
		--Max 2.3,2.3 \
		--Label "ln c_{1}","ln c_{2}" \
		--Input1 STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Input2 STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt \
		--Legend Combined,Hadron

Figure2: RunFigure2
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt --Output Figure2Raw1.pdf --Label "Combined" --DoJet true --DoOld false --Color -4
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt --Output Figure2Raw2.pdf --Label "Combined" --DoJet true --DoOld true --OldFileName root/LBT.root --OldLabel "LBT" --Color -4
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt --Output Figure2Raw3.pdf --Label "Combined" --DoJet true --DoOld true --OldFileName root/Matter.root --OldLabel "MATTER" --Color -4
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt --Output Figure2Raw4.pdf --Label "Combined" --DoJet true --DoOld true --OldFileName root/MatterLBT1.root --OldLabel "MATTER+LBT 1" --Color -4
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_ObservableJet/txt/MCMCSamples.txt \
		--Label "Hadron","Jet" --DoJet false \
		--Output Figure3LRaw.pdf --Color -2,-1
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_ObservableJet/txt/MCMCSamples.txt \
		--Label "Hadron","Jet" --DoJet false --DoOld true --OldFileName root/MatterLBT1.root --OldLabel "MATTER+LBT 1" \
		--Output Figure3LWithJETSCAPERaw.pdf --Color -2,-1
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_ObservableJet/txt/MCMCSamples.txt \
		--Label "This analysis: Hadron","This analysis: Jet" --DoJet false --DoOld true --OldFileName root/MatterLBT2.root --OldLabel "MATTER+LBT2"\
		--Output Figure3LWithJETSCAPERaw2.pdf --Color -2,-1
	./RunFigure2 --Input STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt --Output Figure2Raw5.pdf --Label "This analysis: Combined (hadron+jet)" --DoJet true --DoOld true --OldFileName root/MatterLBT2.root --OldLabel "MATTER+LBT2" --Color -4

Figure5: RunFigure5
	./RunFigure5 --Input STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_ObservableJet/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_PTCut10_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_PTCut20_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_PTCut30_ObservableHadron/txt/MCMCSamples.txt \
		--Output Figure5Raw.pdf --Label "Hadron","Jet","Hadron p_{T} > 10 GeV","Hadron p_{T} > 20 GeV","Hadron p_{T} > 30 GeV" --Color -2,-1,-3,-6,-5
	./RunFigure5 --Input STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_ObservableJet/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Output Figure3RRaw.pdf --Label "Hadron","Jet","Combined" --Color -2,-1,-4

FigureExtra: RunFigure5
	./RunFigure5 --Input STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_ObservableJet/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_PTUpTo100/txt/MCMCSamples.txt \
		--Output FigureHighPTRaw.pdf --Label "Hadron","Jet","Combined","Combined p_{T} < 100 GeV" --Color -2,-1,-4,-5
	./RunFigure5 --Input STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_ObservableJet/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_PTUpTo200/txt/MCMCSamples.txt \
		--Output FigureHighPT2Raw.pdf --Label "Hadron","Jet","Combined","Combined p_{T} < 200 GeV" --Color -2,-1,-4,-5
	# ./RunFigure5 --Input STAT/result/STAT20230320ExponentialRBF_N4_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_ObservableJet/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_PTUpTo20_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_PTUpTo30_ObservableHadron/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_PTUpTo30_ObservableJet/txt/MCMCSamples.txt  \
	# 	--Output FigureHighPTRaw.pdf --Label "Hadron","Jet","Hadron p_{T} < 20 GeV","Hadron p_{T} < 30 GeV","Jet p_{T} < 30 GeV" --Color -2,-1,-3,-6,-5
	./RunFigure5 --Input STAT/result/STAT20230320ExponentialRBF_N3_Long_CentralityMax10/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_CentralityMin20/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4/txt/MCMCSamples.txt \
		--Output FigureCentralityRaw.pdf --Label "0-10%","20-50%","Combined" --Color -3,-5,-4 \
		--WorldYMax 0.70   # 1.09 for gluon CA
	./RunFigure5 --Input STAT/result/STAT20230320ExponentialRBF_N3_Long_CentralityMax10/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_CentralityMax10_ObservableJet/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_CentralityMax10_ObservableHadron/txt/MCMCSamples.txt \
		--Output FigureCentrality0to10Raw.pdf --Label "0-10%","0-10% Jet","0-10% Hadron" --Color -3,-1,-2 \
		--WorldYMax 0.70
	./RunFigure5 --Input STAT/result/STAT20230320ExponentialRBF_N4_CentralityMin20/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_CentralityMin20_ObservableJet/txt/MCMCSamples.txt,STAT/result/STAT20230320ExponentialRBF_N4_CentralityMin20_ObservableHadron/txt/MCMCSamples.txt \
		--Output FigureCentrality20to50Raw.pdf --Label "20-50%","20-50% Jet","20-50% Hadron" --Color -5,-1,-2 --Style 1,7,7 \
		--WorldYMax 0.70


