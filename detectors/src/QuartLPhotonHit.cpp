#include "QuartLPhotonHit.h"

ClassImp(Quartic::QuartLPhotonHit)

namespace Quartic
{
  QuartLPhotonHit::QuartLPhotonHit() :
    fParentID(-1)
  {}
  
  QuartLPhotonHit::QuartLPhotonHit(int par_id) :
    fParentID(par_id)
  {}
  
  QuartLPhotonHit::~QuartLPhotonHit()
  {}
}
