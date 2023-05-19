// -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2008 University of Washington
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include <vector>
#include <iomanip>
#include "ns3/names.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/network-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/net-device.h"
#include "ns3/ipv4-route.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/ipv4-packet-info-tag.h"
#include "ns3/boolean.h"
#include "ns3/node.h"
#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ipv4-dgr-routing.h"
#include "dgr-header.h"
#include "ns3/udp-header.h"
#include "dgr-route-manager.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-list-routing.h"
#include "dgrv2-queue-disc.h"
#include "dgr-tags.h"

#define DGR_PORT 666



namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Ipv4DGRRouting");

NS_OBJECT_ENSURE_REGISTERED (Ipv4DGRRouting);

TypeId 
Ipv4DGRRouting::GetTypeId (void)
{ 
  static TypeId tid = TypeId ("ns3::Ipv4DGRRouting")
    .SetParent<Ipv4RoutingProtocol> ()
    .SetGroupName ("DGR-routing")
    .AddConstructor<Ipv4DGRRouting> ()
    .AddAttribute ("RandomEcmpRouting",
                   "Set to true if packets are randomly routed among ECMP; set to false for using only one route consistently",
                   BooleanValue (false),
                   MakeBooleanAccessor (&Ipv4DGRRouting::m_randomEcmpRouting),
                   MakeBooleanChecker ())
    .AddAttribute ("RespondToInterfaceEvents",
                   "Set to true if you want to dynamically recompute the global routes upon Interface notification events (up/down, or add/remove address)",
                   BooleanValue (false),
                   MakeBooleanAccessor (&Ipv4DGRRouting::m_respondToInterfaceEvents),
                   MakeBooleanChecker ())
  ;
  return tid;
}

Ipv4DGRRouting::Ipv4DGRRouting () 
  : m_randomEcmpRouting (false),
    m_respondToInterfaceEvents (false)
{
  NS_LOG_FUNCTION (this);

  m_rand = CreateObject<UniformRandomVariable> ();

  // initialize unicast socket and broadcast socket of DGR
  Ipv4Address loopback("127.0.0.1");
  // for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i ++)
  //   {
  //     Ipv4Address addr = m_ipv4->GetAddress(i, 0).GetLocal ();
  //     if (addr == loopback)
  //       {
  //         continue;
  //       }
      
  //     // Create a socket ot listen to all the interfaces
  //     if (!m_multicastRecvSocket)
  //       {
  //         m_multicastRecvSocket = Socket::CreateSocket (GetObject<Node> (), UdpSocketFactory::GetTypeId ());
  //         m_multicastRecvSocket->SetAllowBroadcast(true);
  //         InetSocketAddress inetAddr (Ipv4Address::GetAny (), DGR_PORT);
  //         m_multicastRecvSocket->SetRecvCallback (MakeCallback (&DGR::Recv, this));
  //         if (m_multicastRecvSocket->Bind (inetAddr))
  //           {
  //             NS_FATAL_ERROR ("Failed to bind () DGR socket");
  //           }
  //         m_multicastRecvSocket->SetIpRecvTtl (true);
  //         m_multicastRecvSocket->SetRecvPktInfo (true);
  //       }
      
  //     // Create a socket to send packets from this specific interfaces
  //     Ptr<Socket> socket = Socket::CreateSocket(GetObject<Node> (), UdpSocketFactory::GetTypeId ());
  //     socket->SetAllowBroadcast (true);
  //     socket->SetIpTtl (1);
  //     InetSocketAddress inetAddr (m_ipv4->GetAddress (i, 0).GetLocal (), DGR_PORT);
  //     socket->SetRecvCallback (MakeCallback (&DGR::Recv, this));
  //     socket->BindToNetDevice (m_ipv4->GetNetDevice (i));
  //     if (socket->Bind (inetAddr))
  //       {
  //         NS_FATAL_ERROR ("Failed to bind() DGR socket");
  //       }
  //     socket->SetRecvPktInfo (true);
  //     m_unicastSocketList[socket] = m_ipv4->GetAddress(i, 0);
  //   }



}

Ipv4DGRRouting::~Ipv4DGRRouting ()
{
  NS_LOG_FUNCTION (this);
}

void 
Ipv4DGRRouting::AddHostRouteTo (Ipv4Address dest, 
                                   Ipv4Address nextHop, 
                                   uint32_t interface)
{
  NS_LOG_FUNCTION (this << dest << nextHop << interface);
  Ipv4DGRRoutingTableEntry *route = new Ipv4DGRRoutingTableEntry ();
  *route = Ipv4DGRRoutingTableEntry::CreateHostRouteTo (dest, nextHop, interface);
  m_hostRoutes.push_back (route);
}

void 
Ipv4DGRRouting::AddHostRouteTo (Ipv4Address dest, 
                                   uint32_t interface)
{
  NS_LOG_FUNCTION (this << dest << interface);
  Ipv4DGRRoutingTableEntry *route = new Ipv4DGRRoutingTableEntry ();
  *route = Ipv4DGRRoutingTableEntry::CreateHostRouteTo (dest, interface);
  m_hostRoutes.push_back (route);
}

/**
  * \author Pu Yang
  * \brief Add a host route to the global routing table with the distance 
  * between root and destination
  * \param dest The Ipv4Address destination for this route.
  * \param nextHop The next hop Ipv4Address
  * \param interface The network interface index used to send packets to the
  *  destination
  * \param distance The distance between root and destination
 */
void
Ipv4DGRRouting::AddHostRouteTo (Ipv4Address dest,
                       Ipv4Address nextHop,
                       uint32_t interface,
                       uint32_t nextInterface,
                       uint32_t distance)
{
  NS_LOG_FUNCTION (this << dest << nextHop << interface << nextInterface << distance);
  Ipv4DGRRoutingTableEntry *route = new Ipv4DGRRoutingTableEntry ();
  // std::cout << "add host route with the distance = " << distance;
  *route = Ipv4DGRRoutingTableEntry::CreateHostRouteTo(dest, nextHop, interface, nextInterface, distance);
  m_hostRoutes.push_back (route);
}



void 
Ipv4DGRRouting::AddNetworkRouteTo (Ipv4Address network, 
                                      Ipv4Mask networkMask, 
                                      Ipv4Address nextHop, 
                                      uint32_t interface)
{
  NS_LOG_FUNCTION (this << network << networkMask << nextHop << interface);
  Ipv4DGRRoutingTableEntry *route = new Ipv4DGRRoutingTableEntry ();
  *route = Ipv4DGRRoutingTableEntry::CreateNetworkRouteTo (network,
                                                        networkMask,
                                                        nextHop,
                                                        interface);
  m_networkRoutes.push_back (route);
}

void 
Ipv4DGRRouting::AddNetworkRouteTo (Ipv4Address network, 
                                      Ipv4Mask networkMask, 
                                      uint32_t interface)
{
  NS_LOG_FUNCTION (this << network << networkMask << interface);
  Ipv4DGRRoutingTableEntry *route = new Ipv4DGRRoutingTableEntry ();
  *route = Ipv4DGRRoutingTableEntry::CreateNetworkRouteTo (network,
                                                        networkMask,
                                                        interface);
  m_networkRoutes.push_back (route);
}

void 
Ipv4DGRRouting::AddASExternalRouteTo (Ipv4Address network, 
                                         Ipv4Mask networkMask,
                                         Ipv4Address nextHop,
                                         uint32_t interface)
{
  NS_LOG_FUNCTION (this << network << networkMask << nextHop << interface);
  Ipv4DGRRoutingTableEntry *route = new Ipv4DGRRoutingTableEntry ();
  *route = Ipv4DGRRoutingTableEntry::CreateNetworkRouteTo (network,
                                                        networkMask,
                                                        nextHop,
                                                        interface);
  m_ASexternalRoutes.push_back (route);
}


