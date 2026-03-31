#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4LogicalVolume.hh"
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

    if (postVolume == fScoringVolume && preVolume != fScoringVolume && step->GetTrack()->GetParentID() == 0) {
        
        G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
        fEventAction->AddDetected(particleName); 

       
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleSColumn(0, particleName);
        analysisManager->FillNtupleDColumn(1, step->GetPreStepPoint()->GetKineticEnergy() / MeV);
        analysisManager->FillNtupleDColumn(2, step->GetPreStepPoint()->GetPosition().x() / cm);
        analysisManager->FillNtupleDColumn(3, step->GetPreStepPoint()->GetPosition().y() / cm);
        analysisManager->AddNtupleRow();

        std::cout <<  " Hạt " << particleName << " vừa đâm vào máy dò với năng lượng = " << step->GetPreStepPoint()->GetKineticEnergy() << " MeV \n" ;
    }
}