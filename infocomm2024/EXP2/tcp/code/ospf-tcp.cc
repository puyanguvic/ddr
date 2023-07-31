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

std::string expName = "OSPF";
NS_LOG_COMPONENT_DEFINE(expName);

std::string dir;
uint32_t prev = 0;
Time prevTime = Seconds(0);

// Calculate throughput
static void
TraceThroughput(Ptr<FlowMonitor> monitor)
{
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();
    auto itr = stats.begin();
    Time curTime = Now();
    std::ofstream thr(dir + "/throughput.txt", std::ios::out | std::ios::app);
    thr << curTime.GetMilliSeconds () << " "
        << 8 * (itr->second.rxBytes - prev) /
               (1000 * 1000 * (curTime.GetSeconds() - prevTime.GetSeconds()))
        << std::endl;
    prevTime = curTime;
    prev = itr->second.rxBytes;
    Simulator::Schedule(Seconds(0.2), &TraceThroughput, monitor);
}

// Check the queue size
void
CheckQueueSize(Ptr<QueueDisc> qd)
{
    uint32_t qsize = qd->GetCurrentSize().GetValue();
    Simulator::Schedule(Seconds(0.2), &CheckQueueSize, qd);
    std::ofstream q(dir + "/queueSize.txt", std::ios::out | std::ios::app);
    q << Simulator::Now().GetSeconds() << " " << qsize << std::endl;
    q.close();
}

// Trace congestion window
static void
CwndTracer(Ptr<OutputStreamWrapper> stream, uint32_t oldval, uint32_t newval)
{
    *stream->GetStream() << Simulator::Now().GetSeconds() << " " << newval / 1448.0 << std::endl;
}

void
TraceCwnd(uint32_t nodeId, uint32_t socketId)
{
    AsciiTraceHelper ascii;
    Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream(dir + "cwnd.txt");
    Config::ConnectWithoutContext("/NodeList/" + std::to_string(nodeId) +
                                      "/$ns3::TcpL4Protocol/SocketList/" +
                                      std::to_string(socketId) + "/CongestionWindow",
                                  MakeBoundCallback(&CwndTracer, stream));
}


