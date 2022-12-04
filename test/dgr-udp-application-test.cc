/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/dgr-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevs;
  p2pDevs = p2p.Install (nodes);
  p2p.EnableAsciiAll("simple-udp");

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaces;
  ifaces = address.Assign (p2pDevs);
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
  Packet::EnablePrinting (); 

  // create a sink application
  uint16_t sinkPort = 9;
  Address sinkAddress (InetSocketAddress (ifaces.GetAddress (1), sinkPort));
  DGRSinkHelper dsrSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer sinkApps = dsrSinkHelper.Install (nodes.Get (1));
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (20.));

  // create a sender application
  Ptr<Socket> udpSocket = Socket::CreateSocket (nodes.Get (0), UdpSocketFactory::GetTypeId ());
  uint32_t budget = 30;
  Ptr<DGRUdpApplication> app = CreateObject<DGRUdpApplication> ();
  app->Setup (udpSocket, sinkAddress, 200, 5, DataRate ("1Mbps"), budget, false);
  // app->Setup (udpSocket, sinkAddress, 200, 5, DataRate ("1Mbps"), false);
  nodes.Get (0)->AddApplication (app);
  app->SetStartTime (Seconds (1.));
  app->SetStopTime (Seconds (20.));

  AnimationInterface anim("dsr-udp-application-test.xml");

  Ptr<FlowMonitor> flowmon;
  FlowMonitorHelper flowmonHelper;
  flowmon = flowmonHelper.InstallAll();
  flowmon->SerializeToXmlFile("dsr-application.xml", true, true);
  Simulator::Stop(Seconds(20));
  Simulator::Run();
  Simulator::Destroy ();

}