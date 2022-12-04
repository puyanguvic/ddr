/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DGR_VIRTUAL_QUEUE_DISC_H
#define DGR_VIRTUAL_QUEUE_DISC_H

#include "ns3/queue-disc.h"

namespace ns3 {

class DGRVirtualQueueDisc : public QueueDisc {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief PfifoFastQueueDisc constructor
   *
   * Creates a queue with a depth of 1000 packets per band by default
   */
  DGRVirtualQueueDisc ();

  virtual ~DGRVirtualQueueDisc();

  // Reasons for dropping packets
  static constexpr const char* LIMIT_EXCEEDED_DROP = "Queue disc limit exceeded";  //!< Packet dropped due to queue disc limit exceeded

private:
  // packet size = 1kB
  // packet size for test = 52B
  uint32_t LinesSize[3] = {17,68,1000};

  virtual bool DoEnqueue (Ptr<QueueDiscItem> item);
  virtual Ptr<QueueDiscItem> DoDequeue (void);
  // virtual void DoPrioDequeue (void);
  virtual Ptr<const QueueDiscItem> DoPeek (void);
  virtual bool CheckConfig (void);
  virtual void InitializeParams (void);
  virtual uint32_t EnqueueClassify (Ptr<QueueDiscItem> item);
};

}

#endif /* DGR_VIRTUAL_QUEUE_DISC_H */
