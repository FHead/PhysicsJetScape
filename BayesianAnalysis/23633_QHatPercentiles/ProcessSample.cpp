#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "TSpline.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TGraphErrors.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[]);
void ProcessPosterior(TFile &F, string Tag, vector<string> &Name, vector<double> &Min, vector<double> &Max, bool DoTransform);
void ProcessQHat(TFile &F, string Tag, int N, string Setting, string Type = "QHat", string Suffix = "");
vector<vector<double>> ProcessData(TFile &F, string Tag);
void ProcessRAAPosterior(TFile &F, string Tag, vector<vector<double>> Xs,
   vector<double> XMin, vector<double> XMax, vector<double> YMin, vector<double> YMax, bool IsPrior = false);
void ProcessMedian(TFile &F, string Tag, vector<vector<double>> Xs);
void Set1DHistogram(TH1D *H);
void Set2DHistogram(TH2D *H);
double Formula1(double A, double B, double C, double D, double E, double T);
double Formula2(double A, double B, double C, double D, double E, double T, bool LBTOnly);

int main(int argc, char *argv[])
{
   SetThesisStyle();

   // gStyle->SetPalette(51);

   CommandLine CL(argc, argv);

   string FileTag         = CL.Get("tag");
   vector<string> Name    = CL.GetStringVector("name", {"A", "B", "C", "D"});
   vector<double> Min     = CL.GetDoubleVector("min", vector<double>({0, 0, 0, 0}));
   vector<double> Max     = CL.GetDoubleVector("max", vector<double>({2, 20, 2, 20}));
   vector<double> XMin    = CL.GetDoubleVector("xmin", vector<double>({0, 0, 0, 0, 0, 0}));
   vector<double> XMax    = CL.GetDoubleVector("xmax", vector<double>({20, 20, 20, 20, 20, 20}));
   vector<double> YMin    = CL.GetDoubleVector("ymin", vector<double>({0, 0, 0, 0, 0, 0}));
   vector<double> YMax    = CL.GetDoubleVector("ymax", vector<double>({1, 1, 1, 1, 1, 1}));
   bool DoTransform       = CL.GetBool("transform", false);
   string Output          = CL.Get("output", "Scatter.root");
   string Setting         = CL.Get("setting", "Matter");

   TFile OutputFile(Output.c_str(), "RECREATE");

   // ProcessPosterior(OutputFile, FileTag, Name, Min, Max, DoTransform);
   ProcessQHat(OutputFile, FileTag, Name.size(), Setting);
   ProcessQHat(OutputFile, FileTag, Name.size(), Setting, "Prior", "Prior");
   // vector<vector<double>> Xs = ProcessData(OutputFile, FileTag);
   // ProcessRAAPosterior(OutputFile, FileTag, Xs, XMin, XMax, YMin, YMax, true);
   // ProcessRAAPosterior(OutputFile, FileTag, Xs, XMin, XMax, YMin, YMax, false);
   // ProcessMedian(OutputFile, FileTag, Xs);

   OutputFile.Close();

   return 0;
}

void ProcessPosterior(TFile &F, string Tag, vector<string> &Name, vector<double> &Min, vector<double> &Max, bool DoTransform)
{
   int N = Name.size();

   F.cd();

   vector<TH1D *> HDiagonal;
   vector<vector<TH2D *>> HScatter;

   int NBin = 100;

   for(int i = 0; i < N; i++)
   {
      HDiagonal.push_back(new TH1D(Form("HPosterior%d", i),
         Form(";%s;", Name[i].c_str()),
         NBin, Min[i], Max[i]));
      Set1DHistogram(HDiagonal[i]);
   }
   for(int i = 0; i < N; i++)
   {
      HScatter.push_back(vector<TH2D *>());
      for(int j = 0; j < N; j++)
      {
         if(i == j)
            HScatter[i].push_back(nullptr);
         else
         {
            HScatter[i].push_back(new TH2D(Form("HPosterior%d%d", i, j),
               Form(";%s;%s", Name[i].c_str(), Name[j].c_str()),
               NBin, Min[i], Max[i], NBin, Min[j], Max[j]));
            Set2DHistogram(HScatter[i][j]);
         }
      }
   }

   ifstream in("txt/" + Tag + "_MCMCSamples.txt");

   while(in)
   {
      char ch[10240];
      ch[0] = '\0';
      in.getline(ch, 10239, '\n');
      if(ch[0] == '\0')
         continue;

      stringstream str(ch);

      double Temp[10] = {0};
      for(int i = 0; i < N; i++)
         str >> Temp[i];

      if(DoTransform == true)
      {
         double A = Temp[0];
         double B = Temp[1];

         Temp[0] = A * B;
         Temp[1] = A - A * B;
      }

      for(int i = 0; i < N; i++)
         HDiagonal[i]->Fill(Temp[i]);
      for(int i = 0; i < N; i++)
      {
         for(int j = 0; j < N; j++)
         {
            if(i == j)
               continue;
            HScatter[i][j]->Fill(Temp[i], Temp[j]);
         }
      }
   }

   in.close();

   F.cd();

   for(int i = 0; i < N; i++)
      HDiagonal[i]->Write();
   for(int i = 0; i < N; i++)
      for(int j = 0; j < N; j++)
         if(HScatter[i][j] != nullptr)
            HScatter[i][j]->Write();
}

