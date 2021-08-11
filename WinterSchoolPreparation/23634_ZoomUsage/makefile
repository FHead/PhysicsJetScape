
default: RunFinal

Execute: GenerateHistogram.cpp
	CompileRootMacro GenerateHistogram.cpp Execute

RunAll: Execute
	for i in Jul19 Jul20 Jul21 Jul22 Jul23 Jul26 Jul27 Jul28 Jul29 Jul30; do \
		bash ExtractTime.sh $${i}; \
		./Execute --Input List$${i}.root --Output Curve$${i}.root --Tag $${i}; \
	done
	hadd -f Curves.root CurveJul*root
	rm -f CurveJul*root

ExecutePlot: FinalPlot.cpp
	CompileRootMacro FinalPlot.cpp ExecutePlot

RunFinal: ExecutePlot
	./ExecutePlot
