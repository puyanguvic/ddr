/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/address.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/boolean.h"
#include "ns3/network-module.h"
#include "dgr-tcp-application.h"
#include "dgr-tags.h"

#define MAX_UINT_32 0xffffffff

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DGRTcpApplication");

NS_OBJECT_ENSURE_REGISTERED (DGRTcpApplication);

TypeId
DGRTcpApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DGRTcpApplication")
    .SetParent<Application> ()
    .SetGroupName("Applications") 
    .AddConstructor<DGRTcpApplication> ()
    .AddAttribute ("SendSize", "The amount of data to send each time.",
                   UintegerValue (512),
                   MakeUintegerAccessor (&DGRTcpApplication::m_sendSize),
                   MakeUintegerChecker<uint32_t> (1))
    .AddAttribute ("Remote", "The address of the destination",
                   AddressValue (),
                   MakeAddressAccessor (&DGRTcpApplication::m_peer),
                   MakeAddressChecker ())
    .AddAttribute ("Local",
                   "The Address on which to bind the socket. If not set, it is generated automatically.",
                   AddressValue (),
                   MakeAddressAccessor (&DGRTcpApplication::m_local),
                   MakeAddressChecker ())
    .AddAttribute ("MaxBytes",
                   "The total number of bytes to send. "
                   "Once these bytes are sent, "
                   "no data  is sent again. The value zero means "
                   "that there is no limit.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&DGRTcpApplication::m_maxBytes),
                   MakeUintegerChecker<uint64_t> ())
    .AddAttribute ("Protocol", "The type of protocol to use.",
                   TypeIdValue (TcpSocketFactory::GetTypeId ()),
                   MakeTypeIdAccessor (&DGRTcpApplication::m_tid),
                   MakeTypeIdChecker ())
    .AddAttribute ("Budget", "The budget of certain packet.",
                   UintegerValue (MAX_UINT_32),
                   MakeUintegerAccessor (&DGRTcpApplication::m_budget),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("EnableFlag",
                   "EnableFalg in DGR header for test",
                   BooleanValue (false),
                   MakeBooleanAccessor (&DGRTcpApplication::m_flag),
                   MakeBooleanChecker ())
    .AddTraceSource ("Tx", "A new packet is sent",
                     MakeTraceSourceAccessor (&DGRTcpApplication::m_txTrace),
                     "ns3::Packet::TracedCallback")
  ;
  return tid;
}


DGRTcpApplication::DGRTcpApplication ()
  : m_socket (0),
    m_connected (false),
    m_totBytes (0),
    m_unsentPacket (0),
    m_budget (MAX_UINT_32),
    m_flag (false)
{
  NS_LOG_FUNCTION (this);
}

DGRTcpApplication::~DGRTcpApplication ()
{
  NS_LOG_FUNCTION (this);
}

void
DGRTcpApplication::SetMaxBytes (uint64_t maxBytes)
{
  NS_LOG_FUNCTION (this << maxBytes);
  m_maxBytes = maxBytes;
}

void 
DGRTcpApplication::SetFlag (bool flag)
{
  m_flag = flag;
}

void
DGRTcpApplication::Setup (Ptr<Socket> socket, Address sinkAddress, uint64_t maxBytes, uint32_t budget, bool flag)
{
  m_socket = socket;
  m_peer = sinkAddress;
  m_maxBytes = maxBytes;
  m_budget = budget;
  m_flag = flag;
}

void
DGRTcpApplication::Setup (Ptr<Socket> socket, Address sinkAddress, uint64_t maxBytes, bool flag)
{
  m_socket = socket;
  m_peer = sinkAddress;
  m_maxBytes = maxBytes;
  m_flag = flag;
}

Ptr<Socket>
DGRTcpApplication::GetSocket (void) const
{
  NS_LOG_FUNCTION (this);
  return m_socket;
}

void
DGRTcpApplication::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  m_socket = 0;
  m_unsentPacket = 0;
  // chain up
  Application::DoDispose ();
}

