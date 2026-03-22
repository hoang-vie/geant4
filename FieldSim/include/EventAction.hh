#ifndef EventAction_h
#define EventAction_h 1
#include "G4UserEventAction.hh"
class EventAction : public G4UserEventAction {
public:
    EventAction() = default;
    ~EventAction() override = default;
};
#endif