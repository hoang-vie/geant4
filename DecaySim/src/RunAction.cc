#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction() : G4UserRunAction(), fRunHitCount(0) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    
    // Ntuple 0: Chi tiết tia
    analysisManager->CreateNtuple("ParticleHits", "Details");
    analysisManager->CreateNtupleSColumn("Particle");
    analysisManager->CreateNtupleDColumn("Energy_keV");
    analysisManager->CreateNtupleDColumn("Time_ns");
    analysisManager->FinishNtuple();

    // Ntuple 1: Thống kê Poisson
    analysisManager->CreateNtuple("PoissonStats", "Counts per Run");
    analysisManager->CreateNtupleIColumn("RunID");
    analysisManager->CreateNtupleIColumn("TotalDetected");
    analysisManager->FinishNtuple();

    analysisManager->OpenFile("RadioactiveData");
}

RunAction::~RunAction() {
    auto analysisManager = G4AnalysisManager::Instance();
   
    analysisManager->Write();
    analysisManager->CloseFile();
}

void RunAction::BeginOfRunAction(const G4Run*) {
    fRunHitCount = 0; // Vẫn reset bộ đếm cho mỗi Run đo đạc mới
}

void RunAction::EndOfRunAction(const G4Run* run) {
    auto analysisManager = G4AnalysisManager::Instance();
    
  
    analysisManager->FillNtupleIColumn(1, 0, run->GetRunID());
    analysisManager->FillNtupleIColumn(1, 1, fRunHitCount);
    analysisManager->AddNtupleRow(1);

    if (run->GetRunID() % 10 == 0) { 
        G4cout << ">>> Completed " << run->GetRunID() << " runs..." << G4endl;
    }
}