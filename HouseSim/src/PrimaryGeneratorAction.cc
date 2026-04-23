#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4RandomDirection.hh" // Thư viện quan trọng để bắn mọi hướng

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    auto pTable = G4ParticleTable::GetParticleTable();
    fParticleGun->SetParticleDefinition(pTable->FindParticle("gamma"));
    fParticleGun->SetParticleEnergy(2.0*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    // Đặt vị trí súng tại tâm ngôi nhà (0,0,0)
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));

    // THIẾT LẬP BẮN MỌI HƯỚNG: 
    // Thay vì dùng một hướng cố định, ta dùng hàm ngẫu nhiên 360 độ
    fParticleGun->SetParticleMomentumDirection(G4RandomDirection());

    fParticleGun->GeneratePrimaryVertex(anEvent);
}