void ProcessQHat(TFile &F, string Tag, int N, string Setting, string Type, string Suffix)
{
   vector<vector<double>> Samples;

   string FileName = "txt/" + Tag + "_MCMCSamples.txt";
   if(Type == "Prior")
      FileName = "txt/" + Tag + "_QHatPrior.txt";
   ifstream in(FileName);

   while(in)
   {
      char ch[10240];
      ch[0] = '\0';
      in.getline(ch, 10239, '\n');
      if(ch[0] == '\0')
         continue;

      stringstream str(ch);

      double Temp[10] = {0};
      for(int i = 0; i < N; i++)
         str >> Temp[i];

      Samples.push_back(vector<double>(Temp, Temp + N));

      if(Samples.size() >= 250000)
         break;
   }

   in.close();

   cout << "Sample size for QHat = " << Samples.size() << endl;

   if(Setting != "MatterLBT2")
   {
      bool DoTransform = (Setting == "MatterLBT1");

      // T dependence at p = 100 GeV
      {
         int Bin = 200;
         double MinT = 0.17;
         double MaxT = 0.78;
         double E = 100;

         vector<double> TH99, TL99;
         vector<double> TH68, TL68;
         vector<vector<double>> THX0(9), TLX0(9);
         vector<double> TM, TA;

         vector<double> QHat(Samples.size());
         double QHatSum = 0;

         for(int i = 0; i < Bin; i++)
         {
            double T = MinT + (MaxT - MinT) / Bin * (i + 0.5);

            for(int j = 0; j < (int)Samples.size(); j++)
            {
               double A, B, C, D;
               if(DoTransform == false)
               {
                  A = Samples[j][0];
                  B = Samples[j][1];
                  C = Samples[j][2];
                  D = Samples[j][3];
               }
               else
               {
                  A = Samples[j][0] * Samples[j][1];
                  C = Samples[j][0] - Samples[j][0] * Samples[j][1];
                  B = Samples[j][2];
                  D = Samples[j][3];
               }

               QHat[j] = Formula1(A, B, C, D, E, T);
               QHatSum = QHatSum + QHat[j];
            }

            sort(QHat.begin(), QHat.end());

            TH99.push_back(QHat[QHat.size()*0.0050]);
            TL99.push_back(QHat[QHat.size()*0.9950]);
            TH68.push_back(QHat[QHat.size()*0.8415]);
            TL68.push_back(QHat[QHat.size()*0.1585]);
            for(int X = 1; X <= 9; X++)
            {
               double XMin = 0.5 - X * 0.05;
               double XMax = 0.5 + X * 0.05;
               THX0[X-1].push_back(QHat[QHat.size()*XMin]);
               TLX0[X-1].push_back(QHat[QHat.size()*XMax]);
            }
            TM.push_back(QHat[QHat.size()*0.5]);
            TA.push_back(QHatSum / Samples.size());
         }

         TGraph QHatT99, QHatT90, QHatT68, QHatTM, QHatTA;
         vector<TGraph> QHatTX0(9);
         QHatT99.SetName(("QHatT" + Suffix + "99").c_str());
         for(int X = 1; X <= 9; X++)
            QHatTX0[X-1].SetName(("QHatT" + Suffix + to_string(X) + "0").c_str());
         QHatT68.SetName(("QHatT" + Suffix + "68").c_str());
         QHatTM.SetName(("QHatT" + Suffix + "0").c_str());
         QHatTA.SetName(("QHatT" + Suffix + "Average").c_str());

         for(int i = 0; i < Bin; i++)
         {
            double T = MinT + (MaxT - MinT) / Bin * (i + 0.5);
            QHatT99.SetPoint(i, T, TL99[i]);
            QHatT99.SetPoint(2 * Bin - i - 1, T, TH99[i]);
            for(int X = 1; X <= 9; X++)
            {
               QHatTX0[X-1].SetPoint(i, T, TLX0[X-1][i]);
               QHatTX0[X-1].SetPoint(2 * Bin - i - 1, T, THX0[X-1][i]);
            }
            QHatT68.SetPoint(i, T, TL68[i]);
            QHatT68.SetPoint(2 * Bin - i - 1, T, TH68[i]);
            QHatTM.SetPoint(i, T, TM[i]);
            QHatTA.SetPoint(i, T, TA[i]);
         }

         QHatT99.Write();
         for(int X = 1; X <= 9; X++)
            QHatTX0[X-1].Write();
         QHatT68.Write();
         QHatTM.Write();
         QHatTA.Write();
      }

      // p dependence at fixed T
      {
         int Bin = 200;
         double MinP = 5;
         double MaxP = 200;
         double T = 0.3;

         vector<double> PH99, PL99, PH68, PL68, PM, PA;
         vector<vector<double>> PHX0(9), PLX0(9);

         vector<double> QHat(Samples.size());
         double QHatSum = 0;

         for(int i = 0; i < Bin; i++)
         {
            double P = MinP + (MaxP - MinP) / Bin * (i + 0.5);

            for(int j = 0; j < (int)Samples.size(); j++)
            {
               double A, B, C, D;
               if(DoTransform == false)
               {
                  A = Samples[j][0];
                  B = Samples[j][1];
                  C = Samples[j][2];
                  D = Samples[j][3];
               }
               else
               {
                  A = Samples[j][0] * Samples[j][1];
                  C = Samples[j][0] - Samples[j][0] * Samples[j][1];
                  B = Samples[j][2];
                  D = Samples[j][3];
               }

               QHat[j] = Formula1(A, B, C, D, P, T);
               QHatSum = QHatSum + QHat[j];
            }

            sort(QHat.begin(), QHat.end());

            PH99.push_back(QHat[QHat.size()*0.0050]);
            PL99.push_back(QHat[QHat.size()*0.9950]);
            PH68.push_back(QHat[QHat.size()*0.8415]);
            PL68.push_back(QHat[QHat.size()*0.1585]);
            for(int X = 1; X <= 9; X++)
            {
               double XMin = 0.5 - X * 0.05;
               double XMax = 0.5 + X * 0.05;
               PHX0[X-1].push_back(QHat[QHat.size()*XMin]);
               PLX0[X-1].push_back(QHat[QHat.size()*XMax]);
            }
            PM.push_back(QHat[QHat.size()*0.5]);
            PA.push_back(QHatSum / Samples.size());
         }

         TGraph QHatP99, QHatP68, QHatPM, QHatPA;
         vector<TGraph> QHatPX0(9);
         QHatP99.SetName(("QHatP" + Suffix + "99").c_str());
         for(int X = 1; X <= 9; X++)
            QHatPX0[X-1].SetName(("QHatP" + Suffix + to_string(X) + "0").c_str());
         QHatP68.SetName(("QHatP" + Suffix + "68").c_str());
         QHatPM.SetName(("QHatP" + Suffix + "0").c_str());
         QHatPA.SetName(("QHatP" + Suffix + "Average").c_str());

         for(int i = 0; i < Bin; i++)
         {
            double P = MinP + (MaxP - MinP) / Bin * (i + 0.5);
            QHatP99.SetPoint(i, P, PL99[i]);
            QHatP99.SetPoint(2 * Bin - i - 1, P, PH99[i]);
            for(int X = 1; X <= 9; X++)
            {
               QHatPX0[X-1].SetPoint(i, P, PLX0[X-1][i]);
               QHatPX0[X-1].SetPoint(2 * Bin - i - 1, P, PHX0[X-1][i]);
            }
            QHatP68.SetPoint(i, P, PL68[i]);
            QHatP68.SetPoint(2 * Bin - i - 1, P, PH68[i]);
            QHatPM.SetPoint(i, P, PM[i]);
            QHatPA.SetPoint(i, P, PA[i]);
         }

         QHatP99.Write();
         for(int X = 1; X <= 9; X++)
            QHatPX0[X-1].Write();
         QHatP68.Write();
         QHatPM.Write();
         QHatPA.Write();
      }
   }
   else
   {
      // T dependence at p = 100 GeV
      {
         int Bin = 200;
         double MinT = 0.17;
         double MaxT = 0.78;
         double E = 100;

         vector<double> TMH99, TML99, TMH68, TML68, TMM, TMA;
         vector<double> TLH99, TLL99, TLH68, TLL68, TLM, TLA;
         vector<vector<double>> TMHX0(9), TMLX0(9);
         vector<vector<double>> TLHX0(9), TLLX0(9);

         vector<double> QHatM(Samples.size());
         vector<double> QHatL(Samples.size());
         double QHatMSum = 0;
         double QHatLSum = 0;

         for(int i = 0; i < Bin; i++)
         {
            double T = MinT + (MaxT - MinT) / Bin * (i + 0.5);

            for(int j = 0; j < (int)Samples.size(); j++)
            {
               double A, C, D, Q;
               A = Samples[j][0] * Samples[j][1];
               C = Samples[j][0] - Samples[j][0] * Samples[j][1];
               D = Samples[j][2];
               Q = Samples[j][3];

               QHatM[j] = Formula2(A, C, D, Q, E, T, false);
               QHatL[j] = Formula2(A, C, D, Q, E, T, true);

               QHatMSum = QHatMSum + QHatM[j];
               QHatLSum = QHatLSum + QHatL[j];
            }

            sort(QHatM.begin(), QHatM.end());
            sort(QHatL.begin(), QHatL.end());

            TMH99.push_back(QHatM[QHatM.size()*0.0050]);
            TML99.push_back(QHatM[QHatM.size()*0.9950]);
            for(int X = 1; X <= 9; X++)
            {
               double XMin = 0.5 - X * 0.05;
               double XMax = 0.5 + X * 0.05;
               TMHX0[X-1].push_back(QHatM[QHatM.size()*XMin]);
               TMLX0[X-1].push_back(QHatM[QHatM.size()*XMax]);
            }
            TMH68.push_back(QHatM[QHatM.size()*0.8415]);
            TML68.push_back(QHatM[QHatM.size()*0.1585]);
            TMM.push_back(  QHatM[QHatM.size()*0.5000]);
            TMA.push_back(QHatMSum / Samples.size());
            TLH99.push_back(QHatL[QHatL.size()*0.0050]);
            TLL99.push_back(QHatL[QHatL.size()*0.9950]);
            for(int X = 1; X <= 9; X++)
            {
               double XMin = 0.5 - X * 0.05;
               double XMax = 0.5 + X * 0.05;
               TLHX0[X-1].push_back(QHatL[QHatL.size()*XMin]);
               TLLX0[X-1].push_back(QHatL[QHatL.size()*XMax]);
            }
            TLH68.push_back(QHatL[QHatL.size()*0.8415]);
            TLL68.push_back(QHatL[QHatL.size()*0.1585]);
            TLM.push_back(  QHatL[QHatL.size()*0.5000]);
            TLA.push_back(QHatLSum / Samples.size());
         }

         TGraph QHatTM99, QHatTM68, QHatTMM, QHatTMA;
         TGraph QHatTL99, QHatTL68, QHatTLM, QHatTLA;
         vector<TGraph> QHatTMX0(9);
         vector<TGraph> QHatTLX0(9);
         QHatTM99.SetName(("QHatTM" + Suffix + "99").c_str());
         for(int X = 1; X <= 9; X++)
            QHatTMX0[X-1].SetName(("QHatTM" + Suffix + to_string(X) + "0").c_str());
         QHatTM68.SetName(("QHatTM" + Suffix + "68").c_str());
         QHatTMM .SetName(("QHatTM" + Suffix + "0").c_str());
         QHatTMA .SetName(("QHatTM" + Suffix + "Average").c_str());
         QHatTL99.SetName(("QHatTL" + Suffix + "99").c_str());
         for(int X = 1; X <= 9; X++)
            QHatTLX0[X-1].SetName(("QHatTL" + Suffix + to_string(X) + "0").c_str());
         QHatTL68.SetName(("QHatTL" + Suffix + "68").c_str());
         QHatTLM .SetName(("QHatTL" + Suffix + "0").c_str());
         QHatTLA .SetName(("QHatTL" + Suffix + "Average").c_str());

         for(int i = 0; i < Bin; i++)
         {
            double T = MinT + (MaxT - MinT) / Bin * (i + 0.5);
            QHatTM99.SetPoint(i, T, TML99[i]);
            QHatTM99.SetPoint(2 * Bin - i - 1, T, TMH99[i]);
            for(int X = 1; X <= 9; X++)
            {
               QHatTMX0[X-1].SetPoint(i, T, TMLX0[X-1][i]);
               QHatTMX0[X-1].SetPoint(2 * Bin - i - 1, T, TMHX0[X-1][i]);
            }
            QHatTM68.SetPoint(i, T, TML68[i]);
            QHatTM68.SetPoint(2 * Bin - i - 1, T, TMH68[i]);
            QHatTMM.SetPoint(i, T, TMM[i]);
            QHatTMA.SetPoint(i, T, TMA[i]);
            QHatTL99.SetPoint(i, T, TLL99[i]);
            QHatTL99.SetPoint(2 * Bin - i - 1, T, TLH99[i]);
            for(int X = 1; X <= 9; X++)
            {
               QHatTLX0[X-1].SetPoint(i, T, TLLX0[X-1][i]);
               QHatTLX0[X-1].SetPoint(2 * Bin - i - 1, T, TLHX0[X-1][i]);
            }
            QHatTL68.SetPoint(i, T, TLL68[i]);
            QHatTL68.SetPoint(2 * Bin - i - 1, T, TLH68[i]);
            QHatTLM.SetPoint(i, T, TLM[i]);
            QHatTLA.SetPoint(i, T, TLA[i]);
         }

         QHatTM99.Write();
         for(int X = 1; X <= 9; X++)
            QHatTMX0[X-1].Write();
         QHatTM68.Write();
         QHatTMM.Write();
         QHatTMA.Write();
         QHatTL99.Write();
         for(int X = 1; X <= 9; X++)
            QHatTLX0[X-1].Write();
         QHatTL68.Write();
         QHatTLM.Write();
         QHatTLA.Write();
      }

      // P dependence at T = 0.3 GeV
      {
         int Bin = 200;
         double MinP = 5;
         double MaxP = 200;
         double T = 0.3;

         vector<double> PMH99, PML99, PMH68, PML68, PMM, PMA;
         vector<double> PLH99, PLL99, PLH68, PLL68, PLM, PLA;
         vector<vector<double>> PMHX0(9), PMLX0(9);
         vector<vector<double>> PLHX0(9), PLLX0(9);

         vector<double> QHatM(Samples.size());
         vector<double> QHatL(Samples.size());
         double QHatMSum = 0;
         double QHatLSum = 0;

         for(int i = 0; i < Bin; i++)
         {
            double P = MinP + (MaxP - MinP) / Bin * (i + 0.5);

            for(int j = 0; j < (int)Samples.size(); j++)
            {
               double A, C, D, Q;
               A = Samples[j][0] * Samples[j][1];
               C = Samples[j][0] - Samples[j][0] * Samples[j][1];
               D = Samples[j][2];
               Q = Samples[j][3];

               QHatM[j] = Formula2(A, C, D, Q, P, T, false);
               QHatL[j] = Formula2(A, C, D, Q, P, T, true);

               QHatMSum = QHatMSum + QHatM[j];
               QHatLSum = QHatLSum + QHatL[j];
            }

            sort(QHatM.begin(), QHatM.end());
            sort(QHatL.begin(), QHatL.end());

            PMH99.push_back(QHatM[QHatM.size()*0.0050]);
            PML99.push_back(QHatM[QHatM.size()*0.9950]);
            for(int X = 1; X <= 9; X++)
            {
               double XMin = 0.5 - X * 0.05;
               double XMax = 0.5 + X * 0.05;
               PMHX0[X-1].push_back(QHatM[QHatM.size()*XMin]);
               PMLX0[X-1].push_back(QHatM[QHatM.size()*XMax]);
            }
            PMH68.push_back(QHatM[QHatM.size()*0.8415]);
            PML68.push_back(QHatM[QHatM.size()*0.1585]);
            PMM.push_back(  QHatM[QHatM.size()*0.5000]);
            PMA.push_back(QHatMSum / Samples.size());
            PLH99.push_back(QHatL[QHatL.size()*0.0050]);
            PLL99.push_back(QHatL[QHatL.size()*0.9950]);
            for(int X = 1; X <= 9; X++)
            {
               double XMin = 0.5 - X * 0.05;
               double XMax = 0.5 + X * 0.05;
               PLHX0[X-1].push_back(QHatL[QHatL.size()*XMin]);
               PLLX0[X-1].push_back(QHatL[QHatL.size()*XMax]);
            }
            PLH68.push_back(QHatL[QHatL.size()*0.8415]);
            PLL68.push_back(QHatL[QHatL.size()*0.1585]);
            PLM.push_back(  QHatL[QHatL.size()*0.5000]);
            PLA.push_back(QHatMSum / Samples.size());
         }

         TGraph QHatPM99, QHatPM68, QHatPMM, QHatPMA;
         TGraph QHatPL99, QHatPL68, QHatPLM, QHatPLA;
         vector<TGraph> QHatPMX0(9);
         vector<TGraph> QHatPLX0(9);
         QHatPM99.SetName(("QHatPM" + Suffix + "99").c_str());
         for(int X = 1; X <= 9; X++)
            QHatPMX0[X-1].SetName(("QHatPM" + Suffix + to_string(X) + "0").c_str());
         QHatPM68.SetName(("QHatPM" + Suffix + "68").c_str());
         QHatPMM .SetName(("QHatPM" + Suffix + "0").c_str());
         QHatPMA .SetName(("QHatPM" + Suffix + "Average").c_str());
         QHatPL99.SetName(("QHatPL" + Suffix + "99").c_str());
         for(int X = 1; X <= 9; X++)
            QHatPLX0[X-1].SetName(("QHatPL" + Suffix + to_string(X) + "0").c_str());
         QHatPL68.SetName(("QHatPL" + Suffix + "68").c_str());
         QHatPLM .SetName(("QHatPL" + Suffix + "0").c_str());
         QHatPLA .SetName(("QHatPL" + Suffix + "Average").c_str());

         for(int i = 0; i < Bin; i++)
         {
            double P = MinP + (MaxP - MinP) / Bin * (i + 0.5);
            QHatPM99.SetPoint(i, P, PML99[i]);
            QHatPM99.SetPoint(2 * Bin - i - 1, P, PMH99[i]);
            for(int X = 1; X <= 9; X++)
            {
               QHatPMX0[X-1].SetPoint(i, P, PMLX0[X-1][i]);
               QHatPMX0[X-1].SetPoint(2 * Bin - i - 1, P, PMHX0[X-1][i]);
            }
            QHatPM68.SetPoint(i, P, PML68[i]);
            QHatPM68.SetPoint(2 * Bin - i - 1, P, PMH68[i]);
            QHatPMM.SetPoint(i, P, PMM[i]);
            QHatPMA.SetPoint(i, P, PMA[i]);
            QHatPL99.SetPoint(i, P, PLL99[i]);
            QHatPL99.SetPoint(2 * Bin - i - 1, P, PLH99[i]);
            for(int X = 1; X <= 9; X++)
            {
               QHatPLX0[X-1].SetPoint(i, P, PLLX0[X-1][i]);
               QHatPLX0[X-1].SetPoint(2 * Bin - i - 1, P, PLHX0[X-1][i]);
            }
            QHatPL68.SetPoint(i, P, PLL68[i]);
            QHatPL68.SetPoint(2 * Bin - i - 1, P, PLH68[i]);
            QHatPLM.SetPoint(i, P, PLM[i]);
            QHatPLA.SetPoint(i, P, PLA[i]);
         }

         QHatPM99.Write();
         for(int X = 1; X <= 9; X++)
            QHatPMX0[X-1].Write();
         QHatPM68.Write();
         QHatPMM.Write();
         QHatPMA.Write();
         QHatPL99.Write();
         for(int X = 1; X <= 9; X++)
            QHatPLX0[X-1].Write();
         QHatPL68.Write();
         QHatPLM.Write();
         QHatPLA.Write();
      }
   }
}

