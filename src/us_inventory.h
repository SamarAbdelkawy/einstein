#ifndef EINSTEIN_SRC_US_INVENTORY_H_
#define EINSTEIN_SRC_US_INVENTORY_H_

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "cyclus.h"

#pragma cyclus exec from cyclus.system import CY_LARGE_DOUBLE

namespace einstein {

/// This facility represents a supply inventory of used nuclear fuel assemblies.
/// It reads assembly masses and isotopic compositions from CSV files, stores
/// them internally as inventory bins, and offers material on a configured
/// output commodity. Material is supplied from the available bins subject to a
/// per-timestep throughput limit.
class USInventory : public cyclus::Facility {
 public:
  USInventory(cyclus::Context* ctx);

  virtual ~USInventory();

  #pragma cyclus note { \
    "doc": "This facility represents a supply inventory of used nuclear fuel " \
           "assemblies. It reads assembly masses and isotopic compositions " \
           "from CSV files, stores them internally as inventory bins, and " \
           "offers material on a configured output commodity. Material is " \
           "supplied from the available bins subject to a per-timestep " \
           "throughput limit.", \
  }

  #pragma cyclus def clone
  #pragma cyclus def schema
  #pragma cyclus def annotations
  #pragma cyclus def infiletodb
  #pragma cyclus def snapshot
  #pragma cyclus def snapshotinv
  #pragma cyclus def initinv

  virtual void InitFrom(USInventory* m);

  virtual void InitFrom(cyclus::QueryableBackend* b);

  virtual void Tick() {};

  virtual void Tock() {};

  virtual std::string str();

  virtual void EnterNotify();

  virtual std::set<cyclus::BidPortfolio<cyclus::Material>::Ptr>
  GetMatlBids(cyclus::CommodMap<cyclus::Material>::type& commod_requests);

  virtual void GetMatlTrades(
      const std::vector<cyclus::Trade<cyclus::Material> >& trades,
      std::vector<std::pair<cyclus::Trade<cyclus::Material>,
                            cyclus::Material::Ptr> >& responses);

 private:
  #pragma cyclus var { \
    "tooltip": "Commodity this facility supplies.", \
    "doc": "Output commodity on which the USInventory facility offers " \
           "used nuclear fuel material.", \
    "uilabel": "Output Commodity", \
    "uitype": "outcommodity", \
  }
  std::string outcommod;

  #pragma cyclus var { \
    "tooltip": "Path to assemblies CSV file.", \
    "doc": "Path to a CSV file containing assembly IDs and total available " \
           "masses. Expected columns include assembly_id and total_mass_kg. " \
           "An optional count column may be used to scale the total mass.", \
    "uilabel": "Assemblies File", \
  }
  std::string assemblies_file;

  #pragma cyclus var { \
    "tooltip": "Path to composition CSV file.", \
    "doc": "Path to a CSV file containing isotopic mass fractions for each " \
           "assembly. Expected columns include assembly_id, nuclide, and " \
           "mass_fraction.", \
    "uilabel": "Composition File", \
  }
  std::string composition_file;

  #pragma cyclus var { \
    "default": CY_LARGE_DOUBLE, \
    "tooltip": "Maximum material mass this facility can supply per time step.", \
    "units": "kg/(time step)", \
    "uilabel": "Maximum Throughput", \
    "uitype": "range", \
    "range": [0.0, CY_LARGE_DOUBLE], \
    "doc": "amount of commodity that can be supplied at each time step", \
  }
  double throughput_kg;

  #pragma cyclus var { \
    "default": True, \
    "tooltip": "Allow partial fulfillment of material requests.", \
    "doc": "If true, the facility may partially satisfy a material trade when " \
           "the requested mass exceeds the available mass or remaining " \
           "throughput. If false, trades are only fulfilled when the full " \
           "requested quantity can be supplied.", \
    "uilabel": "Allow Partial Fulfillment", \
  }
  bool allow_partial;

  struct Bin {
    std::string assembly_id;
    double available_kg;
    cyclus::Composition::Ptr comp;
  };

  std::vector<Bin> bins_;

  std::unordered_map<std::string, size_t> idx_;

  void LoadAssembliesCSV_(const std::string& path);

  void LoadCompositionCSV_(const std::string& path);

  int NucIdFromString_(const std::string& s) const;
};

}  // namespace einstein

//comment
#endif  // EINSTEIN_SRC_US_INVENTORY_H_