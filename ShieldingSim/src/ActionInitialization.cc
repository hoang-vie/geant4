#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

void ActionInitialization::BuildForMaster() const {
    SetUserAction(new RunAction);
}

void ActionInitialization::Build() const {
    auto runAction = new RunAction;
    SetUserAction(runAction);

    // Truyền RunAction vào Generator để đếm hạt phát ra
    SetUserAction(new PrimaryGeneratorAction(runAction));

    auto eventAction = new EventAction(runAction);
    SetUserAction(eventAction);

    SetUserAction(new SteppingAction(eventAction));
}