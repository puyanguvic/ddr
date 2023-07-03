/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/object-factory.h"
#include "ns3/queue.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "dgrv2-queue-disc.h"
#include "dgr-tags.h"

#define DELAY_SENSITIVE 0
#define BEST_EFFORT 1

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DGRv2QueueDisc");

NS_OBJECT_ENSURE_REGISTERED (DGRv2QueueDisc);

TypeId DGRv2QueueDisc::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DGRv2QueueDisc")
    .SetParent<QueueDisc> ()
    .SetGroupName ("DGRv2")
    .AddConstructor<DGRv2QueueDisc> ()
    .AddAttribute ("MaxSize",
                   "The maximum size accepted by this queue disc.",
                   QueueSizeValue (QueueSize ("125KiB")),
                   MakeQueueSizeAccessor (&QueueDisc::SetMaxSize,
                                          &QueueDisc::GetMaxSize),
                   MakeQueueSizeChecker ())
  ;
  return tid;
}

DGRv2QueueDisc::DGRv2QueueDisc ()
  : QueueDisc (QueueDiscSizePolicy::MULTIPLE_QUEUES, QueueSizeUnit::BYTES)
{
  NS_LOG_FUNCTION (this);
}

DGRv2QueueDisc::~DGRv2QueueDisc ()
{
  NS_LOG_FUNCTION (this);
}

uint32_t
DGRv2QueueDisc::GetQueueStatus ()
{
  uint32_t currentSize = GetInternalQueue (0)->GetCurrentSize ().GetValue ();
  uint32_t maxSize = GetInternalQueue (0)->GetMaxSize ().GetValue ();
  return currentSize * 10 / maxSize;
}


bool
DGRv2QueueDisc::DoEnqueue (Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << item);
  uint32_t band = EnqueueClassify (item);
  // uint32_t itemSize = item->GetSize ();
  // uint32_t currentQueueSize = GetInternalQueue (band)->GetCurrentSize ().GetValue ();
  // uint32_t maxQueueSize = GetInternalQueue (band)->GetMaxSize ().GetValue ();
  // if (currentQueueSize + itemSize > maxQueueSize)
  //   {
  //     NS_LOG_LOGIC ("Queue disc limit exceeded -- drop packet");
  //     DropBeforeEnqueue (item, LIMIT_EXCEEDED_DROP);
  //   }
  bool retval = GetInternalQueue (band)->Enqueue (item);
  if (!retval)
    {
      NS_LOG_WARN ("Packet enqueue failed. Check the size of the internal queues");
    }

  NS_LOG_LOGIC ("Band current size " << band << ": " << GetInternalQueue (band)->GetCurrentSize ());
  return retval;
}

Ptr<QueueDiscItem>
DGRv2QueueDisc::DoDequeue (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<QueueDiscItem> item;

  for (uint32_t i = 0; i < GetNInternalQueues (); i ++)
    {
      if (item = GetInternalQueue (i)->Dequeue ())
        {
          NS_LOG_LOGIC ("Popped from band " << i << ": " << item);
          NS_LOG_LOGIC ("Number packets band " << i << ": " << GetInternalQueue (i)->GetNPackets ());
          return item;
        }
    }

  NS_LOG_LOGIC ("Queue empty");
  return item;
}

Ptr<const QueueDiscItem>
DGRv2QueueDisc::DoPeek (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<const QueueDiscItem> item;

  for (uint32_t i = 0; i < GetNInternalQueues (); i++)
    {
      if (item = GetInternalQueue (i)->Peek ())
        {
          NS_LOG_LOGIC ("Peeked from band " << i << ": " << item);
          NS_LOG_LOGIC ("Number packets band " << i << ": " << GetInternalQueue (i)->GetNPackets ());
          return item;
        }
    }

  NS_LOG_LOGIC ("Queue empty");
  return item;
}

bool
DGRv2QueueDisc::CheckConfig (void)
{
  NS_LOG_FUNCTION (this);
  if (GetNQueueDiscClasses () > 0)
    {
      NS_LOG_ERROR ("DGRv2QueueDisc cannot have classes");
      return false;
    }

  if (GetNPacketFilters () != 0)
    {
      NS_LOG_ERROR ("DGRv2QueueDisc needs no packet filter");
      return false;
    }
  
  if (GetNInternalQueues () == 0)
    {
      // create 2 DropTail queues with GetLimit() packets each
      // std::cout << "Create 2 DropTail Queue" << std::endl;
      ObjectFactory factory;
      factory.SetTypeId ("ns3::DropTailQueue<QueueDiscItem>");
      factory.Set ("MaxSize", QueueSizeValue (QueueSize ("250KB")));
      AddInternalQueue (factory.Create<InternalQueue> ());
      factory.Set ("MaxSize", QueueSizeValue (QueueSize ("2500KB")));
      AddInternalQueue (factory.Create<InternalQueue> ());
      
      // GetInternalQueue (0)->GetMaxSize ();
      // GetInternalQueue (0)->GetMaxSize ();
    }

  if (GetNInternalQueues () != 2)
    {
      NS_LOG_ERROR ("DGRVirtualQueueDisc needs 2 internal queues");
      return false;
    }

  if (GetInternalQueue (0)-> GetMaxSize ().GetUnit () != QueueSizeUnit::BYTES ||
      GetInternalQueue (1)-> GetMaxSize ().GetUnit () != QueueSizeUnit::BYTES)
    {
      NS_LOG_ERROR ("DGRv2QueueDisc needs 2 internal queues operating in BYTES mode");
      return false;
    }
  return true;
}

void
DGRv2QueueDisc::InitializeParams (void)
{
  m_fastWeight = 10;
  m_normalWeight = 1;
  m_currentFastWeight = m_fastWeight;
  m_currentNormalWeight = m_normalWeight;
  NS_LOG_FUNCTION (this);
}

uint32_t
DGRv2QueueDisc::EnqueueClassify (Ptr<QueueDiscItem> item)
{
  BudgetTag budgetTag;
  if (item->GetPacket ()->PeekPacketTag (budgetTag))
    {
      return DELAY_SENSITIVE;
    }
  else
    {
      return BEST_EFFORT;
    }
}

// ------------------------------ QueuePacketFilter --------------------------------

TypeId
DGRv2PacketFilter::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::DGRv2PacketFilter")
          .SetParent<PacketFilter>()
          .SetGroupName("DGRv2")
          ;
    return tid;
}

DGRv2PacketFilter::DGRv2PacketFilter()
{
    NS_LOG_FUNCTION(this);
}

DGRv2PacketFilter::~DGRv2PacketFilter()
{
    NS_LOG_FUNCTION(this);
}

bool
DGRv2PacketFilter::CheckProtocol(Ptr<QueueDiscItem> item) const
{
    NS_LOG_FUNCTION(this << item);
    // Here we don't need check protocol
    return true;
}

int32_t
DGRv2PacketFilter::DoClassify(Ptr<QueueDiscItem> item) const
{
    PriorityTag priorityTag;
    if (item->GetPacket ()->PeekPacketTag (priorityTag))
        {
            return DELAY_SENSITIVE;
        }
    else
        {
            return BEST_EFFORT;
        }
}

} // namespace ns3
