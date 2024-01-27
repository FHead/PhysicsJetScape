#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#include "TCanvas.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TFile.h"

#include "CommandLine.h"
#include "SetStyle.h"

#include "QHat.h"

int main(int argc, char *argv[]);
int GetColor(int ID);

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   vector<string> InputFileName = CL.GetStringVector("Input");
   vector<string> Label = CL.GetStringVector("Label");
   double FixT = 0.2;
   double FixE = 100;
   double FixQ = 100;
   int NBins = 50;
   double WorldXMin = 2.5;
   double WorldXMax = 9.0;
   double WorldYMin = 0;
   double WorldYMax = 1.05;
   bool DoJet = CL.GetBool("DoJet", true);
   string OutputFileName = CL.Get("Output");
   vector<int> ColorIndex = CL.GetIntVector("Color", vector<int>{-1, -2, -3, -4, -5, -6});

   int NFile = InputFileName.size();

   vector<vector<double>> AlphaSFix(NFile);
   for(int iF = 0; iF < NFile; iF++)
      AlphaSFix[iF] = ReadAlphaS(InputFileName[iF]);

   vector<TH1D *> HQHat;
   for(int iF = 0; iF < NFile; iF++)
   {
      HQHat.emplace_back(new TH1D(Form("QHat%d", iF), "", NBins, WorldXMin, WorldXMax));

      HQHat[iF]->SetLineColor(GetColor(ColorIndex[iF]));
      HQHat[iF]->SetLineWidth(3);
   }

   for(int iF = 0; iF < NFile; iF++)
   {
      for(int i = 0; i < AlphaSFix[iF].size(); i++)
        HQHat[iF]->Fill(QHat(AlphaSFix[iF][i], FixT, FixE, FixQ));
      if(HQHat[iF]->Integral() > 0)
         HQHat[iF]->Scale(1 / HQHat[iF]->Integral() / ((WorldXMax - WorldXMin) / NBins));
   }

   TCanvas Canvas("Canvas", "", 1024, 1024);

   TH2D HWorld("HWorld", ";#hat{q}/T^{3};a.u.", 100, WorldXMin, WorldXMax, 100, WorldYMin, WorldYMax);
   HWorld.SetStats(0);
   HWorld.GetXaxis()->CenterTitle();
   HWorld.GetYaxis()->CenterTitle();

   HWorld.Draw("axis");

   for(int iF = 0; iF < NFile; iF++)
      HQHat[iF]->Draw("hist same");

   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.035);
   Latex.SetNDC();
   Latex.SetTextAlign(33);
   Latex.DrawLatex(0.87, 0.87, "E = 100 GeV, T = 200 MeV");
   Latex.DrawLatex(0.87, 0.82, "Posterior");

   TLegend Legend(0.15, 0.88, 0.40, 0.88 - 0.06 * NFile);
   Legend.SetTextFont(42);
   Legend.SetTextSize(0.035);
   Legend.SetFillStyle(0);
   Legend.SetBorderSize(0);
   for(int iF = 0; iF < NFile; iF++)
      Legend.AddEntry(HQHat[iF], Label[iF].c_str(), "l");
   Legend.Draw();

   Canvas.SaveAs(OutputFileName.c_str());

   return 0;
}

int GetColor(int ID)
{
   static vector<int> Colors = GetCVDColors6();

   if(ID == -1)   return Colors[0];
   if(ID == -2)   return Colors[1];
   if(ID == -3)   return Colors[2];
   if(ID == -4)   return Colors[3];
   if(ID == -5)   return Colors[4];
   if(ID == -6)   return Colors[5];

   return ID;
}



