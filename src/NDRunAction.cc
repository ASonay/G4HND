#include "NDRunAction.hh"

#include "NDRecorder.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"


NDRunAction::NDRunAction(NDRecorder *rec)
  : recorder(rec)
{
}


NDRunAction::~NDRunAction()
{
}


void NDRunAction::BeginOfRunAction(const G4Run *aRun)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  recorder->BeginOfRecordingRun(aRun);
}


void NDRunAction::EndOfRunAction(const G4Run *aRun)
{
  recorder->EndOfRecordingRun(aRun);
}

