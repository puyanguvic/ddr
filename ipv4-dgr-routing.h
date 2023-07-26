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
//

// #ifndef IPV4_GLOBAL_ROUTING_H
// #define IPV4_GLOBAL_ROUTING_H

#ifndef IPV4_DGR_ROUTING_H
#define IPV4_DGR_ROUTING_H

#include <list>
#include <map>
#include <stdint.h>
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ptr.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/random-variable-stream.h"
#include "ns3/nstime.h"
#include "dgr-route-manager-impl.h"
#include "dgr-header.h"
#include "ipv4-dgr-routing-table-entry.h"
#include "neighbor-status-database.h"

namespace ns3 {

class Packet;
class NetDevice;
class Ipv4Interface;
class Ipv4Address;
class Ipv4Header;
class Ipv4DGRRoutingTableEntry;
class Ipv4MulticastRoutingTableEntry;
class Node;

typedef enum
{
  NONE,
  KSHORT,
  DGR,
  DDR
}RouteSelectMode_t;

/**
 * \ingroup ipv4
 *
 * \brief Global routing protocol for IPv4 stacks.
 *
 * In ns-3 we have the concept of a pluggable routing protocol.  Routing
 * protocols are added to a list maintained by the Ipv4L3Protocol.  Every 
 * stack gets one routing protocol for free -- the Ipv4StaticRouting routing
 * protocol is added in the constructor of the Ipv4L3Protocol (this is the 
 * piece of code that implements the functionality of the IP layer).
 *
 * As an option to running a dynamic routing protocol, a GlobalRouteManager
 * object has been created to allow users to build routes for all participating
 * nodes.  One can think of this object as a "routing oracle"; it has
 * an omniscient view of the topology, and can construct shortest path
 * routes between all pairs of nodes.  These routes must be stored 
 * somewhere in the node, so therefore this class Ipv4GlobalRouting
 * is used as one of the pluggable routing protocols.  It is kept distinct
 * from Ipv4StaticRouting because these routes may be dynamically cleared
 * and rebuilt in the middle of the simulation, while manually entered
 * routes into the Ipv4StaticRouting may need to be kept distinct.
 *
 * This class deals with Ipv4 unicast routes only.
 *
 * \see Ipv4RoutingProtocol
 * \see GlobalRouteManager
 */
class Ipv4DGRRouting : public Ipv4RoutingProtocol
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief Construct an empty Ipv4GlobalRouting routing protocol,
   *
   * The Ipv4GlobalRouting class supports host and network unicast routes.
   * This method initializes the lists containing these routes to empty.
   *
   * \see Ipv4GlobalRouting
   */
  Ipv4DGRRouting ();
  ~Ipv4DGRRouting ();

