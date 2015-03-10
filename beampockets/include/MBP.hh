#ifndef MBP_h
#define MBP_h

#include "G4GDMLParser.hh"
#include "globals.hh"

#include "GeometryComponent.hh"

namespace MBP
{
  class MBP : public PPS::GeometryComponent
  {
  public:
    MBP(G4String name="");
    virtual ~MBP();
    
    G4VPhysicalVolume* BuildOneStation();
    inline void BeforeConstruct() {}
    inline G4VPhysicalVolume* Construct() { return BuildOneStation(); }
    
  private:
    G4GDMLParser* fParser;
    G4VPhysicalVolume* fWorld;
  };
}

#endif
