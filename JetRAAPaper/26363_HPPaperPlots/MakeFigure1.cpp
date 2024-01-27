#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

#include "TH2D.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TExec.h"
#include "TLegend.h"

#include "CommandLine.h"

int main(int argc, char *argv[]);
vector<vector<double>> ReadSample(string FileName, int MaxSample);
void SetAxis(TGaxis *A);

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string FileName1     = CL.Get("Input1");
   string FileName2     = CL.Get("Input2");
   vector<int> Index    = CL.GetIntVector("Index", vector<int>{0, 1, 2});
   vector<int> NBin     = CL.GetIntVector("NBin", vector<int>{50, 50, 50});
   vector<double> Min   = CL.GetDoubleVector("Min", vector<double>{0.1, 1, 0});
   vector<double> Max   = CL.GetDoubleVector("Max", vector<double>{0.5, 10, 1.4});
   vector<int> AxisN    = CL.GetIntVector("AxisN", vector<int>{505, 505, 505, 505, 505, 505});
   vector<string> Label = CL.GetStringVector("Label", vector<string>{"A", "B", "C"});
   vector<string> LegendText = CL.GetStringVector("Legend", vector<string>{"X", "Y"});
   string Suffix        = CL.Get("Suffix");
   int MaxSample        = CL.GetInt("MaxSample", -1);

   vector<vector<double>> Sample1 = ReadSample(FileName1, MaxSample);
   vector<vector<double>> Sample2 = ReadSample(FileName2, MaxSample);

   int N = Index.size();

   // Helper functions
   TExec Exe1("Exe1", "double S1[2] = {0.000, 1.000}; double R1[2] = {1.000, 0.100}; double G1[2] = {1.000, 0.100}; double B1[2] = {1.000, 1.000}; TColor::CreateGradientColorTable(2, S1, R1, G1, B1, 99, 1.0);");
   TExec Exe2("Exe2", "double S1[2] = {0.000, 1.000}; double R1[2] = {1.000, 1.000}; double G1[2] = {1.000, 0.100}; double B1[2] = {1.000, 0.100}; TColor::CreateGradientColorTable(2, S1, R1, G1, B1, 99, 1.0);");
   
   // Declare 1D histograms
   vector<TH1D *> Histogram1(N);
   vector<TH1D *> Histogram2(N);
   for(int i = 0; i < N; i++)
   {
      Histogram1[i] = new TH1D(Form("H1_%d", i), "", NBin[i], Min[i], Max[i]);
      Histogram2[i] = new TH1D(Form("H2_%d", i), "", NBin[i], Min[i], Max[i]);

      Histogram1[i]->SetStats(0);
      Histogram2[i]->SetStats(0);
   }

   // Declare 2D histograms
   vector<vector<TH2D *>> Density(N);
   for(int i = 0; i < N; i++)
      Density[i].resize(N);
   for(int i = 0; i < N; i++)
   {
      for(int j = 0; j < N; j++)
      {
         Density[i][j] = new TH2D(Form("H%d%d", i, j), "",
            NBin[i], Min[i], Max[i], NBin[j], Min[j], Max[j]);
         Density[i][j]->SetStats(0);
         Density[i][j]->GetXaxis()->SetTickLength(0);
         Density[i][j]->GetYaxis()->SetTickLength(0);
      }
   }

   // Fill 1D histograms
   for(int iS = 0; iS < Sample1.size(); iS++)
      for(int i = 0; i < N; i++)
         Histogram1[i]->Fill(Sample1[iS][Index[i]]);
   for(int iS = 0; iS < Sample2.size(); iS++)
      for(int i = 0; i < N; i++)
         Histogram2[i]->Fill(Sample2[iS][Index[i]]);

   for(int i = 0; i < N; i++)
   {
      Histogram1[i]->Scale(1 / Histogram1[i]->Integral());
      Histogram1[i]->SetMinimum(0);
      if(Sample2.size() > 0)
      {
         Histogram2[i]->Scale(1 / Histogram2[i]->Integral());
         Histogram1[i]->SetMaximum(max(Histogram1[i]->GetMaximum(), Histogram2[i]->GetMaximum()) * 1.2);
      }

      Histogram1[i]->SetLineWidth(2);
      Histogram1[i]->SetLineColor(kBlue);
      Histogram1[i]->GetXaxis()->SetTickLength(0);
      Histogram1[i]->GetYaxis()->SetTickLength(0);
      Histogram2[i]->SetLineWidth(2);
      Histogram2[i]->SetLineColor(kRed);
      Histogram2[i]->GetXaxis()->SetTickLength(0);
      Histogram2[i]->GetYaxis()->SetTickLength(0);
   }

   // Fill 2D histograms
   for(int iS = 0; iS < Sample1.size(); iS++) // primary
      for(int i = 0; i < N; i++)
         for(int j = 0; j < N; j++)
            if(i < j)   // primary
               Density[i][j]->Fill(Sample1[iS][Index[i]], Sample1[iS][Index[j]]);
   for(int iS = 0; iS < Sample2.size(); iS++) // alternate
      for(int i = 0; i < N; i++)
         for(int j = 0; j < N; j++)
            if(i > j)
               Density[i][j]->Fill(Sample2[iS][Index[i]], Sample2[iS][Index[j]]);

   // Dimensions
   double MarginL = 75;
   double MarginR = 75;
   double MarginT = 75;
   double MarginB = 75;
   double PadW = 200 * 3 / N;
   double PadH = 200 * 3 / N;

   double CanvasW = MarginL + PadW * N + MarginR;
   double CanvasH = MarginB + PadH * N + MarginT;

   MarginL = MarginL / CanvasW;
   MarginR = MarginR / CanvasW;
   MarginB = MarginB / CanvasH;
   MarginT = MarginT / CanvasH;
   PadW = PadW / CanvasW;
   PadH = PadH / CanvasH;

   // Setup canvas and pad and so on
   TCanvas Canvas("Canvas", "", CanvasW, CanvasH);

   vector<vector<TPad *>> Pad(N);
   for(int i = 0; i < N; i++)
      Pad[i].resize(N);
   for(int i = 0; i < N; i++)
   {
      for(int j = 0; j < N; j++)
      {
         double XMin = MarginL + PadW * i;
         double XMax = MarginL + PadW * (i + 1);
         double YMin = MarginB + PadH * (N - 1 - j);
         double YMax = MarginB + PadH * (N - j);
         Pad[i][j] = new TPad(Form("Pad%d%d", i, j), "", XMin, YMin, XMax, YMax);
         Pad[i][j]->SetLeftMargin(0);
         Pad[i][j]->SetTopMargin(0);
         Pad[i][j]->SetRightMargin(0);
         Pad[i][j]->SetBottomMargin(0);
         Pad[i][j]->SetTickx(0);
         Pad[i][j]->SetTicky(0);
         Pad[i][j]->Draw();
      }
   }

   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.035);
   Latex.SetTextAlign(22);

   vector<TGaxis *> XAxisB(N), XAxisT(N), YAxisL(N), YAxisR(N);
   for(int i = 0; i < N; i++)
   {
      double AxisMin, AxisMax, AxisFix;

      AxisMin = MarginL + PadW * i;
      AxisMax = MarginL + PadW * (i + 1);
      AxisFix = MarginB;
      XAxisB[i] = new TGaxis(AxisMin, AxisFix, AxisMax, AxisFix, Min[i], Max[i], 505, "");
      SetAxis(XAxisB[i]);
      Latex.DrawLatex((AxisMin + AxisMax) / 2, AxisFix * 0.4, Label[i].c_str());
      
      AxisMin = MarginL + PadW * i;
      AxisMax = MarginL + PadW * (i + 1);
      AxisFix = MarginB + PadH * N;
      XAxisT[i] = new TGaxis(AxisMin, AxisFix, AxisMax, AxisFix, Min[i], Max[i], 505, "-");
      if(i != 0 && Sample2.size() > 0)
         SetAxis(XAxisT[i]);
      if(Sample2.size() > 0)
         Latex.DrawLatex((AxisMin + AxisMax) / 2, AxisFix + MarginT * 0.6, Label[i].c_str());

      AxisMin = MarginB + PadH * i;
      AxisMax = MarginB + PadH * (i + 1);
      AxisFix = MarginL;
      YAxisL[i] = new TGaxis(AxisFix, AxisMin, AxisFix, AxisMax, Min[N-1-i], Max[N-1-i], 505, "");
      if(i != N - 1)
         SetAxis(YAxisL[i]);
      Latex.DrawLatex(AxisFix * 0.4, (AxisMin + AxisMax) / 2, Label[N-1-i].c_str());
      
      AxisMin = MarginB + PadH * i;
      AxisMax = MarginB + PadH * (i + 1);
      AxisFix = MarginL + PadW * N;
      YAxisR[i] = new TGaxis(AxisFix, AxisMin, AxisFix, AxisMax, Min[N-1-i], Max[N-1-i], 505, "+L");
      if(i != 0 && Sample2.size() > 0)
         SetAxis(YAxisR[i]);
      if(Sample2.size() > 0)
         Latex.DrawLatex(AxisFix + MarginR * 0.6, (AxisMin + AxisMax) / 2, Label[N-1-i].c_str());
   }

   // Draw things
   for(int i = 0; i < N; i++)
   {
      for(int j = 0; j < N; j++)
      {
         if(i != j)
         {
            if(i > j && Sample2.size() == 0)
               continue;

            Pad[i][j]->cd();
            
            Density[i][j]->Draw("axis");
            (i < j) ? Exe1.Draw() : Exe2.Draw();
            Density[i][j]->Draw("col same");
            Density[i][j]->Draw("axis same");
         }
         if(i == j)
         {
            Pad[i][j]->cd();
            
            Histogram1[i]->Draw("hist");
            if(Sample2.size() != 0)
               Histogram2[i]->Draw("hist same");
         }
      }
   }

   TLegend Legend(0.1, 0.6, 0.7, 0.9);
   Legend.SetTextFont(42);
   Legend.SetTextSize(0.025 / PadH * (PadH * N + MarginB + MarginT));
   Legend.SetBorderSize(0);
   Legend.SetFillStyle(0);
   Legend.AddEntry(Histogram1[0], LegendText[0].c_str(), "l");
   Legend.AddEntry(Histogram2[0], LegendText[1].c_str(), "l");
   if(Sample2.size() > 0)
   {
      Pad[0][0]->cd();
      Legend.Draw();
   }

   // Write to file
   Canvas.SaveAs(("Figure1Raw" + Suffix + ".pdf").c_str());

   // Clean up
   for(int i = 0; i < N; i++)
   {
      delete XAxisB[i];
      delete XAxisT[i];
      delete YAxisL[i];
      delete YAxisR[i];
   }
   for(int i = 0; i < N; i++)
      for(int j = 0; j < N; j++)
         delete Pad[i][j];
   for(int i = 0; i < N; i++)
      for(int j = 0; j < N; j++)
         delete Density[i][j];
   for(int i = 0; i < N; i++)
   {
      delete Histogram1[i];
      delete Histogram2[i];
   }

   return 0;
}

vector<vector<double>> ReadSample(string FileName, int MaxSample)
{
   int N = -1;
   vector<vector<double>> Result;

   ifstream in(FileName);

   while(in)
   {
      char ch[1024];
      ch[0] = '\0';
      in.getline(ch, 1023, '\n');

      if(ch[0] == '\0')
         continue;

      stringstream str(ch);
      
      vector<double> Sample;
      while(str)
      {
         double Temp = -1000;
         str >> Temp;
         if(Temp > -999)
            Sample.push_back(Temp);
      }

      if(N < 0)
         N = Sample.size();
      else
         Sample.resize(N);

      Result.push_back(Sample);

      if(MaxSample > 0 && Result.size() >= MaxSample)
         break;
   }

   in.close();

   return Result;
}

void SetAxis(TGaxis *A)
{
   if(A == nullptr)
      return;
   A->SetLabelFont(42);
   A->SetLabelSize(0.030);
   // A.SetMaxDigits(6);
   // A.SetMoreLogLabels();
   // A.SetNoExponent();
   A->Draw();
}


