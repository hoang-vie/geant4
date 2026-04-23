#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"

EventAction::EventAction(RunAction* runAction) : G4UserEventAction(), fRunAction(runAction) {}

void EventAction::BeginOfEventAction(const G4Event*) {
    // Khởi tạo lại các cờ đánh dấu cho mỗi sự kiện mới
    fDetAlpha = 0; 
    fDetElectron = 0; 
    fDetPositron = 0; 
    fDetGamma = 0;
}

void EventAction::AddDetected(const G4String& type) {
    // Kiểm tra loại hạt và đánh dấu theo thứ tự yêu cầu
    if (type == "alpha") fDetAlpha = 1;
    else if (type == "e-") fDetElectron = 1;
    else if (type == "e+") fDetPositron = 1;
    else if (type == "gamma") fDetGamma = 1;
}

void EventAction::EndOfEventAction(const G4Event*) {
    // Tổng hợp kết quả từ các cờ đánh dấu đẩy lên RunAction
    if(fDetAlpha > 0)    fRunAction->AddDetectedParticle("alpha");
    if(fDetElectron > 0) fRunAction->AddDetectedParticle("e-");
    if(fDetPositron > 0) fRunAction->AddDetectedParticle("e+");
    if(fDetGamma > 0)    fRunAction->AddDetectedParticle("gamma");
}