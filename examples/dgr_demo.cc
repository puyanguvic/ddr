/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include <ctime>
#include <sstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/dgrv2-module.h"
#include "ns3/topology-read-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/node-list.h"
#include <stdlib.h> 

#include <list>
#include <fstream>
#include <sstream>
#include <string>

using namespace ns3;

std::string expName = "dgrv2_demo";

NS_LOG_COMPONENT_DEFINE (expName);

int main (int argc, char *argv[])
{
  std::string topo ("2by2");
  std::string format ("Inet");

  // Set up command line parameters used to control the experiment.
  CommandLine cmd (__FILE__);
  cmd.AddValue ("format", "Format to use for data input [Orbis|Inet|Rocketfuel].",
                format);
  cmd.AddValue ("topo", "topology", topo);
  cmd.Parse (argc, argv);
  std::string input ("contrib/dgrv2/infocomm2023/topo/Inet_" + topo + "_topo.txt");
  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------
  
  // Pick a topology reader based in the requested format.
  TopologyReaderHelper topoHelp;
  topoHelp.SetFileName (input);
  topoHelp.SetFileType (format);
  Ptr<TopologyReader> inFile = topoHelp.GetTopologyReader ();

  NodeContainer nodes;

  if (inFile)
    {
      nodes = inFile->Read ();
    }

  if (inFile->LinksSize () == 0)
    {
      NS_LOG_ERROR ("Problems reading the topology file. Failing.");
      return -1;
    }

  // ------------------------------------------------------------
  // -- Create nodes and network stacks
  // --------------------------------------------
  NS_LOG_INFO ("creating internet stack");
  InternetStackHelper stack;

  // Setup Routing algorithm
  Ipv4DGRRoutingHelper dgr;
  Ipv4ListRoutingHelper list;
  list.Add (dgr, 10);
  InternetStackHelper internet;
  internet.SetRoutingHelper (list);
  internet.Install (nodes);

  NS_LOG_INFO ("creating ipv4 addresses");
  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.255.255.252");

  int totlinks = inFile->LinksSize ();

  NS_LOG_INFO ("creating node containers");
  NodeContainer* nc = new NodeContainer[totlinks];
  NetDeviceContainer* ndc = new NetDeviceContainer[totlinks];
  PointToPointHelper p2p;
  TrafficControlHelper tch;
  tch.SetRootQueueDisc ("ns3::DGRVirtualQueueDisc");
  NS_LOG_INFO ("creating ipv4 interfaces");
  Ipv4InterfaceContainer* ipic = new Ipv4InterfaceContainer[totlinks];
  std::cout << "totlinks number: " << totlinks << std::endl;
  TopologyReader::ConstLinksIterator iter;
  int i = 0;
  for ( iter = inFile->LinksBegin (); iter != inFile->LinksEnd (); iter++, i++)
    {
      nc[i] = NodeContainer (iter->GetFromNode (), iter->GetToNode ());
      std::string delay = iter->GetAttribute ("Weight");
      std::stringstream ss;
      ss << delay;
      uint16_t metric;
      ss >> metric;
      p2p.SetChannelAttribute ("Delay", StringValue (delay + "ms"));
      p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
      ndc[i] = p2p.Install (nc[i]);
      tch.Install (ndc[i]);
      ipic[i] = address.Assign (ndc[i]);
      ipic[i].SetMetric (0, metric + 1);
      ipic[i].SetMetric (1, metric + 1);
      address.NewNetwork ();
    }

  Ipv4DGRRoutingHelper::PopulateRoutingTables ();

  // ------------------------------------------------------------
  // -- Print routing table
  // ---------------------------------------------
  Ipv4DGRRoutingHelper d;
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>
  (topo + expName + ".routes", std::ios::out);
  d.PrintRoutingTableAllAt (Seconds (0), routingStream);

  // // -------------- Udp traffic 0-->2 ------------------
  // uint16_t udpPort = 9;
  // uint32_t udpSink = 3;
  // uint32_t udpSender = 0;
  // Ptr<Node> udpSinkNode = nodes.Get (udpSink);
  // Ptr<Ipv4> ipv4UdpSink = udpSinkNode->GetObject<Ipv4> ();
  // Ipv4InterfaceAddress iaddrUdpSink = ipv4UdpSink->GetAddress (1,0);
  // Ipv4Address ipv4AddrUdpSink = iaddrUdpSink.GetLocal ();

  // DGRSinkHelper sinkHelper ("ns3::UdpSocketFactory",
  //                        InetSocketAddress (Ipv4Address::GetAny (), udpPort));
  // ApplicationContainer sinkApp = sinkHelper.Install (nodes.Get (udpSink));
  // sinkApp.Start (Seconds (0.0));
  // sinkApp.Stop (Seconds (20.0));
  
  // // udp sender
  // Ptr<Socket> udpSocket = Socket::CreateSocket (nodes.Get (udpSender), UdpSocketFactory::GetTypeId ());
  // Ptr<DGRUdpApplication> app = CreateObject<DGRUdpApplication> ();
  // app->Setup (udpSocket, InetSocketAddress (ipv4AddrUdpSink, udpPort), 150, 10, DataRate ("1Mbps"), 1000, false);
  // nodes.Get (udpSender)->AddApplication (app);
  // app->SetStartTime (Seconds (1.0));
  // app->SetStopTime (Seconds (20.0));
  
  // -------------- TCP Back ground traffic 0-->2 ------------------
  uint16_t tcpPort = 8080;
  uint32_t tcpSink = 3;
  uint32_t tcpSender = 0;
  Ptr<Node> tcpSinkNode = nodes.Get (tcpSink);
  Ptr<Ipv4> ipv4TcpSink = tcpSinkNode->GetObject<Ipv4> ();
  Ipv4InterfaceAddress iaddrTcpSink = ipv4TcpSink->GetAddress (1,0);
  Ipv4Address ipv4AddrTcpSink = iaddrTcpSink.GetLocal ();

  DGRSinkHelper sinkHelper ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), tcpPort));
  ApplicationContainer sinkApp = sinkHelper.Install (nodes.Get (tcpSink));
  sinkApp.Start (Seconds (0.0));
  sinkApp.Stop (Seconds (20.0));
  
  // tcp send
  DGRTcpAppHelper sourceHelper ("ns3::TcpSocketFactory",
                               InetSocketAddress (ipv4AddrTcpSink, tcpPort));
  sourceHelper.SetAttribute ("MaxBytes", UintegerValue (7500000));
  sourceHelper.SetAttribute ("Budget", UintegerValue (1000));
  ApplicationContainer sourceApp = sourceHelper.Install (nodes.Get (tcpSender));
  sourceApp.Start (Seconds (1.0));
  sourceApp.Stop (Seconds (20.0));

  // ------------------------------------------------------------
  // -- Net anim
  // ---------------------------------------------
  AnimationInterface anim (topo + expName + ".xml");
  std::ifstream topoNetanim (input);
  std::istringstream buffer;
  std::string line;
  getline (topoNetanim, line);
  for (uint32_t i = 0; i < nodes.GetN (); i ++)
  {
    getline (topoNetanim, line);
    buffer.clear ();
    buffer.str (line);
    int no;
    double x, y;
    buffer >> no;
    buffer >> x;
    buffer >> y;
    anim.SetConstantPosition (nodes.Get (no), x*10, y*10);
  }

  // ------------------------------------------------------------
  // -- Run the simulation
  // --------------------------------------------
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();

  delete[] ipic;
  delete[] ndc;
  delete[] nc;

  NS_LOG_INFO ("Done.");

  return 0;
}
