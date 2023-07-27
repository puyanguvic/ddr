/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "neighbor-status-database.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("NeighborStatusDatabase");

StatusUnit::StatusUnit ()
  : m_matrix {0},
    m_state (0)
{
}

StatusUnit::~StatusUnit ()
{
}

int
StatusUnit::GetEstimateState () const
{
  int counter = m_matrix[m_state][m_state];
  int ret = m_state;
  for (int i = 0; i < 10; i ++)
    {
      if (m_matrix[m_state][i] > counter)
        {
          counter = m_matrix[m_state][i];
          ret = i;
        }
    }
  return ret;
}

uint32_t
StatusUnit::GetEstimateDelayDDR () const
{
  // std::cout << "current delay: " << GetEstimateDelayDGR () << std::endl;
  // Print (std::cout);
  uint32_t ret = 0;
  int counter = 0;
  for (int i = 0; i < STATESIZE; i ++)
    {
      ret += m_matrix[m_state][i]* (i);
      counter += m_matrix[m_state][i];
    }
  return ret*2000/counter;
}
uint32_t
StatusUnit::GetEstimateDelayDGR () const
{
  return m_state*2000;
}

int
StatusUnit::GetCurrentState () const
{
  int counter = m_matrix[m_state][m_state];
  int ret = m_state;
  for (int i = 0; i < 10; i ++)
    {
      if (m_matrix[m_state][i] > counter)
        {
          counter = m_matrix[m_state][i];
          ret = i;
        }
    }
  return ret;
}

int
StatusUnit::GetLastState () const
{
  return m_state;
}

void
StatusUnit::Update (int state)
{
  m_matrix[m_state][state] ++;
  m_state = state;
}

void
StatusUnit::Print (std::ostream &os) const
{
  os << "Last state = " << GetLastState ()
     << ", Current State = " << GetEstimateState ()
     << std::endl;
  os << "current Markov Transition Probability Matrix: "
     << std::endl;
  for (int i = 0; i < 10; i ++)
    {
      for (int j = 0; j < 10; j ++)
        {
          os << m_matrix[i][j];
          os << " ";
        }
      os << std::endl;
    }
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

StatusUnit*
NeighborStatusEntry::GetStatusUnit (uint32_t n_iface) const
{
  NS_LOG_FUNCTION (this << n_iface);
  //
  // Look up a SU by it's interface.
  //
  NSMap_t::const_iterator ci = m_database.find (n_iface);
  if (ci != m_database.end ())
    {
      return ci->second;
    }
  return nullptr;
}


uint32_t
NeighborStatusEntry::GetNumStatusUnit () const
{
  return m_database.size ();
}

void
NeighborStatusEntry::Print (std::ostream &os) const
{
  os << "Next_Iface    StatusUnit" << std::endl;
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
  // NSDBMap_t::iterator i;
  // for (i = m_database.begin (); i != m_database.end (); i ++)
  //   {
  //     NS_LOG_LOGIC ("Free NSE");
  //     NeighborStatusEntry* temp = i->second;
  //     delete temp;
  //   }
  NS_LOG_LOGIC ("Clear map");
  Initialize ();
}

void
DgrNSDB::Initialize ()
{
  NS_LOG_FUNCTION (this);
  m_database.clear ();
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

NeighborStatusEntry*
DgrNSDB::HandleNeighborStatusEntry (uint32_t iface)
{
  NS_LOG_FUNCTION (this << iface);
  //
  // Look up a NSE by it's interface.
  //
  NSDBMap_t::iterator iter = m_database.find (iface);
  if (iter != m_database.end ())
    {
      return iter->second;
    }
  return nullptr;
}


void
DgrNSDB::Insert (uint32_t iface, NeighborStatusEntry* nse)
{
  NS_LOG_FUNCTION (this << iface << nse);
  NSDBMap_t::iterator it = m_database.find (iface);
  if (it != m_database.end ())
    {
      std::cout << "Find a current nse" << std::endl;
      it->second = nse;
    }
  else
    {
      // std::cout << "not find, insert a new one";
      m_database.insert (NSDBPair_t (iface, nse));
    }
}

void
DgrNSDB::Print (std::ostream &os) const
{
  os << "At node: ???" << std::endl;
  NSDBMap_t::const_iterator ci;
  std::cout << "const iterator";
  for (ci = m_database.begin (); ci != m_database.end (); ci ++)
    {
      os << "Interface = " << ci->first << std::endl;
      ci->second->Print (os);
    }

}

}