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
/// \file hadronic/Hadr03/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// $Id: HistoManager.cc 80190 2014-04-07 10:18:04Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("Hadr03"), fFactoryOn(false)
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  //  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);     //enable inactivation of histograms
  

  // Create directories
  //  analysisManager->SetHistoDirectoryName("histo");
  //  analysisManager->SetNtupleDirectoryName("ntuple");


  // Define histograms start values
  const G4int kMaxHisto = 15;
  const G4String id[] = {"0","1","2","3","4","5","6","7","8","9",
                         "10","11","12","13", "14"};
  const G4String title[] = 
                { "dummy",                                          //0
                  "kinetic energy of scattered primary particle",   //1
                  "kinetic energy of gamma",                        //2
                  "kinetic energy of neutrons",                     //3
                  "kinetic energy of protons",                      //4
                  "kinetic energy of deuterons",                    //5
                  "kinetic energy of alphas",                       //6
                  "kinetic energy of nuclei",                       //7
                  "kinetic energy of mesons",                       //8
                  "kinetic energy of baryons",                      //9
                  "Q = Ekin out - Ekin in",                         //10
                  "Pbalance = mag(P_out - P_in)",                   //11
                  "atomic mass of nuclei",                          //12
				  "dE_dx",
				  "momentum"
                 };  

  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  for (G4int k=0; k<kMaxHisto; k++) {
    G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, false);
  }
   analysisManager->CreateNtuple("Ntuple1", "dE_dx & momentum");
   analysisManager->CreateNtupleDColumn("dE_dx"); // column Id = 0
   analysisManager->CreateNtupleDColumn("momentum"); // column Id = 1
   analysisManager->FinishNtuple(0);
  // analysisManager->SetNtupleActivation(false);
  // analysisManager->SetActivation(false);
   fFactoryOn = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
  if (! fFactoryOn) return;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

 // G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;

  delete G4AnalysisManager::Instance();
  fFactoryOn = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 void HistoManager::FillNtuple(G4double dE_dx, G4double momentum)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // Fill Ntuple
  analysisManager->FillNtupleDColumn(0, 0, dE_dx);
  analysisManager->FillNtupleDColumn(0, 1, momentum);
  analysisManager->AddNtupleRow(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
