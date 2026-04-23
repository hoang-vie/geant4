#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction() {}

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddInitialParticle(const G4String& type);
    void AddDetectedParticle(const G4String& type);

private:
    G4Accumulable<G4int> fInitAlpha = 0;
    G4Accumulable<G4int> fInitElectron = 0; // Tên phải khớp với file .cc
    G4Accumulable<G4int> fInitPositron = 0;
    G4Accumulable<G4int> fInitGamma = 0;
    G4Accumulable<G4int> fInitNeutron = 0;

    G4Accumulable<G4int> fDetAlpha = 0;
    G4Accumulable<G4int> fDetElectron = 0; 
    G4Accumulable<G4int> fDetPositron = 0;
    G4Accumulable<G4int> fDetGamma = 0;
    G4Accumulable<G4int> fDetNeutron = 0;
};
#endif