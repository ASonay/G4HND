
#include "NDEventAction.hh"

#include "NDRecorder.hh"
#include "G4Event.hh"
#include "Randomize.hh"


NDEventAction::NDEventAction(NDRecorder *rec)
    : recorder(rec)
{
}


NDEventAction::~NDEventAction()
{
}


void NDEventAction::BeginOfEventAction(const G4Event *evt)
{
    recorder->BeginOfRecordingEvent(evt);
}


void NDEventAction::EndOfEventAction(const G4Event *evt)
{
    recorder->EndOfRecordingEvent(evt);
}
