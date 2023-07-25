/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DGRTAGS_H
#define DGRTAGS_H

#include "ns3/core-module.h"
#include "ns3/tag.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include <list>

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
     * \param budget the delay budget in microsecond
    */
    void SetBudget (uint32_t m_budget);

    /**
     * \brief Get the tag's budget
     * \return the budget in microsecond
    */
    uint32_t GetBudget (void) const;
    
    /**
     * \brief Get the type ID
     * \return the object TypeId
    */
    static TypeId GetTypeId (void);

    // inherited function, no need to doc.
    TypeId GetInstanceTypeId (void) const;
    
    // inherited function, no need to doc.
    uint32_t GetSerializedSize (void) const override;

    // inherited function, no need to doc.
    void Serialize (TagBuffer i) const override;

    // inherited function, no need to doc.
    void Deserialize (TagBuffer i) override;

    // inherited function, no need to doc.
    void Print (std::ostream &os) const override;

private:
    uint32_t m_budget; // in microsecond  
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
    void SetPriority (bool priority);
    
    /**
     * \brief Get the tag's priority
     * \returns the priority
    */
    bool GetPriority (void) const;

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
    bool m_priority;
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

} // namespace ns3

#endif /* DGRTAGS_H */