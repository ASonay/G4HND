
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "Randomize.hh"


//User Scripts
//------------------------------------------
#include "NDDetectorConstruction.hh"
#include "NDPrimaryGeneratorAction.hh"
#include "NDRunAction.hh"
#include "NDEventAction.hh"
#include "NDSteppingAction.hh"
#include "NDRecorder.hh"
//------------------------------------------

//Physic List
#include "PhysicsList.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char **argv)
{
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
  
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif 
 
      
  //------------------------
  runManager->SetUserInitialization(new NDDetectorConstruction);
  runManager->SetUserInitialization(new PhysicsList);
 
  G4VUserPrimaryGeneratorAction *gen_action = new NDPrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);

  NDRecorder *recorder = new NDRecorder();
  
  G4UserRunAction *run_action = new NDRunAction(recorder);
  runManager->SetUserAction(run_action);
  G4UserEventAction *event_action = new NDEventAction(recorder);
  runManager->SetUserAction(event_action);
  G4UserSteppingAction *stepping_action = new NDSteppingAction(recorder);
  runManager->SetUserAction(stepping_action);
  //------------------------
  
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( ! ui ) { 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;

  return 0;
}
