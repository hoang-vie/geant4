#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh" 
#include "G4RunManager.hh"
PrimaryGeneratorAction::PrimaryGeneratorAction(RunAction* runAction) 
: G4VUserPrimaryGeneratorAction(), fRunAction(runAction) {
    fParticleGun = new G4ParticleGun(1);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    //fParticleGun->SetParticleEnergy(5.0 * MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    auto particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    
    G4double randChoice = G4UniformRand();
    if (randChoice < 0.25) particleName = "alpha";
    else if (randChoice < 0.50) particleName = "e-";
    else if (randChoice < 0.75) particleName = "gamma";
    else particleName = "neutron";

    const auto detConstruction = static_cast<const DetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fParticleGun->SetParticleEnergy(detConstruction->GetGunEnergy());
    fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName));

    G4double x0 = 30 * cm * (G4UniformRand() - 0.5);
    G4double y0 = 30 * cm * (G4UniformRand() - 0.5);
    G4double z0 = detConstruction->GetGunZPosition();
    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    fParticleGun->GeneratePrimaryVertex(event);

    // BÁO CÁO NGAY LẬP TỨC: Đếm hạt ban đầu tại đây!
    if (fRunAction) fRunAction->AddInitialParticle(particleName);
}