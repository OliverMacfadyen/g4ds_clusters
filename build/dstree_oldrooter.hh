//Declaration of leaves types
   Int_t           ev;
   Int_t           pdg;
   Float_t         ene0;
   Float_t         tpcene;
   Float_t         x;
   Float_t         y;
   Float_t         z;
   Float_t         px;
   Float_t         py;
   Float_t         pz;
   Float_t         tdrift;
   Int_t           ndaughters;
   Int_t           ndeposits;
   Int_t           ndepositsTPC;
   Int_t           nusers;
   Int_t           nbars;
   Int_t           nbars_prompt;
   Int_t           nbars_prompt10;
   Int_t           nbars_prompt100;
   Int_t           nbars_late;
   Int_t           nbars_late10;
   Int_t           nbars_late100;
   Int_t           vnpe;
   Float_t         bar_tot_energy;
   Float_t         bar_late_energy;
   Float_t         bar_step[22];
   Float_t         bar_late_ene[20];
   Float_t         bar_prompt_ene[20];
   Double_t        tdep_min;
   Double_t        tdep_max;
   Float_t         mat_fraction[100];
   Float_t         prompt_qdepMat[75];
   Float_t         late_qdepMat[75];
   Float_t         prompt_depMat[75];
   Float_t         late_depMat[75];
   Float_t         late_timeMat[75];
   Float_t         prompt_eneVeto_PS;
   Float_t         prompt_eneVeto_LS;
   Float_t         prompt_eneVeto_Ar;
   Float_t         prompt_eneVeto;
   Float_t         late_eneVeto_PS;
   Float_t         late_eneVeto_LS;
   Float_t         late_eneVeto_Ar;
   Float_t         late_eneVeto;
   Int_t           pdgCap;
   Float_t         cap_x;
   Float_t         cap_y;
   Float_t         cap_z;
   Float_t         cap_gamma_ene;
   Int_t           cap_gamma_mult;
   Double_t        cap_time;
   Float_t         depVeto;
   Int_t           isFV5;
   Int_t           isFV10;
   Int_t           isFV15;
   Int_t           isFV20;
   Int_t           isFV30[100000];
   Int_t           isFV35;
   Int_t           userint1[0];
   Int_t           userint2[0];
   Float_t         userfloat1[0];
   Float_t         userfloat2[0];
   Double_t        userdouble0;
   Int_t           nclus;
   Int_t           nclus_thr;
   Float_t         cl_ene[100000];
   Float_t         cl_true_ene[100000];
   Int_t           cl_ndep[100000];
   Float_t         cl_x[100000];
   Float_t         cl_y[100000];
   Float_t         cl_z[100000];
   Float_t         cl_t[100000];
   Int_t           cl_npe[100000];
   Float_t         cl_nucl[100000];
   Float_t         cl_elec[100000];
   Int_t           dep_mat[100000];

   Float_t         dep_z[100000];
   Float_t         dep_ene[100000];

   Float_t         ene_shell_0;
   Float_t         ene_shell_1;
   Float_t         ene_shell_2;
   Float_t         ene_shell_3;
   Float_t         ene_shell_4;
   Float_t         ene_shell_5;
   Float_t         ene_shell_6;
   Float_t         ene_shell_7;

   Int_t           nclusNR;
   Int_t           nclusER;
   Int_t           goodNRidx;

   Float_t         energyER;
   Float_t         energyERinel;
   Float_t         dau_ene[10000];