vector<vector<double>> ProcessData(TFile &F, string Tag)
{
   vector<vector<double>> Xs;

   F.cd();

   ifstream inX("txt/" + Tag + "_X.txt");
   ifstream inY("txt/" + Tag + "_Y.txt");
   ifstream inE("txt/" + Tag + "_E.txt");
   ifstream inS("txt/" + Tag + "_S.txt");

   int Count = 0;

   while(inX && inY && inE && inS)
   {
      char chX[10240] = "";
      char chY[10240] = "";
      char chE[10240] = "";
      char chS[10240] = "";

      inX.getline(chX, 10239, '\n');
      inY.getline(chY, 10239, '\n');
      inE.getline(chE, 10239, '\n');
      inS.getline(chS, 10239, '\n');

      if(chX[0] == '\0')   continue;
      if(chY[0] == '\0')   continue;
      if(chE[0] == '\0')   continue;
      if(chS[0] == '\0')   continue;

      stringstream strX(chX);
      stringstream strY(chY);
      stringstream strE(chE);
      stringstream strS(chS);

      TGraphErrors G, GS;
      G.SetName(Form("Data%d", Count));
      GS.SetName(Form("DataS%d", Count));

      vector<double> X;

      while(strX && strY && strE && strS)
      {
         double TempX = -1, TempY = -1, TempE = -1, TempS = -1;
         strX >> TempX;
         strY >> TempY;
         strE >> TempE;
         strS >> TempS;

         if(TempX < 0 || TempY < 0 || TempE < 0 || TempS < 0)
            continue;

         int i = G.GetN();
         G.SetPoint(i, TempX, TempY);
         G.SetPointError(i, 0, TempE);

         GS.SetPoint(i, TempX, TempY);
         GS.SetPointError(i, 0, TempS);

         X.push_back(TempX);
      }

      Xs.push_back(X);

      G.Write();
      GS.Write();
      
      Count = Count + 1;
   }

   inS.close();
   inE.close();
   inY.close();
   inX.close();

   return Xs;
}

