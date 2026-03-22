#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4Event.hh" // <--- THÊM THƯ VIỆN NÀY ĐỂ TRÁNH LỖI G4Event
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

void SteppingAction::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    if (track->GetParentID() != 0) return;

    // Lệnh này cần G4Event.hh mới chạy được
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID(); 
    G4int trackID = track->GetTrackID();
    G4String particleName = track->GetDefinition()->GetParticleName();
    
    G4StepPoint* prePoint = step->GetPreStepPoint();
    
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, eventID);
    analysisManager->FillNtupleIColumn(1, trackID);
    analysisManager->FillNtupleSColumn(2, particleName);
    analysisManager->FillNtupleDColumn(3, prePoint->GetPosition().x() / cm);
    analysisManager->FillNtupleDColumn(4, prePoint->GetPosition().y() / cm);
    analysisManager->FillNtupleDColumn(5, prePoint->GetPosition().z() / cm);
    analysisManager->FillNtupleDColumn(6, prePoint->GetGlobalTime() / ns);
    analysisManager->FillNtupleDColumn(7, prePoint->GetKineticEnergy() / MeV);
    analysisManager->AddNtupleRow();
}