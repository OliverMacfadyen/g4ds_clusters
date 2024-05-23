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
#include <sstream>
using namespace std ;

  void neutron_analysis_chain_update() {

 
//***********TO BE CHANGED FOR EACH CONFIGURATION************
  
  int num_runs = 1000;
  
  ///// Count neutrons generated at step 1 

  /*
  TChain * nentree1 =new TChain("nentree");
  const char * step1_path_1 = "/storage/gpfs_ds50/darkside/users/mattrossi/MC/BIASING/devel_biasing2/g4ds11/build/neutrons_step1_HP_highstat_sphere_7m_rootfiles/outrun";
  const char * step1_path_2 = "_final.root";  
  for (int i=0; i<num_runs; i++) {
  std::stringstream ss;
  ss << step1_path_1 << i << step1_path_2;
  std::string step1_path = ss.str();
  std::cout << step1_path << std::endl;
  TFile *file = TFile::Open(step1_path.c_str());
  if (file && file->Get("nentree")) {
  nentree1->Add(step1_path.c_str());
  } 
  else cout << "nentree not found in file " << i << endl;
  file->Close();
  }
  */

  ///// Step2: from SS vessel
  TChain * dstree=new TChain("dstree");
  const char * step2_path_1 = "/storage/gpfs_ds50/darkside/users/mattrossi/MC/hybrid/g4ds11/build/neutrons_step2_HP_highstat_sphere_7m_rootfiles/outrun";
  const char * step2_path_2 = ".root";
  for (int i=0; i<num_runs; i++) {
  std::stringstream ss;
  ss << step2_path_1 << i << step2_path_2;
  std::string step2_path = ss.str();
  std::cout << step2_path << std::endl;
  TFile *file = TFile::Open(step2_path.c_str());
  if (file && file->Get("dstree")) {
  dstree->Add(step2_path.c_str());
  }
  else cout << "dstree not found in file " << i << endl; 
  file->Close();
  }


  ofstream ofile("analysis.output");

//**********************************************************

  SetBranches (dstree) ;

   
  dstree->SetBranchStatus("*",0) ; 
  
  dstree->SetBranchStatus("ev",1) ;
  dstree->SetBranchStatus("nclus",1) ; 

  dstree->SetBranchStatus("z",1) ;
 
  dstree->SetBranchStatus("ndeposits",1) ; 
 
  dstree->SetBranchStatus("cl_ene",1) ; 
  
  dstree->SetBranchStatus("cl_nucl",1) ; 
  dstree->SetBranchStatus("cl_elec",1) ; 
  dstree->SetBranchStatus("cl_x",1) ; 
  dstree->SetBranchStatus("cl_y",1) ; 
  dstree->SetBranchStatus("cl_z",1) ; 
  
  dstree->SetBranchStatus("isFV30",1) ; 

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

  dstree->SetBranchStatus("nusers",1) ;
  dstree->SetBranchStatus("userdouble0",1) ;

//******************************************************
  /////  Count neutrons generated at step 1 
  int nentries1 =  0 ;
  double nneutrons1 = 0 ;
  nentree1->SetBranchAddress("nentries", &nentries1 ) ;
  for(int m=0;m<nentree1->GetEntries();++m)  {
    nentree1->GetEntry(m) ;
    nneutrons1 += nentries1 ;
  }
  /////

  double n0 = 0, n1 = 0 , n2 = 0, n3 = 0, n4 = 0, n5 =0; 

 
  //THRESHOLD ENERGIES

  vector<float> energyER_th;
  vector<float> ene_veto_th;

  energyER_th.push_back(50);
  ene_veto_th.push_back(200);

  vector<double> nn_before_veto(energyER_th.size(),0);
  vector<vector<double> > nn_after_veto_blind(energyER_th.size(),vector<double> (ene_veto_th.size(),0));

  double sum_squared = 0;
  int n_biased_ev = 0;

  TH1D * ene_tpc_h = new TH1D ("ene_tpc", "Energy deposited in the TPC", 200, 0, 16000 ) ;
  TH1D * ene_veto_h = new TH1D ("ene_veto", "Energy deposited in the veto", 200, 0, 16000 ) ;
  
//******************************************************** CUTS *************************************************************
for (Long64_t k=0;k<dstree->GetEntries();k++) { 
  
	if (k%(Long64_t (dstree->GetEntries()/10.))==0) cout << k/(dstree->GetEntries()/10.)*10 << "%" << endl ;

        dstree->GetEntry(k); 
        
        float ene_veto = late_eneVeto_Ar;
        double w = userdouble0;
    
        ene_tpc_h->Fill(energyER,w);
        ene_veto_h->Fill(ene_veto,w);

        if (nclusNR!=1) continue ;
        n0 += w;

        if (isFV30[goodNRidx] == 0) continue;
        n1 += w;

        if (abs(cl_z[goodNRidx])>100) continue;     
        n2 += w;

        if (cl_elec[goodNRidx] > cl_nucl[goodNRidx]) continue ; 
        n3 += w ; 

        if (energyERinel > 10) continue;
        n4 += w;

        if (cl_ene[goodNRidx] > 50 || cl_ene[goodNRidx]< 7.5 ) continue;
        n5 += w;

        ofile << "cl_ene: " << cl_ene[goodNRidx] << endl;
        ofile << "cl_elec: " << cl_elec[goodNRidx] << endl;
        ofile << "cl_nucl: " << cl_nucl[goodNRidx] << endl;
        ofile << "energyER: " << energyER << endl;
        ofile << "ene_veto: " << ene_veto << endl;

	for (int i=0;i<int(energyER_th.size());i++) {    
	        if (energyER > energyER_th[i]) continue;
		nn_before_veto[i] += w;
		for (int j=0;j<int(ene_veto_th.size());j++) {
			if (ene_veto < ene_veto_th[j]) {
				nn_after_veto_blind[i][j] += w;
				sum_squared += w*w;
				n_biased_ev++;
    			}
		}
	}
} 

  sum_squared = sqrt(sum_squared);

  ofile << "Number of simulated neutrons: "<< nneutrons1 << endl; 
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
  ofile << "Not tagged by veto - biased: " << n_biased_ev << endl;
  ofile << "Not tagged by veto: " << nn_after_veto_blind[i][j] << " +/- " << sum_squared << endl; 
  ofile << "Combined inefficiency: " << nn_after_veto_blind[i][j]/nneutrons1<< endl;
  ofile << "Veto inefficiency: "  << nn_after_veto_blind[i][j]/nn_before_veto[i] << endl;
 
  ofile << " " << endl;
 
  }
  }

  TCanvas *c1 = new TCanvas("c1");
  c1->SetLogy();
  ene_tpc_h->SetLineColor(kBlue);
  ene_veto_h->SetLineColor(kRed);
  ene_tpc_h->Draw() ;
  ene_veto_h->Draw("SAME") ;
  c1->BuildLegend();

} 




int main () {

  neutron_analysis_chain_update();

}
