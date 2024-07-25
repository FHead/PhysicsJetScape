#include <iostream>
using namespace std;

#include "CommandLine.h"

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string BaseDirectory = CL.Get("Base");
   vector<string> Tags = CL.GetStringVector("Tag");

   return 0;
}



