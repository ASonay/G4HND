//---------------------------------------------------------------------//
//---------------------------------------------------------------------//

#include "NDRecorder.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Step.hh"

#include "G4RunManager.hh"
#include "G4VProcess.hh"
#include "G4TrajectoryContainer.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Track.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "math.h"


#define SIGNAL_COUT

//---------------------------------------------------------------------//

NDRecorder::NDRecorder()
{
  ND_trig=0;
  rootFileName = "knownFile.root";
  recMessenger = new  NDRecorderMessenger(this);
}

//---------------------------------------------------------------------//

NDRecorder::~NDRecorder()
{
  delete rootFile;
  delete recMessenger;
}

//---------------------------------------------------------------------//

void NDRecorder::BeginOfRecordingRun(const G4Run *aRun)
{
  ccountPar=0; countPar=0; countParnuebar=0; countParelectron=0; countParpositron=0; countParalpha=0;
  evt_id=0;
 
  for (int i=0;i<500;i++){
    parNameDec[i] = "NULL";
    countAlphaDec_tot[i]=0;
    countBetaDec_tot[i]=0;
    countAlphaDec[i]=0;
    countBetaDec[i]=0;
    countParDec[i]=0;
  }
  
  G4cout << "### Run " << aRun->GetRunID() << " start." << " Last Event: " << aRun->GetNumberOfEventToBeProcessed() <<G4endl;
  
  G4cout << "Create ROOT file to record data .........." << G4endl;

  rootFile = new TFile(rootFileName, "recreate");

  if (rootFile->IsZombie()) {
    G4cout << "Error opening file" << G4endl;
    exit(-1);
  }
  else {
    tr = new TTree("tr", "ND Detector Simulation");
    tra = new TTree("tra", "ND Detector Simulation Alpha Branch");
    trb = new TTree("trb", "ND Detector Simulation Beta Branch");

    // ID
    tr->Branch("evt_id", &evt_id, "evt_id/I");
    
    tr->Branch("locT", &locT, "locT/D");
    tr->Branch("globT", &globT, "globT/D");
    tr->Branch("propT", &propT, "propT/D");

    tr->Branch("parName", &parName, "parName[20]/C");
    tr->Branch("volName", &volName, "volName[20]/C");
    tr->Branch("procName", &procName, "procName[20]/C");
	
    // position
    tr->Branch("x", &x, "x/D");
    tr->Branch("y", &y, "y/D");
    tr->Branch("z", &z, "z/D");
    
    // energy deposition
    tr->Branch("Egamma", &Egamma, "Egamma/D");
    tr->Branch("Enp", &Enp, "Enp/D");
    tr->Branch("Enc", &Enc, "Enc/D");
    tr->Branch("EnBe9", &EnBe9, "EnBe9/D");
    tr->Branch("En3a", &En3a, "En3a/D");
    tr->Branch("Etn", &Etn, "Etn/D");
    tr->Branch("Enp_qf", &Enp_qf, "Enp_qf/D");
    tr->Branch("En3a_qf", &En3a_qf, "En3a_qf/D");

    tr->Branch("Er_tc", Er_tc, "Er_tc[500]/D");
    tr->Branch("Ee_tc", Ee_tc, "Ee_tc[500]/D");
    tr->Branch("Eqr_tc", Eqr_tc, "Eqr_tc[500]/D");
    
    //Interaction Counting
    tr->Branch("ic_np", &icnp, "ic_np/I");
    tr->Branch("ic_nc", &icnc, "ic_nc/I");
    tr->Branch("ic_nBe9", &icnBe9, "ic_nBe9/I");
    tr->Branch("ic_n3a", &icn3a, "ic_n3a/I");

  
    tr->Branch("tdc", &tdc, "tdc/D");
    tr->Branch("livT", &livT, "livT/D");
    tr->Branch("DT_Bi_Po", &dt_bi_po, "DT_Bi_Po/D");
    tr->Branch("DT_Rn_Po", &dt_rn_po, "DT_Rn_Po/D");
    
    tra->Branch("Ealpha_qf", &E_alpha_qf, "Ealpha_qf/D");
    tra->Branch("Ealpha", &E_alpha, "Ealpha/D");
    trb->Branch("Ebeta", &E_beta, "Ebeta/D");
    
  }

  mul_evt = aRun->GetNumberOfEventToBeProcessed();
}

