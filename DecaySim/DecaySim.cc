#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "QGSP_BIC_HP.hh"
#include "G4RadioactiveDecayPhysics.hh" // Thư viện lõi cho phân rã

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "Randomize.hh" 
#include <ctime>        

int main(int argc, char** argv) {

    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(time(nullptr));
    //auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

    runManager->SetUserInitialization(new DetectorConstruction());

    auto physicsList = new QGSP_BIC_HP();
    physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics()); // Đăng ký vật lý phóng xạ
    runManager->SetUserInitialization(physicsList);

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