Ptr<Ipv4Route>
Ipv4DGRRouting::LookupUniRoute (Ipv4Address dest, Ptr<NetDevice> oif)
{
  /**
   * Get the shortest path in the routing table
  */
  NS_LOG_FUNCTION (this << dest << oif);
  NS_LOG_LOGIC ("Looking for route for destination " << dest);
  // dest.Print (std::cout);
  // std::cout << "\n";

  Ptr<Ipv4Route> rtentry = 0;
  // store all available routes that bring packets to their destination
  typedef std::vector<Ipv4DGRRoutingTableEntry*> RouteVec_t;
  RouteVec_t allRoutes;

  NS_LOG_LOGIC ("Number of m_hostRoutes = " << m_hostRoutes.size ());
  for (HostRoutesCI i = m_hostRoutes.begin (); 
       i != m_hostRoutes.end (); 
       i++) 
    {
      NS_ASSERT ((*i)->IsHost ());
      if ((*i)->GetDest () == dest)
        {
          if (oif)
            {
              if (oif != m_ipv4->GetNetDevice ((*i)->GetInterface ()))
                {
                  NS_LOG_LOGIC ("Not on requested interface, skipping");
                  continue;
                }
            }
          allRoutes.push_back (*i);
          NS_LOG_LOGIC (allRoutes.size () << "Found DGR host route" << *i); 
        }
    }
  if (allRoutes.size () > 0 ) // if route(s) is found
    {
      uint32_t routRef = 0;
      uint32_t shortestDist = allRoutes.at(0)->GetDistance ();
      for (uint32_t i = 0; i < allRoutes.size (); i ++)
      {
        if (allRoutes.at (i)->GetDistance () <  shortestDist)
        {
          routRef = i;
          shortestDist = allRoutes.at (i)->GetDistance ();
        }
      }
      Ipv4DGRRoutingTableEntry* route = allRoutes.at (routRef);

      // create a Ipv4Route object from the selected routing table entry
      rtentry = Create<Ipv4Route> ();
      rtentry->SetDestination (route->GetDest ());
      /// \todo handle multi-address case
      rtentry->SetSource (m_ipv4->GetAddress (route->GetInterface (), 0).GetLocal ());
      rtentry->SetGateway (route->GetGateway ());
      uint32_t interfaceIdx = route->GetInterface ();
      rtentry->SetOutputDevice (m_ipv4->GetNetDevice (interfaceIdx));
      return rtentry;
    }
  else 
    {
      return 0;
    }
}

