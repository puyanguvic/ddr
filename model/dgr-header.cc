/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "dgr-header.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DgrHeader");


//----------------------------------------------------------------------
//-- DgrNse
//------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (DgrNse);

DgrNse::DgrNse ()
    : m_iface (0),
      m_qSize (0)
{
}

TypeId
DgrNse::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::DgrNse")
                          .SetParent<Header>()
                          .SetGroupName ("dgr")
                          .AddConstructor<DgrNse> ();
  return tid;
}

TypeId
DgrNse::GetInstanceTypeId () const
{
  return GetTypeId ();
}

void
DgrNse::Print (std::ostream& os) const
{
  os << "Iface: " << m_iface << ", QueueSize: " << m_qSize;
}

uint32_t
DgrNse::GetSerializedSize () const
{
  return 4 + 4;
}

void
DgrNse::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteHtonU32 (m_iface);
  i.WriteHtonU32 (m_qSize);
}

uint32_t
DgrNse::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_iface = i.ReadNtohU32 ();
  m_qSize = i.ReadNtohU32 ();
  return GetSerializedSize ();
}

void
DgrNse::SetInterface (uint32_t iface)
{
  m_iface = iface;
}

uint32_t
DgrNse::GetInterface () const
{
  return m_iface;
}

void
DgrNse::SetQueueSize (uint32_t qSize)
{
  m_qSize = qSize;
}

uint32_t
DgrNse::GetQueueSize () const
{
  return m_qSize;
}

std::ostream&
operator<< (std::ostream& os, const DgrNse& h)
{
  h.Print (os);
  return os;
}

//----------------------------------------------------------------------
//-- DgrHeader
//------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (DgrHeader);

DgrHeader::DgrHeader ()
    : m_command (1)
{
}

TypeId
DgrHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::DgrHeader")
                          .SetParent<Header> ()
                          .SetGroupName ("dgr")
                          .AddConstructor<DgrHeader> ();
  return tid;
}

TypeId
DgrHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

void
DgrHeader::Print (std::ostream &os) const
{
  os << "command " << int (m_command);
  for (std::list<DgrNse>::const_iterator iter = m_nseList.begin ();
       iter != m_nseList.end ();
       iter ++)
    {
      os << " | ";
      iter->Print (os);
    }
}

uint32_t
DgrHeader::GetSerializedSize () const
{
  DgrNse nse;
  return 4 + m_nseList.size () * nse.GetSerializedSize ();
}

void
DgrHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (uint8_t(m_command));
  i.WriteU8 (2);
  i.WriteU16 (0);

  for (std::list<DgrNse>::const_iterator iter = m_nseList.begin ();
       iter != m_nseList.end ();
       iter ++)
    {
      iter->Serialize (i);
      i.Next (iter->GetSerializedSize ());
    }
}

uint32_t
DgrHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  uint8_t temp;
  temp = i.ReadU8 ();
  if ((temp == REQUEST) || (temp == RESPONSE))
    {
      m_command = temp;
    }
  else
    {
      return 0;
    }

  if (i.ReadU8 () != 2)
    {
      NS_LOG_LOGIC ("DGR received a message with mismatch version, ignoring.");
      return 0;
    }
  
  if (i.ReadU16 () != 0)
    {
      NS_LOG_LOGIC ("DGR received a message with invalid filled flags, ignoring.");
      return 0;
    }
  
  DgrNse nse;
  uint32_t nseSize = nse.GetSerializedSize ();
  uint8_t nseNumber = i.GetRemainingSize ()/ nseSize; // !!!!!!!!!!!!! the size should be the same with nse.
  for (uint8_t n = 0; n < nseNumber; n ++)
    {
      i.Next (nse.Deserialize (i));
      m_nseList.push_back (nse);
    }

    return GetSerializedSize ();
}

void
DgrHeader::SetCommand (Command_e command)
{
  m_command = command;
}

DgrHeader::Command_e
DgrHeader::GetCommand () const
{
  return Command_e (m_command);
}

void
DgrHeader::AddNse (DgrNse nse)
{
  m_nseList.push_back (nse);
}

void
DgrHeader::ClearNses ()
{
  m_nseList.clear ();
}

uint16_t
DgrHeader::GetNseNumber () const
{
  return m_nseList.size ();
}

std::list<DgrNse>
DgrHeader::GetNseList () const
{
  return m_nseList;
}

std::ostream&
operator<< (std::ostream& os, const DgrHeader& h)
{
  h.Print (os);
  return os;
}

}

