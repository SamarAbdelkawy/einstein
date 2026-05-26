#include "us_inventory.h"

namespace einstein {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
us_inventory::us_inventory(cyclus::Context* ctx) : cyclus::Facility(ctx) {}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string us_inventory::str() {
  return Facility::str();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void us_inventory::Tick() {}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void us_inventory::Tock() {}

// WARNING! Do not change the following this function!!! This enables your
// archetype to be dynamically loaded and any alterations will cause your
// archetype to fail.
extern "C" cyclus::Agent* Constructus_inventory(cyclus::Context* ctx) {
  return new us_inventory(ctx);
}

}  // namespace einstein
