#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class EventAction;
class G4LogicalVolume;

class SteppingAction : public G4UserSteppingAction {
  public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction() override = default;

    virtual void UserSteppingAction(const G4Step*) override;

  private:
    EventAction* fEventAction;
    G4LogicalVolume* fScoringVolume; // PHẢI CÓ DÒNG NÀY
};

#endif