#ifndef DetectorSD_h
#define DetectorSD_h 1

#include "QuartLRunAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4VSensitiveDetector.hh"

class G4Step;
class RunAction;

class DetectorSD: public G4VSensitiveDetector 
{
  public:
    DetectorSD(G4String);
    ~DetectorSD();

    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    
  private:
    RunAction* runAction;
    G4double detEnergy;
    G4double detTime;		//Track Time
    G4double firstPh;	        //First Photon
};

#endif