Ptr<Ipv4Route>
Ipv4DGRRouting::LookupDGRRoute (Ipv4Address dest, Ptr<Packet> p, Ptr<const NetDevice> idev)
{
  PriorityTag prioTag;
  BudgetTag bgtTag;
  TimestampTag timeTag;
  p->PeekPacketTag (prioTag);
  p->PeekPacketTag (bgtTag);
  p->PeekPacketTag (timeTag);
  DistTag distTag;
  uint32_t dist = UINT32_MAX;
  dist -= 1;
  if (p->PeekPacketTag (distTag)) dist = distTag.GetDistance ();
  // budget in microseconds
  uint32_t bgt;
  if (bgtTag.GetBudget ()*1000 + timeTag.GetTimestamp ().GetMicroSeconds () < Simulator::Now ().GetMicroSeconds ())
    {
      bgt = 0;
    }
  else
    bgt = (bgtTag.GetBudget ()*1000 + timeTag.GetTimestamp ().GetMicroSeconds () - Simulator::Now ().GetMicroSeconds ());
  /**
   * Lookup a Route to forward the DGR packets.
  */
  NS_LOG_FUNCTION (this << dest << idev);
  NS_LOG_LOGIC ("Looking for route for destination " << dest);
  Ptr<Ipv4Route> rtentry = 0;
  // store all available routes that bring packets to their destination
  typedef std::vector<Ipv4DGRRoutingTableEntry*> RouteVec_t;
  // typedef std::vector<Ipv4DGRRoutingTableEntry *>::const_iterator RouteVecCI_t;
  RouteVec_t allRoutes;

  NS_LOG_LOGIC ("Number of m_hostRoutes = " << m_hostRoutes.size ());
  for (HostRoutesCI i = m_hostRoutes.begin (); 
       i != m_hostRoutes.end (); 
       i++) 
    {
      NS_ASSERT ((*i)->IsHost ());
      if ((*i)->GetDest () == dest)
        {
          // std::cout << "find routes continue" << std::endl;
          if (idev)
            {
              if (idev == m_ipv4->GetNetDevice ((*i)->GetInterface ()))
                {
                  NS_LOG_LOGIC ("Not on requested interface, skipping");
                  continue;
                }
            }

          // if interface is down, continue
          if (!m_ipv4->IsUp ((*i)->GetInterface ())) continue;

          // get the output device
          Ptr <NetDevice> dev_local = m_ipv4->GetNetDevice ((*i)->GetInterface ());
          //get the queue disc on the device
          Ptr<QueueDisc> disc = m_ipv4->GetObject<Node> ()->GetObject<TrafficControlLayer> ()->GetRootQueueDiscOnDevice (dev_local);
          Ptr<DGRv2QueueDisc> dvq = DynamicCast <DGRv2QueueDisc> (disc);
          uint32_t dvq_length = dvq->GetInternalQueue (1) ->GetCurrentSize ().GetValue ();
          uint32_t dvq_max = dvq->GetInternalQueue (1)->GetMaxSize ().GetValue ();
          if (dvq_length >= dvq_max * 0.75)
            {
              // std::cout << "congestion happend at node: " << dev->GetNode ()->GetId () << std::endl;
              NS_LOG_LOGIC ("Congestion happened, skipping");
              continue;
            }
        
          //
          // Get queue info on the other side
          //
          if ((*i)->GetNextInterface () != 0xffffffff)
            {
              // get the nexthop queue infomation
              Ptr<Channel> ch = dev_local->GetChannel ();
              //
              // Get the net device on the other side of point-to-point
              //
              Ptr<NetDevice> dev_remote = ch->GetDevice (0) == dev_local ? ch->GetDevice (1) : ch->GetDevice (0);
              Ptr<Node> node_remote = dev_remote->GetNode ();
              Ptr<NetDevice> dev_next = node_remote->GetDevice ((*i)->GetNextInterface ());

              Ptr<QueueDisc> queue_next = dev_next->GetObject <TrafficControlLayer> ()->GetRootQueueDiscOnDevice (dev_next);
              Ptr<DGRv2QueueDisc> dvq_next = DynamicCast <DGRv2QueueDisc> (queue_next);
              uint32_t dvq_slow_next = dvq_next->GetInternalQueue (1)->GetCurrentSize ().GetValue ();
              uint32_t dvq_slow_max_next = dvq_next->GetInternalQueue (1)->GetMaxSize ().GetValue ();
              uint32_t dvq_normal_next = dvq_next->GetInternalQueue (2)->GetCurrentSize ().GetValue ();
              uint32_t dvq_normal_max_next = 155;  // next_dvq->GetInternalQueue (2)-> GetMaxSize ().GetValue ();
              // if (next_dvq_slow_length != 0) std::cout << "slow lane current: " << next_dvq_slow_length  << "slow_max: " << next_dvq_slow_max << std::endl;
              // std::cout <<"next node " << d_node->GetId () << " next dvq_length: " << next_dvq_length  << "    " << next_dvq_max << std::endl;
              if (dvq_slow_next >= dvq_slow_max_next * 0.75 || dvq_normal_next >= dvq_normal_max_next * 0.75)
                {
                  // std::cout << "congestion happend at next node: " << d_node->GetId () << std::endl;
                  NS_LOG_LOGIC ("Congestion happend in next hop, skipping");
                  continue;
                }
              if (dev_remote == dev_next)
                {
                  continue;
                }
            }

          if ((*i)->GetDistance () > bgt || (*i)->GetDistance () > dist)
            {
              // std::cout << "at node: " << dev->GetNode ()->GetId ();
              // std::cout << "budget continue" << std::endl;
              // std::cout << (*i)->GetDistance () << ", " << bgt << std::endl;
              // std::cout << "budget: " << bgt << " distance: " << (*i)->GetDistance () << " dist: " << dist << std::endl;
              NS_LOG_LOGIC ("Too far to the destination, skipping");
              continue;
            }
          allRoutes.push_back (*i);
          // std::cout << allRoutes.size () << "Found DGR host route" << *i << " with Cost: " << (*i)->GetDistance ();
          NS_LOG_LOGIC (allRoutes.size () << "Found DGR host route" << *i << " with Cost: " << (*i)->GetDistance ()); 
        }
    }
  if (allRoutes.size () > 0 ) // if route(s) is found
    {
      // random select
      uint32_t selectIndex = m_rand->GetInteger (0, allRoutes.size ()-1);

      // // optimal
      // uint32_t selectIndex = 0;
      // uint32_t min = UINT32_MAX;
      // for (uint32_t i = 0; i < allRoutes.size (); i++)
      // {
      //   if (allRoutes.at (i)->GetDistance () < min)
      //   {
      //     min = allRoutes.at (i)->GetDistance ();
      //     selectIndex = i;
      //   }
      //  }

      // // worst
      // uint32_t selectIndex = 0;
      // uint32_t max = 0;
      // for (uint32_t i = 0; i < allRoutes.size (); i++)
      // {
      //   if (allRoutes.at (i)->GetDistance () > max)
      //   {
      //     max = allRoutes.at (i)->GetDistance ();
      //     selectIndex = i;
      //   }
      // }

      // // back pressure selection
      // uint32_t selectIndex = 0;
      // double minPressure = 1.0;
      // uint32_t k = 0;
      // for (RouteVecCI_t i = allRoutes.begin (); 
      //      i != allRoutes.end (); 
      //      i++, k ++)
      //   {
      //     // get the output device
      //     Ptr <NetDevice> dev = m_ipv4->GetNetDevice ((*i)->GetInterface ());   
      //     // get the nexthop queue infomation
      //     Ptr<Channel> channel = dev->GetChannel ();
      //     PointToPointChannel *p2pchannel = dynamic_cast <PointToPointChannel *> (PeekPointer (channel));
      //     if (p2pchannel != 0)
      //       {
      //         Ptr<Node> node = dev->GetNode ();
      //         Ptr<PointToPointNetDevice> d_dev = p2pchannel->GetDestination (0) == dev ? p2pchannel->GetDestination (1) : p2pchannel->GetDestination (0);
      //         Ptr<Node> d_node = d_dev->GetNode ();
      //         if ((*i)->GetNextInterface () != 0xffffffff)
      //           {
      //             Ptr<NetDevice> next_dev = d_node->GetDevice ((*i)->GetNextInterface ());
      //             // std::cout << "next node: " << d_node->GetId () << "next interface: " << (*i)->GetNextInterface () << std::endl;
      //             Ptr<QueueDisc> next_disc = d_node->GetObject<TrafficControlLayer> ()->GetRootQueueDiscOnDevice (next_dev);
      //             Ptr<DGRVirtualQueueDisc> next_dvq = DynamicCast <DGRVirtualQueueDisc> (next_disc);
      //             uint32_t next_dvq_length = next_dvq->GetInternalQueue (1)->GetCurrentSize ().GetValue ();
      //             uint32_t next_dvq_max = next_dvq->GetInternalQueue (1)->GetMaxSize ().GetValue ();
      //             uint32_t next_dvq_slow_length = next_dvq->GetInternalQueue (2)->GetCurrentSize ().GetValue ();
      //             // uint32_t next_dvq_slow_max = next_dvq->GetInternalQueue (2)-> GetMaxSize ().GetValue ();
      //             double pressure1 = next_dvq_length*1.0/next_dvq_max;
      //             double pressure2 = next_dvq_slow_length*1.0/155;
      //             // if (next_dvq_slow_length != 0) std::cout << "slow lane current: " << next_dvq_slow_length  << "slow_max: " << next_dvq_slow_max << std::endl;
      //             if (pressure1 < pressure2) pressure1 = pressure2;
      //             // if (pressure1 > 0.2) std::cout << pressure1 << std::endl;
      //             if (pressure1 < minPressure)
      //               {
      //                 minPressure = pressure1;
      //                 selectIndex = k;
      //               }
      //           }
      //       }
      //   }

      // // back pressure + random selection
      // uint32_t selectIndex = 0;
      // double minPressure = 1.0;
      // uint32_t k = 0;
      // for (RouteVecCI_t i = allRoutes.begin (); 
      //      i != allRoutes.end (); 
      //      i++, k ++)
      //   {
      //     // get the output device
      //     Ptr <NetDevice> dev = m_ipv4->GetNetDevice ((*i)->GetInterface ());   
      //     // get the nexthop queue infomation
      //     Ptr<Channel> channel = dev->GetChannel ();
      //     PointToPointChannel *p2pchannel = dynamic_cast <PointToPointChannel *> (PeekPointer (channel));
      //     if (p2pchannel != 0)
      //       {
      //         Ptr<Node> node = dev->GetNode ();
      //         Ptr<PointToPointNetDevice> d_dev = p2pchannel->GetDestination (0) == dev ? p2pchannel->GetDestination (1) : p2pchannel->GetDestination (0);
      //         Ptr<Node> d_node = d_dev->GetNode ();
      //         if ((*i)->GetNextInterface () != 0xffffffff)
      //           {
      //             Ptr<NetDevice> next_dev = d_node->GetDevice ((*i)->GetNextInterface ());
      //             // std::cout << "next node: " << d_node->GetId () << "next interface: " << (*i)->GetNextInterface () << std::endl;
      //             Ptr<QueueDisc> next_disc = d_node->GetObject<TrafficControlLayer> ()->GetRootQueueDiscOnDevice (next_dev);
      //             Ptr<DGRVirtualQueueDisc> next_dvq = DynamicCast <DGRVirtualQueueDisc> (next_disc);
      //             uint32_t next_dvq_length = next_dvq->GetInternalQueue (1)->GetCurrentSize ().GetValue ();
      //             uint32_t next_dvq_max = next_dvq->GetInternalQueue (1)->GetMaxSize ().GetValue ();
      //             uint32_t next_dvq_slow_length = next_dvq->GetInternalQueue (2)->GetCurrentSize ().GetValue ();
      //             // uint32_t next_dvq_slow_max = next_dvq->GetInternalQueue (2)-> GetMaxSize ().GetValue ();
      //             double pressure1 = next_dvq_length*1.0/next_dvq_max;
      //             double pressure2 = next_dvq_slow_length*1.0/155;
      //             // if (next_dvq_slow_length != 0) std::cout << "slow lane current: " << next_dvq_slow_length  << "slow_max: " << next_dvq_slow_max << std::endl;
      //             if (pressure1 < pressure2) pressure1 = pressure2;
      //             // if (pressure1 > 0.2) std::cout << pressure1 << std::endl;
      //             if (pressure1 < minPressure)
      //               {
      //                 minPressure = pressure1;
      //                 selectIndex = k;
      //               }
      //           }
      //       }
      //   }

      Ipv4DGRRoutingTableEntry* route = allRoutes.at (selectIndex);
      rtentry = Create<Ipv4Route> ();
      rtentry->SetDestination (route->GetDest ());
      /// \todo handle multi-address case
      rtentry->SetSource (m_ipv4->GetAddress (route->GetInterface (), 0).GetLocal ());
      rtentry->SetGateway (route->GetGateway ());
      uint32_t interfaceIdx = route->GetInterface ();
      rtentry->SetOutputDevice (m_ipv4->GetNetDevice (interfaceIdx));
      // if (route->GetDistance () > 30000) std::cout << "budget: " << bgt << " distance: " << route->GetDistance () << std::endl;
      if (bgt - route->GetDistance () <= 20)
        {
          prioTag.SetPriority (0);
        }
      else
        {
          prioTag.SetPriority (1);
        }
      distTag.SetDistance (route->GetDistance ());
      p->ReplacePacketTag (prioTag);
      
      p->ReplacePacketTag (distTag);
      return rtentry;
    }
  else 
    {
      return 0;
    }
}


