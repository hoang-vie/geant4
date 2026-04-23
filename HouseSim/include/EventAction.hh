#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction {
  public:
    EventAction();
    virtual ~EventAction() override = default;

    virtual void BeginOfEventAction(const G4Event*) override;
    virtual void EndOfEventAction(const G4Event*) override;

    // Hàm để SteppingAction gọi khi có hạt lọt vào nhà
    void AddEdep(G4double edep) { fEdep += edep; } 

  private:
    G4double fEdep;
};

#endif