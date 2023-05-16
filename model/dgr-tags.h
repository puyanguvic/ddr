/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DGRTAGS_H
#define DGRTAGS_H

#include "ns3/core-module.h"
#include "ns3/tag.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"

namespace ns3 {

/**
 * \brief This class implements a tag that carries the budget time
 * of a packet to the IP layer
*/
class BudgetTag : public Tag
{
public:
    BudgetTag ();

    /**
     * \brief Set the tag's budget
     * \param budget the delay budget in millisecond
    */
    void SetBudget (uint32_t m_budget);

    /**
     * \brief Get the tag's budget
     * \return the budget in millisecond
    */
    uint32_t GetBudget (void) const;
    
    /**
     * \brief Get the type ID
     * \return the object TypeId
    */
    static TypeId GetTypeId (void);

    // inherited function, no need to doc.
    TypeId GetInstanceTypeId (void) const override;
    
    // inherited function, no need to doc.
    uint32_t GetSerializedSize (void) const override;

    // inherited function, no need to doc.
    void Serialize (TagBuffer i) const override;

    // inherited function, no need to doc.
    void Deserialize (TagBuffer i) override;

    // inherited function, no need to doc.
    void Print (std::ostream &os) const override;

private:
    uint32_t m_budget; // in millisecond  
};

/**
 * \brief This class implements a tag that carries the distance to 
 * destination node of a packet to the IP layer
*/
class DistTag : public Tag
{
public:
    DistTag ();
    /**
     * \brief Set the tag's distance 
     * \param distance
    */
    void SetDistance (uint32_t distance);

    /**
     * \brief Get the tag's distance
     * \return the distance
    */
    uint32_t GetDistance (void) const;
    
    /**
     * \brief Get the Type ID
     * \return the object TypeId
    */
    static TypeId GetTypeId (void);

    // inherited function, no need to doc.
    TypeId GetInstanceTypeId (void) const override;
    
    // inherited function, no need to doc.
    uint32_t GetSerializedSize (void) const override;

    // inherited function, no need to doc.
    void Serialize (TagBuffer i) const override;

    // inherited function, no need to doc.
    void Deserialize (TagBuffer i) override;

    // inherited function, no need to doc.
    void Print (std::ostream &os) const override;

private:
    uint32_t m_distance; // in millisecond  
};

/**
 * \brief This class implements a tag that carries a flag to 
 * the application layer
*/
class FlagTag : public Tag
{
public:
    FlagTag ();

    /**
     * \brief Set the tag's flag
     * \param flag the flag
    */
    void SetFlag (bool flag);

    /**
     * \brief Get the tag's flag
     * \return the flag
    */
    bool GetFlag (void) const;

    /**
     * \brief Get the Type ID
     * \return the object TypeId
    */
    static TypeId GetTypeId (void);

    // inherited function, no need to doc.
    TypeId GetInstanceTypeId (void) const override;
    
    // inherited function, no need to doc.
    uint32_t GetSerializedSize (void) const override;

    // inherited function, no need to doc.
    void Serialize (TagBuffer i) const override;

    // inherited function, no need to doc.
    void Deserialize (TagBuffer i) override;

    // inherited function, no need to doc.
    void Print (std::ostream &os) const override;

private:
    bool m_flag;  
};

class PriorityTag : public Tag
{
public:
    PriorityTag ();

    /**
     * \brief Set the tag's priority
     * \param priority the priority
    */
    void SetPriority (uint32_t priority);
    
    /**
     * \brief Get the tag's priority
     * \returns the priority
    */
    uint32_t GetPriority (void) const;

    /**
     * \brief Get the Type ID
     * \return the object TypeId
    */
    static TypeId GetTypeId (void);

    // inherited function, no need to doc.
    TypeId GetInstanceTypeId (void) const override;
    
    // inherited function, no need to doc.
    uint32_t GetSerializedSize (void) const override;

