#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

void ActionInitialization::Build() const {
    SetUserAction(new PrimaryGeneratorAction());

    EventAction* eventAction = new EventAction(); // Gọi không tham số
    SetUserAction(eventAction);

    SetUserAction(new SteppingAction(eventAction));
}

void ActionInitialization::BuildForMaster() const {}