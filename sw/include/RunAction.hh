#ifndef RunAction_h
#define RunAction_h

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4Timer.hh"
#include "globals.hh"

#include "FileWriter.hh"
#include "MaterialManager.hh"
#include "RunInformation.h"

#include "TFile.h"
#include "TTree.h"

class RunAction : public G4UserRunAction
{
  public:
    RunAction(FileWriter* analyzer=0);
    ~RunAction();

    void BeginOfRunAction(const G4Run* aRun);
    void EndOfRunAction(const G4Run* aRun);
    /** \brief Returns a pointer to the FileWriter object used to collect all tracks' information in an external ROOT tree */
    inline FileWriter* GetFileWriter() { return fOutput; }
    inline MaterialManager* GetMaterialManager() { return fMaterialManager; }
    inline PPS::RunInformation* GetRunInformation() { return fRunInfo; }

  private:
    G4Timer* fTimer;
    FileWriter* fOutput;
    MaterialManager* fMaterialManager;
    PPS::RunInformation* fRunInfo;
};

#endif
