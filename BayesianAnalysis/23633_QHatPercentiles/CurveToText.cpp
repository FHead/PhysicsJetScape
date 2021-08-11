#include <iostream>
#include <fstream>
using namespace std;

#include "TGraph.h"
#include "TFile.h"

#include "CommandLine.h"

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string InputFileName  = CL.Get("Input");
   string OutputFileName = CL.Get("Output");
   string CurveName      = CL.Get("Curve");

   TFile InputFile(InputFileName.c_str());

   TGraph *G = (TGraph *)InputFile.Get(CurveName.c_str());

   if(G != nullptr)
   {
      ofstream out(OutputFileName);

      int N = G->GetN();
      for(int i = 0; i < N; i++)
      {
         double x, y;
         G->GetPoint(i, x, y);
         out << x << " " << y << endl;
      }

      out.close();
   }

   InputFile.Close();

   return 0;
}


