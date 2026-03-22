void plot() {
    // 1. Mở file dữ liệu ROOT mà Geant4 vừa đẻ ra
    TFile *file = new TFile("/home/huyhoang/software/GEANT4/DecaySim/build/RadioactiveData.root", "READ");
    if (!file || file->IsZombie()) {
        cout << "Lỗi: Không tìm thấy file RadioactiveData.root!" << endl;
        return;
    }

    // 2. Kéo Bảng thống kê Poisson ra (Ntuple ID = 1)
    TTree *tree = (TTree*)file->Get("PoissonStats");
    
    // Tìm số hạt đếm được Nhỏ nhất và Lớn nhất để chia vạch đồ thị cho đẹp
    int minHits = tree->GetMinimum("TotalDetected");
    int maxHits = tree->GetMaximum("TotalDetected");
    int numBins = maxHits - minHits + 4; // Mỗi số đếm là 1 cột (Bin)

    // 3. Khởi tạo biểu đồ Histogram
    TH1F *hPoisson = new TH1F("hPoisson", 
        "Khao sat Tinh Ngau nhien cua Phan ra Phong xa;So hat ghi nhan duoc trong 1 lan do;So lan xuat hien (Tan suat)", 
        numBins, minHits - 2, maxHits + 2);

    // Đổ toàn bộ 500 con số từ Tree vào Histogram
    tree->Draw("TotalDetected>>hPoisson", "", "goff");

    // 4. TRANG TRÍ ĐỒ THỊ
    TCanvas *c1 = new TCanvas("c1", "Phan tich Poisson", 800, 600);
    hPoisson->SetFillColor(kCyan-9);
    hPoisson->SetLineColor(kBlue);
    hPoisson->SetLineWidth(2);
    hPoisson->SetMarkerStyle(20);
    
    // 5. FIT (KHỚP) BẰNG HÀM POISSON TOÁN HỌC THUẦN TÚY
    // Công thức Poisson: C * (e^-L * L^x) / x!
    TF1 *fPoisson = new TF1("fPoisson", "[0]*TMath::Poisson(x, [1])", minHits-2, maxHits+2);
    fPoisson->SetParameters(tree->GetEntries(), hPoisson->GetMean()); // Cấp giá trị mồi ban đầu
    fPoisson->SetLineColor(kRed);
    fPoisson->SetLineWidth(3);

    hPoisson->Fit("fPoisson", "R"); // Tiến hành khớp đường cong
    hPoisson->Draw("HIST"); // Vẽ các cột
    fPoisson->Draw("SAME"); // Vẽ đường cong đè lên

    // Hiển thị bảng thông số Fit (Chi-square, Mean, Constant)
    gStyle->SetOptFit(1111);
    
    c1->Update();
}