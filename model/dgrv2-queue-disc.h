/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DGR_v2_QUEUE_DISC_H
#define DGR_v2_QUEUE_DISC_H

#include "ns3/queue-disc.h"
#include "ns3/object.h"
#include "ns3/packet-filter.h"
#include "dgr-header.h"

namespace ns3 {

class DGRv2QueueDisc : public QueueDisc {
  public:
    /**
     * \brief Get the type ID.
     * \return the object TypeId
     */
    static TypeId GetTypeId (void);
    /**
     * \brief DGRv2QueueDisc constructor
     */
    DGRv2QueueDisc ();
    /**
     * \brief DGRv2QueueDisc Destructor
    */
    ~DGRv2QueueDisc();

    // Reasons for dropping packets
    static constexpr const char* LIMIT_EXCEEDED_DROP = "Queue disc limit exceeded";  //!< Packet dropped due to queue disc limit exceeded

    // Get current queue state
    uint32_t GetQueueStatus ();
    bool CheckConfig (void) override;

    
  private:
    uint32_t m_fastWeight;
    uint32_t m_normalWeight;
    uint32_t m_slowWeight;
    uint32_t m_currentFastWeight;
    uint32_t m_currentNormalWeight;
    uint32_t m_currentSlowWeight;
    
    bool DoEnqueue (Ptr<QueueDiscItem> item) override;
    Ptr<QueueDiscItem> DoDequeue (void) override;
    Ptr<const QueueDiscItem> DoPeek (void) override;
    // bool CheckConfig (void) override;
    void InitializeParams (void) override;

    uint32_t EnqueueClassify (Ptr<QueueDiscItem> item);
    uint32_t Classify ();
};

class DGRv2PacketFilter : public PacketFilter
{
  public:
    /**
     * \brief Get the type ID.
     * \return the object TypeId
     */
    static TypeId GetTypeId();

    DGRv2PacketFilter();
    ~DGRv2PacketFilter() override;

  private:
    bool CheckProtocol(Ptr<QueueDiscItem> item) const override;
    int32_t DoClassify(Ptr<QueueDiscItem> item) const override;
};

}

#endif /* DGRv2_QUEUE_DISC_H */
