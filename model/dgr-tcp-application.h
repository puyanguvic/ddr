/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef DGR_TCP_APPLICATION_H
#define DGR_TCP_APPLICATION_H

#include "ns3/address.h"
#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/seq-ts-size-header.h"

namespace ns3 {

class Address;
class Socket;

/**
 * \ingroup DGR-routing
 * This moduel comes from bulk Send Application
 * TCP application need to enable packet tags in TcpRxBuffer
 */
class DGRTcpApplication : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  DGRTcpApplication ();

  virtual ~DGRTcpApplication ();

  /**
   * \brief Set the upper bound for the total number of bytes to send.
   * \param maxBytes the upper bound of bytes to send
   */
  void SetMaxBytes (uint64_t maxBytes);

  /**
   * \brief Set the flag of packet used in the sink to help monitor the packet delay
   * \param flag the flag
  */
  void SetFlag (bool flag);

  /**
   * \brief Get the socket this application is attached to.
   * \return pointer to associated socket
   */
  Ptr<Socket> GetSocket (void) const;
  void Setup (Ptr<Socket> socket, Address sinkAddress, uint64_t maxBytes, uint32_t budget, bool flag);
  void Setup (Ptr<Socket> socket, Address sinkAddress, uint64_t maxBytes, bool flag);

protected:
  virtual void DoDispose (void);
private:
  // inherited from Application base class.
  virtual void StartApplication (void);    // Called at time specified by Start
  virtual void StopApplication (void);     // Called at time specified by Stop

  /**
   * \brief Send data until the L4 transmission buffer is full.
   * \param from From address
   * \param to To address
   */
  void SendData (const Address &from, const Address &to);

  Ptr<Socket>     m_socket;       //!< Associated socket
  Address         m_peer;         //!< Peer address
  Address         m_local;        //!< Local address to bind to
  bool            m_connected;    //!< True if connected
  uint32_t        m_sendSize;     //!< Size of data to send each time
  uint64_t        m_maxBytes;     //!< Limit total number of bytes sent
  uint64_t        m_totBytes;     //!< Total bytes sent so far
  TypeId          m_tid;          //!< The type of protocol to use.
  uint32_t        m_seq {0};      //!< Sequence
  Ptr<Packet>     m_unsentPacket; //!< Variable to cache unsent packet
  uint32_t        m_budget;       //!< Budget time in ms
  bool            m_flag {false}; //!< flag for test
  // bool            m_enableSeqTsSizeHeader {false}; //!< Enable or disable the SeqTsSizeHeader

  /// Traced Callback: sent packets
  TracedCallback<Ptr<const Packet> > m_txTrace;

private:
  /**
   * \brief Connection Succeeded (called by Socket through a callback)
   * \param socket the connected socket
   */
  void ConnectionSucceeded (Ptr<Socket> socket);
  /**
   * \brief Connection Failed (called by Socket through a callback)
   * \param socket the connected socket
   */
  void ConnectionFailed (Ptr<Socket> socket);
  /**
   * \brief Send more data as soon as some has been transmitted.
   */
  void DataSend (Ptr<Socket>, uint32_t); // for socket's SetSendCallback
};

} // namespace ns3

#endif /* DGR_TCP_SEND_APPLICATION_H */
