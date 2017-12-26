
#include "NDSteppingAction.hh"

#include "NDRecorder.hh"
#include "G4Step.hh"


NDSteppingAction::NDSteppingAction(NDRecorder *rec)
    : recorder(rec)
{
}


NDSteppingAction::~NDSteppingAction()
{
}


void NDSteppingAction::UserSteppingAction(const G4Step *aStep)
{
    recorder->BeginOfRecordingStep(aStep);
    recorder->EndOfRecordingStep(aStep);
}
