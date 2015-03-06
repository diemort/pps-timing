#include "EventInformation.h"

ClassImp(PPS::EventInformation)

namespace PPS
{
  EventInformation::EventInformation() :
    TTree("events", "Information on the event"), fLocked(false)
  {
    fDetCollection = new DetectorsRef;
    Clear("");
  }

  EventInformation::~EventInformation()
  {
    delete fDetCollection;
  }
}