// Ptr<Ipv4Route>
// Ipv4DGRRouting::LookupDGRRoute (Ipv4Address dest, Ptr <Packet> p, Ptr<const NetDevice> idev)
// {
//   PriorityTag prioTag;
//   BudgetTag bgtTag;
//   TimestampTag timeTag;
//   p->PeekPacketTag (prioTag);
//   p->PeekPacketTag (bgtTag);
//   p->PeekPacketTag (timeTag);
//   DistTag distTag;
//   uint32_t dist = UINT32_MAX;
//   dist -= 1;
//   if (p->PeekPacketTag (distTag)) dist = distTag.GetDistance ();
//   // budget in microseconds
//   uint32_t bgt;
//   if (bgtTag.GetBudget () + timeTag.GetTimestamp ().GetMicroSeconds () < Simulator::Now ().GetMicroSeconds ())
//     {
//       bgt = 0;
//     }
//   else
//     bgt = (bgtTag.GetBudget () + timeTag.GetTimestamp ().GetMicroSeconds () - Simulator::Now ().GetMicroSeconds ())/100;
//   /**
//    * Lookup a Route to forward the DGR packets.
//   */
//   NS_LOG_FUNCTION (this << dest << idev);
//   NS_LOG_LOGIC ("Looking for route for destination " << dest);
//   Ptr<Ipv4Route> rtentry = 0;
//   // store all available routes that bring packets to their destination
//   typedef std::vector<Ipv4DGRRoutingTableEntry*> RouteVec_t;
//   // typedef std::vector<Ipv4DGRRoutingTableEntry *>::const_iterator RouteVecCI_t;
//   RouteVec_t allRoutes;

//   NS_LOG_LOGIC ("Number of m_hostRoutes = " << m_hostRoutes.size ());
//   for (HostRoutesCI i = m_hostRoutes.begin (); 
//        i != m_hostRoutes.end (); 
//        i++) 
//     {
//       NS_ASSERT ((*i)->IsHost ());
//       if ((*i)->GetDest () == dest)
//         {
//           if (idev)
//             {
//               if (idev == m_ipv4->GetNetDevice ((*i)->GetInterface ()))
//                 {
//                   NS_LOG_LOGIC ("Not on requested interface, skipping");
//                   continue;
//                 }
//             }

//           // if interface is down, continue
//           if (!m_ipv4->IsUp ((*i)->GetInterface ())) continue;

//           // get the output device
//           Ptr <NetDevice> dev = m_ipv4->GetNetDevice ((*i)->GetInterface ());
//           //get the queue disc on the device
//           Ptr<QueueDisc> disc = m_ipv4->GetNetDevice (0)->GetNode ()->GetObject<TrafficControlLayer> ()->GetRootQueueDiscOnDevice (dev);
//           Ptr<DGRVirtualQueueDisc> dvq = DynamicCast <DGRVirtualQueueDisc> (disc);
//           uint32_t dvq_length = dvq->GetInternalQueue (1) ->GetCurrentSize ().GetValue ();
//           uint32_t dvq_max = dvq->GetInternalQueue (1)->GetMaxSize ().GetValue ();
//           if (dvq_length >= dvq_max * 0.75)
//             {
//               // std::cout << "congestion happend at node: " << dev->GetNode ()->GetId () << std::endl;
//               NS_LOG_LOGIC ("Congestion happened, skipping");
//               continue;
//             }
//           // get the nexthop queue infomation
//           Ptr<Channel> channel = dev->GetChannel ();
//           PointToPointChannel *p2pchannel = dynamic_cast <PointToPointChannel *> (PeekPointer (channel));
//           if (p2pchannel != 0)
//             {
//               Ptr<Node> node = dev->GetNode ();
//               Ptr<PointToPointNetDevice> d_dev = p2pchannel->GetDestination (0) == dev ? p2pchannel->GetDestination (1) : p2pchannel->GetDestination (0);
//               Ptr<Node> d_node = d_dev->GetNode ();
//               // std::cout << "at node: " << node->GetId () << ", " << std::endl;
//               if ((*i)->GetNextInterface () != 0xffffffff)
//                 {
//                   Ptr<NetDevice> next_dev = d_node->GetDevice ((*i)->GetNextInterface ());
//                   // std::cout << "next node: " << d_node->GetId () << "next interface: " << (*i)->GetNextInterface () << std::endl;
//                   Ptr<QueueDisc> next_disc = d_node->GetObject<TrafficControlLayer> ()->GetRootQueueDiscOnDevice (next_dev);
//                   Ptr<DGRVirtualQueueDisc> next_dvq = DynamicCast <DGRVirtualQueueDisc> (next_disc);
//                   uint32_t next_dvq_length = next_dvq->GetInternalQueue (1)->GetCurrentSize ().GetValue ();
//                   uint32_t next_dvq_max = next_dvq->GetInternalQueue (1)->GetMaxSize ().GetValue ();
//                   uint32_t next_dvq_slow_length = next_dvq->GetInternalQueue (2)->GetCurrentSize ().GetValue ();
//                   uint32_t next_dvq_slow_max = 155;  // next_dvq->GetInternalQueue (2)-> GetMaxSize ().GetValue ();
//                   // if (next_dvq_slow_length != 0) std::cout << "slow lane current: " << next_dvq_slow_length  << "slow_max: " << next_dvq_slow_max << std::endl;
//                   // std::cout <<"next node " << d_node->GetId () << " next dvq_length: " << next_dvq_length  << "    " << next_dvq_max << std::endl;
//                   if (next_dvq_length >= next_dvq_max * 0.75 || next_dvq_slow_length >= next_dvq_slow_max * 0.75)
//                     {
//                       // std::cout << "congestion happend at next node: " << d_node->GetId () << std::endl;
//                       NS_LOG_LOGIC ("Congestion happend in next hop, skipping");
//                       continue;
//                     }
//                   if (d_dev == next_dev)
//                     {continue;}
//                 }
//             }

//           // find the nexthop queue

//           if ((*i)->GetDistance () > bgt || (*i)->GetDistance () > dist)
//             {
//               // std::cout << "at node: " << dev->GetNode ()->GetId ();
//               // std::cout << "budget: " << bgt << " distance: " << (*i)->GetDistance () << " dist: " << dist << std::endl;
//               NS_LOG_LOGIC ("Too far to the destination, skipping");
//               continue;
//             }
//           allRoutes.push_back (*i);
//           NS_LOG_LOGIC (allRoutes.size () << "Found DGR host route" << *i << " with Cost: " << (*i)->GetDistance ()); 
//         }
//     }
//   if (allRoutes.size () > 0 ) // if route(s) is found
//     {
//       // std::cout << "routes size: " << allRoutes.size () << std::endl;
//       // random select
//       uint32_t selectIndex = m_rand->GetInteger (0, allRoutes.size ()-1);
//       // // optimal
//       // uint32_t selectIndex = 0;
//       // uint32_t min = UINT32_MAX;
//       // for (uint32_t i = 0; i < allRoutes.size (); i++)
//       // {
//       //   if (allRoutes.at (i)->GetDistance () < min)
//       //   {
//       //     min = allRoutes.at (i)->GetDistance ();
//       //     selectIndex = i;
//       //   }
//       //  }

//       // // worst
//       // uint32_t selectIndex = 0;
//       // uint32_t max = 0;
//       // for (uint32_t i = 0; i < allRoutes.size (); i++)
//       // {
//       //   if (allRoutes.at (i)->GetDistance () > max)
//       //   {
//       //     max = allRoutes.at (i)->GetDistance ();
//       //     selectIndex = i;
//       //   }
//       // }

