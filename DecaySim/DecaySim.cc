#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "QGSP_BIC_HP.hh"
#include "G4RadioactiveDecayPhysics.hh" 

// 3 THƯ VIỆN ĐỂ CAN THIỆP LÕI VẬT LÝ
#include "G4EmParameters.hh"
#include "G4NuclearLevelData.hh"
#include "G4DeexPrecoParameters.hh"

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "Randomize.hh" 
#include <ctime>        

int main(int argc, char** argv) {

    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(time(nullptr));
    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

    runManager->SetUserInitialization(new DetectorConstruction());

    auto physicsList = new QGSP_BIC_HP();
    physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics());
    runManager->SetUserInitialization(physicsList);

    // ====================================================================
    // ÉP GEANT4 CHỈ MÔ PHỎNG HẠT NHÂN (TẮT LỚP VỎ NGUYÊN TỬ)
    // ====================================================================
    // 1. Tắt Auger Electron, Tia X huỳnh quang của lớp vỏ
    G4EmParameters* emParams = G4EmParameters::Instance();
    emParams->SetAuger(false);
    emParams->SetFluo(false);
    emParams->SetPixe(false);

    // 2. Tắt Internal Conversion (Cấm hạt nhân đá electron vỏ ra ngoài)
    G4DeexPrecoParameters* deexParams = G4NuclearLevelData::GetInstance()->GetParameters();
    deexParams->SetInternalConversionFlag(false);
    // ====================================================================

    runManager->SetUserInitialization(new ActionInitialization());

    auto visManager = new G4VisExecutive(argc, argv);
    visManager->Initialize();

    auto UImanager = G4UImanager::GetUIpointer();

    if (argc == 1) {
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    } else {
        UImanager->ApplyCommand(G4String("/control/execute ") + argv[1]);
    }

    delete visManager;
    delete runManager;
    return 0;
}