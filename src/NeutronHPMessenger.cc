#include "NeutronHPMessenger.hh"

#include "NeutronHPphysics.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

NeutronHPMessenger::NeutronHPMessenger(NeutronHPphysics* phys)
:G4UImessenger(),fNeutronPhysics(phys),
 fPhysDir(0), fThermalCmd(0)
{ 
  fPhysDir = new G4UIdirectory("/testhadr/phys/");
  fPhysDir->SetGuidance("physics list commands");
   
  fThermalCmd = new G4UIcmdWithABool("/testhadr/phys/thermalScattering",this);
  fThermalCmd->SetGuidance("set thermal scattering model");
  fThermalCmd->SetParameterName("thermal",false);
  fThermalCmd->AvailableForStates(G4State_PreInit);  
}


NeutronHPMessenger::~NeutronHPMessenger()
{
  delete fThermalCmd;
  delete fPhysDir;
}


void NeutronHPMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{   
  if (command == fThermalCmd)
   {fNeutronPhysics->SetThermalPhysics(fThermalCmd->GetNewBoolValue(newValue));}
}

