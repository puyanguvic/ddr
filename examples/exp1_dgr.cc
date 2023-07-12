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
#include <list>
#include <fstream>
#include <sstream>
#include <string>

using namespace ns3;

std::string ExpName = "dgrv2";

NS_LOG_COMPONENT_DEFINE (ExpName);

int main (int argc, char *argv[])
{
  std::string topo ("geant");
  std::string format ("Inet");
  std::string dataRate ("10Mbps");
  uint32_t sink = 14;
  uint32_t sender = 16;
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
  
  std::string input ("contrib/dgrv2/infocomm2024/topo/Inet_" + topo + "_topo.txt");

  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------
  
  // Pick a topology reader based in the requested format.
  TopologyReaderHelper topoHelp;
  topoHelp.SetFileName (input);
  topoHelp.SetFileType (format);
  Ptr<TopologyReader> inFile = topoHelp.GetTopologyReader ();

  NodeContainer nodes;

  if (inFile != nullptr)
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
  tch.SetRootQueueDisc ("ns3::DGRv2QueueDisc");
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
      address.NewNetwork ();
    }

  Ipv4DGRRoutingHelper::PopulateRoutingTables ();


  // -------------- Udp traffic ------------------
  Ptr<Node> udpSinkNode = nodes.Get (sink);
  Ptr<Ipv4> ipv4UdpSink = udpSinkNode->GetObject<Ipv4> ();
  Ipv4InterfaceAddress iaddrUdpSink = ipv4UdpSink->GetAddress (1,0);
  Ipv4Address ipv4AddrUdpSink = iaddrUdpSink.GetLocal ();

  DGRSinkHelper sinkHelper ("ns3::UdpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), udpPort));
  ApplicationContainer sinkApp = sinkHelper.Install (nodes.Get (sink));
  sinkApp.Start (Seconds (0.0));
  sinkApp.Stop (Seconds (11.0));
  
  // udp sender
  Ptr<Socket> udpSocket = Socket::CreateSocket (nodes.Get (sender), UdpSocketFactory::GetTypeId ());
  Ptr<DGRUdpApplication> app = CreateObject<DGRUdpApplication> ();
  uint32_t budget = 100;
  app->Setup (udpSocket, InetSocketAddress (ipv4AddrUdpSink, udpPort), 150, 7400, DataRate (dataRate), budget, true);
  nodes.Get (sender)->AddApplication (app);
  app->SetStartTime (Seconds (1.0));
  app->SetStopTime (Seconds (10.0));


  // NodeList::Iterator listEnd = NodeList::End ();
  // for (NodeList::Iterator i = NodeList::Begin (); i != listEnd; i++)
  //   {
  //     Ptr<Node> node = *i;
  //     std::cout << "Node: " << node->GetId ();
  //     uint32_t totdev = node->GetNDevices ();
  //     std::cout << ", Has " << totdev << " neighbours," << std::endl;
  //     Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  //     // for (uint32_t j = 1; j < totdev; j ++)
  //     //   {
  //     //     ipv4->GetAddress (j, 0).GetLocal ().Print (std::cout);
  //     //     std::cout << " ";
  //     //     Ptr<NetDevice> dev = node->GetDevice (j);
  //     //     Ptr<Channel> channel = dev->GetChannel ();
  //     //     PointToPointChannel *p2pchannel = dynamic_cast <PointToPointChannel *> (PeekPointer (channel));
  //     //     Ptr<PointToPointNetDevice> d_dev = p2pchannel->GetDestination (0);
  //     //     Ptr<Node> neighbour_node = d_dev->GetNode ();
  //     //     Ptr<Ipv4> neighbour_ipv4 = neighbour_node->GetObject<Ipv4> ();
  //     //     neighbour_ipv4->GetAddress (neighbour_ipv4->GetInterfaceForDevice (d_dev), 0).GetLocal ().Print (std::cout);
  //     //     std::cout << std::endl;
  //     //     // std::cout << "neighbour " << j << "Node: " << neighbour_node->GetId () << std::endl; 
  //     //   }
  //   }

  // ------------------------------------------------------------
  // -- Print routing table
  // ---------------------------------------------
  Ipv4DGRRoutingHelper d;
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>
  (topo + ExpName + ".routes", std::ios::out);
  d.PrintRoutingTableAllAt (Seconds (0), routingStream);

  // // ------------------------------------------------------------
  // // -- Net anim
  // // ---------------------------------------------
  // AnimationInterface anim (ExpName + ".xml");
  // std::ifstream topo (input);
  // std::istringstream buffer;
  // std::string line;
  // getline (topo, line);
  // for (uint32_t i = 0; i < nodes.GetN (); i ++)
  // {
  //   getline (topo, line);
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