  // These methods inherited from base class
  Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);

  bool RouteInput  (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                            UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                            LocalDeliverCallback lcb, ErrorCallback ecb);
  void NotifyInterfaceUp (uint32_t interface);
  void NotifyInterfaceDown (uint32_t interface);
  void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
  void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
  void SetIpv4 (Ptr<Ipv4> ipv4);
  void PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;

  /**
   * \brief Add a host route to the global routing table.
   *
   * \param dest The Ipv4Address destination for this route.
   * \param nextHop The Ipv4Address of the next hop in the route.
   * \param interface The network interface index used to send packets to the
   * destination.
   *
   * \see Ipv4Address
   */
  void AddHostRouteTo (Ipv4Address dest, 
                       Ipv4Address nextHop, 
                       uint32_t interface);
  /**
   * \brief Add a host route to the global routing table.
   *
   * \param dest The Ipv4Address destination for this route.
   * \param interface The network interface index used to send packets to the
   * destination.
   *
   * \see Ipv4Address
   */
  void AddHostRouteTo (Ipv4Address dest, 
                       uint32_t interface);

  /**
   * \brief Add a host route to the global routing table with the distance 
   * between root and destination
   * \param dest The Ipv4Address destination for this route.
   * \param nextHop The next hop Ipv4Address
   * \param interface The network interface index used to send packets to the
   *  destination
   * \param distance The distance between root and destination
  */
  void AddHostRouteTo (Ipv4Address dest,
                       Ipv4Address nextHop,
                       uint32_t interface,
                       uint32_t nextInterface,
                       uint32_t distance);

  /**
   * \brief Add a network route to the global routing table.
   *
   * \param network The Ipv4Address network for this route.
   * \param networkMask The Ipv4Mask to extract the network.
   * \param nextHop The next hop in the route to the destination network.
   * \param interface The network interface index used to send packets to the
   * destination.
   *
   * \see Ipv4Address
   */
  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          Ipv4Address nextHop, 
                          uint32_t interface);

  /**
   * \brief Add a network route to the global routing table.
   *
   * \param network The Ipv4Address network for this route.
   * \param networkMask The Ipv4Mask to extract the network.
   * \param interface The network interface index used to send packets to the
   * destination.
   *
   * \see Ipv4Address
   */
  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          uint32_t interface);

  /**
   * \brief Add an external route to the global routing table.
   *
   * \param network The Ipv4Address network for this route.
   * \param networkMask The Ipv4Mask to extract the network.
   * \param nextHop The next hop Ipv4Address
   * \param interface The network interface index used to send packets to the
   * destination.
   */
  void AddASExternalRouteTo (Ipv4Address network,
                             Ipv4Mask networkMask,
                             Ipv4Address nextHop,
                             uint32_t interface);

  /**
   * \brief Get the number of individual unicast routes that have been added
   * to the routing table.
   *
   * \warning The default route counts as one of the routes.
   * \returns the number of routes
   */
  uint32_t GetNRoutes (void) const;

  /**
   * \brief Get a route from the global unicast routing table.
   *
   * Externally, the unicast global routing table appears simply as a table with
   * n entries.  The one subtlety of note is that if a default route has been set
   * it will appear as the zeroth entry in the table.  This means that if you
   * add only a default route, the table will have one entry that can be accessed
   * either by explicitly calling GetDefaultRoute () or by calling GetRoute (0).
   *
   * Similarly, if the default route has been set, calling RemoveRoute (0) will
   * remove the default route.
   *
   * \param i The index (into the routing table) of the route to retrieve.  If
   * the default route has been set, it will occupy index zero.
   * \return If route is set, a pointer to that Ipv4RoutingTableEntry is returned, otherwise
   * a zero pointer is returned.
   *
   * \see Ipv4RoutingTableEntry
   * \see Ipv4GlobalRouting::RemoveRoute
   */
  Ipv4DGRRoutingTableEntry *GetRoute (uint32_t i) const;

  /**
   * \brief Remove a route from the global unicast routing table.
   *
   * Externally, the unicast global routing table appears simply as a table with
   * n entries.  The one subtlety of note is that if a default route has been set
   * it will appear as the zeroth entry in the table.  This means that if the
   * default route has been set, calling RemoveRoute (0) will remove the
   * default route.
   *
   * \param i The index (into the routing table) of the route to remove.  If
   * the default route has been set, it will occupy index zero.
   *
   * \see Ipv4RoutingTableEntry
   * \see Ipv4GlobalRouting::GetRoute
   * \see Ipv4GlobalRouting::AddRoute
   */
  void RemoveRoute (uint32_t i);


  /**
   * @brief Build the routing database by gathering Link State Advertisements
   * from each node exporting a DGRRouter interface.
   */
  // void BuildDGRNeighbourStateDatabase ();

  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model.  Return the number of streams (possibly zero) that
   * have been assigned.
   *
   * \param stream first stream index to use
   * \return the number of stream indices assigned by this model
   */
  int64_t AssignStreams (int64_t stream);

  // static bool CompareRouteCost(Ipv4DGRRoutingTableEntry* route1, Ipv4DGRRoutingTableEntry* route2);
    /**
   * \brief Lookup in the forwarding table for destination.
   * \param dest destination address
   * \param oif output interface if any (put 0 otherwise)
   * \return Ipv4Route to route the packet to reach dest address
   */
  Ptr<Ipv4Route> LookupUniRoute (Ipv4Address dest, Ptr<NetDevice> oif = 0);
  Ptr<Ipv4Route> LookupDGRRoute (Ipv4Address dest, Ptr<Packet> p, Ptr<const NetDevice> idev = 0); // budget in microsecond
  // Ptr<Ipv4Route> LookupKShortRoute (Ipv4Address dest, Ptr<Packet> p, Ptr<const NetDevice> idev = 0);
  // Ptr<Ipv4Route> LookupDDRRoute (Ipv4Address dest, Ptr<Packet> p, Ptr<const NetDevice> idev = 0);

  /**
   * Start protocol operation
  */
  void DoInitialize () override;
protected:
  /**
   * \brief Dispose this object
  */
  void DoDispose (void) override;


