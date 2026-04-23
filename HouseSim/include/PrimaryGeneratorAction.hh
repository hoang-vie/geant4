#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction() override;
    virtual void GeneratePrimaries(G4Event* anEvent) override;
  private:
    G4ParticleGun* fParticleGun;
};
#endif