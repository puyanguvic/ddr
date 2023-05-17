/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "neighbor-status-database.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("NeighborStatusDatabase");

StatusUnit::StatusUnit ()
  : m_sigma (0),
    m_mean (0),
    m_total (0)
{
}

StatusUnit::~StatusUnit ()
{
}

double_t
StatusUnit::GetVariance (void) const
{
  return m_sigma;
}

void
StatusUnit::SetVariance (double_t sigma)
{
  m_sigma = sigma;
}

double_t
StatusUnit::GetAverage (void) const
{
  return m_mean;
}

void
StatusUnit::SetAverage (double_t mean)
{
  m_mean = mean;
}

uint32_t
StatusUnit::GetNSample (void) const
{
  return m_total;
}

void
StatusUnit::SetNSample (uint32_t total)
{
  m_total = total;
}

void
StatusUnit::Print (std::ostream &os) const
{
  os << "variance = " << m_sigma
     << ", average = " << m_mean
     << ", sample number" << m_total;
}

//----------------------------------------------------------------------
//-- NeighborStatusEntry
//------------------------------------------------------
NeighborStatusEntry::NeighborStatusEntry ()
    : m_database ()
{
}

NeighborStatusEntry::~NeighborStatusEntry ()
{
  NSMap_t::iterator i;
  for (i = m_database.begin (); i != m_database.end (); i ++)
    {
      StatusUnit* temp = i->second;
      delete temp;
    }
  m_database.clear ();
}

void
NeighborStatusEntry::Insert (uint32_t n_iface, StatusUnit* su)
{
  NSMap_t::iterator it = m_database.find (n_iface);
  if (it != m_database.end ())
    {
      it->second = su;
    }
  else
    {
      m_database.insert (NSPair_t (n_iface, su));
    }
}

uint32_t
NeighborStatusEntry::GetNumStatusUnit () const
{
  return m_database.size ();
}

void
NeighborStatusEntry::Print (std::ostream &os) const
{
  os << "Interface    StatusUnit" << std::endl;
  NSMap_t::const_iterator ci;
  for (ci = m_database.begin (); ci != m_database.end (); ci ++)
    {
      os << ci->first << "    ";
      ci->second->Print (os);
    }
}

//----------------------------------------------------------------------
//-- DgrNSDB
//------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (DgrNSDB);

DgrNSDB::DgrNSDB ()
    : m_database ()
{
  NS_LOG_FUNCTION (this);
}

DgrNSDB::~DgrNSDB ()
{
  NS_LOG_FUNCTION (this);
  NSDBMap_t::iterator i;
  for (i = m_database.begin (); i != m_database.end (); i ++)
    {
      NS_LOG_LOGIC ("Free NSE");
      NeighborStatusEntry* temp = i->second;
      delete temp;
    }
  NS_LOG_LOGIC ("Clear map");
  m_database.clear ();
}

void
DgrNSDB::Initialize ()
{
  NS_LOG_FUNCTION (this);
  NSDBMap_t::iterator i;
  for (i = m_database.begin(); i != m_database.end (); i ++)
    {
      NeighborStatusEntry* temp = i->second;
      delete temp;
    }
}

NeighborStatusEntry*
DgrNSDB::GetNeighborStatusEntry (uint32_t iface) const
{
  NS_LOG_FUNCTION (this << iface);
  //
  // Look up a NSE by it's interface.
  //
  NSDBMap_t::const_iterator ci = m_database.find (iface);
  if (ci != m_database.end ())
    {
      return ci->second;
    }
  return nullptr;
}

void
DgrNSDB::Update (uint32_t iface, NeighborStatusEntry* nse)
{
  NS_LOG_FUNCTION (this << iface << nse);
  NSDBMap_t::iterator it = m_database.find (iface);
  if (it != m_database.end ())
    {
      it->second = nse;
    }
  else
    {
      m_database.insert (NSDBPair_t (iface, nse));
    }
}

}