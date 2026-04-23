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
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    auto particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    
    // Logic chọn hạt: Chia đều cho 4 loại (Alpha, e-, gamma, e+)
    G4double randChoice = G4UniformRand();
    
    if (randChoice < 0.25) {
        particleName = "alpha";
    } else if (randChoice < 0.50) {
        particleName = "e-";      // Electron (Beta-)
    } else if (randChoice < 0.75) {
        particleName = "gamma";
    } else {
        particleName = "e+";      // Positron (Beta+)
    }

    // Lấy thông số từ DetectorConstruction
    const auto detConstruction = static_cast<const DetectorConstruction*>(
        G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    // Thiết lập năng lượng từ file macro (vis.mac) thông qua DetectorConstruction
    fParticleGun->SetParticleEnergy(detConstruction->GetGunEnergy());
    
    // Thiết lập loại hạt tương ứng
    fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName));

    // Thiết lập vị trí bắn (chùm tia có độ rộng 30x30 cm)
    G4double x0 = 30 * cm * (G4UniformRand() - 0.5);
    G4double y0 = 30 * cm * (G4UniformRand() - 0.5);
    G4double z0 = detConstruction->GetGunZPosition();
    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    // Bắn hạt
    fParticleGun->GeneratePrimaryVertex(event);

    // Báo cáo số lượng hạt ban đầu vào RunAction
    if (fRunAction) fRunAction->AddInitialParticle(particleName);
}