//---------------------------------------------------------------------//
void NDRecorder::EndOfRecordingRun(const G4Run *aRun)
{

  G4double Emeana,Emeanb;
  
  G4cout << std::setw(17) << "  Name:"
	 << std::setw(14) << "  NoE:"
	 << std::setw(9) << "  NoE Alpha:"
	 << std::setw(9) << "  NoE Beta:"
	 << std::setw(11) << "  Emean:"
	 << std::setw(17) << "  Total Time:" <<G4endl;
  for (int i=0;i<countPar;i++)
    {
      if (countAlphaDec_tot[i]==0)
	Emeana=0;
      else
	Emeana=Emean_alpha[i]/(double)(countAlphaDec_tot[i]);
	  
      if (countBetaDec_tot[i]==0)
	Emeanb=0;
      else
	Emeanb=Emean_beta[i]/(double)(countBetaDec_tot[i]);
      
      G4cout << "  " << std::setw(17) <<parNameDec[i]
	     << "  " << std::setw(9) <<countParDec[i]
	     << "  " << std::setw(9) <<countAlphaDec[i]
	     << "  " << std::setw(9) <<countBetaDec[i]
	     << "  " << std::setw(11) <<Emeana+Emeanb
	     << "  " << std::setw(14) <<total_time[i]<<G4endl;  
    }

  G4cout << " Total anti electron neutrino:    " <<countParnuebar<<G4endl;
  G4cout << " Total alpha:    " <<countParalpha<<G4endl;
  G4cout << " Total electron:    " <<countParelectron<<G4endl;
  G4cout << " Total positron:    " <<countParpositron<<G4endl;
  
  
  evtNb=aRun->GetNumberOfEvent();
  ratio_R=100;
  
  G4cout <<"%"<< ratio_R <<" has Done." <<" Total Triger: "<<ND_trig<<G4endl;
   
  
  G4cout << "" << G4endl;
  G4cout << "====================================================================" << G4endl;
  
  rootFile->Write();
  rootFile->Close();

  G4cout << ".................... Close ROOT file" << G4endl;
}

//---------------------------------------------------------------------//

void NDRecorder::BeginOfRecordingEvent(const G4Event *anEvent)
{
  ResetAllMembers();

  evt_id++;
  evt_id2 = anEvent->GetEventID();
}

//---------------------------------------------------------------------//

void NDRecorder::EndOfRecordingEvent(const G4Event *anEvent)
{
  // information==========================================================//
  evtTot=mul_evt;
  evtNb = anEvent->GetEventID();
  ratio_R = 100.*((float)evtNb/(float)evtTot);

  if(evtNb%1000==0)
    G4cout <<"%"<< ratio_R <<" has Done." <<" Total Triger: "<<ND_trig<<G4endl;
  //======================================================================//
  nul = "no hit ND";
  evt_id = evt_id;

  // Chang units
  x = x / cm;
  y = y / cm;
  z = z / cm;

  E_alpha_qf=0.;
  E_alpha=0.;
  E_beta=0.;
  Egamma = edep_gamma / MeV;
  Enp = edep_np / MeV;
  Enc = edep_nc / MeV;
  EnBe9 = edep_nBe9 / MeV;
  En3a = edep_n3a / MeV;
  Etn = edep_tn / MeV;

  Enp_qf = 0.83*Enp - 2.82*(1-exp(-0.25*pow(Enp,0.93)));
  En3a_qf = 0.41*En3a - 5.9*(1-exp(-0.065*pow(En3a,1.01)));

  for (int i=0;i<500;i++)
    Eqr_tc[i] += 0.41*Er_tc[i]-5.9*(1-exp(-0.065*pow(Er_tc[i],1.01)));
 

  if (hit_ND==0)
    {
      strcpy(parName, nul.c_str());
      strcpy(volName, nul.c_str());
      strcpy(procName, nul.c_str());
      x = 0;
      y = 0;
      z = 0;
    }

  dt_rn_po = dt_rn_po / second;

  if(Egamma>0||Enp>0||Enc>0||EnBe9>0||En3a>0){
    ND_trig++;
    tr->Fill();
  }
}

//---------------------------------------------------------------------//

