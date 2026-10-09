# Báo cáo Dò vết & Sửa lỗi – Subscription Report (Streamify)

## 1. Xác định lỗi

**Dòng sai (vòng `for`, bên trong thân vòng lặp):**

```c
if (user_list[0].days_overdue <= 3) {          // LỖI 1
    total_revenue += user_list[0].monthly_fee; // LỖI 2
```

**Nguyên nhân kỹ thuật:** chỉ số mảng bị gán cứng bằng `0` thay vì dùng biến đếm `i`.
Vòng lặp vẫn chạy `i = 0..3`, nhưng điều kiện và phép cộng luôn đọc phần tử `user_list[0]`
(tài khoản 1001: `days_overdue = 0`, `monthly_fee = 180000`).

- Lỗi 1: điều kiện `user_list[0].days_overdue <= 3` luôn đúng (0 <= 3) với mọi vòng lặp
  → nhánh `"Hop le"` luôn được chọn, kể cả tài khoản 1002 (quá hạn 5 ngày) và 1004 (quá hạn 4 ngày).
- Lỗi 2: `total_revenue += user_list[0].monthly_fee` cộng đi cộng lại 180000
  → tổng doanh thu bị lặp giá trị phí của tài khoản đầu tiên.
- Riêng lệnh `printf` dùng đúng `user_list[i]` nên dữ liệu hiển thị (mã TK, phí, ngày quá hạn)
  vẫn đúng, chỉ có **trạng thái** và **tổng doanh thu** sai – đây là lý do lỗi khó phát hiện bằng mắt.

## 2. Dò vết (trace) chương trình gốc

| Vòng | i | Phần tử thực sự bị đọc | days_overdue đọc | Điều kiện `<= 3` | Cộng vào tổng | total_revenue |
|------|---|------------------------|------------------|------------------|---------------|---------------|
| 1 | 0 | user_list[0] | 0 | Đúng | +180000 | 180000 |
| 2 | 1 | user_list[0] (sai, đúng ra là [1]) | 0 | Đúng | +180000 | 360000 |
| 3 | 2 | user_list[0] (sai, đúng ra là [2]) | 0 | Đúng | +180000 | 540000 |
| 4 | 3 | user_list[0] (sai, đúng ra là [3]) | 0 | Đúng | +180000 | 720000 |

Kết quả sai: cả 4 tài khoản đều "Hop le", tổng = **720000 VND**.

## 3. Bảng Test Cases đối chứng

| Trường hợp kiểm thử | Dữ liệu đầu vào | Kết quả sai thực tế (code gốc) | Kết quả đúng mong đợi |
|---------------------|-----------------|--------------------------------|------------------------|
| TC01 – Trạng thái tài khoản quá hạn > 3 ngày | TK 1002: phí 90000, quá hạn 5 ngày; TK 1004: phí 90000, quá hạn 4 ngày | Cả hai hiển thị **"Hop le"** | Cả hai hiển thị **"Qua han"** |
| TC02 – Tổng doanh thu thực thu | 1001 (180000, 0 ngày), 1002 (90000, 5 ngày), 1003 (260000, 1 ngày), 1004 (90000, 4 ngày) | **720000 VND** (180000 × 4) | **440000 VND** (180000 + 260000) |

## 4. Cách sửa

Thay chỉ số cố định `0` bằng biến đếm `i` ở cả hai vị trí:

```c
if (user_list[i].days_overdue <= 3) {
    total_revenue += user_list[i].monthly_fee;
```

(Ngoài ra sửa chú thích "Gói Premium" thành "Gói Personal" cho đúng nghiệp vụ.)
Không dùng hàm tự định nghĩa hay cấu trúc nâng cao; chỉ dùng mảng struct, `for`, `if/else`, `printf`.

## 5. Kiểm chứng sau khi sửa

Biên dịch: `gcc -Wall -Wextra -std=c11 subscription_report.c -o subscription_report` → **0 warning**.

Kết quả chạy:

```
=== DANH SACH TAI KHOAN SUBSCRIPTION ===
MA TK      PHI THANG    NO CUOC (NGAY)  TRANG THAI
---------------------------------------------------
1001       180000       0               Hop le
1002       90000        5               Qua han
1003       260000       1               Hop le
1004       90000        4               Qua han
---------------------------------------------------
Tong doanh thu thuc thu: 440000 VND
```

Khớp 100% với kết quả mong đợi của TC01 và TC02.
