#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

// Forward declarations: Giúp giảm thời gian biên dịch và tránh phụ thuộc chéo
class EventAction;
class G4Step;

/**
 * @brief Lớp SteppingAction kiểm soát và phân tích dữ liệu tại mỗi bước của hạt.
 * * Các tính năng cốt lõi cho dự án của Quyen:
 * 1. Khảo sát quy luật N(t): Nhận diện hạt nhân mẹ (TrackID=1) để ghi nhận thời điểm rã.
 * 2. Bộ lọc vật lý: Tách chính xác các hạt thứ cấp từ phân rã (Alpha, Beta, Gamma, Neutrino).
 * 3. Xuất dữ liệu văn bản: Gửi dữ liệu trực tiếp tới RunAction để ghi vào các file:
 * - DuLieuPhanRa.dat (Ghi theo trình tự thời gian thực - Real-time)
 * - Quyluatphanra.dat (Thông qua bộ đệm thời gian rã để sắp xếp cuối Run)
 */
class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    // Hàm callback thực thi logic tại mỗi bước của hạt
    virtual void UserSteppingAction(const G4Step* step) override;

private:
    EventAction* fEventAction; // Con trỏ quản lý dữ liệu cấp độ Event
};

#endif