void NDRecorder::BeginOfRecordingStep(const G4Step *aStep)
{
  edep = aStep->GetTotalEnergyDeposit();

  G4Track *fTrack = aStep->GetTrack();   
  trackID = fTrack->GetTrackID();
  parentID = fTrack->GetParentID();
  //G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
  G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

  if (postStepPoint->GetProcessDefinedStep() != 0)
    processName = postStepPoint->GetProcessDefinedStep()->GetProcessName();

  particleName = fTrack->GetDefinition()->GetParticleName();
  volumeName = fTrack->GetVolume()->GetName();


  if (hit_ND !=1){
    locT = fTrack->GetLocalTime() / ns;
    globT = fTrack->GetGlobalTime() / ns;
    propT = fTrack->GetProperTime() / ns;
    strcpy(parName, particleName.c_str());
    strcpy(volName, volumeName.c_str());
    strcpy(procName, processName.c_str());
    x = fTrack->GetPosition().x();
    y = fTrack->GetPosition().y();
    z = fTrack->GetPosition().z();
    tdc=fTrack->GetGlobalTime() / ns;
  }
  
  if (volumeName == "LSci"){
    hit_ND=1;
    x1 = fTrack->GetPosition().x();
    y1 = fTrack->GetPosition().y();
    z1 = fTrack->GetPosition().z();
    h1 = z1;
    r1 = sqrt(x1*x1+y1*y1);

  }

  if (trackID == 1)
    {alphaID1=0; alphaID2=0; alpha_tmp=0;}
  if (particleName == "alpha" && alphaID1 == 0 && trackID != alphaID1)
      {alphaID1 = trackID; alpha_tmp += 1;}
  if (particleName == "alpha" && alphaID1 != 0 && alphaID2 == 0 && alphaID1 != trackID)
      {alphaID2 = trackID; alpha_tmp += 1;}
  if (particleName == "alpha" && alphaID1 != 0 && alphaID2 != 0 && alphaID1 != trackID && alphaID2 != trackID)
      {alphaID3 = trackID; alpha_tmp += 1;}

  
  if ((volumeName == "LSci") && edep > 0 && particleName == "proton")
    {edep_np += edep; icnp = 1;}
  if ((volumeName == "LSci") && edep > 0 && (particleName == "C12[0.0]" || particleName == "C12"))
    {edep_nc += edep; icnc = 1;}
  if ((volumeName == "LSci") && edep > 0 && particleName == "Be9[0.0]")
    {edep_nBe9 += edep; icnBe9 = 1;}
  if ((volumeName == "LSci") && alpha_tmp == 3)
    {edep_n3a += edep; icn3a = 1;}
  if ((volumeName == "LSci") && edep >0 && particleName != "proton" && particleName != "C12[0.0]" && particleName != "Be9[0.0]" && alpha_tmp == 0)
    {edep_gamma += edep;}

  if (volumeName == "TND")
    {edep_tn += edep;}
  
  //--------------------------------------
  if (fTrack->GetLocalTime()>4500 && (processName == "RadioactiveDecay")){
    time_inf[ntc] = (fTrack->GetGlobalTime());
    timeloc_inf[ntc] = fTrack->GetLocalTime();
    nindex_tc[ntc] = ntc;
    ntc++;
  }
  
  if ((volumeName == "LSci")){

    if (particleName == "alpha")
      Er_tc[ntc] += edep;
    
    if (particleName == "e-")
      Ee_tc[ntc] += edep;
  }
  //--------------------------------------  
  
  
  index_ctrl=0;
  if (volumeName == "LSci" && particleName != "anti_nu_e" && particleName != "gamma" && particleName != "e-" && particleName != "e+" && particleName != "alpha"){
    for (int i=0;i<500;i++){
      if (parNameDec[i]==particleName){
	getTrackDec[i]=fTrack->GetTrackID();
	if (processName == "RadioactiveDecay")
	  total_time[i]=fTrack->GetLocalTime()*pow(10,-9)/86400.;
	if (trackID!=pretrackID0&&total_time[i]<1e+32)
	  {countParDec[i]++; pretrackID0=trackID;}
	index_ctrl=1;
      }
    }
    if (index_ctrl==0)
      {parNameDec[countPar]=particleName; countPar++;}
  }

  if (volumeName == "LSci")
    {  
      for (int i=0;i<countPar;i++){
	if (getTrackDec[i]==parentID&&particleName=="alpha"&&trackID!=pretrackID1){
	  index_alpha=i;
	  pretrackID1=trackID;
	  if (total_time[i]<1e+32){
	    countAlphaDec[i]++;
	    alpha_ctrl=1;
	  }
	}
      }
    

      for (int i=0;i<countPar;i++){ 
	if (getTrackDec[i]==parentID&&particleName=="e-"&&trackID!=pretrackID2){
	  index_beta=i;
	  pretrackID2=trackID;
	  if (total_time[i]<1e+32){
	    countBetaDec[i]++;
	    beta_ctrl=1;
	  }
	}
      }
    }


  if (alpha_ctrl==1 && particleName == "alpha" && processName != "RadioactiveDecay")
    {
      edep_alpha+=edep;
    }
  else if (processName == "RadioactiveDecay" && getTrackDec[index_alpha]==parentID)
    {
      alpha_ctrl=0;
      E_alpha_qf = 0.41*edep_alpha-5.9*(1-exp(-0.065*pow(edep_alpha,1.01)));
      E_alpha = edep_alpha;
      Emean_alpha[index_alpha]+=E_alpha;
      countAlphaDec_tot[index_alpha]++;
      edep_alpha=0;
      if (E_alpha_qf>0)
	tra->Fill();
    }
  
  if (beta_ctrl==1 && particleName == "e-" && processName != "RadioactiveDecay")
    {
      edep_beta+=edep;
    }
  else if (processName == "RadioactiveDecay" && getTrackDec[index_beta]==parentID)
    {
      beta_ctrl=0;
      E_beta = edep_beta;
      Emean_beta[index_beta]+=E_beta;
      countBetaDec_tot[index_beta]++;
      edep_beta=0;
      if (E_beta>0)
	trb->Fill();
    }

    
  if (volumeName == "LSci" && particleName == "anti_nu_e" && trackID!=pretrackID3){
    {countParnuebar++; pretrackID3=trackID;}
  }
  if (volumeName == "LSci" && particleName == "e-" && trackID!=pretrackID4){
    {countParelectron++; pretrackID4=trackID;}
  }
  if (volumeName == "LSci" && particleName == "e+" && trackID!=pretrackID5){
    {countParpositron++; pretrackID5=trackID;}
  }
  if (volumeName == "LSci" && particleName == "alpha" && trackID!=pretrackID6){
    {countParalpha++; pretrackID6=trackID;}
  }

  if (volumeName == "LSci" && particleName == "Po212" && processName == "RadioactiveDecay")
    dt_bi_po = fTrack->GetLocalTime();
  
  if (volumeName == "LSci" && particleName == "Po218" && processName == "RadioactiveDecay")
    dt_rn_po = fTrack->GetLocalTime();

}

