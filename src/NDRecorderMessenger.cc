#include "NDRecorder.hh"
#include "NDRecorderMessenger.hh"


//#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"

NDRecorderMessenger::NDRecorderMessenger(NDRecorder *rec)
  : Recorder(rec)
{

  RecMsg = new G4UIdirectory("/ND/RecMsg/");
  RecMsg->SetGuidance("UI commands specific to NDRecorder");

  FileName = new G4UIcmdWithAString("/ND/RecMsg/FileName", this);
  FileName->SetGuidance("Specify the output ROOT file name.");
  FileName->SetParameterName("fName", false);
  FileName->AvailableForStates(G4State_PreInit, G4State_Idle);
}


NDRecorderMessenger::~NDRecorderMessenger()
{
  delete RecMsg;
  delete FileName;
}

void NDRecorderMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{	
  if(command == FileName)
    Recorder->SetFileName(newValue);
}
