#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() {
    fParticleGun = new G4ParticleGun(1);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0.1, 1.)); // Hướng z
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    auto particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* alpha = particleTable->FindParticle("alpha");
    G4ParticleDefinition* electron = particleTable->FindParticle("e-");
    G4ParticleDefinition* gamma = particleTable->FindParticle("gamma");

    G4double z0 = -1.0 * m; // Bắn từ đầu mút của không gian
    G4double energy = 10.0 * MeV; // Năng lượng khá lớn để hạt bay xa
    fParticleGun->SetParticleEnergy(energy);

    // Bắn 3 hạt đồng thời, cách nhau 5mm theo trục Y để dễ quan sát
    fParticleGun->SetParticleDefinition(alpha);
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 5*mm, z0));
    fParticleGun->GeneratePrimaryVertex(event);

    fParticleGun->SetParticleDefinition(electron);
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., z0));
    fParticleGun->GeneratePrimaryVertex(event);

    fParticleGun->SetParticleDefinition(gamma);
    fParticleGun->SetParticlePosition(G4ThreeVector(0., -5*mm, z0));
    fParticleGun->GeneratePrimaryVertex(event);
}