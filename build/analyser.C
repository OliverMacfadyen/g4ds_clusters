#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TH1.h"
#include "iostream"
#include "dstree_oldrooter.hh"
#include "fstream"
#include <vector>
#include <string>

using namespace std ;

//To be executed with: root -l 'analyser.C("/storage/gpfs_ds50/darkside/users/mattrossi/MC/veto_newplan/g4ds11/build/neutrons_alternatives/neutrons_ref_wOP_bgd_sipm", "test_output.output")'

  void analyser(const char* sim_path, const char* ofile_name) {

  string root_chain = string(sim_path) + "/outrun*.root";

  TChain * dstree=new TChain("dstree");
  dstree->Add(root_chain.c_str());  

  TChain * nentree=new TChain("nentree");
  nentree->Add(root_chain.c_str());

  ofstream ofile(ofile_name);


  // Gamma rates root file
  /*
  TFile * fin = TFile::Open("/storage/gpfs_ds50/darkside/users/mattrossi/MC/DS/branch_planc/g4ds10/Linux-g++/sim_gamma/output.root") ;
  TH1D * htpc = (TH1D*) fin->Get("tpc");
  TH1D * hvetoOut = (TH1D*) fin->Get("vetoOut");      
  TH1D * htpc_Ar39 = (TH1D*) fin->Get("tpc_39Ar"); 
  TH1D * hvetoOut_Ar39 = (TH1D*) fin->Get("vetoOut_39Ar"); 
  */


//**********************************************************

  SetBranches (dstree) ;

   
  dstree->SetBranchStatus("*",0) ; 
  
  dstree->SetBranchStatus("ev",1) ;
  dstree->SetBranchStatus("nclus",1) ; 
 
  dstree->SetBranchStatus("ndeposits",1) ; 
  //dstree->SetBranchStatus("dep_mat",1) ;
  //dstree->SetBranchStatus("dep_z",1) ;
  //dstree->SetBranchStatus("dep_ene",1) ;
 
  dstree->SetBranchStatus("cl_ene",1) ; 
  
  dstree->SetBranchStatus("cl_nucl",1) ; 
  dstree->SetBranchStatus("cl_elec",1) ; 
  dstree->SetBranchStatus("cl_x",1) ; 
  dstree->SetBranchStatus("cl_y",1) ; 
  dstree->SetBranchStatus("cl_z",1) ; 
  
  dstree->SetBranchStatus("isFV30",1) ; 
  dstree->SetBranchStatus("isFV5",1) ;
  //dstree->SetBranchStatus("isFV40",1) ;
  //dstree->SetBranchStatus("isFV45",1) ;


  dstree->SetBranchStatus("late_qdepMat",1) ; 
  dstree->SetBranchStatus("prompt_qdepMat",1) ; 

  dstree->SetBranchStatus("prompt_eneVeto_Ar",1);
  dstree->SetBranchStatus("late_eneVeto_Ar",1);


  dstree->SetBranchStatus("cap_x",1) ;
  dstree->SetBranchStatus("cap_y",1) ;
  dstree->SetBranchStatus("cap_z",1) ;
  dstree->SetBranchStatus("pdgCap",1) ;
  dstree->SetBranchStatus("cap_time",1);

  dstree->SetBranchStatus("nclusNR",1) ;
  dstree->SetBranchStatus("nclusER",1) ;
  dstree->SetBranchStatus("energyER",1) ;
  dstree->SetBranchStatus("goodNRidx",1) ;
  dstree->SetBranchStatus("energyERinel",1) ;


//******************************************************


  int nn =  0 ;
  int nneutrons = 0 ;
  nentree->SetBranchAddress("nentries", &nn ) ;
  for(int m=0;m<nentree->GetEntries();++m)  {
    nentree->GetEntry(m) ;
    nneutrons += nn ;
  }

  int n0 = 0, n1 = 0 , n2 = 0, n3 = 0, n4 = 0, n5 =0; 

    
  //THRESHOLD ENERGIES

  vector<float> energyER_th;
  vector<float> ene_veto_th;

  energyER_th.push_back(50);

  ene_veto_th.push_back(200);

  vector<int> nn_before_veto(energyER_th.size(),0);
  vector<vector<int>> nn_after_veto_blind(energyER_th.size(),vector<int> (ene_veto_th.size(),0));
 
  /*
  vector<double> tpcRate(energyER_th.size(),0);
  vector<double> vetoRate(ene_veto_th.size(),0);
  vector<double> tpcRate_Ar39(energyER_th.size(),0);
  vector<double> vetoRate_Ar39(ene_veto_th.size(),0);
  */
//******************************************************** CUTS *************************************************************

for (int k=0;k<dstree->GetEntries();++k) { 
  
	if (dstree->GetEntries() >= 10 && k%(int (dstree->GetEntries()/10.))==0) cout << k/(dstree->GetEntries()/10.)*10 << "%" << endl ;

        dstree->GetEntry(k); 

        float ene_veto = late_eneVeto_Ar;

        if (nclusNR!=1) continue ;
        n0 +=1;

        if (isFV30[goodNRidx] == 0) continue;
        n1++;

        if (abs(cl_z[goodNRidx])>100) continue;     
        n2++;

        if (cl_elec[goodNRidx] > cl_nucl[goodNRidx]) continue ; 
        n3 += 1 ; 

        if (energyERinel > 10) continue;
        n4 +=1;

        if (cl_ene[goodNRidx] > 50 || cl_ene[goodNRidx]< 7.5 ) continue;
        n5 +=1;


	for (int i=0;i<int(energyER_th.size());i++) {    

	        if (energyER > energyER_th[i]) continue;

		nn_before_veto[i]++;
	
                //tpcRate[i] = htpc->GetBinContent(299) - htpc->GetBinContent(energyER_th[i]/10); 
		//tpcRate_Ar39[i] = htpc_Ar39->GetBinContent(299) - htpc_Ar39->GetBinContent(energyER_th[i]/10); 

		for (int j=0;j<int(ene_veto_th.size());j++) {

			//vetoRate[j] = hvetoOut->GetBinContent(299) - hvetoOut->GetBinContent(ene_veto_th[j]/10);
			//vetoRate_Ar39[j] = hvetoOut_Ar39->GetBinContent(299) - hvetoOut_Ar39->GetBinContent(ene_veto_th[j]/10);		
 			
			if (ene_veto < ene_veto_th[j]) nn_after_veto_blind[i][j]++;

    		}
    			
	}


} 



//******************************************************** Dead time fraction computation *************************************************************
/*
double DeltaT = 800e-6;   //seconds
double DeltaT_pileup  = 200e-9;  //seconds

vector<double> deadtime_TPC(energyER_th.size(),0);
vector<double> deadtime_Veto(ene_veto_th.size(),0);


for (int i=0;i<int(energyER_th.size());i++) {   

	double pileup_Ar39_Ar39_TPC = tpcRate_Ar39[i]*tpcRate_Ar39[i]*DeltaT_pileup;
	double pileup_gamma_gamma_TPC = tpcRate[i]*tpcRate[i]*DeltaT_pileup;
	double pileup_Ar39_gamma_TPC = 2*tpcRate_Ar39[i]*tpcRate[i]*DeltaT_pileup;

	deadtime_TPC[i] = (tpcRate_Ar39[i] + tpcRate[i] + pileup_Ar39_Ar39_TPC + pileup_gamma_gamma_TPC + pileup_Ar39_gamma_TPC)*DeltaT;

}


for (int i=0;i<int(ene_veto_th.size());i++) {

	double pileup_Ar39_Ar39_Veto = vetoRate_Ar39[i]*vetoRate_Ar39[i]*DeltaT_pileup;
	double pileup_gamma_gamma_Veto = vetoRate[i]*vetoRate[i]*DeltaT_pileup;
	double pileup_Ar39_gamma_Veto = 2*vetoRate_Ar39[i]*vetoRate[i]*DeltaT_pileup;
	
	deadtime_Veto[i] = (vetoRate_Ar39[i] + vetoRate[i] + pileup_Ar39_Ar39_Veto + pileup_gamma_gamma_Veto + pileup_Ar39_gamma_Veto)*DeltaT;
}

*/
//********************************************************************************************************************************************



  ofile << "Number of simulated neutrons: "<< nneutrons << endl; 
  ofile << "Remaining after single cluster cut: " << n0 << endl; 
  ofile << "Remaining after FV cut: " << n1 << endl;
  ofile << "Remaining after cl_z cut: " << n2 << endl;
  ofile << "Remaining after NR cut: " <<  n3 << endl; 
  ofile << "Remaining after inelastic cut: " <<  n4 << endl;
  ofile << "Remaining after ROI cut: " << n5 << endl;
  
  ofile << " " << endl;

  for (int i=0;i<int(energyER_th.size());i++) {
 
  for (int j=0;j<int(ene_veto_th.size());j++) {

  ofile << ene_veto_th[j] << " keV veto threshold and " << energyER_th[i] << " keV TPC threshold: " <<endl;
  
  ofile << "nn_before_veto: "  << nn_before_veto[i] << endl;
  
  ofile << "Not tagged by veto:" << nn_after_veto_blind[i][j] << endl; 
  
  ofile << "Combined inefficiency: " << double(nn_after_veto_blind[i][j])/nneutrons << endl;
  
  ofile << "Veto inefficiency: "  << double(nn_after_veto_blind[i][j])/nn_before_veto[i] << endl;
 
  //ofile << "TPC rate Ar39: "  << tpcRate_Ar39[i] << " TPC rate gammas: " << tpcRate[i] << endl;

  //ofile << "Veto rate Ar39: " <<  vetoRate_Ar39[j] << " Veto rate gammas: " << vetoRate[j] << endl;

  //ofile << "Dead time fraction TPC: " << deadtime_TPC[i] <<  endl;

  //ofile << "Dead time fraction Veto: " << deadtime_Veto[j] <<  endl;

  //ofile << "Total dead time: " << deadtime_Veto[j] + deadtime_TPC[i]<<  endl;


  ofile << " " << endl;
 
  }
  }
} 