void ProcessRAAPosterior(TFile &F, string Tag, vector<vector<double>> Xs,
   vector<double> XMin, vector<double> XMax, vector<double> YMin, vector<double> YMax, bool IsPrior)
{
   vector<TH2D *> H;

   F.cd();

   int N = Xs.size();
   vector<double> XBinSize(N);
   vector<double> YBinSize(N);
   for(int i = 0; i < N; i++)
   {
      int NBin = 300;
      string Name = Form("HRAA%s%d", (IsPrior ? "Prior" : "Posterior"), i);
      H.push_back(new TH2D(Name.c_str(), ";p_{T};R_{AA}", NBin, XMin[i], XMax[i], NBin, YMin[i], YMax[i]));
      XBinSize[i] = (XMax[i] - XMin[i]) / NBin;
      YBinSize[i] = (YMax[i] - YMin[i]) / NBin;
   }

   string FileName = "txt/" + Tag + (IsPrior ? "_Prior" : "_Posterior");

   for(int i = 0; i < N; i++)
   {
      cout << i + 1 << "/" << N << endl;

      ifstream in(FileName + Form("%d", i + 1) + ".txt");

      int Count = 0;

      while(in)
      {
         // cout << Count << "!" << endl;

         char ch[10240] = "";
         in.getline(ch, 10239, '\n');
         if(ch[0] == '\0')
            continue;
         
         stringstream str(ch);

         bool HasNegative = false;

         TGraph G;
         for(int j = 0; j < (int)Xs[i].size(); j++)
         {
            double Temp = -1;
            str >> Temp;
            if(Temp < 0)
            {
               HasNegative = true;
               continue;
            }

            G.SetPoint(j, Xs[i][j], Temp);
         }

         if(HasNegative == true)
            continue;

         TSpline3 L("L", &G);

         for(int j = 0; j < (int)H[i]->GetNbinsX(); j++)
         {
            double X = H[i]->GetXaxis()->GetBinCenter(j);
            double Y = L.Eval(X);
            H[i]->Fill(X,               Y,               0.90 * 0.90);
            H[i]->Fill(X,               Y - YBinSize[i], 0.05 * 0.90);
            H[i]->Fill(X,               Y + YBinSize[i], 0.05 * 0.90);
            H[i]->Fill(X - XBinSize[i], Y,               0.90 * 0.05);
            H[i]->Fill(X - XBinSize[i], Y - YBinSize[i], 0.05 * 0.05);
            H[i]->Fill(X - XBinSize[i], Y + YBinSize[i], 0.05 * 0.05);
            H[i]->Fill(X + XBinSize[i], Y,               0.90 * 0.05);
            H[i]->Fill(X + XBinSize[i], Y - YBinSize[i], 0.05 * 0.05);
            H[i]->Fill(X + XBinSize[i], Y + YBinSize[i], 0.05 * 0.05);
         }

         Count = Count + 1;
      }

      in.close();
   }

   for(int i = 0; i < N; i++)
      H[i]->Write();
}