//       // // back pressure selection
//       // uint32_t selectIndex = 0;
//       // double minPressure = 1.0;
//       // uint32_t k = 0;
//       // for (RouteVecCI_t i = allRoutes.begin (); 
//       //      i != allRoutes.end (); 
//       //      i++, k ++)
//       //   {
//       //     // get the output device
//       //     Ptr <NetDevice> dev = m_ipv4->GetNetDevice ((*i)->GetInterface ());   
//       //     // get the nexthop queue infomation
//       //     Ptr<Channel> channel = dev->GetChannel ();
//       //     PointToPointChannel *p2pchannel = dynamic_cast <PointToPointChannel *> (PeekPointer (channel));
//       //     if (p2pchannel != 0)
//       //       {
//       //         Ptr<Node> node = dev->GetNode ();
//       //         Ptr<PointToPointNetDevice> d_dev = p2pchannel->GetDestination (0) == dev ? p2pchannel->GetDestination (1) : p2pchannel->GetDestination (0);
//       //         Ptr<Node> d_node = d_dev->GetNode ();
//       //         if ((*i)->GetNextInterface () != 0xffffffff)
//       //           {
//       //             Ptr<NetDevice> next_dev = d_node->GetDevice ((*i)->GetNextInterface ());
//       //             // std::cout << "next node: " << d_node->GetId () << "next interface: " << (*i)->GetNextInterface () << std::endl;
//       //             Ptr<QueueDisc> next_disc = d_node->GetObject<TrafficControlLayer> ()->GetRootQueueDiscOnDevice (next_dev);
//       //             Ptr<DGRVirtualQueueDisc> next_dvq = DynamicCast <DGRVirtualQueueDisc> (next_disc);
//       //             uint32_t next_dvq_length = next_dvq->GetInternalQueue (1)->GetCurrentSize ().GetValue ();
//       //             uint32_t next_dvq_max = next_dvq->GetInternalQueue (1)->GetMaxSize ().GetValue ();
//       //             uint32_t next_dvq_slow_length = next_dvq->GetInternalQueue (2)->GetCurrentSize ().GetValue ();
//       //             // uint32_t next_dvq_slow_max = next_dvq->GetInternalQueue (2)-> GetMaxSize ().GetValue ();
//       //             double pressure1 = next_dvq_length*1.0/next_dvq_max;
//       //             double pressure2 = next_dvq_slow_length*1.0/155;
//       //             // if (next_dvq_slow_length != 0) std::cout << "slow lane current: " << next_dvq_slow_length  << "slow_max: " << next_dvq_slow_max << std::endl;
//       //             if (pressure1 < pressure2) pressure1 = pressure2;
//       //             // if (pressure1 > 0.2) std::cout << pressure1 << std::endl;
//       //             if (pressure1 < minPressure)
//       //               {
//       //                 minPressure = pressure1;
//       //                 selectIndex = k;
//       //               }
//       //           }
//       //       }
//       //   }

//       // // back pressure + random selection
//       // uint32_t selectIndex = 0;
//       // double minPressure = 1.0;
//       // uint32_t k = 0;
//       // for (RouteVecCI_t i = allRoutes.begin (); 
//       //      i != allRoutes.end (); 
//       //      i++, k ++)
//       //   {
//       //     // get the output device
//       //     Ptr <NetDevice> dev = m_ipv4->GetNetDevice ((*i)->GetInterface ());   
//       //     // get the nexthop queue infomation
//       //     Ptr<Channel> channel = dev->GetChannel ();
//       //     PointToPointChannel *p2pchannel = dynamic_cast <PointToPointChannel *> (PeekPointer (channel));
//       //     if (p2pchannel != 0)
//       //       {
//       //         Ptr<Node> node = dev->GetNode ();
//       //         Ptr<PointToPointNetDevice> d_dev = p2pchannel->GetDestination (0) == dev ? p2pchannel->GetDestination (1) : p2pchannel->GetDestination (0);
//       //         Ptr<Node> d_node = d_dev->GetNode ();
//       //         if ((*i)->GetNextInterface () != 0xffffffff)
//       //           {
//       //             Ptr<NetDevice> next_dev = d_node->GetDevice ((*i)->GetNextInterface ());
//       //             // std::cout << "next node: " << d_node->GetId () << "next interface: " << (*i)->GetNextInterface () << std::endl;
//       //             Ptr<QueueDisc> next_disc = d_node->GetObject<TrafficControlLayer> ()->GetRootQueueDiscOnDevice (next_dev);
//       //             Ptr<DGRVirtualQueueDisc> next_dvq = DynamicCast <DGRVirtualQueueDisc> (next_disc);
//       //             uint32_t next_dvq_length = next_dvq->GetInternalQueue (1)->GetCurrentSize ().GetValue ();
//       //             uint32_t next_dvq_max = next_dvq->GetInternalQueue (1)->GetMaxSize ().GetValue ();
//       //             uint32_t next_dvq_slow_length = next_dvq->GetInternalQueue (2)->GetCurrentSize ().GetValue ();
//       //             // uint32_t next_dvq_slow_max = next_dvq->GetInternalQueue (2)-> GetMaxSize ().GetValue ();
//       //             double pressure1 = next_dvq_length*1.0/next_dvq_max;
//       //             double pressure2 = next_dvq_slow_length*1.0/155;
//       //             // if (next_dvq_slow_length != 0) std::cout << "slow lane current: " << next_dvq_slow_length  << "slow_max: " << next_dvq_slow_max << std::endl;
//       //             if (pressure1 < pressure2) pressure1 = pressure2;
//       //             // if (pressure1 > 0.2) std::cout << pressure1 << std::endl;
//       //             if (pressure1 < minPressure)
//       //               {
//       //                 minPressure = pressure1;
//       //                 selectIndex = k;
//       //               }
//       //           }
//       //       }
//       //   }

//       Ipv4DGRRoutingTableEntry* route = allRoutes.at (selectIndex);
//       rtentry = Create<Ipv4Route> ();
//       rtentry->SetDestination (route->GetDest ());
//       /// \todo handle multi-address case
//       rtentry->SetSource (m_ipv4->GetAddress (route->GetInterface (), 0).GetLocal ());
//       rtentry->SetGateway (route->GetGateway ());
//       uint32_t interfaceIdx = route->GetInterface ();
//       rtentry->SetOutputDevice (m_ipv4->GetNetDevice (interfaceIdx));
//       // if (route->GetDistance () > 30000) std::cout << "budget: " << bgt << " distance: " << route->GetDistance () << std::endl;
//       if (bgt - route->GetDistance () <= 20)
//         {
//           prioTag.SetPriority (0);
//         }
//       else
//         {
//           prioTag.SetPriority (1);
//         }
//       distTag.SetDistance (route->GetDistance ());
//       p->ReplacePacketTag (prioTag);
      
//       p->ReplacePacketTag (distTag);
//       return rtentry;
//     }
//   else 
//     {
//       return 0;
//     }
// }

uint32_t 
Ipv4DGRRouting::GetNRoutes (void) const
{
  NS_LOG_FUNCTION (this);
  uint32_t n = 0;
  n += m_hostRoutes.size ();
  n += m_networkRoutes.size ();
  n += m_ASexternalRoutes.size ();
  return n;
}

