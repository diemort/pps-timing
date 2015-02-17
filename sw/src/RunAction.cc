#include "RunAction.hh"

RunAction::RunAction(FileWriter* analyzer) :
  fOutput(analyzer)
{
  fTimer = new G4Timer;
  fMaterialManager = new MaterialManager;
  fRunInfo = new PPS::RunInformation;
}

RunAction::~RunAction()
{
  delete fTimer;
  delete fMaterialManager;
  delete fRunInfo;
}

void
RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "##### Run " << aRun->GetRunID() << " is starting ! #####" << G4endl;
  fTimer->Start();
}

void
RunAction::EndOfRunAction(const G4Run* aRun)
{   
  fTimer->Stop();
  G4cout << " Number of events = "
         << aRun->GetNumberOfEvent() << G4endl
         << " Timing : " << *fTimer << G4endl;
  fOutput->StoreRun();
}
