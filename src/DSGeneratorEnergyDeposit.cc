// --------------------------------------------------------------------------//
/**
 * AUTHOR: A. Meregaglia
 */
// --------------------------------------------------------------------------//

#include "DSGeneratorEnergyDeposit.hh"
#include "DSEventHandler.hh"
#include "DSG4DSReader.hh"
#include "DSGeneratorEnergyDepositMessenger.hh"
#include "DSIO.hh"
#include "DSLogger.hh"
#include "DSStorage.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SPSRandomGenerator.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4RunManager.hh"
//---------------------------------------------------------------------------//

DSGeneratorEnergyDeposit::DSGeneratorEnergyDeposit() : DSVGenerator("DSGeneratorEnergyDeposit") {

  fRead = false;
  fnumev = 0;
  fSkipEvents = 0 ;
  DSStorage::Get()->SetOverwriteCounter(false);
  G4SPSRandomGenerator* RndGen = new G4SPSRandomGenerator;
  fSPSPos = new G4SPSPosDistribution;
  fSPSAng = new G4SPSAngDistribution;
  fSPSAng->SetBiasRndm(RndGen);
  fSPSPos->SetBiasRndm(RndGen);
  fSPSAng->SetAngDistType("iso");
  fSPSAng->SetPosDistribution(fSPSPos);

  fParticleTable = G4ParticleTable::GetParticleTable();

  fMessenger = new DSGeneratorEnergyDepositMessenger(this);

  DSStorage::Get()->SetIsEnDepGenerator(true);

  DSLog(routine) << "EnergyDeposit Generatore Built" << endlog;
}
//---------------------------------------------------------------------------//

DSGeneratorEnergyDeposit::~DSGeneratorEnergyDeposit() {
  delete fMessenger;
}

//---------------------------------------------------------------------------//

