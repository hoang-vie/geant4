#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4String.hh"
#include "globals.hh"
#include <fstream>
#include <vector>

class G4Run;

/**
 * @brief Lớp điều khiển quá trình Run và quản lý xuất dữ liệu ra file văn bản (.dat).
 * Lớp này quản lý việc ghi các file:
 * 1. DuLieuPhanRa.dat: Nhật ký phân rã theo trình tự thời gian thực (real-time).
 * 2. Quyluatphanra.dat: Dữ liệu quy luật phân rã N(t) đã được sắp xếp thời gian tăng dần.
 */
class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();

    // Các hàm vòng đời của Geant4
    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;

    /**
     * @brief Tích lũy thời điểm phân rã của hạt nhân mẹ (TrackID=1).
     * Dữ liệu này sẽ được vector fDecayTimes lưu trữ để sắp xếp ở cuối Run.
     */
    void AddDecayTime(G4double t);

    /**
     * @brief Ghi dữ liệu thô vào DuLieuPhanRa.dat ngay khi phát hiện bước phân rã.
     * Ghi theo đúng trình tự mà các sự kiện xảy ra trong mô phỏng.
     */
    void WriteToDatFile(G4String daughterName, G4String radiationName, 
                        G4double energy, G4double timeSeconds);

private:
    // Các luồng ghi file văn bản
    std::ofstream fOutFile;        // Nhật ký chi tiết phân rã (Real-time)
    std::ofstream fDecayLawFile;   // Quy luật N(t) (Ghi sau khi sắp xếp ở EndOfRun)

    /**
     * @brief Vector lưu trữ các mốc thời gian phân rã để xử lý sắp xếp.
     */
    std::vector<G4double> fDecayTimes;
};

#endif