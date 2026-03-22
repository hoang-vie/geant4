Màu Xanh lá (Green): Tia Gamma (Photon).
Màu Đỏ (Red): Electron (Beta -).
Màu Vàng (Yellow): Hạt Alpha.

Để khảo sát phân bố poisson của hạt:
    1. cd build
    2. ./DecaySim run.mac 
    # Lưu ý, nếu muốn thay đổi nguồn phóng xạ, hãy vào run.mac trong build chọn lại nguồn phù hợp.
    Hiện tại có sẵn các nguồn:  #/gps/ion 55 137 0 0 # Cs 137
                                #/gps/ion 38 90 0 0  #Strontium-90 (Sr-90) 
                                #/gps/ion 92 238 0 0  #Uranium-238 (U-238) 
                                #/gps/ion 95 241 0 0  #Americium-241
                                #/gps/ion 27 60 0 0 #Cobalt-60 (Co-60)
    Nếu sữa run.mac ở ngoài build thì phải make -j8 lại.


Để khảo sát mô phỏng phân rã của nguồn phóng xạ.
    1. cd build
    2. ./DecaySim
    3. run/beamOn nevent_decay 
    nevent_decay = 1 là 1 lần phân rã của nguồn


                        