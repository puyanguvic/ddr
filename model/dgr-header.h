/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef NEIGHBORINFO_HEADER_H
#define NEIGHBORINFO_HEADER_H

#include "ns3/header.h"
#include "ns3/nstime.h"

namespace ns3 {

class NeighborInfoHeader : public Header
 {
 public:
   NeighborInfoHeader ();
   ~NeighborInfoHeader ();
   static TypeId GetTypeId (void);
   virtual TypeId GetInstanceTypeId (void) const;
   virtual void Print (std::ostream &os) const;
   virtual void Serialize (Buffer::Iterator start) const;
   virtual uint32_t Deserialize (Buffer::Iterator start);
   virtual uint32_t GetSerializedSize (void) const;

   void SetTxTime (Time txTime);
   Time GetTxTime (void) const;
   void SetBudget (uint32_t budget);
   uint32_t GetBudget (void) const;
   void SetPriority (uint8_t priority);
   uint8_t GetPriority (void) const;
   void SetFlag (bool flag);
   bool GetFlag (void) const;

 private:
   uint8_t ID;
   Time m_txTime;
   uint32_t m_budget;
   uint8_t m_priority;
   bool m_flag;
 };

}

#endif /* NEIGHBORINFO_HEADER_H */

