#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction) 
: G4UserSteppingAction(), fEventAction(eventAction), fScoringVolume(nullptr) {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    if (!fScoringVolume) {
        const auto detConstruction = static_cast<const DetectorConstruction*>(
            G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fScoringVolume = detConstruction->GetScoringVolume();
    }

    G4LogicalVolume* preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    G4LogicalVolume* postVolume = nullptr;
    if (step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()) {
        postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    }

   
    if (postVolume == fScoringVolume && preVolume != fScoringVolume) {
        
        G4Track* track = step->GetTrack();
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = step->GetPreStepPoint()->GetKineticEnergy();
        G4double time = step->GetPreStepPoint()->GetGlobalTime();

        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleSColumn(0, 0, particleName);
        analysisManager->FillNtupleDColumn(0, 1, energy / keV);
        analysisManager->FillNtupleDColumn(0, 2, time / ns);
        analysisManager->AddNtupleRow(0);

        fEventAction->GetRunAction()->CountHit();
    }
}