Ipv4DGRRoutingTableEntry *
Ipv4DGRRouting::GetRoute (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);
  if (index < m_hostRoutes.size ())
    {
      uint32_t tmp = 0;
      for (HostRoutesCI i = m_hostRoutes.begin (); 
           i != m_hostRoutes.end (); 
           i++) 
        {
          if (tmp  == index)
            {
              return *i;
            }
          tmp++;
        }
    }
  index -= m_hostRoutes.size ();
  uint32_t tmp = 0;
  if (index < m_networkRoutes.size ())
    {
      for (NetworkRoutesCI j = m_networkRoutes.begin (); 
           j != m_networkRoutes.end ();
           j++)
        {
          if (tmp == index)
            {
              return *j;
            }
          tmp++;
        }
    }
  index -= m_networkRoutes.size ();
  tmp = 0;
  for (ASExternalRoutesCI k = m_ASexternalRoutes.begin (); 
       k != m_ASexternalRoutes.end (); 
       k++) 
    {
      if (tmp == index)
        {
          return *k;
        }
      tmp++;
    }
  NS_ASSERT (false);
  // quiet compiler.
  return 0;
}
void 
Ipv4DGRRouting::RemoveRoute (uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  if (index < m_hostRoutes.size ())
    {
      uint32_t tmp = 0;
      for (HostRoutesI i = m_hostRoutes.begin (); 
           i != m_hostRoutes.end (); 
           i++) 
        {
          if (tmp  == index)
            {
              NS_LOG_LOGIC ("Removing route " << index << "; size = " << m_hostRoutes.size ());
              delete *i;
              m_hostRoutes.erase (i);
              NS_LOG_LOGIC ("Done removing host route " << index << "; host route remaining size = " << m_hostRoutes.size ());
              return;
            }
          tmp++;
        }
    }
  index -= m_hostRoutes.size ();
  uint32_t tmp = 0;
  for (NetworkRoutesI j = m_networkRoutes.begin (); 
       j != m_networkRoutes.end (); 
       j++) 
    {
      if (tmp == index)
        {
          NS_LOG_LOGIC ("Removing route " << index << "; size = " << m_networkRoutes.size ());
          delete *j;
          m_networkRoutes.erase (j);
          NS_LOG_LOGIC ("Done removing network route " << index << "; network route remaining size = " << m_networkRoutes.size ());
          return;
        }
      tmp++;
    }
  index -= m_networkRoutes.size ();
  tmp = 0;
  for (ASExternalRoutesI k = m_ASexternalRoutes.begin (); 
       k != m_ASexternalRoutes.end ();
       k++)
    {
      if (tmp == index)
        {
          NS_LOG_LOGIC ("Removing route " << index << "; size = " << m_ASexternalRoutes.size ());
          delete *k;
          m_ASexternalRoutes.erase (k);
          NS_LOG_LOGIC ("Done removing network route " << index << "; network route remaining size = " << m_networkRoutes.size ());
          return;
        }
      tmp++;
    }
  NS_ASSERT (false);
}

int64_t
Ipv4DGRRouting::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_rand->SetStream (stream);
  return 1;
}

void
Ipv4DGRRouting::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  for (HostRoutesI i = m_hostRoutes.begin (); 
       i != m_hostRoutes.end (); 
       i = m_hostRoutes.erase (i)) 
    {
      delete (*i);
    }
  for (NetworkRoutesI j = m_networkRoutes.begin (); 
       j != m_networkRoutes.end (); 
       j = m_networkRoutes.erase (j)) 
    {
      delete (*j);
    }
  for (ASExternalRoutesI l = m_ASexternalRoutes.begin (); 
       l != m_ASexternalRoutes.end ();
       l = m_ASexternalRoutes.erase (l))
    {
      delete (*l);
    }

  Ipv4RoutingProtocol::DoDispose ();
}

// Formatted like output of "route -n" command
void
Ipv4DGRRouting::PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
{
  NS_LOG_FUNCTION (this << stream);
  std::ostream* os = stream->GetStream ();
  // Copy the current ostream state
  std::ios oldState (nullptr);
  oldState.copyfmt (*os);

  *os << "Node: " << m_ipv4->GetObject<Node> ()->GetId ()
      << ", Time: " << Now().As (unit)
      << ", Local time: " << m_ipv4->GetObject<Node> ()->GetLocalTime ().As (unit)
      << ", Ipv4DGRRouting table" << std::endl;

  if (GetNRoutes () > 0)
    {
      *os << "  Destination     Gateway    Flags   Metric  Iface   NextIface" << std::endl;
      for (uint32_t j = 0; j < GetNRoutes (); j++)
        {
          std::ostringstream dest, gw, mask, flags, metric;
          Ipv4DGRRoutingTableEntry route = GetRoute (j);
          dest << route.GetDest ();
          *os  << std::setw (13) << dest.str ();
          gw << route.GetGateway ();
          *os  << std::setw (13) << gw.str ();
          flags << "U";
          if (route.IsHost ())
            {
              flags << "H";
            }
          else if (route.IsGateway ())
            {
              flags << "G";
            }
          *os << std::setiosflags (std::ios::left) << std::setw (6) << flags.str ();
          metric << route.GetDistance ();
          if (route.GetDistance () == 0xffffffff)
            {
              *os << std::setw (9) << "-";
            }
          else
            {
              *os << std::setw(9) << metric.str();
            }

          if (Names::FindName (m_ipv4->GetNetDevice (route.GetInterface ())) != "")
            {
              *os << Names::FindName (m_ipv4->GetNetDevice (route.GetInterface ()));
            }
          else
            {
              *os << std::setw (7) << route.GetInterface ();
              if (route.GetNextInterface () != 0xffffffff)
                {
                  *os << std::setw (8) << route.GetNextInterface ();
                }
              else
                {
                  *os << std::setw (8) << "-";
                }
            }
          *os << std::endl;
        }
    }
  *os << std::endl;
  (*os).copyfmt (oldState);
}



Ptr<Ipv4Route>
Ipv4DGRRouting::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
{
  // std::cout << "at Node: " << m_ipv4->GetNetDevice (0)->GetNode ()->GetId () << "RouteOutput" << std::endl;
  NS_LOG_FUNCTION (this << p << &header << oif << &sockerr);
  //
  // First, see if this is a multicast packet we have a route for.  If we
  // have a route, then send the packet down each of the specified interfaces.
  //
  if (header.GetDestination ().IsMulticast ())
    {
      NS_LOG_LOGIC ("Multicast destination-- returning false");
      return 0; // Let other routing protocols try to handle this
    }
        
  //
  // See if this is a Delay-Guarenteed packet we have a route for.
  //
  NS_LOG_LOGIC ("Delay-Guarenteed destination- looking up");
  Ptr<Ipv4Route> rtentry;
  BudgetTag budgetTag;
  if (!p)
    {
      rtentry = LookupUniRoute (header.GetDestination (), oif);
    }
  else if (p->PeekPacketTag (budgetTag))
    {
      rtentry = LookupDGRRoute (header.GetDestination (), p, oif);
      }
  else
    {
      rtentry = LookupUniRoute (header.GetDestination (), oif);
    }

  if (rtentry)
    {
      sockerr = Socket::ERROR_NOTERROR;
    }
  else
    {
      sockerr = Socket::ERROR_NOROUTETOHOST;
    }
  return rtentry;
}



// bool 
// Ipv4DGRRouting::RouteInput  (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
//                                 UnicastForwardCallback ucb, MulticastForwardCallback mcb,
//                                 LocalDeliverCallback lcb, ErrorCallback ecb)
// { 
//   Ptr <Packet> p_copy = p->Copy();
//   NS_LOG_FUNCTION (this << p << header << header.GetSource () << header.GetDestination () << idev << &lcb << &ecb);
//   // Check if input device supports IP
//   NS_ASSERT (m_ipv4->GetInterfaceForDevice (idev) >= 0);
//   uint32_t iif = m_ipv4->GetInterfaceForDevice (idev);

//   if (m_ipv4->IsDestinationAddress (header.GetDestination (), iif))
//     {
//       if (!lcb.IsNull ())
//         {
//           NS_LOG_LOGIC ("Local delivery to " << header.GetDestination ());
//           // std::cout << "Local delivery to " << header.GetDestination () << std::endl;
//           lcb (p, header, iif);
//           return true;
//         }
//       else
//         {
//           // The local delivery callback is null.  This may be a multicast
//           // or broadcast packet, so return false so that another
//           // multicast routing protocol can handle it.  It should be possible
//           // to extend this to explicitly check whether it is a unicast
//           // packet, and invoke the error callback if so
//           // std::cout << "ERROR !!!!" << std::endl;
//           return false;
//         }
//     }
//   // Check if input device supports IP forwarding
//   if (m_ipv4->IsForwarding (iif) == false)
//     {
//       NS_LOG_LOGIC ("Forwarding disabled for this interface");
//       // std::cout << "RI: Forwarding disabled for this interface" << std::endl;
//       ecb (p, header, Socket::ERROR_NOROUTETOHOST);
//       return true;
//     }
//   // Next, try to find a route
//   NS_LOG_LOGIC ("Unicast destination- looking up global route");
//   Ptr<Ipv4Route> rtentry; 
//   BudgetTag budgetTag;
  
