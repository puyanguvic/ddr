/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <iostream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/stats-module.h"
#include "dgr-tags.h"
#include "ns3/log.h"

namespace ns3 {

// NS_LOG_COMPONENT_DEFINE ("ns3::DGRTags");

//----------------------------------------------------------------------
//-- BudgetTag
//------------------------------------------------------
NS_LOG_COMPONENT_DEFINE ("BudgetTag");
NS_OBJECT_ENSURE_REGISTERED (BudgetTag);

BudgetTag::BudgetTag ()
{
  NS_LOG_FUNCTION (this);
}

void
BudgetTag::SetBudget (uint32_t budget)
{
  NS_LOG_FUNCTION (this << static_cast<uint32_t> (budget));
  m_budget = budget;
}

uint32_t
BudgetTag::GetBudget (void) const
{
  NS_LOG_FUNCTION (this);
  return m_budget;
}

TypeId
BudgetTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("BudgetTag")
                          .SetParent<Tag> ()
                          .SetGroupName ("dgr-rl")
                          .AddConstructor<BudgetTag> ();
  return tid;
}

TypeId
BudgetTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
BudgetTag::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 4;     // 4 bytes
}

void
BudgetTag::Serialize (TagBuffer i) const
{
  NS_LOG_FUNCTION (this << &i);
  uint32_t t = m_budget;
  i.Write ((const uint8_t *)&t, 4);
}

void
BudgetTag::Deserialize (TagBuffer i)
{
  NS_LOG_FUNCTION (this << &i);
  uint32_t t;
  i.Read ((uint8_t *)&t, 4);
  m_budget = t;
}

void
BudgetTag::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << "budget = " << m_budget;
}

//----------------------------------------------------------------------
//-- DistTag
//------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (DistTag);

DistTag::DistTag ()
{
  NS_LOG_FUNCTION (this);
}

void
DistTag::SetDistance (uint32_t distance)
{
  NS_LOG_FUNCTION (this << distance);
  m_distance = distance;
}

uint32_t
DistTag::GetDistance (void) const
{
  NS_LOG_FUNCTION (this);
  return m_distance;
}

TypeId
DistTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DistTag")
    .SetParent<Tag> ()
    .SetGroupName ("dgr-rl")
    .AddConstructor<DistTag> ();
  return tid;
}

TypeId
DistTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
DistTag::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 4;     // 4 bytes
}

void
DistTag::Serialize (TagBuffer i) const
{
  NS_LOG_FUNCTION (this << &i);
  uint32_t t = m_distance;
  i.Write ((const uint8_t *)&t, 4);
}

void
DistTag::Deserialize (TagBuffer i)
{
  NS_LOG_FUNCTION (this << &i);
  uint32_t t;
  i.Read ((uint8_t *)&t, 4);
  m_distance = t;
}

void
DistTag::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << "Distance = " << m_distance;
}

//----------------------------------------------------------------------
//-- FlagTag
//------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (FlagTag);

FlagTag::FlagTag ()
{
  NS_LOG_FUNCTION (this);
}

void
FlagTag::SetFlag (bool flag)
{
  NS_LOG_FUNCTION (this << flag);
  m_flag = flag;
}

bool
FlagTag::GetFlag (void) const
{
  NS_LOG_FUNCTION (this << m_flag);
  return m_flag;
}

TypeId
FlagTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("FlagTag")
    .SetParent<Tag> ()
    .SetGroupName ("dgr-rl")
    .AddConstructor<FlagTag> ();
  return tid;
}

TypeId
FlagTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
FlagTag::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 1;     // 1 bytes
}

void
FlagTag::Serialize (TagBuffer i) const
{
  NS_LOG_FUNCTION (this << &i);
  bool flag = m_flag;
  i.Write ((const uint8_t *)&flag, 1);
}

void
FlagTag::Deserialize (TagBuffer i)
{
  NS_LOG_FUNCTION (this << &i);
  bool flag;
  i.Read ((uint8_t *)&flag, 1);
  m_flag = flag;
}

void
FlagTag::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << "flag = " << m_flag;
}

//----------------------------------------------------------------------
//-- PriorityTag
//------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (PriorityTag);

PriorityTag::PriorityTag ()
{
  NS_LOG_FUNCTION (this);
}