void DSGeneratorEnergyDeposit::DSGeneratePrimaries(G4Event* event) {
  G4bool isRead = false;
  //G4bool isNDepo = false;
  int counter = 0;
//  int nSkip = DSStorage::Get()->GetSkipEvents();
  if (fSkipEvents > 0) {
    DSG4DSReader::Get()->SkipEvents(fSkipEvents);
    fSkipEvents = 0;
  }
  
  // while (!isNDepo) {
  //   isRead = DSG4DSReader::Get()->ReadEvent();
  //   if (int(DSG4DSReader::Get()->GetVDeposits().size()) > 0) isNDepo = true;
  //   cout << "NClusters obtained in GenEnergyDep: " << int(DSG4DSReader::Get()->GetEvent().NClusters) << endl;
  //   if (int(DSG4DSReader::Get()->GetEvent().NClusters) > 10) {cout << "To be skipped" << endl; }
  // }


  //The idea is to isolate a cluster size of 1 for NR neutrons so that we only generate light for these events




  //for Gamma test run 
  //while (counter != 14) {

  //for neutron run 
  while (counter != 1) {
    DSG4DSReader::Get()->ClearAll();
    isRead = DSG4DSReader::Get()->ReadEvent();
    counter = 0;
    for (int i = 0; i < G4int(DSG4DSReader::Get()->GetVClusters().size()); ++i) {


      //cout << "RecoilID: " << DSG4DSReader::Get()->GetVClusters()[i].RecoilID << "___ Energy: " << DSG4DSReader::Get()->GetVClusters()[i].Energy << endl;
      //this statement is to apply NR cut on the cluster
      if ((DSG4DSReader::Get()->GetVClusters()[i].RecoilID > 0.49) && (DSG4DSReader::Get()->GetVClusters()[i].Energy > 0.4)) {

      //this statement was for the 5 MeV gammas test 
      //if (DSG4DSReader::Get()->GetVClusters()[i].Energy > 0.4) {
        
        counter ++ ;
      }
    }
    if (!isRead) {
      DSIO::Get()->CloseG4DSFile();
      DSLog(routine) << "End of file reached" << endlog;
      
      G4RunManager::GetRunManager()->AbortRun(true);
      return;
    }
    
  }

  //cout << "Just after the while loop the counter is " << counter << endl;




  cout << "VDeposits size: " << G4int(DSG4DSReader::Get()->GetVDeposits().size()) << endl;
  cout << "VClusters size: " << G4int(DSG4DSReader::Get()->GetVClusters().size()) << endl;


  //   Following section uses the values of the CLUSTERS to generate the new events 

  double check_for_energy = 0;
  for (int i=0; i < G4int(DSG4DSReader::Get()->GetVClusters().size()); ++i) {
    G4double xx = DSG4DSReader::Get()->GetVClusters()[i].Position[0] * cm;
    G4double yy = DSG4DSReader::Get()->GetVClusters()[i].Position[1] * cm;
    G4double zz = DSG4DSReader::Get()->GetVClusters()[i].Position[2] * cm;

    fPosition = G4ThreeVector(xx, yy, zz);

    if (DSG4DSReader::Get()->GetVClusters()[i].RecoilID < 0.5)
      fParticle = fParticleTable->FindParticle(11);
    else 
      fParticle = G4IonTable::GetIonTable()->GetIon(1000401800);

    G4ParticleMomentum theMomentum = fSPSAng->GenerateOne();

    G4PrimaryParticle* particle = new G4PrimaryParticle(fParticle);
    cout << "Cluster energy being set as deposit: " << DSG4DSReader::Get()->GetVClusters()[i].Energy << endl; // This has no * keV as there is a / keV value in dsstepptingaction.cc line 37. The lack of it here gives you the actual value being used to generate the new deposits


    particle->SetKineticEnergy((DSG4DSReader::Get()->GetVClusters()[i].Energy * keV)); // see comment above for * keV. Value passed to dssteppingaction.cc 
    particle->SetMomentumDirection(theMomentum);

    G4PrimaryVertex* vertex = new G4PrimaryVertex(fPosition, DSG4DSReader::Get()->GetVClusters()[i].Time * ns);
    check_for_energy += DSG4DSReader::Get()->GetVClusters()[i].Energy ;
    if ( i == G4int(DSG4DSReader::Get()->GetVClusters().size()) - 1) {
      cout << "Total of these energies: " << check_for_energy << endl;
    }
    //G4PrimaryVertex* vertex = new G4PrimaryVertex(fPosition, DSG4DSReader::Get()->GetVDeposits()[i].Time);

    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);

  }


  // '''

  //   Following section uses the values of the DEPOSITS to generate the new events (the original code)

  // '''
  
  // double countDepEnergy = 0;
  // for (int i = 0; i < G4int(DSG4DSReader::Get()->GetVDeposits().size()); ++i) {
  //   countDepEnergy += DSG4DSReader::Get()->GetVDeposits()[i].Energy;
  //   if ( i == G4int(DSG4DSReader::Get()->GetVDeposits().size()) - 1) { cout << "Total deposited energy for this event: " << countDepEnergy << endl; }
    
  //   G4double xx = DSG4DSReader::Get()->GetVDeposits()[i].Position[0] * cm;
  //   G4double yy = DSG4DSReader::Get()->GetVDeposits()[i].Position[1] * cm;
  //   G4double zz = DSG4DSReader::Get()->GetVDeposits()[i].Position[2] * cm;

  //   fPosition = G4ThreeVector(xx, yy, zz);



 
  //   if (DSG4DSReader::Get()->GetVDeposits()[i].PID < 10000)
  //     fParticle = fParticleTable->FindParticle(DSG4DSReader::Get()->GetVDeposits()[i].PID);
  //   else 
  //     fParticle = G4IonTable::GetIonTable()->GetIon(DSG4DSReader::Get()->GetVDeposits()[i].PID);
    
  //   G4ParticleMomentum theMomentum = fSPSAng->GenerateOne();

  //   G4PrimaryParticle* particle = new G4PrimaryParticle(fParticle);
  //   cout << "Deposit energy being set: " << DSG4DSReader::Get()->GetVDeposits()[i].Energy /keV<< endl;


  //   particle->SetKineticEnergy((DSG4DSReader::Get()->GetVDeposits()[i].Energy));
  //   particle->SetMomentumDirection(theMomentum);

  //   G4PrimaryVertex* vertex = new G4PrimaryVertex(fPosition, DSG4DSReader::Get()->GetVDeposits()[i].Time * ns);
  //   //G4PrimaryVertex* vertex = new G4PrimaryVertex(fPosition, DSG4DSReader::Get()->GetVDeposits()[i].Time);

  //   vertex->SetPrimary(particle);
  //   event->AddPrimaryVertex(vertex);
  // }
  
  DSG4DSReader::Get()->ClearAll();
}

/*
 * Revision 1.1  2013/03/20 09:54:28  dfranco
 * New version of g4ds
 *
 *
 */
