#ifndef EventAction_h
#define EventAction_h 1
#include "G4UserEventAction.hh"

class RunAction;

class EventAction : public G4UserEventAction {
public:
    EventAction(RunAction* runAction) : fRunAction(runAction) {}
    ~EventAction() override = default;
    RunAction* GetRunAction() { return fRunAction; }
private:
    RunAction* fRunAction;
};
#endif