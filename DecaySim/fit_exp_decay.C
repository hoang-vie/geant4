void fit_exp_decay() {

    // ===== 1. Mở file =====
    TFile *file = TFile::Open("/home/huyhoang/software/GEANT4/DecaySim/build/RadioactiveData_CS137.root");
    if (!file || file->IsZombie()) {
        std::cout << "Cannot open file!" << std::endl;
        return;
    }

    // ===== 2. Lấy tree =====
    TTree *tree = (TTree*)file->Get("ParticleHits");
    if (!tree) {
        std::cout << "Cannot find TTree!" << std::endl;
        return;
    }

    // ===== 3. Đọc branch =====
    double time_ns;
    tree->SetBranchAddress("Time_ns", &time_ns);

    // ===== 4. Tạo histogram =====
    TH1F *h = new TH1F("h", "Time distribution;Time (ns);Counts", 500, 0, 5e18);

    // ===== 5. Fill dữ liệu =====
    Long64_t nentries = tree->GetEntries();

    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i);

        // lọc giá trị hợp lý để tránh phá histogram
        if (time_ns > 0 && time_ns < 5e18) {
            h->Fill(time_ns);
        }
    }

    // ===== 6. Vẽ =====
    TCanvas *c1 = new TCanvas("c1", "Exponential Fit", 800, 600);
    h->Draw();

    // ===== 7. Hàm fit exponential =====
    // Dùng dạng vật lý đúng: exp(-t/tau)
    TF1 *fexp = new TF1("fexp", "[0]*exp(-x/[1])", 0, 5e18);

    // ===== 8. Giá trị khởi tạo =====
    fexp->SetParameters(h->GetMaximum(), 1e18);

    // ===== 9. Fit =====
    h->Fit(fexp, "R");

    // ===== 10. Vẽ lại hàm =====
    fexp->SetLineColor(kRed);
    fexp->Draw("same");

    // ===== 11. Kết quả =====
    double tau = fexp->GetParameter(1);
    double tau_err = fexp->GetParError(1);

    std::cout << "Tau = " << tau << " ns" << std::endl;
    std::cout << "Tau error = " << tau_err << std::endl;

    // Half-life
    double T12 = tau * log(2);
    std::cout << "T1/2 = " << T12 << " ns" << std::endl;
}