void SetBranches (TTree* dstree) { 
   // Set branch addresses.
   dstree->SetBranchAddress("ev",&ev);
   //dstree->SetBranchAddress("pdg",&pdg);
   dstree->SetBranchAddress("ene0",&ene0);
   dstree->SetBranchAddress("tpcene",&tpcene);
   dstree->SetBranchAddress("x",&x);
   dstree->SetBranchAddress("y",&y);
   dstree->SetBranchAddress("z",&z);
   dstree->SetBranchAddress("px",&px);
   dstree->SetBranchAddress("py",&py);
   dstree->SetBranchAddress("pz",&pz);
   dstree->SetBranchAddress("tdrift",&tdrift);
   dstree->SetBranchAddress("ndaughters",&ndaughters);
   dstree->SetBranchAddress("ndeposits",&ndeposits);
   dstree->SetBranchAddress("ndepositsTPC",&ndepositsTPC);
   dstree->SetBranchAddress("nusers",&nusers);
   dstree->SetBranchAddress("vnpe",&vnpe);
   //dstree->SetBranchAddress("nbars",&nbars);
   //dstree->SetBranchAddress("nbars_prompt",&nbars_prompt);
   //dstree->SetBranchAddress("nbars_prompt10",&nbars_prompt10);
   //dstree->SetBranchAddress("nbars_prompt100",&nbars_prompt100);
   //dstree->SetBranchAddress("nbars_late",&nbars_late);
   //dstree->SetBranchAddress("nbars_late10",&nbars_late10);
   //dstree->SetBranchAddress("nbars_late100",&nbars_late100);
   //dstree->SetBranchAddress("bar_tot_energy",&bar_tot_energy);
   //dstree->SetBranchAddress("bar_late_energy",&bar_late_energy);
   //dstree->SetBranchAddress("bar_step",bar_step);
   //dstree->SetBranchAddress("bar_late_ene",bar_late_ene);
   //dstree->SetBranchAddress("bar_prompt_ene",bar_prompt_ene);
   //dstree->SetBranchAddress("tdep_min",&tdep_min);
   //dstree->SetBranchAddress("tdep_max",&tdep_max);
   dstree->SetBranchAddress("mat_fraction",mat_fraction);
   dstree->SetBranchAddress("prompt_qdepMat",prompt_qdepMat);
   dstree->SetBranchAddress("late_qdepMat",late_qdepMat);
   dstree->SetBranchAddress("prompt_depMat",prompt_depMat);
   dstree->SetBranchAddress("late_depMat",late_depMat);
   dstree->SetBranchAddress("late_timeMat",late_timeMat);
   //dstree->SetBranchAddress("prompt_eneVeto_PS",&prompt_eneVeto_PS);
   //dstree->SetBranchAddress("prompt_eneVeto_LS",&prompt_eneVeto_LS);
   dstree->SetBranchAddress("prompt_eneVeto_Ar",&prompt_eneVeto_Ar);
   //dstree->SetBranchAddress("prompt_eneVeto",&prompt_eneVeto);
   //dstree->SetBranchAddress("late_eneVeto_PS",&late_eneVeto_PS);
   //dstree->SetBranchAddress("late_eneVeto_LS",&late_eneVeto_LS);
   dstree->SetBranchAddress("late_eneVeto_Ar",&late_eneVeto_Ar);
   //dstree->SetBranchAddress("late_eneVeto",&late_eneVeto);
   dstree->SetBranchAddress("pdgCap",&pdgCap);
   dstree->SetBranchAddress("cap_x",&cap_x);
   dstree->SetBranchAddress("cap_y",&cap_y);
   dstree->SetBranchAddress("cap_z",&cap_z);
   dstree->SetBranchAddress("cap_gamma_ene",&cap_gamma_ene);
   dstree->SetBranchAddress("cap_gamma_mult",&cap_gamma_mult);
   dstree->SetBranchAddress("cap_time",&cap_time);
   dstree->SetBranchAddress("depVeto",&depVeto);
   dstree->SetBranchAddress("isFV5",&isFV5);
   dstree->SetBranchAddress("isFV10",&isFV10);
   dstree->SetBranchAddress("isFV15",&isFV15);
   dstree->SetBranchAddress("isFV20",&isFV20);
   dstree->SetBranchAddress("isFV30",isFV30);
   dstree->SetBranchAddress("isFV35",&isFV35);
   dstree->SetBranchAddress("userint1",&userint1);
   dstree->SetBranchAddress("userint2",&userint2);
   dstree->SetBranchAddress("userfloat1",&userfloat1);
   dstree->SetBranchAddress("userfloat2",&userfloat2);
   dstree->SetBranchAddress("userdouble0",&userdouble0);
   dstree->SetBranchAddress("nclus",&nclus);
   dstree->SetBranchAddress("nclus_thr",&nclus_thr);
   dstree->SetBranchAddress("cl_ene",cl_ene);
   dstree->SetBranchAddress("cl_true_ene",cl_true_ene);
   dstree->SetBranchAddress("cl_ndep",cl_ndep);
   dstree->SetBranchAddress("cl_x",cl_x);
   dstree->SetBranchAddress("cl_y",cl_y);
   dstree->SetBranchAddress("cl_z",cl_z);
   dstree->SetBranchAddress("cl_t",cl_t);
   dstree->SetBranchAddress("cl_npe",cl_npe);
   dstree->SetBranchAddress("cl_nucl",cl_nucl);
   dstree->SetBranchAddress("cl_elec",cl_elec);


   //dstree->SetBranchAddress("dep_mat",dep_mat);
   //dstree->SetBranchAddress("dep_z",dep_z);
   //dstree->SetBranchAddress("dep_ene",dep_ene);


   //dstree->SetBranchAddress("ene_shell_0",&ene_shell_0);
   //dstree->SetBranchAddress("ene_shell_1",&ene_shell_1);
   //dstree->SetBranchAddress("ene_shell_2",&ene_shell_2);
   //dstree->SetBranchAddress("ene_shell_3",&ene_shell_3);
   //dstree->SetBranchAddress("ene_shell_4",&ene_shell_4);
   //dstree->SetBranchAddress("ene_shell_5",&ene_shell_5);
   //dstree->SetBranchAddress("ene_shell_6",&ene_shell_6);
   //dstree->SetBranchAddress("ene_shell_7",&ene_shell_7);

   dstree->SetBranchAddress("nclusNR",&nclusNR);
   dstree->SetBranchAddress("nclusER",&nclusER);
   dstree->SetBranchAddress("energyER",&energyER);
   dstree->SetBranchAddress("goodNRidx",&goodNRidx);
   dstree->SetBranchAddress("energyERinel",&energyERinel);

   dstree->SetBranchAddress("dau_ene",&dau_ene);


//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// dstree->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

//   for (Long64_t i=0; i<nentries;i++) {
//      nbytes += dstree->GetEntry(i);
//   }
}
