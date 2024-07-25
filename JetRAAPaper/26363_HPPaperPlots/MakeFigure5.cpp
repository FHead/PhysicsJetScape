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
double CalculateOverlap(TH1D *H1, TH1D *H2);

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   vector<string> InputFileName = CL.GetStringVector("Input");
   vector<string> Label = CL.GetStringVector("Label");
   double FixT = 0.2;
   double FixE = 100;
   double FixQ = 100;
   int NBins = 50;
   double WorldXMin = CL.GetDouble("WorldXMin", 1.2);    // 2.5 for gluon CA, 1.1 for quark CA
   double WorldXMax = CL.GetDouble("WorldXMax", 12.0);   // 9.0 for gluon CA, 4.0 for quark CA, but 1/fm and alphaS
   double WorldYMin = CL.GetDouble("WorldYMin", 0);
   double WorldYMax = CL.GetDouble("WorldYMax", 0.68);   // 1.05 for gluon CA, 2.38 for quark CA
   bool DoJet = CL.GetBool("DoJet", true);
   string OutputFileName = CL.Get("Output");
   vector<int> ColorIndex = CL.GetIntVector("Color", vector<int>{-1, -2, -3, -4, -5, -6});
   vector<int> LineStyle = CL.GetIntVector("Style", vector<int>{1, 1, 1, 1, 1, 1});

   int NFile = InputFileName.size();

   vector<vector<double>> AlphaSFix(NFile);
   for(int iF = 0; iF < NFile; iF++)
      AlphaSFix[iF] = ReadAlphaS(InputFileName[iF]);

   vector<TH1D *> HQHat;
   for(int iF = 0; iF < NFile; iF++)
   {
      HQHat.emplace_back(new TH1D(Form("QHat%d", iF), "", NBins, WorldXMin, WorldXMax));

      HQHat[iF]->SetLineColor(GetColor(ColorIndex[iF]));
      HQHat[iF]->SetLineStyle(LineStyle[iF]);
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

   TH2D HWorld("HWorld", ";#hat{q}/T^{3};Posterior (a.u.)", 100, WorldXMin, WorldXMax, 100, WorldYMin, WorldYMax);
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
   Latex.DrawLatex(0.87, 0.87, "E_{ref} = 100 GeV, T_{ref} = 200 MeV");
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

   for(int iF1 = 0; iF1 < NFile; iF1++)
      for(int iF2 = iF1; iF2 < NFile; iF2++)
         cout << "\"" << Label[iF1] << "\" \"" << Label[iF2] << "\" "
            << CalculateOverlap(HQHat[iF1], HQHat[iF2]) << endl;

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

double CalculateOverlap(TH1D *H1, TH1D *H2)
{
   if(H1 == nullptr || H2 == nullptr)
      return -1;

   double Answer = 0;
   for(int i = 1; i <= H1->GetNbinsX(); i++)
   {
      double L = H1->GetXaxis()->GetBinLowEdge(i);
      double R = H1->GetXaxis()->GetBinUpEdge(i);
      Answer = Answer + min(H1->GetBinContent(i), H2->GetBinContent(i)) * (R - L);
   }
  
   return Answer;
}


