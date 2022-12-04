#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/dsr-header.h"
#include "ns3/simulator.h"
#include <iostream>

using namespace ns3;

int main (int argc, char *argv[])
{
  Packet::EnablePrinting ();
  DsrHeader dsrHeader;
  dsrHeader.SetTxTime (Simulator::Now ());
  uint32_t budget = 100;
  dsrHeader.SetBudget (budget);
  bool flag = true;
  dsrHeader.SetFlag (flag);

  Ptr<Packet> p = Create<Packet> ();
  std::cout << "packet size1 = " << p->GetSize () << std::endl;
  p->AddHeader (dsrHeader);
  p->Print (std::cout);
  std::cout << std::endl;
  std::cout << "packet size2 = " << p->GetSize () << std::endl;

  DsrHeader peekHeader;
  p->PeekHeader(peekHeader);
  peekHeader.Print (std::cout);
}