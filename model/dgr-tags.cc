/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <iostream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/stats-module.h"
#include "dgr-tags.h"

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

NS_OBJECT_ENSURE_REGISTERED (BudgetTag);

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
NS_LOG_COMPONENT_DEFINE ("DistTag");
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

NS_OBJECT_ENSURE_REGISTERED (DistTag);

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
NS_LOG_COMPONENT_DEFINE ("FlagTag");
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

NS_OBJECT_ENSURE_REGISTERED (FlagTag);

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
NS_LOG_COMPONENT_DEFINE ("PriorityTag");
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


NS_OBJECT_ENSURE_REGISTERED (PriorityTag);

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

// NS_OBJECT_ENSURE_REGISTERED (TimestampTag);

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

NS_LOG_COMPONENT_DEFINE ("NSTag");
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

NS_OBJECT_ENSURE_REGISTERED (NSTag);

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
NS_LOG_COMPONENT_DEFINE ("DgrNse");
NS_OBJECT_ENSURE_REGISTERED (DgrNse);

DgrNse::DgrNse ()
    : m_iface (0),
      m_queueLength (0)
{
}

TypeId
DgrNse::GetTypeId ()
{
  static TypeId tid = 
      TypeId ("ns3::DgrNse").SetParent<Header>().SetGroupName ("dgr").AddConstructor<DgrNse> ();
  return tid;
}



}
