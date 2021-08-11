#include <iostream>
using namespace std;

#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"

#include "CommandLine.h"

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string InputFileName = CL.Get("Input");
   string OutputFileName = CL.Get("Output");
   string Tag = CL.Get("Tag");

   int BeginH = CL.GetInt("BeginH", 15);
   int BeginM = CL.GetInt("BeginM", 0);
   int BeginS = CL.GetInt("BeginS", 0);
   int TotalSeconds = CL.GetInt("TotalSeconds", 4.0 * 3600);

   TFile InputFile(InputFileName.c_str());
   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TTree *Tree = (TTree *)InputFile.Get("Tree");

   double StartH, StartM, StartS;
   double EndH, EndM, EndS;
   Tree->SetBranchAddress("StartH", &StartH);
   Tree->SetBranchAddress("StartM", &StartM);
   Tree->SetBranchAddress("StartS", &StartS);
   Tree->SetBranchAddress("EndH", &EndH);
   Tree->SetBranchAddress("EndM", &EndM);
   Tree->SetBranchAddress("EndS", &EndS);

   TH1D H(Form("H%s", Tag.c_str()), ";Time since start (h);", TotalSeconds, 0, (double)TotalSeconds / 3600);

   int EntryCount = Tree->GetEntries();
   for(int iE = 0; iE < EntryCount; iE++)
   {
      Tree->GetEntry(iE);

      double Start = (StartH - BeginH) + (StartM - BeginM) / 60 + (StartS - BeginS) / 3600;
      double End = (EndH - BeginH) + (EndM - BeginM) / 60 + (EndS - BeginS) / 3600;

      for(int i = 1; i <= TotalSeconds; i++)
      {
         double X = H.GetBinCenter(i);

         if(X < Start || X > End)
            continue;

         H.Fill(X);
      }
   }

   OutputFile.cd();
   H.Write();

   OutputFile.Close();
   InputFile.Close();

   return 0;
}