//   if (p->PeekPacketTag (budgetTag) && budgetTag.GetBudget () != 0)
//   {
//     rtentry = LookupDGRRoute (header.GetDestination (), p_copy, idev); 
//   }
//   else
//   {
//     rtentry = LookupUniRoute (header.GetDestination ());
//   }
//   if (rtentry)
//     {
//       const Ptr <Packet> p_c = p_copy->Copy();
//       NS_LOG_LOGIC ("Found unicast destination- calling unicast callback");
//       ucb (rtentry, p_c, header);
//       return true; 
//     }
//   else
//     {
//       NS_LOG_LOGIC ("Did not find unicast destination- returning false");
//       return false; // Let other routing protocols try to handle this
//                     // route request.
//     }
// }




bool 
Ipv4DGRRouting::RouteInput  (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                                UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                                LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_LOG_FUNCTION (this << p << header << header.GetSource () << header.GetDestination () << idev << &lcb << &ecb);
  // Check if input device supports IP
  NS_ASSERT (m_ipv4->GetInterfaceForDevice (idev) >= 0);
  uint32_t iif = m_ipv4->GetInterfaceForDevice (idev);

  if (m_ipv4->IsDestinationAddress (header.GetDestination (), iif))
    {
      if (!lcb.IsNull ())
        {
          NS_LOG_LOGIC ("Local delivery to " << header.GetDestination ());
          // std::cout << "Local delivery to " << header.GetDestination () << std::endl;
          lcb (p, header, iif);
          return true;
        }
      else
        {
          // The local delivery callback is null.  This may be a multicast
          // or broadcast packet, so return false so that another
          // multicast routing protocol can handle it.  It should be possible
          // to extend this to explicitly check whether it is a unicast
          // packet, and invoke the error callback if so
          // std::cout << "ERROR !!!!" << std::endl;
          return false;
        }
    }
  // Check if input device supports IP forwarding
  if (m_ipv4->IsForwarding (iif) == false)
    {
      NS_LOG_LOGIC ("Forwarding disabled for this interface");
      // std::cout << "RI: Forwarding disabled for this interface" << std::endl;
      ecb (p, header, Socket::ERROR_NOROUTETOHOST);
      return true;
    }
  // Next, try to find a route
  NS_LOG_LOGIC ("Unicast destination- looking up global route");
  Ptr<Ipv4Route> rtentry; 
  BudgetTag budgetTag;
  Ptr <Packet> p_copy;
  if (p->PeekPacketTag (budgetTag))
  {
    p_copy = p->Copy();
    rtentry = LookupDGRRoute (header.GetDestination (), p_copy, idev); 
  }
  else
  {
    rtentry = LookupUniRoute (header.GetDestination ());
  }
  if (rtentry)
    {
      // std::cout << "find a way" << std::endl;
      if (p_copy)
      {
        p = p_copy->Copy();
      }
      NS_LOG_LOGIC ("Found unicast destination- calling unicast callback");
      ucb (rtentry, p, header);
      return true; 
    }
  else
    {
      NS_LOG_LOGIC ("Did not find unicast destination- returning false");
      return false; // Let other routing protocols try to handle this
                    // route request.
    }
}

void 
Ipv4DGRRouting::NotifyInterfaceUp (uint32_t i)
{
  NS_LOG_FUNCTION (this << i);
  if (m_respondToInterfaceEvents && Simulator::Now ().GetSeconds () > 0)  // avoid startup events
    {
      DGRRouteManager::DeleteDGRRoutes ();
      DGRRouteManager::BuildDGRRoutingDatabase ();
      DGRRouteManager::InitializeRoutes ();
    }
}

void 
Ipv4DGRRouting::NotifyInterfaceDown (uint32_t i)
{
  NS_LOG_FUNCTION (this << i);
  if (m_respondToInterfaceEvents && Simulator::Now ().GetSeconds () > 0)  // avoid startup events
    {
      DGRRouteManager::DeleteDGRRoutes ();
      DGRRouteManager::BuildDGRRoutingDatabase ();
      DGRRouteManager::InitializeRoutes ();
    }
}

void 
Ipv4DGRRouting::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << interface << address);
  if (m_respondToInterfaceEvents && Simulator::Now ().GetSeconds () > 0)  // avoid startup events
    {
      DGRRouteManager::DeleteDGRRoutes ();
      DGRRouteManager::BuildDGRRoutingDatabase ();
      DGRRouteManager::InitializeRoutes ();
    }
}

void 
Ipv4DGRRouting::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << interface << address);
  if (m_respondToInterfaceEvents && Simulator::Now ().GetSeconds () > 0)  // avoid startup events
    {
      DGRRouteManager::DeleteDGRRoutes ();
      DGRRouteManager::BuildDGRRoutingDatabase ();
      DGRRouteManager::InitializeRoutes ();
    }
}

void 
Ipv4DGRRouting::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_LOG_FUNCTION (this << ipv4);
  NS_ASSERT (!m_ipv4 && ipv4);
  m_ipv4 = ipv4;
}

void
Ipv4DGRRouting::Receive (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  Address sender;
  Ptr<Packet> packet = socket->RecvFrom (sender);
  InetSocketAddress senderAddr = InetSocketAddress::ConvertFrom (sender);
  NS_LOG_INFO ("Received " << *packet << " from " << senderAddr.GetIpv4 () << ":"
                           << senderAddr.GetPort ());
  
  Ipv4Address senderAddress = senderAddr.GetIpv4 ();
  uint32_t senderport = senderAddr.GetPort ();

  if (socket == m_multicastRecvSocket)
    {
      NS_LOG_LOGIC ("Received a packet from the multicast socket");
    }
  else
    {
      NS_LOG_LOGIC ("Received a packet from one of the unicast sockets");
    }

  Ipv4PacketInfoTag interfaceInfo;
  if (!packet->RemovePacketTag (interfaceInfo))
    {
      NS_ABORT_MSG ("No incoming interface on This message, aborting,");
    }
  uint32_t incomingIf = interfaceInfo.GetRecvIf ();
  Ptr<Node> node = this->GetObject<Node> ();
  Ptr<NetDevice> dev = node->GetDevice (incomingIf);
  uint32_t ipInterfaceIndex = m_ipv4->GetInterfaceForDevice (dev);

  SocketIpTtlTag hoplimitTag;
  if (!packet->RemovePacketTag (hoplimitTag))
  {
    NS_ABORT_MSG ("No incoming Hop count on message, aborting");
  }
  uint8_t hopLimit = hoplimitTag.GetTtl ();
  
  int32_t interfaceForAddress = m_ipv4->GetInterfaceForAddress (senderAddress);
  if (interfaceForAddress != -1)
    {
      NS_LOG_LOGIC ("Ignoring a packet sent by myself.");
      return ;
    }

  DgrHeader hdr;
  packet->RemoveHeader (hdr);
  if (hdr.GetCommand () == DgrHeader::RESPONSE)
    {
      NS_LOG_LOGIC ("The message is a Response from " << senderAddr.GetIpv4 () << ":"
                                                      << senderAddr.GetPort ());
      HandleResponses (hdr, senderAddress, ipInterfaceIndex, hopLimit);

    }
  else if (hdr.GetCommand () == DgrHeader::REQUEST)
    {
      NS_LOG_LOGIC ("This message is a Request from " << senderAddr.GetIpv4 () << ":"
                                                      << senderAddr.GetPort ());
      HandleRequests (hdr, senderAddress, senderPort, ipInterfaceIndex, hopLimit);
    }
  else
    {
      NS_LOG_LOGIC ("Ignoring message with unknown command: " << int(hdr.GetCommand ()));
    }
}

void
Ipv4DGRRouting::SendTriggeredNeighborStatusUpdate ()
{
  NS_LOG_FUNCTION (this);

  if (m_nextTriggeredUpdate.IsRunning ())
    {
      NS_LOG_LOGIC ("Skipping Triggered Update due to cooldown.");
      return;
    }

  Time delay = Seconds (m_rand->GetValue (m_minTriggeredUpdateDelay.GetSeconds (),
                                         m_maxTriggeredUpdateDelay.GetSeconds ()));
  m_nextTriggeredUpdate = Simulator::Schedule (delay, &Ipv4DGRRouting::DoSendNeighborStatusUpdate, this, false);
}