void ProcessMedian(TFile &F, string Tag, vector<vector<double>> Xs)
{
   ifstream in("txt/" + Tag + "_MedianPrediction.txt");

   int Count = 0;

   while(in)
   {
      char ch[10240] = "";

      in.getline(ch, 10239, '\n');

      if(ch[0] == '\0')   continue;

      stringstream str(ch);

      TGraphErrors G;
      G.SetName(Form("MedianGraph%d", Count));

      while(str)
      {
         double Temp = -1;
         str >> Temp;

         if(Temp < 0)
            continue;

         int i = G.GetN();
         G.SetPoint(i, Xs[Count][i], Temp);
      }

      TSpline3 L("", &G);
      L.SetName(Form("Median%d", Count));

      L.Write();
      
      Count = Count + 1;
   }

   in.close();
}

void Set1DHistogram(TH1D *H)
{
   H->SetStats(0);
   H->SetLineColor(kBlack);
   H->SetLineWidth(2);
   H->GetXaxis()->SetNdivisions(305);
   H->SetTickLength(0, "y");
}

void Set2DHistogram(TH2D *H)
{
   H->SetStats(0);
   H->GetXaxis()->SetNdivisions(305);
   H->GetYaxis()->SetNdivisions(305);
   // H->SetTickLength(0, "y");
   // H->SetTickLength(0, "x");
}

