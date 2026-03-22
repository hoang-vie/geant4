Màu Xanh lá (Green): Tia Gamma (Photon).

Màu Đỏ (Red): Electron (Beta -).

Màu Vàng (Yellow): Hạt Alpha.


Để nhìn thấy hình ảnh 3D của hạt trong điện/từ trường
    1. cd ~/build
    2. ./FieldSim
    3. /control/execute run.mac

Để thay đổi độ mạnh yếu điện từ trường ảnh hưởng lên hạt:
    1. Vào file run.mac trong build hoặc run.mac ở ngoài ( nhưng phải make lại trong build sau khi sửa)
    Sưa giá trị ở:
         /field/magValue 0.1 tesla # ĐỘ MẠNH TỪ TRƯỜNG
         /field/elecValue 200    # ĐỘ MẠNH ĐIỆN TRƯỜNG
