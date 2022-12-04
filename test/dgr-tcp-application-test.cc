/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/dsr-module.h"
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

  // create send application
  uint16_t port = 8080;
  DGRTcpAppHelper sourceHelper ("ns3::TcpSocketFactory",
                               InetSocketAddress (ifaces.GetAddress (1), port));
  sourceHelper.SetAttribute ("MaxBytes", UintegerValue (3000));
  sourceHelper.SetAttribute ("Budget", UintegerValue (30));
  ApplicationContainer sourceApp = sourceHelper.Install (nodes.Get (0));
  sourceApp.Start (Seconds (1.0));
  sourceApp.Stop (Seconds (10.0));

  // create sink application
  DGRSinkHelper sinkHelper ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApp = sinkHelper.Install (nodes.Get (1));
  sinkApp.Start (Seconds (0.0));
  sinkApp.Stop (Seconds (10.0));


  AnimationInterface anim("dsr-tcp-application-test.xml");

  Ptr<FlowMonitor> flowmon;
  FlowMonitorHelper flowmonHelper;
  flowmon = flowmonHelper.InstallAll();
  flowmon->SerializeToXmlFile("dsr-application.xml", true, true);
  Simulator::Stop(Seconds(20));
  Simulator::Run();
  Simulator::Destroy ();

}