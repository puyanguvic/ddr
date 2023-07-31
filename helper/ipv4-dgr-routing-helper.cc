/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "ipv4-dgr-routing-helper.h"
#include "ns3/dgr-router-interface.h"
#include "ns3/ipv4-dgr-routing.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/traffic-control-layer.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DGRRoutingHelper");

Ipv4DGRRoutingHelper::Ipv4DGRRoutingHelper ()
{
}

Ipv4DGRRoutingHelper::Ipv4DGRRoutingHelper (const Ipv4DGRRoutingHelper &o)
{
}

Ipv4DGRRoutingHelper*
Ipv4DGRRoutingHelper::Copy (void) const
{
  return new Ipv4DGRRoutingHelper (*this);
}

Ptr<Ipv4RoutingProtocol>
Ipv4DGRRoutingHelper::Create (Ptr<Node> node) const
{
  NS_LOG_LOGIC ("Adding DGRRouter interface to node " <<
                node->GetId ());
  // install DGRv2 Queue to netdevices

  // install DGR router to node.
  Ptr<DGRRouter> dgrRouter = CreateObject<DGRRouter> ();
  node->AggregateObject (dgrRouter);

  NS_LOG_LOGIC ("Adding DGRRouting Protocol to node " << node->GetId ());
  Ptr<Ipv4DGRRouting> dgrRouting = CreateObject<Ipv4DGRRouting> ();
  dgrRouter->SetRoutingProtocol (dgrRouting);

  return dgrRouting;
}

void 
Ipv4DGRRoutingHelper::PopulateRoutingTables (void)
{
  std::clock_t t;
  t = clock();
  DGRRouteManager::BuildDGRRoutingDatabase ();
  DGRRouteManager::InitializeRoutes ();
  
  t = clock() - t;
  uint32_t time_init_ms = 1000000.0 * t / CLOCKS_PER_SEC;
  std::cout << "CPU time used for DGR Init: " << time_init_ms << " ms\n";
}
void 
Ipv4DGRRoutingHelper::RecomputeRoutingTables (void)
{
  DGRRouteManager::DeleteDGRRoutes ();
  DGRRouteManager::BuildDGRRoutingDatabase ();
  DGRRouteManager::InitializeRoutes ();
}


QueueDiscContainer
Ipv4DGRRoutingHelper::Install (Ptr<Node> node)
{
  NetDeviceContainer container;
  for (uint32_t i = 0; i < node->GetNDevices (); i ++)
    {
      container.Add (node->GetDevice (i));
    }
  return Install (container);
}

QueueDiscContainer 
Ipv4DGRRoutingHelper::Install(NetDeviceContainer c)
{
  QueueDiscContainer container;
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      container.Add (Install (*i));
    }
  return container;
}

QueueDiscContainer
Ipv4DGRRoutingHelper::Install (Ptr<NetDevice> d)
{
  QueueDiscContainer container;
  // A TrafficControlLayer object is aggregated by the InternetStackHelper, but check
  // anyway because a queue disc has no effect without a TrafficControlLayer object
  Ptr<TrafficControlLayer> tc = d->GetNode ()->GetObject<TrafficControlLayer> ();
  NS_ASSERT (tc);

  // Generate the DGRv2Qeueu Object
  ObjectFactory queueFactory;
  queueFactory.SetTypeId ("ns3::DGRv2QueueDisc");
  Ptr<DGRv2QueueDisc> qdisc = queueFactory.Create<DGRv2QueueDisc> ();
  tc->SetRootQueueDiscOnDevice (d, qdisc);
  container.Add (qdisc);
  return container;
}

} // namespace ns3
