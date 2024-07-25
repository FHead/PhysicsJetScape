#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

double RunningAlphaS(double MuSquare, double AlphaS);
double QHat(double AlphaSFix, double T = 0, double E = 0, double Q = 0);
std::vector<double> ReadAlphaS(std::string FileName);

double RunningAlphaS(double MuSquare, double AlphaS)
{
   if(MuSquare < 1.0)
      return AlphaS;
   
   const int ActiveFlavor = 3;
   double SquareLambdaQCDHTL = std::exp(-12 * M_PI / ((33 - 2 * ActiveFlavor) * AlphaS));
   double Answer = 12 * M_PI / ((33 - 2 * ActiveFlavor) * std::log(MuSquare / SquareLambdaQCDHTL));
   return Answer;
}

double QHat(double AlphaSFix, double T, double E, double Q)
{
   const int ActiveFlavor = 3;
   // double CA = 3.0;
   double CA = 4.0 / 3.0;
   double DebyeMassSquare = AlphaSFix * 4 * M_PI * std::pow(T, 2.0) * (6.0 + ActiveFlavor) / 6;
   double ScaleNet = max(2 * E * T, 1.0);

   // The answer here is qhat/T^3 in fact
   // double Answer = (CA * 50.4864 / M_PI) * RunningAlphaS(ScaleNet, ScaleNet) * AlphaSFix * std::fabs(std::log(ScaleNet / DebyeMassSquare));
   double Answer = (CA * 50.4864 / M_PI) * RunningAlphaS(ScaleNet, AlphaSFix) * AlphaSFix * std::fabs(std::log(ScaleNet / DebyeMassSquare));
   
   // return Answer * 0.19732698;   // 1/GeV to fm
   return Answer;
}

std::vector<double> ReadAlphaS(std::string FileName)
{
   std::vector<double> Result;

   std::ifstream in(FileName);
   while(in)
   {
      double Temp = -1;
      char ch[10240];
      ch[0] = '\0';
      in.getline(ch, 10239, '\n');
      std::stringstream str(ch);
      str >> Temp;
      if(Temp >= 0)
         Result.push_back(Temp);
   }
   in.close();

   return Result;
}




