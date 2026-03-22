#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction {
public:
    RunAction();
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    void AddInitialParticle(const G4String& type);
    void AddDetectedParticle(const G4String& type);

private:
    G4Accumulable<G4int> fInitAlpha{0}, fInitBeta{0}, fInitGamma{0}, fInitNeutron{0};
    G4Accumulable<G4int> fDetAlpha{0}, fDetBeta{0}, fDetGamma{0}, fDetNeutron{0};
};
#endif