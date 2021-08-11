#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLatex.h"

#include "SetStyle.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();
   vector<int> Colors = GetPrimaryColors();

   TFile File("Curves.root");

   vector<string> Tags{"Jul19", "Jul20", "Jul21", "Jul22", "Jul23", "Jul26", "Jul27", "Jul28", "Jul29", "Jul30"};

   vector<TH1D *> H(Tags.size());
   for(int i = 0; i < (int)Tags.size(); i++)
      H[i] = (TH1D *)File.Get(Form("H%s", Tags[i].c_str()));

   TCanvas Canvas("Canvas", "", 2048, 1024);
   Canvas.SetLeftMargin(0.06);
   Canvas.SetRightMargin(0.05);
   Canvas.SetTopMargin(0.10);
   Canvas.SetBottomMargin(0.10);

   TH2D HWorld("HWorld", ";Time since 9AM [h];Participant count [head]", 100, 0, 4.0, 80, 0, 80);
   HWorld.SetStats(0);
   HWorld.GetXaxis()->SetNdivisions(510);
   HWorld.GetYaxis()->SetNdivisions(505);
   HWorld.GetXaxis()->SetTickLength(0.03);
   HWorld.GetYaxis()->SetTickLength(0.015);

   HWorld.GetXaxis()->SetLabelSize(0.05);
   HWorld.GetXaxis()->SetTitleSize(0.05);
   HWorld.GetXaxis()->SetTitleOffset(0.9);
   HWorld.GetXaxis()->CenterTitle();
   HWorld.GetYaxis()->SetLabelSize(0.05);
   HWorld.GetYaxis()->SetTitleSize(0.05);
   HWorld.GetYaxis()->SetTitleOffset(0.55);
   HWorld.GetYaxis()->CenterTitle();

   HWorld.Draw();

   for(int i = 0; i < (int)Tags.size(); i++)
   {
      if(H[i] != nullptr)
      {
         H[i]->SetLineColor(Colors[i%5]);
         H[i]->SetLineWidth(2);
         if(i >= 5)
            H[i]->SetLineWidth(1);
         H[i]->Draw("same");
      }
   }

   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.05);
   Latex.SetNDC();
   
   Latex.SetTextAlign(12);
   Latex.DrawLatex(0.06, 0.93, Form("#bf{#color[%d]{Mon} #color[%d]{Tue} #color[%d]{Wed} #color[%d]{Thu} #color[%d]{Fri}}", Colors[0], Colors[1], Colors[2], Colors[3], Colors[4]));
   Latex.SetTextAlign(32);
   Latex.DrawLatex(0.95, 0.93, "Bold = week 1, Thin = week 2");

   Canvas.SaveAs("Participant.pdf");
}



