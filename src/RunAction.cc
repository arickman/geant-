//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file hadronic/Hadr03/src/RunAction.cc
/// \brief Implementation of the RunAction class
//
// $Id: RunAction.cc 80197 2014-04-07 15:25:49Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "Run.hh"
#include "RunMessenger.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "SteppingAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
  : G4UserRunAction(),
	fDetector(det), fPrimary(prim), fRun(0), fHistoManager(0),
    fRunMessenger(0), fPrint(true)//, dE_dx(0.), momentum(0.)
{
 fHistoManager = new HistoManager();
// fStep         = new SteppingAction();
 fRunMessenger = new RunMessenger(this);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
 delete fHistoManager;
 delete fRunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run(fDetector); 
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{    
  // save Rndm status
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  if (isMaster) G4Random::showEngineStatus();
    
  // keep run condition
  if (fPrimary) { 
    G4ParticleDefinition* particle 
      = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }
       
  //histograms
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
 // if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile("Hadr03");
    analysisManager->CreateNtuple("Ntuple1", "dE_dx & momentum");
    analysisManager->CreateNtupleDColumn("dE_dx"); // column Id = 0
    analysisManager->CreateNtupleDColumn("momentum"); // column Id = 1
    analysisManager->FinishNtuple(0);

//  }
  //initialization per event
  // dE_dx = momentum = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  if (isMaster) fRun->EndOfRun(fPrint);    
  
  //save histograms      
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
 // if ( analysisManager->IsActive() ) {
    analysisManager->Write();
    analysisManager->CloseFile();
    delete G4AnalysisManager::Instance();
 // }
 // G4Ntuple* ntuple1 = analysisManager->GetNtuple();
  // fHistoManager->Save();
  //fStep->fillPerEvent(dE_dx, momentum);

  //fill ntuple
    //
  // fHistoManager->FillNtuple(dE_dx, momentum);


  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();
}
// Complete clean-up
 // delete G4AnalysisManager::Instance();


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::SetPrintFlag(G4bool flag)
{ 
  fPrint = flag;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
