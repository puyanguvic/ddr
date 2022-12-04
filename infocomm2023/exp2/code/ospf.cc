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

std::string expName = "ospf";

NS_LOG_COMPONENT_DEFINE (expName);

int main (int argc, char *argv[])
{
  std::string topo ("abilene");
  std::string format ("Inet");
  std::string dataRate ("1Mbps");
  uint32_t sink = 10;
  uint32_t sender = 0;
  uint32_t tcpSink = 5;
  uint32_t tcpSender = 2;

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
  cmd.AddValue ("tcpSender", "Node# of tcpSender", tcpSender);
  cmd.AddValue ("tcpSink", "Node# of tcpSink", tcpSink);
  cmd.Parse (argc, argv);
  std::string input ("contrib/dgr/infocomm2023/topo/Inet_" + topo + "_topo.txt");
  uint32_t sink1 = tcpSink;
  uint32_t sender1 = tcpSender;

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
  Ipv4GlobalRoutingHelper global;
  Ipv4ListRoutingHelper list;
  list.Add (global, 10);
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
      address.NewNetwork ();
    }

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
    // -------------- UDP DGR traffic 0-->10 -------------
  uint16_t udpPort = 9;
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
  Ptr<DGRUdpApplication> app = CreateObject<DGRUdpApplication> ();
  app->Setup (ns3udpSocket, sinkAddress, 150, 10000, DataRate (dataRate), true);
  nodes.Get (sender)->AddApplication (app);
  app->SetStartTime (Seconds (0.));
  app->SetStopTime (Seconds (20.));

  // // -------------- UDP DGR traffic 2-->5 -------------
  Ptr<Node> sinkNode1 = nodes.Get (sink1);
  Ptr<Ipv4> ipv4Sink1 = sinkNode1->GetObject<Ipv4> ();
  Ipv4InterfaceAddress iaddrSink1 = ipv4Sink1->GetAddress (1,0);
  Ipv4Address ipv4AddrSink1 = iaddrSink1.GetLocal ();

  Address sinkAddress1 (InetSocketAddress (ipv4AddrSink1, udpPort));
  DGRSinkHelper dsrSinkHelper1 ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), udpPort));
  ApplicationContainer sinkApps1 = dsrSinkHelper1.Install (nodes.Get (sink1));
  sinkApps1.Start (Seconds (0.));
  sinkApps1.Stop (Seconds (20.));

  // // sender
  Ptr<Socket> ns3udpSocket1 = Socket::CreateSocket (nodes.Get (sender1), UdpSocketFactory::GetTypeId ());
  Ptr<DGRUdpApplication> app1 = CreateObject<DGRUdpApplication> ();
  // uint32_t budget1 = 8;
  app1->Setup (ns3udpSocket1, sinkAddress1, 150, 1000000, DataRate ("6Mbps"), false);
  nodes.Get (sender1)->AddApplication (app1);
  app1->SetStartTime (Seconds (0.));
  app1->SetStopTime (Seconds (20.));

  // // -------------- TCP Back ground traffic 2-->5 ------------------

  // uint16_t tcpPort = 8080;
  // Ptr<Node> tcpSinkNode = nodes.Get (tcpSink);
  // Ptr<Ipv4> ipv4TcpSink = tcpSinkNode->GetObject<Ipv4> ();
  // Ipv4InterfaceAddress iaddrTcpSink = ipv4TcpSink->GetAddress (1,0);
  // Ipv4Address ipv4AddrTcpSink = iaddrTcpSink.GetLocal ();

  // DGRSinkHelper sinkHelper ("ns3::TcpSocketFactory",
  //                        InetSocketAddress (Ipv4Address::GetAny (), tcpPort));
  // ApplicationContainer sinkApp = sinkHelper.Install (nodes.Get (tcpSink));
  // sinkApp.Start (Seconds (0.0));
  // sinkApp.Stop (Seconds (20.0));
  
  // // tcp send
  // DGRTcpAppHelper sourceHelper ("ns3::TcpSocketFactory",
  //                              InetSocketAddress (ipv4AddrTcpSink, tcpPort));
  // sourceHelper.SetAttribute ("MaxBytes", UintegerValue (7500000));
  // // sourceHelper.SetAttribute ("Budget", UintegerValue (1000));
  // ApplicationContainer sourceApp = sourceHelper.Install (nodes.Get (tcpSender));
  // sourceApp.Start (Seconds (0.0));
  // sourceApp.Stop (Seconds (20.0));
 
 
  // NodeList::Iterator listEnd = NodeList::End ();
  // for (NodeList::Iterator i = NodeList::Begin (); i != listEnd; i++)
  //   {
  //     Ptr<Node> node = *i;
  //     std::cout << "Node: " << node->GetId ();
  //     uint32_t totdev = node->GetNDevices ();
  //     std::cout << ", Has " << totdev << " neighbours," << std::endl;
  //     Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  //     for (uint32_t j = 1; j < totdev; j ++)
  //       {
  //         ipv4->GetAddress (j, 0).GetLocal ().Print (std::cout);
  //         std::cout << " ";
  //         Ptr<NetDevice> dev = node->GetDevice (j);
  //         Ptr<Channel> channel = dev->GetChannel ();
  //         PointToPointChannel *p2pchannel = dynamic_cast <PointToPointChannel *> (PeekPointer (channel));
  //         Ptr<PointToPointNetDevice> d_dev = p2pchannel->GetDestination (0);
  //         Ptr<Node> neighbour_node = d_dev->GetNode ();
  //         Ptr<Ipv4> neighbour_ipv4 = neighbour_node->GetObject<Ipv4> ();
  //         neighbour_ipv4->GetAddress (neighbour_ipv4->GetInterfaceForDevice (d_dev), 0).GetLocal ().Print (std::cout);
  //         std::cout << std::endl;
  //         // std::cout << "neighbour " << j << "Node: " << neighbour_node->GetId () << std::endl; 
  //       }
  //   }

  // // ------------------------------------------------------------
  // // -- Print routing table
  // // ---------------------------------------------
  // Ipv4GlobalRoutingHelper g;
  // Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>
  // (topo + expName + ".routes", std::ios::out);
  // g.PrintRoutingTableAllAt (Seconds (0), routingStream);

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
