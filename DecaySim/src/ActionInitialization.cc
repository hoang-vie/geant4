#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

void ActionInitialization::BuildForMaster() const {
    // Khởi tạo RunAction cho luồng Master (gom dữ liệu cuối cùng)
    SetUserAction(new RunAction);
}

void ActionInitialization::Build() const {
    // 1. Khởi tạo nguồn phát
    SetUserAction(new PrimaryGeneratorAction);

    // 2. Khởi tạo các Action theo dây chuyền (Chuyền tay con trỏ)
    auto runAction = new RunAction;
    SetUserAction(runAction);

    // Truyền runAction vào trong EventAction
    auto eventAction = new EventAction(runAction);
    SetUserAction(eventAction);

    // Truyền eventAction vào trong SteppingAction
    SetUserAction(new SteppingAction(eventAction));
}