int
main(int argc, char* argv[])
{
  std::string topo ("abilene");
  std::string format ("Inet");

  uint32_t budget (30000);

  uint32_t routeSelectMode = 1;
  
  uint32_t tcpSink = 10;
  uint32_t tcpSender = 0;
  uint16_t tcp_port = 80; // well-known echo port number


  // Naming the output directory using local system time
  time_t rawtime;
  struct tm* timeinfo;
  char buffer[80];
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, sizeof(buffer), "%d-%m-%Y-%I-%M-%S", timeinfo);
  std::string currentTime(buffer);

  std::string transport_prot = "TcpBbr";
  // std::string queueDisc = "FifoQueueDisc";
  uint32_t delAckCount = 2;
  // bool bql = true;
  bool enablePcap = false;
  Time stopTime = Seconds(100);

  // Set up command line parameters used to control the experiment
  CommandLine cmd(__FILE__);
  cmd.AddValue ("format", "Format to use for data input [Orbis|Inet|Rocketfuel].",
                format);
  cmd.AddValue ("topo", "topology", topo);
 
  cmd.AddValue ("EcmpMode", "EcmpMode: (0 none, 1 KShort, 2 DGR, 3 DDR)", routeSelectMode);

  cmd.AddValue ("tcpSender","Node # of tcpSender", tcpSender);
  cmd.AddValue ("tcpSink","Node # of tcpSender", tcpSink);
  
  // cmd.AddValue("tcpTypeId", "Transport protocol to use: TcpNewReno, TcpBbr", tcpTypeId);
  cmd.AddValue("delAckCount", "Delayed ACK count", delAckCount);
  cmd.AddValue("enablePcap", "Enable/Disable pcap file generation", enablePcap);
  cmd.AddValue("stopTime",
                "Stop time for applications / simulation time will be stopTime + 1",
                stopTime);
  cmd.AddValue("transport_prot",
              "Transport protocol to use: TcpNewReno, TcpLinuxReno, "
              "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, "
              "TcpBic, TcpYeah, TcpIllinois, TcpWestwoodPlus, TcpLedbat, "
              "TcpLp, TcpDctcp, TcpCubic, TcpBbr",
              transport_prot);
  cmd.Parse(argc, argv);

  transport_prot = std::string("ns3::") + transport_prot;
  // queueDisc = std::string("ns3::") + queueDisc;

  Config::SetDefault("ns3::TcpL4Protocol::SocketType",
                    TypeIdValue(TypeId::LookupByName(transport_prot)));
  Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(4194304));
  Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(6291456));
  Config::SetDefault("ns3::TcpSocket::InitialCwnd", UintegerValue(10));
  Config::SetDefault("ns3::TcpSocket::DelAckCount", UintegerValue(delAckCount));
  Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(1448));
  Config::SetDefault("ns3::DropTailQueue<Packet>::MaxSize", QueueSizeValue(QueueSize("1p")));
  // Config::SetDefault(queueDisc + "::MaxSize", QueueSizeValue(QueueSize("100p")));


  switch (routeSelectMode)
    {
      case 0:
        break;
      case 1:
        Config::SetDefault ("ns3::Ipv4DGRRouting::RouteSelectMode", StringValue ("KSHORT"));
        break;
      case 2:
        Config::SetDefault ("ns3::Ipv4DGRRouting::RouteSelectMode", StringValue ("DGR"));
        break;  
      case 3:
        Config::SetDefault ("ns3::Ipv4DGRRouting::RouteSelectMode", StringValue ("DDR"));
        break;
      default:
        break;
    }

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
  stack.Install (nodes);

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
      p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
      ndc[i] = p2p.Install (nc[i]);
      tch.Install (ndc[i]);
      ipic[i] = address.Assign (ndc[i]);
      address.NewNetwork ();
    }
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  dir = "exp2-results/";
  std::string dirToSave = "mkdir -p " + dir;
  if (system (dirToSave.c_str ()) == -1)
    {
      exit (1);
    }

  // ------------------------ TCP traffic ------------------------
  Ptr<Node> tcpSinkNode = nodes.Get (tcpSink);
  Ptr<Ipv4> ipv4TcpSink = tcpSinkNode->GetObject<Ipv4> ();
  Ipv4InterfaceAddress iaddrTcpSink = ipv4TcpSink->GetAddress (1,0);
  Ipv4Address ipv4AddrTcpSink = iaddrTcpSink.GetLocal ();

  // Create a PacketSinkApplication and install it on node 5
  DGRSinkHelper tcpSinkHelper("ns3::TcpSocketFactory", 
                            InetSocketAddress(Ipv4Address::GetAny(), tcp_port));
  ApplicationContainer tcpSinkApps = tcpSinkHelper.Install(nodes.Get(tcpSink));
  tcpSinkApps.Start(Seconds(0.0));
  tcpSinkApps.Stop(Seconds(11.0));

  // tcp send
  DGRTcpAppHelper sourceHelper ("ns3::TcpSocketFactory",
                               InetSocketAddress (ipv4AddrTcpSink, tcp_port));
  // Set the amount of data to send in bytes.  Zero is unlimited.
  // uint64_t maxBytes = 4000000;
  // sourceHelper.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  sourceHelper.SetAttribute ("Budget", UintegerValue (budget));
  sourceHelper.SetAttribute ("EnableFlag", BooleanValue (true));
  ApplicationContainer sourceApp = sourceHelper.Install (nodes.Get (tcpSender));
  sourceApp.Start (Seconds (0.0));
  // Hook trace source after application starts
  Simulator::Schedule(Seconds(0.1) + MilliSeconds(1), &TraceCwnd, tcpSender, 0);
  sourceApp.Stop (Seconds (10.0));

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

  // Check for dropped packets using Flow Monitor
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();
  Simulator::Schedule(Seconds(0 + 0.2), &TraceThroughput, monitor);
  
  // -------- Run the simulation --------------------------
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop(Seconds(11.0));
  Simulator::Run ();
  Simulator::Destroy ();

  delete[] ipic;
  delete[] ndc;
  delete[] nc;

  NS_LOG_INFO ("Done.");
  return 0;
}