// Application Methods
void DGRTcpApplication::StartApplication (void) // Called at time specified by Start
{
  NS_LOG_FUNCTION (this);
  Address from;
  // Create the socket if not already
  if (!m_socket)
    {
      m_socket = Socket::CreateSocket (GetNode (), m_tid);
      int ret = -1;

      // Fatal error if socket type is not NS3_SOCK_STREAM or NS3_SOCK_SEQPACKET
      if (m_socket->GetSocketType () != Socket::NS3_SOCK_STREAM &&
          m_socket->GetSocketType () != Socket::NS3_SOCK_SEQPACKET)
        {
          NS_FATAL_ERROR ("Using BulkSend with an incompatible socket type. "
                          "BulkSend requires SOCK_STREAM or SOCK_SEQPACKET. "
                          "In other words, use TCP instead of UDP.");
        }

      if (! m_local.IsInvalid())
        {
          NS_ABORT_MSG_IF ((Inet6SocketAddress::IsMatchingType (m_peer) && InetSocketAddress::IsMatchingType (m_local)) ||
                           (InetSocketAddress::IsMatchingType (m_peer) && Inet6SocketAddress::IsMatchingType (m_local)),
                           "Incompatible peer and local address IP version");
          ret = m_socket->Bind (m_local);
        }
      else
        {
          if (Inet6SocketAddress::IsMatchingType (m_peer))
            {
              ret = m_socket->Bind6 ();
            }
          else if (InetSocketAddress::IsMatchingType (m_peer))
            {
              ret = m_socket->Bind ();
            }
        }

      if (ret == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }

      m_socket->Connect (m_peer);
      m_socket->ShutdownRecv ();
      m_socket->SetConnectCallback (
        MakeCallback (&DGRTcpApplication::ConnectionSucceeded, this),
        MakeCallback (&DGRTcpApplication::ConnectionFailed, this));
      m_socket->SetSendCallback (
        MakeCallback (&DGRTcpApplication::DataSend, this));
    }
  if (m_connected)
    {
      m_socket->GetSockName (from);
      SendData (from, m_peer);
    }
}

void DGRTcpApplication::StopApplication (void) // Called at time specified by Stop
{
  NS_LOG_FUNCTION (this);

  if (m_socket)
    {
      m_socket->Close ();
      m_connected = false;
    }
  else
    {
      NS_LOG_WARN ("DGRTcpApplication found null socket to close in StopApplication");
    }
}


// Private helpers

void DGRTcpApplication::SendData (const Address &from, const Address &to)
{
  NS_LOG_FUNCTION (this);

  while (m_maxBytes == 0 || m_totBytes < m_maxBytes)
    { // Time to send more

      // uint64_t to allow the comparison later.
      // the result is in a uint32_t range anyway, because
      // m_sendSize is uint32_t.
      uint64_t toSend = m_sendSize;
      // Make sure we don't send too many
      if (m_maxBytes > 0)
        {
          toSend = std::min (toSend, m_maxBytes - m_totBytes);
        }

      NS_LOG_LOGIC ("sending packet at " << Simulator::Now ());
      Ptr<Packet> packet;

      TimestampTag txTimeTag;
      FlagTag flagTag;
      BudgetTag budgetTag;
      txTimeTag.SetTimestamp (Simulator::Now ());
      flagTag.SetFlag (m_flag);
      // flagTag.Print (std::cout);

      if (m_unsentPacket)
        {
          packet = m_unsentPacket;
          toSend = packet->GetSize ();
        }
      else
        {
          packet = Create<Packet> (toSend);
          packet->AddPacketTag (txTimeTag);
          packet->AddPacketTag (flagTag);
          if (m_budget != MAX_UINT_32)
            {
              budgetTag.SetBudget (m_budget);
              packet->AddPacketTag (budgetTag);
            }
        }
      int actual = m_socket->Send (packet);
      if ((unsigned) actual == toSend)
        {
          m_totBytes += actual;
          m_txTrace (packet);
          m_unsentPacket = 0;
        }
      else if (actual == -1)
        {
          // We exit this loop when actual < toSend as the send side
          // buffer is full. The "DataSent" callback will pop when
          // some buffer space has freed up.
          NS_LOG_DEBUG ("Unable to send packet; caching for later attempt");
          m_unsentPacket = packet;
          break;
        }
      else if (actual > 0 && (unsigned) actual < toSend)
        {
          // A Linux socket (non-blocking, such as in DCE) may return
          // a quantity less than the packet size.  Split the packet
          // into two, trace the sent packet, save the unsent packet
          NS_LOG_DEBUG ("Packet size: " << packet->GetSize () << "; sent: " << actual << "; fragment saved: " << toSend - (unsigned) actual);
          Ptr<Packet> sent = packet->CreateFragment (0, actual);
          Ptr<Packet> unsent = packet->CreateFragment (actual, (toSend - (unsigned) actual));
          m_totBytes += actual;
          m_txTrace (sent);
          m_unsentPacket = unsent;
          break;
        }
      else
        {
          NS_FATAL_ERROR ("Unexpected return value from m_socket->Send ()");
        }
    }
  // Check if time to close (all sent)
  if (m_totBytes == m_maxBytes && m_connected)
    {
      m_socket->Close ();
      m_connected = false;
    }
}

void DGRTcpApplication::ConnectionSucceeded (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  NS_LOG_LOGIC ("DGRTcpApplication Connection succeeded");
  m_connected = true;
  Address from, to;
  socket->GetSockName (from);
  socket->GetPeerName (to);
  SendData (from, to);
}

void DGRTcpApplication::ConnectionFailed (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  NS_LOG_LOGIC ("DGRTcpApplication, Connection Failed");
}

void DGRTcpApplication::DataSend (Ptr<Socket> socket, uint32_t)
{
  NS_LOG_FUNCTION (this);

  if (m_connected)
    { // Only send new data if the connection has completed
      Address from, to;
      socket->GetSockName (from);
      socket->GetPeerName (to);
      SendData (from, to);
    }
}

} // Namespace ns3
