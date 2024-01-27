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
   double ScanMin = 0.14;
   double ScanMax = 0.51;
   double FixT = 0.2;
   double FixE = 100;
   double FixQ = 100;
   string ScanType = "T";
   int StepCount = 100;
   double WorldXMin = 0.15;
   double WorldXMax = 0.50;
   double WorldYMin = 2.05;
   double WorldYMax = 9.50;
   bool DoJet = CL.GetBool("DoJet", true);
   bool DoOld = CL.GetBool("DoOld", false);
   string OldFileName = DoOld ? CL.Get("OldFileName") : "";
   string OldLabel = DoOld ? CL.Get("OldLabel") : "";
   string OutputFileName = CL.Get("Output");
   vector<int> ColorIndex = CL.GetIntVector("Color", vector<int>{-1, -2, -3, -4, -5, -6});

   int NFile = InputFileName.size();

   vector<vector<double>> AlphaSFix(NFile);
   for(int iF = 0; iF < NFile; iF++)
      AlphaSFix[iF] = ReadAlphaS(InputFileName[iF]);

   vector<double> ScanX(StepCount + 1);
   vector<vector<double>> Q05(NFile), Q50(NFile), Q95(NFile);

   for(int i = 0; i <= StepCount; i++)
      ScanX[i] = ScanMin + (ScanMax - ScanMin) / StepCount * i;

   for(int iF = 0; iF < NFile; iF++)
   {
      for(int iS = 0; iS <= StepCount; iS++)
      {
         vector<double> QHatValues(AlphaSFix[iF].size());
         for(int i = 0; i < AlphaSFix[iF].size(); i++)
         {
            double Value = -1;
            if(ScanType == "T")   Value = QHat(AlphaSFix[iF][i], ScanX[iS], FixE, FixQ);
            if(ScanType == "E")   Value = QHat(AlphaSFix[iF][i], FixT, ScanX[iS], FixQ);
            if(ScanType == "Q")   Value = QHat(AlphaSFix[iF][i], FixT, FixE, ScanX[iS]);
            QHatValues[i] = Value;
         }

         sort(QHatValues.begin(), QHatValues.end());

         Q05[iF].push_back(QHatValues[QHatValues.size()*0.0500]);
         Q50[iF].push_back(QHatValues[QHatValues.size()*0.5000]);
         Q95[iF].push_back(QHatValues[QHatValues.size()*0.9500]);
      }
   }

   vector<TGraph> G50(NFile);
   vector<TGraph> G90(NFile);

   for(int iF = 0; iF < NFile; iF++)
   {
      for(int iS = 0; iS <= StepCount; iS++)
      {
         G50[iF].SetPoint(iS, ScanX[iS], Q50[iF][iS]);

         G90[iF].SetPoint(iS, ScanX[iS], Q05[iF][iS]);
         G90[iF].SetPoint(StepCount * 2 - iS + 1, ScanX[iS], Q95[iF][iS]);
      }
      G90[iF].SetPoint(StepCount * 2 + 2, ScanX[0], Q05[iF][0]);
   }

   TGraphErrors GJet;
   GJet.SetPoint(0, 0.365, 4.6);
   GJet.SetPointError(0, 0, 1.2);
   GJet.SetPoint(1, 0.460, 3.7);
   GJet.SetPointError(1, 0, 1.4);
   GJet.SetMarkerStyle(20);
   GJet.SetMarkerSize(2);
   GJet.SetLineWidth(3);

   TGraph G50Old, G90Old;
   if(DoOld)
   {
      TFile OldFile(OldFileName.c_str());
      G50Old = *((TGraph *)OldFile.Get("QHatT0"));
      G90Old = *((TGraph *)OldFile.Get("QHatT90"));
      OldFile.Close();

      G90Old.SetPoint(G90Old.GetN(), G90Old.GetPointX(0), G90Old.GetPointY(0));
      
      G90Old.SetLineWidth(1);
      G90Old.SetLineColorAlpha(GetColor(-5), 0.50);
      G90Old.SetFillColorAlpha(GetColor(-5), 0.25);
      G50Old.SetLineWidth(2);
      G50Old.SetLineColor(GetColor(-5));
      G50Old.SetFillColorAlpha(GetColor(-5), 0.25);
   }

   TCanvas Canvas("Canvas", "", 1024, 1024);

   TH2D HWorld("HWorld", ";T (GeV);#hat{q}/T^{3}", 100, WorldXMin, WorldXMax, 100, WorldYMin, WorldYMax);
   HWorld.SetStats(0);
   HWorld.GetXaxis()->CenterTitle();
   HWorld.GetYaxis()->CenterTitle();

   HWorld.Draw("axis");

   for(int iF = 0; iF < NFile; iF++)
   {
      G90[iF].SetLineWidth(1);
      G90[iF].SetLineColorAlpha(GetColor(ColorIndex[iF]), 0.50);
      G90[iF].SetFillColorAlpha(GetColor(ColorIndex[iF]), 0.25);
      G50[iF].SetLineWidth(2);
      G50[iF].SetLineColor(GetColor(ColorIndex[iF]));
      G50[iF].SetFillColorAlpha(GetColor(ColorIndex[iF]), 0.25);

      G90[iF].Draw("l");
      G90[iF].Draw("f");
      G50[iF].Draw("l");

      if(DoOld == true)
      {
         G90Old.Draw("lf");
         G50Old.Draw("l");
      }
   }

   if(DoJet == true)
      GJet.Draw("p");

   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.035);
   Latex.SetNDC();
   Latex.SetTextAlign(33);
   Latex.DrawLatex(0.87, 0.87, "E = 100 GeV");
   Latex.DrawLatex(0.87, 0.82, "Posterior median and 90% range");
   if(DoOld == true)
   {
      Latex.SetTextAlign(11);
      Latex.DrawLatex(0.10, 0.91, ("(From previous paper: " + OldLabel + ")").c_str());
   }

   TLegend Legend(0.55, 0.75, 0.80, 0.75 - 0.07 * NFile - 0.07 * DoOld - 0.07 * DoJet);
   Legend.SetTextFont(42);
   Legend.SetTextSize(0.035);
   Legend.SetFillStyle(0);
   Legend.SetBorderSize(0);
   for(int iF = 0; iF < NFile; iF++)
      Legend.AddEntry(&G50[iF], Label[iF].c_str(), "lf");
   if(DoOld == true)
      Legend.AddEntry(&G50Old, "PRC 104, 024905", "lf");
   if(DoJet == true)
      Legend.AddEntry(&GJet, "JET Collaboration", "pl");
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






