#include "NeutronHPphysics.hh"

#include "NeutronHPMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"

#include "G4HadronElasticProcess.hh"
#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPThermalScatteringData.hh"
#include "G4ParticleHPElastic.hh"
#include "G4ParticleHPThermalScattering.hh"

#include "G4NeutronInelasticProcess.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPInelastic.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPCapture.hh"

#include "G4HadronFissionProcess.hh"
#include "G4ParticleHPFissionData.hh"
#include "G4ParticleHPFission.hh"

#include "G4SystemOfUnits.hh"


NeutronHPphysics::NeutronHPphysics(const G4String& name)
:  G4VPhysicsConstructor(name), fThermal(true), fNeutronMessenger(0)
{
  fNeutronMessenger = new NeutronHPMessenger(this);
}


NeutronHPphysics::~NeutronHPphysics()
{
  delete fNeutronMessenger;
}


void NeutronHPphysics::ConstructProcess()
{
  G4ParticleDefinition* neutron = G4Neutron::Neutron();
  G4ProcessManager* pManager = neutron->GetProcessManager();
   
  // delete all neutron processes if already registered
  //
  G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
  G4VProcess* process = 0;
  process = processTable->FindProcess("hadElastic", neutron);      
  if (process) pManager->RemoveProcess(process);
  //
  process = processTable->FindProcess("neutronInelastic", neutron);
  if (process) pManager->RemoveProcess(process);
  //
  process = processTable->FindProcess("nCapture", neutron);      
  if (process) pManager->RemoveProcess(process);
  //
  process = processTable->FindProcess("nFission", neutron);      
  if (process) pManager->RemoveProcess(process);      
         
  // (re) create process: elastic
  //
  G4HadronElasticProcess* process1 = new G4HadronElasticProcess();
  pManager->AddDiscreteProcess(process1);
  //
  // model1a
  G4ParticleHPElastic*  model1a = new G4ParticleHPElastic();
  process1->RegisterMe(model1a);
  process1->AddDataSet(new G4ParticleHPElasticData());
  //
  // model1b
  if (fThermal) {
    model1a->SetMinEnergy(4*eV);   
    G4ParticleHPThermalScattering* model1b = new G4ParticleHPThermalScattering();
    process1->RegisterMe(model1b);
    process1->AddDataSet(new G4ParticleHPThermalScatteringData());         
  }
   
  // (re) create process: inelastic
  //
  G4NeutronInelasticProcess* process2 = new G4NeutronInelasticProcess();
  pManager->AddDiscreteProcess(process2);   
  //
  // cross section data set
  G4ParticleHPInelasticData* dataSet2 = new G4ParticleHPInelasticData();
  process2->AddDataSet(dataSet2);                               
  //
  // models
  G4ParticleHPInelastic* model2 = new G4ParticleHPInelastic();
  process2->RegisterMe(model2);    

  // (re) create process: nCapture   
  //
  G4HadronCaptureProcess* process3 = new G4HadronCaptureProcess();
  pManager->AddDiscreteProcess(process3);    
  //
  // cross section data set
  G4ParticleHPCaptureData* dataSet3 = new G4ParticleHPCaptureData();
  process3->AddDataSet(dataSet3);                               
  //
  // models
  G4ParticleHPCapture* model3 = new G4ParticleHPCapture();
  process3->RegisterMe(model3);
   
  // (re) create process: nFission   
  //
  G4HadronFissionProcess* process4 = new G4HadronFissionProcess();
  pManager->AddDiscreteProcess(process4);    
  //
  // cross section data set
  G4ParticleHPFissionData* dataSet4 = new G4ParticleHPFissionData();
  process4->AddDataSet(dataSet4);                               
  //
  // models
  G4ParticleHPFission* model4 = new G4ParticleHPFission();
  process4->RegisterMe(model4);       
}