    // inherited function, no need to doc.
    void Serialize (TagBuffer i) const override;

    // inherited function, no need to doc.
    void Deserialize (TagBuffer i) override;

    // inherited function, no need to doc.
    void Print (std::ostream &os) const override;

private:
    uint32_t m_priority; // 0-fast, 1-slow, 2-best effort
};


// class TimestampTag : public Tag
// {
// public:
//     TimestampTag ();
//     /**
//      * \brief Set the timestamp
//      * \param timestamp the timestamp
//     */
//     void SetTimestamp (Time timestamp);

//     /**
//      * \brief Get the tag's timestamp
//      * \return the timestamp
//     */
//     Time GetTimestamp (void) const;

//     /**
//      * \brief Get the Type ID
//      * \return the object TypeId
//     */
//     static TypeId GetTypeId (void);

//     // inherited function, no need to doc.
//     TypeId GetInstanceTypeId (void) const override;
    
//     // inherited function, no need to doc.
//     uint32_t GetSerializedSize (void) const override;

//     // inherited function, no need to doc.
//     void Serialize (TagBuffer i) const override;

//     // inherited function, no need to doc.
//     void Deserialize (TagBuffer i) override;

//     // inherited function, no need to doc.
//     void Print (std::ostream &os) const override;

// private:
//     Time m_timestamp;  
// };


/**
 * \brief This class implements a tag used to mark 
 * the neighbor state broadcast packet.
*/
class NSTag : public Tag
{
public:
    NSTag ();

    /**
     * \brief Set the tag's nTag
     * \param nTag the flag
    */
    void SetNS (bool ns);

    /**
     * \brief Get the tag's flag
     * \return the flag
    */
    bool GetNS (void) const;

    /**
     * \brief Get the Type ID
     * \return the object TypeId
    */
    static TypeId GetTypeId (void);

    // inherited function, no need to doc.
    TypeId GetInstanceTypeId (void) const override;
    
    // inherited function, no need to doc.
    uint32_t GetSerializedSize (void) const override;

    // inherited function, no need to doc.
    void Serialize (TagBuffer i) const override;

    // inherited function, no need to doc.
    void Deserialize (TagBuffer i) override;

    // inherited function, no need to doc.
    void Print (std::ostream &os) const override;

private:
    bool m_ns;  
};


/**
 * \ingroup dgr
 * \brief dgr v2 Neighbor Status Entry (DNE) 
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
    void SetIface (uint32_t iface);

    /**
     * \brief Get the iface
     * \returns the iface
    */
    uint32_t GetIface () const;

    /**
     * \brief Set the queue length
     * \param length the queue length
    */
    void SetLength (uint32_t length);

    /**
     * \brief Get the queue length
     * \return the queue length
    */
    uint32_t GetLength () const;

  private:
    uint32_t m_iface;       //!< interafce
    uint32_t m_queueLength; //!< current queue length
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
    * \brief Add a DGR Neighbor Status Entry (DNE) to the message
    * \param dne the DNE
   */
   void AddDnse (DgrNse dne);

   /**
    * \brief Clear all the DNEs from the header
   */
   void ClearDnes ();

   /**
    * \brief Get the number of DNEs includes in the message
    * \returns the number of DNEs in the message
   */
   uint16_t GetDneNumber () const;

   /**
    * \brief Get the list of DNEs included in the message
    * \returns the list of DNEs in the message
   */
   std::list<DgrNse> GetDneList () const;

private:
   uint8_t m_command;           //!< command type
   std::list<DgrNse> m_rteList;  //!< list of the DNEs in the message
};

/**
 * \brief Stream insertion operator
 * 
 * \param os the reference to the output stream
 * \param h the DGR header
 * \returns the reference to the output stream
*/
std::ostream& operator<< (std::ostream& os, const DgrHeader& h);

} // namespace ns3

#endif /* DGRTAGS_H */