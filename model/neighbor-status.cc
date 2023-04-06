/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <cmath>
#include "ns3/core-module.h"
#include "neighbor-status.h"

namespace ns3 {

// NS_LOG_COMPONENT_DEFINE ("NeighborStatus");

StateStatistic::StateStatistic ()
  : m_sigma (0),
    m_mean (0),
    m_total (0)
{
}

StateStatistic::~StateStatistic ()
{
}

double_t
StateStatistic::GetVariance (void) const
{
  return m_sigma;
}

void
StateStatistic::SetVariance (double_t sigma)
{
  m_sigma = sigma;
}

double_t
StateStatistic::GetAverage (void) const
{
  return m_mean;
}

void
StateStatistic::SetAverage (double_t mean)
{
  m_mean = mean;
}

uint32_t
StateStatistic::GetNSample (void) const
{
  return m_total;
}

void
StateStatistic::SetNSample (uint32_t total)
{
  m_total = total;
}


// -----------------------------------------------
// Neighbor Status
// ------------------------------------------------
NeighborStatus::NeighborStatus ()
  : m_currentState (Empty),
    m_state ()
{
}

NeighborStatus::~NeighborStatus ()
{
  NS_LOG_FUNCTION (this);
  NSMap_t::iterator i;
  for (i = m_state.begin (); i != m_state.end (); i ++)
    {
      StateStatistic* temp = i->second;
      delete temp;
    }
  NS_LOG_LOGIC ("clear state");
  m_state.clear ();
}

uint32_t
NeighborStatus::GetInterface (void) const
{
  return m_iface;
}

void
NeighborStatus::SetInterface (uint32_t iface)
{
  m_iface = iface;
}

QStatus
NeighborStatus::GetCurrentState (void) const
{
  return m_currentState;
}

void
NeighborStatus::Insert (QStatus stat, StateStatistic* ss)
{
  NS_LOG_FUNCTION (this << stat << ss);
  m_state.insert (NSPair_t (stat, ss));
}

StateStatistic*
NeighborStatus::GetCurrentStateStatistic (void) const
{
  NS_LOG_FUNCTION (this);
  NSMap_t::const_iterator it = m_state.find (m_currentState);
  if (it != m_state.end())
    {
      return it->second;
    }
  else
    {
      return 0;
    }
}

uint32_t
NeighborStatus::GetNumState () const
{
  NS_LOG_FUNCTION (this);
  return m_state.size ();
}

void
NeighborStatus::UpdateStatus (QStatus status, uint32_t delay)
{
  if (m_currentState != status)
    {
      m_currentState = status;
    }
  
  if (m_state.find (status) == m_state.end())
    {
      StateStatistic* ss = new StateStatistic ();
      Insert (status, ss);
    }
  NSMap_t::const_iterator it = m_state.find (status);
  double_t sigma = it->second->GetVariance ();
  double_t mean = it->second->GetAverage ();
  uint32_t total = it->second->GetNSample ();
  it->second->SetAverage ((mean*total + delay)/(total + 1.0));
  it->second->SetVariance (total / pow(total + 1, 2) * pow(delay - mean, 2) + total/(total+1)*sigma);
  it->second->SetNSample (total + 1);
}

void
NeighborStatus::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << "Current Status: " << m_currentState << std::endl;
  NSMap_t::const_iterator i = m_state.begin();
  for (;i != m_state.end(); i ++)
  {
    os << "Status: " << i->first 
       << ", average delay: " << i->second->GetAverage ()
       << ", variance: " << i->second->GetVariance ()
       <<", Sample Number: " << i->second->GetNSample ()
       << std::endl;

  }
}

}