#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"

EventAction::EventAction(RunAction* runAction) : G4UserEventAction(), fRunAction(runAction) {}

void EventAction::BeginOfEventAction(const G4Event*) {
    fDetAlpha = 0; fDetBeta = 0; fDetGamma = 0; fDetNeutron = 0;
}

void EventAction::AddDetected(const G4String& type) {
    if (type == "alpha") fDetAlpha = 1;
    else if (type == "e-") fDetBeta = 1;
    else if (type == "gamma") fDetGamma = 1;
    else if (type == "neutron") fDetNeutron = 1;
}

void EventAction::EndOfEventAction(const G4Event*) {
    // Chỉ cần tổng hợp số lượng hạt chạm đích đẩy lên RunAction
    if(fDetAlpha > 0) fRunAction->AddDetectedParticle("alpha");
    if(fDetBeta > 0) fRunAction->AddDetectedParticle("e-");
    if(fDetGamma > 0) fRunAction->AddDetectedParticle("gamma");
    if(fDetNeutron > 0) fRunAction->AddDetectedParticle("neutron");
}