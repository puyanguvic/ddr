/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

void GenerateGridTopo (int size);
int main (int argc, char *argv[])
{
  for (int i = 2; i <= 10; i ++)
  {
    GenerateGridTopo (i);
  }
  return 0;
}

void GenerateGridTopo (int size)
{
    std::stringstream ss;
    std::string size_s;
    ss << size;
    ss >> size_s;
    std::ofstream outfile;
    outfile.open ("contrib/dgr/infocomm2023/topo/Inet_" + size_s + "by" + size_s + "_topo.txt");
    int node = size * size;
    int link = 2 * (size - 1) * size;
    int metric = 1;
    outfile << node << " " << link << std::endl;
    for (int i = 0; i < node; i ++)
    {
        outfile  << i << " " << i % size << " " << i/size << std::endl;
    }
    for (int i = 0; i < node; i ++)
    {
        if ((i+1)%size == 0) continue;
        outfile << i << " " << i+1 << " " << metric << std::endl;
    }
    for (int i = 0; i < size * (size - 1); i ++)
    {
        outfile << i << " " << i + size << " " << metric << std::endl;
    }
}