//---------------------------------------------------------------------//

void NDRecorder::EndOfRecordingStep(const G4Step *aStep)
{
  livT+=aStep->GetDeltaTime();
}

//---------------------------------------------------------------------//

void NDRecorder::ResetAllMembers()
{
  // Reset previous events' data
    
  x = -999.;
  y = -999.;
  z = -999.;
    
  x1 = -999.;
  y1 = -999.;
  z1 = -999.;
  r1 = -999.; h1 = -999.;
   
  hit_ND=0;

  alphaID1=0; alphaID2=0; alphaID3=0;
  alpha_tmp=0;

  Etot = 0.;
  Egamma = 0.;
  Enp = 0.;
  Enc = 0.;
  EnBe9 = 0.;
  En3a = 0.;
  Etn = 0.;

  edep = 0.;

  edep_np = 0.;
  edep_nc = 0.;
  edep_nBe9 = 0.;
  edep_n3a = 0.;
  edep_gamma = 0.;
  edep_tn = 0.;
  
  edep_alpha = 0.;
  edep_beta = 0.;

  icnp = 0;
  icnc = 0;
  icnBe9 = 0;
  icn3a = 0;

  locT=0;
  globT=0;
  propT=0;

  tdc=0;

  dt_bi_po=0;
  dt_rn_po=0;

  alpha_ctrl=0;
  beta_ctrl=0;

  for (int i=0;i<500;i++){
    getTrackDec[i]=-100;
    Er_tc[i] = 0.0;
    Ee_tc[i] = 0.0;
    Eqr_tc[i] = 0.0;
  }

  ntc = 0;
  
}

//---------------------------------------------------------------------//

void NDRecorder::SetFileName(const G4String &fName)
{
  rootFileName = fName;
}

//---------------------------------------------------------------------//
