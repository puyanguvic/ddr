/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef NEIGHBOR_STATUS_Database_H
#define NEIGHBOR_STATUS_Database_H

#include "ns3/core-module.h"
#include <map>
#include <utility>
namespace ns3 {


class StatusUnit
{
  public:
    StatusUnit ();
    ~StatusUnit ();
    double_t GetVariance (void) const;
    void SetVariance (double_t sigma);
    double_t GetAverage (void) const;
    void SetAverage (double_t mean);
    uint32_t GetNSample (void) const;
    void SetNSample (uint32_t total);
    void Print (std::ostream &os) const;
  private:
    double_t m_sigma; /** variance */
    double_t m_mean; /** average */
    uint32_t m_total; /** sample number */
};

class NeighborStatusEntry
{
public:
  NeighborStatusEntry ();
  ~NeighborStatusEntry ();

  void Insert (uint32_t n_iface , StatusUnit su);
  StatusUnit* GetStatusUnit (uint32_t n_iface) const;
  uint32_t GetNumStatusUnit () const;
  void Print (std::ostream &os) const;
  
private:
  typedef std::map<uint32_t, StatusUnit*>
      NSMap_t; /** status, statistic*/
  typedef std::pair<uint32_t, StatusUnit*> 
      NSPair_t; //!< pair of <interface, StatusUnit>
  NSMap_t m_database;
};  


/**
 * \brief The DGR neighbor status database
 * 
 * Each node in DGR maintains a neighbor status data base.
*/
class DgrNSDB : public Object
{
  public:
    /**
     * @brief Construct an empty Neighbor Status Database.
     * 
     * The database map composing the Neighbor Status Database in initilaized in
     * this constructor.
    */
    DgrNSDB ();

    /**
     * \brief Destroy an empty Neighbor Status Database.
     * 
     * Before distroy the database, resources should be release by clear ()
    */
    ~DgrNSDB ();

    // Delete copy constructor and assignment operator to avodi misuse.
    DgrNSDB (const DgrNSDB&) = delete; // Disallow copying
    DgrNSDB& operator=(const DgrNSDB&) = delete;

    /**
     * \brief Set NSDB to empty.
    */
    void Initialize ();

    /**
     * \brief Get the NeighborStatusEntry of a Interface
     * 
     * \param iface The interface number
     * \return NeighborStatusEntry*
    */
    NeighborStatusEntry* GetNeighborStatusEntry (uint32_t iface) const;

    /**
     * \brief Update an <interface / Neighbor Status Entry> into the NSDB
     * 
     * The interface and NeighborStatusEntry given as parameters are converted
     * to an STL pair and insert into the database map.
     * \param 
    */
    void Update (uint32_t iface, NeighborStatusEntry* nse);

    /**
     * \brief Print the database
     * 
    */
    void Print (std::ostream &os) const;
  private:
    typedef std::map<uint32_t, NeighborStatusEntry*> 
        NSDBMap_t;  //!< container of <interface, NeighborStatusEntry>
    typedef std::pair<uint32_t, NeighborStatusEntry*> 
        NSDBPair_t; //!< pair of <interface, NeighborStatusEntry>
    NSDBMap_t m_database; //!< database of <interface, NeighborStatusEntry>
};

}

#endif /* NEIGHBOR_STATUS_Database_H */