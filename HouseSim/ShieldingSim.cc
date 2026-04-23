#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "FTFP_BERT_HP.hh"

#include "Randomize.hh"
#include <ctime>        



#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

int main(int argc, char** argv) {
    
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(time(nullptr));
    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(time(nullptr));
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new FTFP_BERT_HP()); // Dùng thư viện HP cho Neutron
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
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    delete visManager;
    delete runManager;
    return 0;
}