double Formula1(double A, double B, double C, double D, double E, double T)
{
   double CR = 1.3333;
   double Zeta3 = 1.2020569031595942;
   double Lambda = 0.2;

   double Prefactor = 42 * CR * Zeta3 / M_PI * (4 * M_PI / 9) * (4 * M_PI / 9);
   double Part1 = A * (log(E / Lambda) - log(B)) / (log(E / Lambda) * log(E / Lambda));
   double Part2 = C * (log(E / T) - log(D)) / (log(E * T / Lambda / Lambda) * log(E * T / Lambda / Lambda));

   return Prefactor * (Part1 + Part2);
}

double Formula2(double A, double C, double D, double Q0, double E, double T, bool LBTOnly)
{
   double CR = 1.3333;
   double Zeta3 = 1.2020569031595942;
   double Lambda = 0.2;
   double Q = E;   // for now

   double Prefactor = 42 * CR * Zeta3 / M_PI * (4 * M_PI / 9) * (4 * M_PI / 9);
   double Part1 = A * (log(Q / Lambda) - log(Q0 / Lambda)) / (log(Q / Lambda) * log(Q / Lambda));
   double Part2 = C * (log(E / T) - log(D)) / (log(E * T / Lambda / Lambda) * log(E * T / Lambda / Lambda));

   if(LBTOnly)
      Part1 = 0;
   return Prefactor * (Part1 + Part2);
}


