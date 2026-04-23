#include "RunAction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"
#include <iomanip>
#include <iostream>
#include <cctype>
#include <algorithm>

RunAction::RunAction() : G4UserRunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    // Mở luồng ghi cho 2 file mục tiêu
    fOutFile.open("DuLieuPhanRa.dat");
    fDecayLawFile.open("Quyluatphanra.dat");

    // Chuẩn bị tiêu đề cột
    G4String header = "#Hat_Nhan_con                     Tia_Buc_Xa     E(keV)            T(s)";
    G4String line = std::string(85, '-');

    // 1. Tiêu đề cho DuLieuPhanRa.dat và in ra màn hình
    if (fOutFile.is_open()) {
        fOutFile << header << std::endl;
        fOutFile << line << std::endl;
        
        // In ra màn hình để Quyên theo dõi trực tiếp
        G4cout << "\n" << header << G4endl;
        G4cout << line << G4endl;
    }

    // 2. Tiêu đề cho Quyluatphanra.dat (Quy luật N(t))
    if (fDecayLawFile.is_open()) {
        fDecayLawFile << std::left << std::setw(30) << "#Thoi_gian_phan_ra(s)" 
                      << "So_hat_nhan_con_lai(N)" << std::endl;
        fDecayLawFile << std::string(60, '-') << std::endl;
    }

    fDecayTimes.clear();
}

void RunAction::AddDecayTime(G4double t) {
    // Lưu thời gian rã vào vector để sắp xếp ở cuối Run
    fDecayTimes.push_back(t);
}

void RunAction::WriteToDatFile(G4String daughterName, G4String radiationName, G4double energy, G4double timeSeconds) {
    // Định dạng tên hạt nhân con (Z, A) để dễ đọc
    G4String formattedName = daughterName;
    size_t firstDigit = daughterName.find_first_of("0123456789");
    if (firstDigit != std::string::npos && firstDigit > 0) {
        G4String symbol = daughterName.substr(0, firstDigit);
        G4String massA = daughterName.substr(firstDigit);
        G4int Z = G4NistManager::Instance()->GetZ(symbol);
        if (Z > 0) {
            formattedName = symbol + " (Z=" + std::to_string(Z) + ", A=" + massA + ")";
        }
    }

    // Tạo chuỗi định dạng dữ liệu
    std::stringstream ss;
    ss << std::left << std::setw(35) << formattedName 
       << std::left << std::setw(15) << radiationName
       << std::fixed << std::setprecision(4) << std::setw(18) << energy 
       << std::fixed << std::setprecision(0) << timeSeconds;

    // GHI REAL-TIME: Ghi vào file ngay khi sự kiện xảy ra
    if (fOutFile.is_open()) {
        fOutFile << ss.str() << std::endl;
    }

    // IN RA MÀN HÌNH: Để Quyên thấy dữ liệu nhảy theo thời gian thực
    G4cout << ss.str() << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* run) {
    // --- XỬ LÝ QUY LUẬT PHÂN RÃ ---

    // 1. Sắp xếp mảng thời gian tăng dần
    std::sort(fDecayTimes.begin(), fDecayTimes.end());

    // 2. Lấy tổng số hạt ban đầu (N0)
    G4int nRemaining = run->GetNumberOfEventToBeProcessed();
    
    // Ghi mốc bắt đầu t=0 vào file quy luật
    if (fDecayLawFile.is_open()) {
        fDecayLawFile << std::left << std::setw(30) << "0" << nRemaining << std::endl;
    }

    // 3. Duyệt danh sách đã sắp xếp để ghi sự suy giảm số hạt N theo t
    for (G4double t_decay : fDecayTimes) {
        nRemaining--; 
        if (fDecayLawFile.is_open()) {
            fDecayLawFile << std::left 
                          << std::setw(30) << std::fixed << std::setprecision(0) << t_decay 
                          << nRemaining << std::endl;
        }
    }

    // Đóng toàn bộ file
    if (fOutFile.is_open()) fOutFile.close();
    if (fDecayLawFile.is_open()) fDecayLawFile.close();
    
    G4cout << std::string(85, '=') << G4endl;
    G4cout << ">>> MO PHONG KET THUC THANH CONG!" << G4endl;
    G4cout << ">>> 1. DuLieuPhanRa.dat: Nhat ky chi tiet." << G4endl;
    G4cout << ">>> 2. Quyluatphanra.dat: So lieu thong tin N(t)." << G4endl;
    G4cout << std::string(85, '=') << G4endl;
}