private:
  /// Set to true if packets are randomly routed among ECMP; set to false for using only one route consistently
  bool m_randomEcmpRouting;
  /// Set to true if this interface should respond to interface events by globallly recomputing routes 
  bool m_respondToInterfaceEvents;
  /// A uniform random number generator for randomly routing packets among ECMP 
  Ptr<UniformRandomVariable> m_rand;

  /// container of Ipv4RoutingTableEntry (routes to hosts)
  typedef std::list<Ipv4DGRRoutingTableEntry *> HostRoutes;
  /// const iterator of container of Ipv4RoutingTableEntry (routes to hosts)
  typedef std::list<Ipv4DGRRoutingTableEntry *>::const_iterator HostRoutesCI;
  /// iterator of container of Ipv4RoutingTableEntry (routes to hosts)
  typedef std::list<Ipv4DGRRoutingTableEntry *>::iterator HostRoutesI;

  /// container of Ipv4RoutingTableEntry (routes to networks)
  typedef std::list<Ipv4DGRRoutingTableEntry *> NetworkRoutes;
  /// const iterator of container of Ipv4RoutingTableEntry (routes to networks)
  typedef std::list<Ipv4DGRRoutingTableEntry *>::const_iterator NetworkRoutesCI;
  /// iterator of container of Ipv4RoutingTableEntry (routes to networks)
  typedef std::list<Ipv4DGRRoutingTableEntry *>::iterator NetworkRoutesI;

  /// container of Ipv4RoutingTableEntry (routes to external AS)
  typedef std::list<Ipv4DGRRoutingTableEntry *> ASExternalRoutes;
  /// const iterator of container of Ipv4RoutingTableEntry (routes to external AS)
  typedef std::list<Ipv4DGRRoutingTableEntry *>::const_iterator ASExternalRoutesCI;
  /// iterator of container of Ipv4RoutingTableEntry (routes to external AS)
  typedef std::list<Ipv4DGRRoutingTableEntry *>::iterator ASExternalRoutesI;

  HostRoutes m_hostRoutes;             //!< Routes to hosts
  NetworkRoutes m_networkRoutes;       //!< Routes to networks
  ASExternalRoutes m_ASexternalRoutes; //!< External routes imported
  Ptr<Ipv4> m_ipv4;                    //!< associated IPv4 instance
  RouteSelectMode_t m_routeSelectMode; //!< route select mode

  DgrNSDB m_nsdb; //!< the Neighbor State DataBase (NSDB) of the DGR Rout

  // use a socket list neighbors
  /// One socket per interface, each bound to that interface's address
  /// (reason: for Neighbor status sensing, we need to know on which interface
  /// the messages arrive)
  typedef std::map<Ptr<Socket>, uint32_t> SocketList;
  /// socket list type iterator
  typedef std::map<Ptr<Socket>, uint32_t>::iterator SocketListI;
  /// socket list type const iterator
  typedef std::map<Ptr<Socket>, uint32_t>::const_iterator SocketListCI;

  SocketList  m_unicastSocketList; //!< list of sockets for unicast messages (socket, interface index)
  Ptr<Socket> m_multicastRecvSocket; //!< multicast receive socket

  EventId m_nextUnsolicitedUpdate; //!< Next Unsolicited Update event
  EventId m_nextTriggeredUpdate; //!< Next Triggered Update event

  Time m_unsolicitedUpdate;             //!< Time between two Unsolicited Neighbor State Updates.

  // Time m_startupDelay;            //!< Random delay before protocol startup
  // Time m_minTriggeredUpdateDelay; //!< Min cooldown delay after a Triggered Update.
  // Time m_maxTriggeredUpdateDelay; //!< Max cooldown delay after a Triggered Update.  
  // Time m_unsolicitedUpdate;       //!< time between two Unsolicited Routing Updates.
  // Time m_timeoutDelay;            //!< Delay before invalidating a status


  std::set<uint32_t> m_interfaceExclusions; //!<Set of excluded interfaces
  
  /**
   * Receive an DGR message
   * 
   * \param socket the socket the packet was received from.
  */
  void Receive (Ptr<Socket> socket);

  /**
   * \brief Sending Neighbor Status Updates on all interfaces.
   * \param periodic true for periodic update, else triggered.
  */
  void DoSendNeighborStatusUpdate (bool periodic);

  // /**
  //  * \brief Send Neighbor Status Request on all interfaces
  // */
  // void SendNeighborStatusRequest ();

  /**
   * \brief Send Triggered Routing Updates on all interfaces.
  */
  void SendTriggeredNeighborStatusUpdate ();

  /**
   * \brief Send Unsolicited neighbor status information Updates on all interfaces.
  */
  void SendUnsolicitedUpdate ();

  // /**
  //  * \brief Handle DGR requests.
  //  * 
  //  * \param hdr message header (Including NSEs)
  //  * \param senderAddress sender address
  //  * \param senderPort sender port
  //  * \param incomingInterface incoming interface
  //  * \param hopLimit packet's hop limit
  // */
  // void HandleRequests (DgrHeader hdr,
  //                     Ipv4Address senderAddress,
  //                     uint16_t senderPort,
  //                     uint32_t incomingInterface,
  //                     uint8_t hopLimit);

  /**
   * \brief Handle DGR responses.
   * 
   * \param hdr message header (including NSEs)
   * \param senderAddress sender address
   * \param incomingInterface incoming interface
   * \param hopLimit packet's hop limit
  */
  void HandleResponses (DgrHeader hdr,
                       Ipv4Address senderAddress,
                       uint32_t incomingInterface,
                       uint8_t hopLimit);

  // Ptr<OutputStreamWrapper> m_outStream = Create<OutputStreamWrapper>
  // ("queueStatusErr.txt", std::ios::out);

  bool m_initialized; //!< flag to allow socket's late-creation.
};


} // Namespace ns3

#endif /* IPV4_DGR_ROUTING_H */
