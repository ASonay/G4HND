#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "stdio.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TCut.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

void simulation_his(Int_t pat)
{
  int bin_val=20;
  int bin2=2;
  float min=0;
  float max=20000;
  
  float bin = (max-min)/bin2;
   
  char fop[1000];
  char fwr[1000];

  Double_t ex;

  FILE *fopE;

  TH1F *Eh;

  TFile *fSim;
  TTree *tr;

  TCut basic = "Egamma==0&&Enp_qf>0";

  int j = pat;

  //sprintf(fop,"./root_files/gamma/gamma_%dkeV.root",j);
  //sprintf(fwr,"./response_matrix/bin%dkeV_gamma/%dkeV.dat",bin2,j);
  sprintf(fop,"./root_files/sim_files_g4102/neutron_%dkeV.root",j);
  sprintf(fwr,"./response_matrix/neutron_g4102_bin2/%dkeV.dat",j);

  fSim = new TFile(fop);
  tr = (TTree*) fSim->Get("tr");
  Eh = new TH1F("Eh", "", bin, min, max);
  tr->Project("Eh","Enp_qf*1000",basic);

  fopE = fopen(fwr,"w+");
  for(int i=0;i<=bin;i++)
    {
      ex=Eh->GetBinContent(i+1);
      fprintf(fopE,"%f %f\n",(float)bin2/2000.+(float)i*(float)bin2/1000.,ex);
      ex=0;
    }
  fclose(fopE);
 
  Eh->Delete();

}