void
PriorityTag::SetPriority (uint32_t priority)
{
  NS_LOG_FUNCTION (this << priority);
  m_priority = priority;
}

uint32_t
PriorityTag::GetPriority () const
{
  NS_LOG_FUNCTION (this);
  return m_priority;
}

TypeId
PriorityTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("PriorityTag")
    .SetParent<Tag> ()
    .SetGroupName ("dgr-rl")
    .AddConstructor<PriorityTag> ();
  return tid;
}

TypeId
PriorityTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
PriorityTag::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 4;     // 4 bytes
}

void
PriorityTag::Serialize (TagBuffer i) const
{
  NS_LOG_FUNCTION (this << &i);
  uint32_t t = m_priority;
  i.Write ((const uint8_t *)&t, 4);
}

void
PriorityTag::Deserialize (TagBuffer i)
{
  NS_LOG_FUNCTION (this << &i);
  uint32_t t;
  i.Read ((uint8_t *)&t, 4);
  m_priority = t;
}

void
PriorityTag::Print (std::ostream &os) const
{
  os << "Priority=" << m_priority;
}

// //----------------------------------------------------------------------
// //-- TimestampTag
// //------------------------------------------------------
// TimestampTag::TimestampTag ()
// {
//   NS_LOG_FUNCTION (this);
// }

// void
// TimestampTag::SetTimestamp (Time timestamp)
// {
//   NS_LOG_FUNCTION (this << timestamp);
//   m_timestamp = timestamp;
// }

// Time
// TimestampTag::GetTimestamp (void) const
// {
//   NS_LOG_FUNCTION (this);
//   return m_timestamp;
// }

// TypeId
// TimestampTag::GetTypeId (void)
// {
//   static TypeId tid = TypeId ("TimestampTag")
//     .SetParent<Tag> ()
//     .SetGroupName ("dgr-rl")
//     .AddConstructor<TimestampTag> ();
//   return tid;
// }

// TypeId
// TimestampTag::GetInstanceTypeId (void) const
// {
//   return GetTypeId ();
// }

// uint32_t
// TimestampTag::GetSerializedSize (void) const
// {
//   NS_LOG_FUNCTION (this);
//   return 8;     // 8 bytes
// }

// void
// TimestampTag::Serialize (TagBuffer i) const
// {
//   NS_LOG_FUNCTION (this << &i);
//   int64_t t = m_timestamp.GetNanoSeconds ();
//   i.Write ((const uint8_t *)&t, 8);
// }

// void
// TimestampTag::Deserialize (TagBuffer i)
// {
//   NS_LOG_FUNCTION (this << &i);
//   int64_t t;
//   i.Read ((uint8_t *)&t, 8);
//   m_timestamp = NanoSeconds (t);
// }

// void
// TimestampTag::Print (std::ostream &os) const
// {
//   NS_LOG_FUNCTION (this << &os);
//   os << "t=" << m_timestamp.GetMilliSeconds () << "ms";
// }


//----------------------------------------------------------------------
//-- NSTag
//------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (NSTag);

NSTag::NSTag ()
{
  NS_LOG_FUNCTION (this);
}

void
NSTag::SetNS (bool ns)
{
  NS_LOG_FUNCTION (this << ns);
  m_ns = ns;
}

bool
NSTag::GetNS (void) const
{
  NS_LOG_FUNCTION (this << m_ns);
  return m_ns;
}

TypeId
NSTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("NSTag")
    .SetParent<Tag> ()
    .SetGroupName ("dgrv2")
    .AddConstructor<NSTag> ();
  return tid;
}

TypeId
NSTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
NSTag::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 1;     // 1 bytes
}

void
NSTag::Serialize (TagBuffer i) const
{
  NS_LOG_FUNCTION (this << &i);
  bool ns = m_ns;
  i.Write ((const uint8_t *)&ns, 1);
}

void
NSTag::Deserialize (TagBuffer i)
{
  NS_LOG_FUNCTION (this << &i);
  bool ns;
  i.Read ((uint8_t *)&ns, 1);
  m_ns = ns;
}

void
NSTag::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << "ns = " << m_ns;
}

//----------------------------------------------------------------------
//-- DgrNse
//------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (DgrNse);

DgrNse::DgrNse ()
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
  os << "Iface: " << m_iface << ", QueueSize" << m_qSize << "\n";
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
    : m_command (0)
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
