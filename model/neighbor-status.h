/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef NEIGHBOR_STATUS_H
#define NEIGHBOR_STATUS_H

#include "ns3/core-module.h"
namespace ns3 {

enum QStatus {
    Empty = 0,  /** Less than 25 % */
    Idle,       /** Larger than 25 % and less than 50% */
    Busy,       /** Larger than 50 % and less than 75% */
    Congestion  /** Larger than 75 % */
  };

// struct StaticState
// {
//   double_t sigma; /** variance */
//   double_t mean;  /** average */
//   uint32_t total; /** sample number*/
//   StaticState () : sigma(0), mean (0), total (0){};
// };

class StateStatistic
{
  public:
  StateStatistic ();
  ~StateStatistic ();
  double_t GetVariance (void) const;
  void SetVariance (double_t sigma);
  double_t GetAverage (void) const;
  void SetAverage (double_t mean);
  uint32_t GetNSample (void) const;
  void SetNSample (uint32_t total);
  private:
  double_t m_sigma; /** variance */
  double_t m_mean; /** average */
  uint32_t m_total; /** sample number */
};

class NeighborStatus
{
public:
  NeighborStatus ();
  ~NeighborStatus ();

  uint32_t GetInterface (void) const;
  void SetInterface (uint32_t iface);
  QStatus GetCurrentState (void) const;
  void Insert (QStatus stat , StateStatistic* ss);
  StateStatistic* GetCurrentStateStatistic (void) const;
  uint32_t GetNumState () const;
  void UpdateStatus (QStatus status, uint32_t delay);
  void Print (std::ostream &os) const;
  
private:
  uint32_t m_iface;
  QStatus m_currentState;
  typedef std::map<uint32_t, StateStatistic*> NSMap_t; /** status, statistic*/
  typedef std::pair<uint32_t, StateStatistic*> NSPair_t;
  NSMap_t m_state;
};  

}

#endif /* ROUTER_STATUS_H */