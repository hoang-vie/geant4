#include "EventAction.hh"
#include "G4Event.hh"

EventAction::EventAction() : G4UserEventAction(), fEdep(0.) 
{}

void EventAction::BeginOfEventAction(const G4Event*) {
    fEdep = 0.; // Reset năng lượng mỗi khi bắt đầu một hạt mới
}

void EventAction::EndOfEventAction(const G4Event*) {
    // Nếu bạn muốn in tổng năng lượng sau mỗi hạt, có thể thêm G4cout ở đây.
    // Hiện tại để trống để tránh làm loạn màn hình khi quan sát.
}