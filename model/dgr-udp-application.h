/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DGR_UDP_APPLICATION_H
#define DGR_UDP_APPLICATION_H


#include <iostream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"

namespace ns3 {

class DGRUdpApplication : public Application
{

public:

  DGRUdpApplication ();
  ~DGRUdpApplication() override;

  /**
   * Register this type
   * \return The TypeId.
  */
  static TypeId GetTypeId (void);
  
  /**
   * Setup the socket.
   * \param socket The socket
   * \param sinkAddress The destination address.
   * \param packetSize The packet size to transmit.
   * \param nPackets The number of packets to transmit
   * \param dataRate The data rate to use.
   * \param budget The packet budget time in millisecond
   * \param flag The packet flag 
  */
  void Setup (Ptr<Socket> socket,
              Address sinkAddress,
              uint32_t packetSize,
              uint32_t nPackets,
              DataRate dataRate,
              uint32_t budget,
              bool flag = false);
  /**
   * Setup the socket.
   * \param socket The socket
   * \param sinkAddress The destination address.
   * \param packetSize The packet size to transmit.
   * \param nPackets The number of packets to transmit
   * \param dataRate The data rate to use.
   * \param flag The packet flag 
  */
  void Setup (Ptr<Socket> socket,
              Address sinkAddress,
              uint32_t packetSize,
              uint32_t nPackets,
              DataRate dataRate,
              bool flag = false);

  /**
   * Update the sending rate.
   * \param newDataRate The new DataRate
  */
  void ChangeRate (DataRate newDataRate);

  /**
   * Set the priority tag.
   * \param priority 
  */
  void SetPriority (bool priority);

private:

  void StartApplication (void) override;
  void StopApplication (void) override;

  /// Schedule a new transmission
  void ScheduleTx ();
  /// Send a packet  
  void SendPacket ();

  Ptr<Socket> m_socket;   //!< The transmission socket
  Address m_peer;         //!< The destination address
  uint32_t m_packetSize;  //!< The packet size
  uint32_t m_nPackets;    //!< The number of packet to send.
  DataRate m_dataRate;    //!< The data rate to use.
  EventId m_sendEvent;    //!< Send event.
  bool m_running;         //!< True if the application is running
  uint32_t m_packetSent;  //!< The number of packets sent.
  uint32_t m_budget;      //!< The budget time in millisecond
  bool m_flag;            //!< The packet flag
  bool m_vbr;             //!< true meanse VBR
  bool m_priority;        //!< priority
};

} // namespace ns3

#endif /* DGR_APPLICATION_H */

