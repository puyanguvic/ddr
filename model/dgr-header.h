/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DGR_HEADER_H
#define DGR_HEADER_H

#include "ns3/header.h"
#include "ns3/nstime.h"

#include <list>


namespace ns3 {

/**
 * \ingroup dgrv2
 * \brief DGR v2 Queue Status Entry (QSE)
*/
class DgrHeader : public Header
 {
 public:
   DgrHeader ();
   
   ~DgrHeader ();

   /**
    * \brief Get the type ID
    * \return The object TypeId.
   */
   static TypeId GetTypeId (void);

   /**
    * \brief Return the instance type identifier
    * \return instance type ID
   */
   TypeId GetInstanceTypeId (void) const;
   
   void Print (std::ostream &os) const;
   
   /**
    * \brief Get the serialize size of the packet.
    * \return size
   */
   uint32_t GetSerializedSize (void) const;

   /**
    * \brief serialize the packet.
    * \param start Buffer iterator
   */
   void Serialize (Buffer::Iterator start) const;

   /**
    * \brief Deserialize the packet.
    * \param start Buffer iterator.
    * \return Size of packet
   */
   uint32_t Deserialize (Buffer::Iterator start);
   

   void SetTxTime (Time txTime);
   Time GetTxTime (void) const;
   void SetBudget (uint32_t budget);
   uint32_t GetBudget (void) const;
   void SetPriority (uint8_t priority);
   uint8_t GetPriority (void) const;
   void SetFlag (bool flag);
   bool GetFlag (void) const;

 private:
    uint8_t m_command;        //!< command type
    std::list<dfd> m_qseList; //!< list of the QSE in the message
 };

}

#endif /* NEIGHBORINFO_HEADER_H */