void
Ipv4DGRRouting::SendUnsolicitedRouteUpdate ()
{
  NS_LOG_FUNCTION (this);
  if (m_nextTriggeredUpdate.IsRunning ())
    {
      m_nextTriggeredUpdate.Cancel ();
    }
  
  DoSendNeighborStatusUpdate (true);
  Time delay = m_unsolicitedUpdate + Seconds (m_rand->GetValue (0, 0.5 * m_unsolicitedUpdate.GetSeconds ()));
  m_nextUnsolicitedUpdate = Simulator::Schedule (delay, &Ipv4DGRRouting::SendUnsolicitedRouteUpdate, this);
}

void
Ipv4DGRRouting::DoSendNeighborStatusUpdate (bool periodic)
{
  NS_LOG_FUNCTION (this << (periodic ? " periodic" : " triggered"));

  for (SocketListI iter = m_unicastSocketList.begin (); iter != m_unicastSocketList.end (); iter ++)
    {
      uint32_t interface = iter->second;

      if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
        {
          uint16_t mtu = m_ipv4->GetMtu (interface);
          uint16_t maxNse = (mtu - Ipv4Header().GetSerializedSize () -
                             UdpHeader ().GetSerializedSize () - DgrHeader ().GetSerializedSize ())/
                            DgrNse ().GetSerializedSize ();
          
          Ptr<Packet> p = Create<Packet> ();
          SocketIpTtlTag ttlTag;
          ttlTag.SetTtl (1);
          p->AddPacketTag (ttlTag);

          DgrHeader hdr;
          hdr.SetCommand (DgrHeader::RESPONSE);
          
          // Find the Status of every netdevice and put it in
          // TODO: Finish this function when finish the NSE definiation
          for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i ++)
            {
              if (!m_ipv4->IsUp (i)) continue;

              // get the device
              Ptr<NetDevice> dev = m_ipv4->GetNetDevice (i);
              // get the queue disc on devic
              Ptr<QueueDisc> disc = m_ipv4->GetObject<Node> ()->
                                    GetObject<TrafficControlLayer> ()->
                                    GetRootQueueDiscOnDevice (dev);
              Ptr<DGRv2QueueDisc> qdisc = DynamicCast <DGRv2QueueDisc> (disc);
              DgrNse nse = qdisc->GetQueueStatus ();
              hdr.AddNse (nse);
              if (hdr.GetNseNumber () == maxNse)
                {
                  p->AddHeader (hdr);
                  NS_LOG_DEBUG ("SendTo: " << *p);
                  iter->first->SendTo (p, 0, InetSocketAddress (senderAddress, port)); // Todo : defind the port for DGR routing
                  p->RemoveHeader (hdr);
                  hdr.ClearNses ();
                }
            }
            if (hdr.GetNseNumber () > 0)
              {
                p->AddHeader (hdr);
                NS_LOG_DEBUG ("SendTo: " << *p);
                iter->first->SendTo(p, 0, InetSocketAddress (senderAddress, Port)); // Todo: Defined the RIP port
              }
        }
    }
}

void
Ipv4DGRRouting::SendNeighborStatusRequest ()
{
  NS_LOG_FUNCTION (this);

  Ptr<Packet> p = Create<Packet> ();
  SocketIpTtlTag ttlTag;
  p->RemovePacketTag (ttlTag);
  ttlTag.SetTtl (1);
  p->AddPacketTag (ttlTag);

  DgrHeader hdr;
  hdr.SetCommand (DgrHeader::REQUEST);

  DgrNse nse;
  nse.SetInterface ();
  nse.SetQueueSize ();

  hdr.AddNse (nse);
  p->AddHeader (hdr);

  for (SocketListI iter = m_unicastSocketList.begin (); iter != m_unicastSocketList.end (); iter ++)
    {
      uint32_t interface = iter->second;
      if (m_interfaceExclusions.find (interface) == m_interfaceExclusions.end ())
        {
          NS_LOG_DEBUG ("SendTo: " << *p);
          iter->first->SendTo (p, 0, InetSocketAddress ( ipadd, port));
        }
    }

}

void
Ipv4DGRRouting::HandleResponses (DgrHeader hdr,
                                Ipv4Address senderAddress,
                                uint32_t incomingInterface,
                                uint8_t hopLimit)
{
  NS_LOG_FUNCTION (this << senderAddress << incomingInterface << int(hopLimit) << hdr);
  if (m_interfaceExclusions.find (incomingInterface) != m_interfaceExclusions.end ())
    {
      NS_LOG_LOGIC ("Ignoring an update message from an excluded interface: " << incomingInterface);
      return;
    }
  std::list<DgrNse> nses = hdr.GetNseList ();

  // process NSEs store the to the NSDB
  

}

void
Ipv4DGRRouting::HandleRequests (DgrHeader hdr,
                                Ipv4Address senderAddress,
                                uint16_t senderPort,
                                uint32_t incomingInterface,
                                uint8_t hopLimit)
{
  NS_LOG_FUNCTION (this << senderAddress << senderPort 
                        << incomingInterface << hopLimit << hdr);
  std::list<DgrNse> nses = hdr.GetNseList ();
  if (nses.empty ())
    {
      return;
    }
  // check if it's a request for a neighbor status info from a neighbor
  if (nses.size () == 1)
    {
      if (nses.begin ()->GetInterface () == 0 &&
          nses.begin ()->GetQueueSize () == 0)
        {
          if (m_interfaceExclusions.find (incomingInterface) == m_interfaceExclusions.end ())
            {
              // We use one of the sending sockets, as they're bound to the right interface
              // and the local address might be used on different interfaces.
              Ptr<Socket> sendingSocket;
              for (SocketListI iter = m_unicastSocketList.begin ();
                  iter != m_unicastSocketList.end ();
                  iter ++)
                {
                  if (iter->second == incomingInterface)
                    {
                      sendingSocket = iter->first;
                    }
                }
              NS_ASSERT_MSG (sendingSocket,
                             "HandleRequest - Impossible to find a socket to send the reply");
              
              uint16_t mtu = m_ipv4->GetMtu (incomingInterface);
              uint16_t maxNse = (mtu - Ipv4Header().GetSerializedSize () -
                                 UdpHeader ().GetSerializedSize () - DgrHeader ().GetSerializedSize ())/DgrNse ().GetSerializedSize ();
              
              Ptr<Packet> p = Create<Packet> ();
              SocketIpTtlTag ttlTag;
              p->RemovePacketTag (ttlTag);
              p->AddPacketTag (ttlTag);

              // Serialize the current Device Status
              DgrHeader hdr;
              hdr.SetCommand (DgrHeader::RESPONSE);
              for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i ++)
                {
                  if (!m_ipv4->IsUp (i)) continue;

                  // get the device
                  Ptr<NetDevice> dev = m_ipv4->GetNetDevice (i);
                  // get the queue disc on devic
                  Ptr<QueueDisc> disc = m_ipv4->GetObject<Node> ()->
                                        GetObject<TrafficControlLayer> ()->
                                        GetRootQueueDiscOnDevice (dev);
                  Ptr<QueueDisc> disc = DynamicCast <DGRv2QueueDisc> (disc);
                  disc->GetCurrentStatus ();
                  DgrNse nse; // TODO: Load neighbor status information
                  hdr.AddNse (nse);
                  if (hdr.GetNseNumber () == maxNse)
                    {
                      p->AddHeader (hdr);
                      NS_LOG_DEBUG ("SendTo: " << *p);
                      sendingSocket->SendTo (p, 0, InetSocketAddress (senderAddress, port)); // Todo : defind the port for DGR routing
                      p->RemoveHeader (hdr);
                      hdr.ClearNses ();
                    }
                }
                if (hdr.GetNseNumber () > 0)
                  {
                    p->AddHeader (hdr);
                    NS_LOG_DEBUG ("SendTo: " << *p);
                    sendingSocket->SendTo(p, 0, InetSocketAddress (senderAddress, Port)); // Todo: Defined the RIP port
                  }
            }
          
        }

    }
}

} // namespace ns3
