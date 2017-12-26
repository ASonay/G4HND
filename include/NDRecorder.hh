#ifndef NDRecorder_h
#define NDRecorder_h 1

#include "NDRecorderMessenger.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "globals.hh"

#include "Rtypes.h"  // Basic types used by ROOT.
#include "TROOT.h"   // Entry point to the system.
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TNtuple.h"


class G4Run;
class G4Event;
class G4Step;

class NDTrajectory;


class NDRecorder
{
public:
  NDRecorder();
  ~NDRecorder();

  void BeginOfRecordingRun(const G4Run *);
  void EndOfRecordingRun(const G4Run *); 
    
  void BeginOfRecordingEvent(const G4Event *);
  void EndOfRecordingEvent(const G4Event *);

  void BeginOfRecordingStep(const G4Step *);
  void EndOfRecordingStep(const G4Step *);

  void ResetAllMembers();
  void SetFileName(const G4String &);

private:
  TFile *rootFile;
  TTree *tr, *tra, *trb;
  
  //---
  //---
  //---
  //---
  G4double ratio_R;
  G4int evtTot,evtNb;
  G4int mul_evt;
  //---
  
  Char_t parName[100];
  Char_t procName[100];
  Char_t volName[100];
  G4int hit_ND;
  G4int index_ctrl;
  G4int ntc,ntc_p;
  G4int ccountPar,countPar,countParnuebar,countParelectron,countParpositron,countParalpha;
  G4int countParDec[500];
  G4int countAlphaDec[500],countAlphaDec_tot[500];
  G4int countBetaDec[500],countBetaDec_tot[500];
  G4int getTrackDec[500];
  G4int nindex_tc[500];
  G4double Emean_alpha[500];
  G4double Emean_beta[500];
  G4double total_time[500];
  G4double Ee_tc[500],Er_tc[500],Eqr_tc[500];
  G4double time_inf[500],timeloc_inf[500];
  G4int ND_trig;
  G4int evt_id,evt_id2;
  G4int idx;
  G4int alphaID1, alphaID2, alphaID3, alpha_tmp;
  G4int trackID,parentID;
  G4int pretrackID0,pretrackID1,pretrackID2,pretrackID3,pretrackID4,pretrackID5,pretrackID6;
  G4int icnp,icnc,icnBe9,icn3a;
  G4int alpha_ctrl,beta_ctrl;
  G4int index_alpha,index_beta;
  G4double E_alpha_qf,E_alpha,edep_alpha;
  G4double E_beta,edep_beta;
  G4double edep,edep_np,edep_nc,edep_nBe9,edep_n3a,edep_gamma,edep_tn;
  G4double x, y, z;
  G4double x1, y1, z1, r1, h1;
  G4double Etot,Egamma,Enp,Enc,EnBe9,En3a,Etn,Enp_qf,En3a_qf;
  G4String rootFileName;
  G4String particleName;
  G4String parNameDec[500];
  G4String volumeName;
  G4String processName;
  G4String nul;
  G4double locT, globT, propT;
  G4double tdc,livT;
  G4double dt_bi_po,dt_rn_po;
  NDRecorderMessenger *recMessenger;
};


#endif
