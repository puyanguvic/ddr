/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DGR_HEADER_H
#define DGR_HEADER_H

#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/packet.h"

#include <list>

namespace ns3 {

/**
 * \ingroup dgr
 * \brief dgr v2 Neighbor Status Entry (NSE) 
*/
class DgrNse : public Header
{
  public:
    DgrNse ();
    
    /**
     * \brief Get the type ID.
     * \return The object TypeId
    */
    static TypeId GetTypeId ();

    /**
     * \brief Return the instance type identifier
     * \return Instance type ID.
    */
    TypeId GetInstanceTypeId () const override;

    void Print (std::ostream& os) const override;

    /**
     * \brief Get the serialized size of the packet
     * \return size
    */
    uint32_t GetSerializedSize () const override;

    /**
     * \brief Serialize the packet.
     * \param start Buffer iterator 
    */
    void Serialize (Buffer::Iterator start) const override;

    /**
     * \brief Deserialize the packet
     * \param start Buffer iterator
     * \return size of the packet
    */
    uint32_t Deserialize (Buffer::Iterator start) override;

    /**
     * \brief Set the interface
     * \param command the interface
    */
    void SetInterface (uint32_t iface);

    /**
     * \brief Get the iface
     * \returns the iface
    */
    uint32_t GetInterface () const;

    void SetQueueSize (uint32_t qSize);
    uint32_t GetQueueSize () const;
  private:
    uint32_t m_iface;
    uint32_t m_qSize;
};

/**
 * \brief Stream insertion operator
 * 
 * \param os the reference to the output stream
 * \param h the Neighbor status Entry
 * \returns the reference to te output stream
*/
std::ostream& operator<<(std::ostream& os, const DgrNse & h);

/**
 * \ingroup dgr
 * \brief dgr header 
*/
class DgrHeader : public Header
{
    public:
        DgrHeader ();
    
    /**
     * \brief Get the type ID
     * \return the object TypeId
    */
    static TypeId GetTypeId ();

    /**
     * \brief Return the instance type identifier.
     * \return the object TypeId
    */
    TypeId GetInstanceTypeId () const override;

    void Print (std::ostream &os) const override;

    /**
     * \brief Get the serialized size of the packet
     * \return size
    */
    uint32_t GetSerializedSize () const override;

    /**
     * \brief Serialize the packet.
     * \param start Buffer iterator 
    */
    void Serialize (Buffer::Iterator start) const override;

    /**
     * \brief Deserialize the packet
     * \param start Buffer iterator
     * \return size of the packet
    */
    uint32_t Deserialize (Buffer::Iterator start) override;

    /**
     * Commands to be used in Dgr headers
    */
    enum Command_e
    {
        REQUEST = 0x1,
        RESPONSE = 0x2,
    };

    /**
     * \brief Set the command
     * \param command the command
    */
    void SetCommand (Command_e command);

    /**
     * \brief Get the command
     * \returns the command
    */
   Command_e GetCommand () const;

   /**
    * \brief Add a DGR Neighbor Status Entry (NSE) to the message
    * \param nse the Neighbor Status Entry
   */
   void AddNse (DgrNse nse);

   /**
    * \brief Clear all the NSEs from the header
   */
   void ClearNses ();

   /**
    * \brief Get the number of NSEs includes in the message
    * \returns the number of DNEs in the message
   */
   uint16_t GetNseNumber () const;

   /**
    * \brief Get the list of NSEs included in the message
    * \returns the list of DNEs in the message
   */
   std::list<DgrNse> GetNseList () const;

private:
   uint8_t m_command;           //!< command type
   std::list<DgrNse> m_nseList;  //!< list of the DNEs in the message
};

/**
 * \brief Stream insertion operator
 * 
 * \param os the reference to the output stream
 * \param h the DGR header
 * \returns the reference to the output stream
*/
std::ostream& operator<< (std::ostream& os, const DgrHeader& h);

}

#endif /* NEIGHBORINFO_HEADER_H */





