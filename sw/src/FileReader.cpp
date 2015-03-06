#include "FileReader.h"

ClassImp(PPS::FileReader)

namespace PPS
{
  FileReader::FileReader() :
    fFilename(""), fFile(0), fRun(0), fEvent(0)
  {}

  FileReader::FileReader(TString file) :
    fFilename(file), fFile(0), fRun(0), fEvent(0)
  {
    Open(fFilename);
  }

  FileReader::~FileReader()
  {
    if (fFile) {
      if (fFile->IsOpen()) fFile->Close();
      delete fFile;
    }
  }

  bool
  FileReader::Open(TString file)
  {
    fFilename = file;
    if (fFilename=="") return false;

    fFile = new TFile(fFilename, "READ");
    if (!fFile->IsOpen()) return false;

    // Extract all the information on the run to produce these events.
    fFile->GetObject("runs", fRun);
    if (!fRun) return false;

    // Extract all the events content.
    fFile->GetObject("events", fEvent);
    if (!fEvent) return false;

    return true;
  }
}
