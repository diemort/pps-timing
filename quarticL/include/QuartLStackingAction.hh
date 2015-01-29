#ifndef QuartLStackingAction_H
#define QuartLStackingAction_H 1

#include "G4UserStackingAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"

#include "globals.hh"

class QuartLStackingAction : public G4UserStackingAction
{
  public:
    QuartLStackingAction();
   ~QuartLStackingAction();

  public:
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    void NewStage();
    void PrepareNewEvent();

  private:
    G4int gammaCounter;
};

#endif

