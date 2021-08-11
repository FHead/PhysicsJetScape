#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

#include "TH1D.h"
#include "TCanvas.h"

#include "CommandLine.h"
#include "SetStyle.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();
   vector<int> Colors = GetPrimaryColors();

   TH1D H1("H1", "", 1000, -4, 4);
   TH1D H2("H2", "", 1000, -4, 4);

   for(int i = 1; i <= 1000; i++)
   {
      double x = H1.GetBinCenter(i);

      H1.SetBinContent(i, 1 / (x * x / 2 + 1) / M_PI / sqrt(2));
      H2.SetBinContent(i, exp(-x * x / 2) / (sqrt(2 * M_PI)));
   }

   H1.SetStats(0);
   H2.SetStats(0);

   H1.SetLineWidth(2);
   H1.SetLineColor(Colors[1]);
   H2.SetLineWidth(2);
   H2.SetLineColor(Colors[5]);

   TCanvas Canvas;

   H2.Draw();
   H1.Draw("same");

   H2.SetMinimum(0.001);

   Canvas.SetLogy();

   Canvas.SaveAs("Meow.pdf");

   return 0;
}



