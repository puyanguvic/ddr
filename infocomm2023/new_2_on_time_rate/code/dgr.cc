/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include <ctime>
#include <sstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/dgr-module.h"
#include "ns3/topology-read-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/node-list.h"
#include <list>
#include <fstream>
#include <sstream>
#include <string>

using namespace ns3;

std::string expName = "dgr";

NS_LOG_COMPONENT_DEFINE (expName);

int main (int argc, char *argv[])
{
  std::string topo ("abilene");
  std::string format ("Inet");
  std::string dataRate ("1Mbps");
  uint32_t sink = 10;
  uint32_t sender = 0;
  uint16_t udpPort = 9;

  // Set up command line parameters used to control the experiment.
  CommandLine cmd (__FILE__);
  cmd.AddValue ("format", "Format to use for data input [Orbis|Inet|Rocketfuel].",
                format);
  cmd.AddValue ("dataRate", "Sending rate of target appplication.",
                dataRate);
  cmd.AddValue ("topo", "topology", topo);
  cmd.AddValue ("sender", "Node# of sender",
                sender);
  cmd.AddValue ("sink", "Node# of sink",
                sink);
  cmd.Parse (argc, argv);
  
  std::string input ("contrib/dgr/infocomm2023/topo/Inet_" + topo + "_topo.txt");

  std::cout << input<< std::endl;

  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------
  
  // Pick a topology reader based in the requested format.
  TopologyReaderHelper topoHelp;
  topoHelp.SetFileName (input);
  topoHelp.SetFileType (format);
  Ptr<TopologyReader> inFile = topoHelp.GetTopologyReader ();

  NodeContainer nodes;

  if (inFile != 0)
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
      // std::cout << "From: " << iter->GetFromNode ()->GetId () << " To: " << iter->GetToNode ()->GetId ()  << "Attr: " << iter->GetAttribute ("Weight") << std::endl;
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
      ipic[i].SetMetric (0, metric * 10 + 1); // microseconds
      ipic[i].SetMetric (1, metric * 10 + 1); // microseconds
      address.NewNetwork ();
    }

  Ipv4DGRRoutingHelper::PopulateRoutingTables ();
  
  // -------------- UDP DGR traffic 0-->10 -------------
  Ptr<Node> sinkNode = nodes.Get (sink);
  Ptr<Ipv4> ipv4Sink = sinkNode->GetObject<Ipv4> ();
  Ipv4InterfaceAddress iaddrSink = ipv4Sink->GetAddress (1,0);
  Ipv4Address ipv4AddrSink = iaddrSink.GetLocal ();

  Address sinkAddress (InetSocketAddress (ipv4AddrSink, udpPort));
  DGRSinkHelper dsrSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), udpPort));
  ApplicationContainer sinkApps = dsrSinkHelper.Install (nodes.Get (sink));
  sinkApps.Start (Seconds (0.));
  sinkApps.Stop (Seconds (20.));

  // sender
  Ptr<Socket> ns3udpSocket = Socket::CreateSocket (nodes.Get (sender), UdpSocketFactory::GetTypeId ());
  uint32_t budget = 50000; // microseconds
  Ptr<DGRUdpApplication> app = CreateObject<DGRUdpApplication> ();
  app->Setup (ns3udpSocket, sinkAddress, 150, 10000, DataRate (dataRate), budget, true);
  nodes.Get (sender)->AddApplication (app);
  app->SetStartTime (Seconds (0.));
  app->SetStopTime (Seconds (20.));

  // // sender 2
  // Ptr<Socket> ns3udpSocket2 = Socket::CreateSocket (nodes.Get (sender), UdpSocketFactory::GetTypeId ());
  // uint32_t budget2 = 40000; // microseconds
  // Ptr<DGRUdpApplication> app2 = CreateObject<DGRUdpApplication> ();
  // app2->Setup (ns3udpSocket2, sinkAddress, 150, 10000, DataRate (dataRate), budget2, true);
  // nodes.Get (sender)->AddApplication (app2);
  // app2->SetStartTime (Seconds (0.));
  // app2->SetStopTime (Seconds (20.));

  // // ------------------------------------------------------------
  // // -- Print routing table
  // // ---------------------------------------------
  // Ipv4DGRRoutingHelper d;
  // Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>
  // (topo + expName + ".routes", std::ios::out);
  // d.PrintRoutingTableAllAt (Seconds (0), routingStream);

  // // ------------------------------------------------------------
  // // -- Net anim
  // // ---------------------------------------------
  // AnimationInterface anim (topo + expName + ".xml");
  // std::ifstream topoNetAnim (input);
  // std::istringstream buffer;
  // std::string line;
  // getline (topoNetAnim, line);
  // for (uint32_t i = 0; i < nodes.GetN (); i ++)
  // {
  //   getline (topoNetAnim, line);
  //   buffer.clear ();
  //   buffer.str (line);
  //   int no;
  //   double x, y;
  //   buffer >> no;
  //   buffer >> x;
  //   buffer >> y;
  //   anim.SetConstantPosition (nodes.Get (no), x*10, y*10);
  // }

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
