
/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include <ctime>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/topology-read-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/node-list.h"
#include "ns3/dgrv2-module.h"
#include <stdlib.h> 
#include "ns3/ipv4-global-routing-helper.h"

#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <iostream>

using namespace ns3;

std::string expName = "ecmp";
NS_LOG_COMPONENT_DEFINE(expName);

int
main(int argc, char* argv[])
{
    std::string topo ("abilene");
    std::string format ("Inet");
    uint32_t budget (20000); // in microsecond
    uint32_t sink = 10;
    uint32_t sender = 0;
    uint16_t udpPort = 9;
    uint32_t nPacket = 1000;
    uint32_t packetSize = 1400; // bytes

    uint32_t tcpSink = 5;
    uint32_t tcpSender = 2;
    uint16_t tcp_port = 80; // well-known echo port number
    
    // Set up command line parameters used to control the experiment
    CommandLine cmd(__FILE__);
    cmd.AddValue ("format", "Format to use for data input [Orbis|Inet|Rocketfuel].",
                format);
    cmd.AddValue ("topo", "topology", topo);
    cmd.AddValue ("budget", "budget", budget);
    cmd.AddValue ("sender", "Node # of sender", sender);
    cmd.AddValue ("sink", "Node # of sink", sink);

    cmd.AddValue ("tcpSender","Node # of tcpSender", tcpSender);
    cmd.AddValue ("tcpSink","Node # of tcpSender", tcpSink);
    
    cmd.Parse(argc, argv);

    // ------------- Read topology data-------------------
    std::string input ("contrib/dgrv2/topo/Inet_" + topo + "_topo.txt");
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

    // -------- Create nodes and network stacks ---------------
    NS_LOG_INFO ("creating internet stack");
    InternetStackHelper stack;
    stack.Install(nodes);

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
    // std::cout << "totlinks number: " << totlinks << std::endl;
    TopologyReader::ConstLinksIterator iter;
    int i = 0;
    for ( iter = inFile->LinksBegin (); iter != inFile->LinksEnd (); iter++, i++)
        {
        nc[i] = NodeContainer (iter->GetFromNode (), iter->GetToNode ());
        std::string delay = iter->GetAttribute ("Weight");
        std::stringstream ss;
        ss << delay;
        uint16_t metric;  //!< metric in milliseconds
        ss >> metric;
        p2p.SetChannelAttribute ("Delay", StringValue (delay + "ms"));
        p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
        ndc[i] = p2p.Install (nc[i]);
        tch.Install (ndc[i]);
        ipic[i] = address.Assign (ndc[i]);
        address.NewNetwork ();
        }
    // Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents", BooleanValue (true));
    // Config::SetDefault("ns3::Ipv4GlobalRouting::RandomEcmpRouting",     BooleanValue(true));
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // -------------------- UDP traffic -----------------
    Ptr<Node> udpSinkNode = nodes.Get (sink);
    Ptr<Ipv4> ipv4UdpSink = udpSinkNode->GetObject<Ipv4> ();
    Ipv4InterfaceAddress iaddrUdpSink = ipv4UdpSink->GetAddress (1,0);
    Ipv4Address ipv4AddrUdpSink = iaddrUdpSink.GetLocal ();

    DGRSinkHelper sinkHelper ("ns3::UdpSocketFactory",
                                InetSocketAddress (Ipv4Address::GetAny (), udpPort));
    ApplicationContainer sinkApp = sinkHelper.Install (nodes.Get (sink));
    sinkApp.Start (Seconds (0.0));
    sinkApp.Stop (Seconds (4.0));

    // udp sender
    Ptr<Socket> udpSocket = Socket::CreateSocket (nodes.Get (sender), UdpSocketFactory::GetTypeId ());
    Ptr<DGRUdpApplication> app = CreateObject<DGRUdpApplication> ();
    app->Setup (udpSocket, InetSocketAddress (ipv4AddrUdpSink, udpPort), packetSize, nPacket, DataRate ("10Mbps"), budget, true);
    nodes.Get (sender)->AddApplication (app);
    app->SetStartTime (Seconds (1.0));
    app->SetStopTime (Seconds (3.0));

    // ------------------------ TCP background traffic ------------------------
    Ptr<Node> tcpSinkNode = nodes.Get (tcpSink);
    Ptr<Ipv4> ipv4TcpSink = tcpSinkNode->GetObject<Ipv4> ();
    Ipv4InterfaceAddress iaddrTcpSink = ipv4TcpSink->GetAddress (1,0);
    Ipv4Address ipv4AddrTcpSink = iaddrTcpSink.GetLocal ();

    // Create a PacketSinkApplication and install it on node 5
    PacketSinkHelper tcpSinkHelper("ns3::TcpSocketFactory", 
                                InetSocketAddress(Ipv4Address::GetAny(), tcp_port));
    ApplicationContainer tcpSinkApps = tcpSinkHelper.Install(nodes.Get(tcpSink));
    tcpSinkApps.Start(Seconds(0.0));
    tcpSinkApps.Stop(Seconds(6.0));

    // Create a BulkSendApplication and install it on node 2
    BulkSendHelper source("ns3::TcpSocketFactory", 
                        InetSocketAddress(ipv4AddrTcpSink, tcp_port));
    // Set the amount of data to send in bytes.  Zero is unlimited.
    // uint64_t maxBytes = 4000000;
    // source.SetAttribute("MaxBytes", UintegerValue(maxBytes));
    ApplicationContainer sourceApps = source.Install(nodes.Get(tcpSender));
    sourceApps.Start(Seconds(0.0));
    sourceApps.Stop(Seconds(5.0));


    // // -------------------- UDP traffic -----------------
    // Ptr<Node> udpSinkNode1 = nodes.Get (tcpSink);
    // Ptr<Ipv4> ipv4UdpSink1 = udpSinkNode1->GetObject<Ipv4> ();
    // Ipv4InterfaceAddress iaddrUdpSink1 = ipv4UdpSink1->GetAddress (1,0);
    // Ipv4Address ipv4AddrUdpSink1 = iaddrUdpSink1.GetLocal ();

    // DGRSinkHelper sinkHelper1 ("ns3::UdpSocketFactory",
    //                             InetSocketAddress (Ipv4Address::GetAny (), udpPort));
    // ApplicationContainer sinkApp1 = sinkHelper1.Install (nodes.Get (tcpSink));
    // sinkApp1.Start (Seconds (0.0));
    // sinkApp1.Stop (Seconds (4.0));

    // // udp sender
    // Ptr<Socket> udpSocket1 = Socket::CreateSocket (nodes.Get (tcpSender), UdpSocketFactory::GetTypeId ());
    // Ptr<DGRUdpApplication> app1 = CreateObject<DGRUdpApplication> ();
    // uint32_t num = 100000;
    // app1->Setup (udpSocket1, InetSocketAddress (ipv4AddrUdpSink1, udpPort), packetSize, num, DataRate ("95Mbps"), budget, false);
    // nodes.Get (tcpSender)->AddApplication (app1);
    // app1->SetStartTime (Seconds (0.0));
    // app1->SetStopTime (Seconds (3.0));

    // // --------------- Net Anim ---------------------
    // AnimationInterface anim (topo + expName + ".xml");
    // std::ifstream topoNetanim (input);
    // std::istringstream buffer;
    // std::string line;
    // getline (topoNetanim, line);
    // for (uint32_t i = 0; i < nodes.GetN (); i ++)
    // {
    //     getline (topoNetanim, line);
    //     buffer.clear ();
    //     buffer.str (line);
    //     int no;
    //     double x, y;
    //     buffer >> no;
    //     buffer >> x;
    //     buffer >> y;
    //     anim.SetConstantPosition (nodes.Get (no), x*10, y*10);
    // }
    
    // -------- Run the simulation --------------------------
    NS_LOG_INFO ("Run Simulation.");
    Simulator::Run ();
    Simulator::Destroy ();

    delete[] ipic;
    delete[] ndc;
    delete[] nc;

    NS_LOG_INFO ("Done.");
    return 0;
}
