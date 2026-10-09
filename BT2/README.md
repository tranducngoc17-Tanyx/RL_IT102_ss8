# Bài tập 02 – Batch Plan Audit Engine (StreamFlow SaaS)

## 1. Phân tích I/O
- **Input:** `N` (1..100); với mỗi tài khoản: `user_id`, `username`, `plan_type` (1..3), `days_overdue` (>= 0), `active_devices` (>= 0).
- **Output:** bảng tài khoản sau rà soát (Mã TK, Tên TK, Mã Gói, Số Thiết Bị, Số Ngày Nợ, Phí Thực Thu); tổng doanh thu kỳ billing; số tài khoản bị hạ cấp về Free.

## 2. Giải pháp
Dùng `struct UserAccount` (thêm trường `fee`) và mảng `users[100]`. Toàn bộ nhập – xử lý – xuất nằm trong `main()`, dùng `do...while` để validate, `for` để duyệt mảng, `if/else` để áp dụng quy tắc nghiệp vụ. Không có hàm tự định nghĩa nào khác.

## 3. Các bước thực thi
1. Nhập `N`, lặp lại đến khi 1 <= N <= 100.
2. Với từng tài khoản, nhập và kiểm tra: `user_id > 0`, `plan_type` thuộc [1,3], `days_overdue >= 0`, `active_devices >= 0`. Sai (kể cả nhập chữ) → báo lỗi, xóa bộ đệm, nhập lại.
3. Duyệt mảng và xử lý:
   - **Quy tắc 1:** `days_overdue >= 3` và đang ở gói trả phí → `plan_type = 1`, tăng bộ đếm hạ cấp.
   - Gán phí theo gói *sau khi hạ cấp*: Free 0, Personal 120.000, Family 250.000.
   - **Quy tắc 2:** nếu `plan_type == 2` và `active_devices > 1` → phí += `(active_devices - 1) * 30.000`.
   - Cộng phí vào tổng doanh thu.
4. In bảng, tổng doanh thu, số tài khoản bị hạ cấp.

## 4. Chống lỗi dữ liệu
- **Lỗi 1:** số âm / gói ngoài [1,3] / nhập không phải số → chặn bằng vòng `do...while`.
- **Lỗi 2:** Family 4 thiết bị nợ 4 ngày → hạ về Free **trước** khi xét Quy tắc 2, nên phí = 0, không bị phạt thiết bị.

## 5. Test mẫu
| user_id | Gói | Ngày nợ | Thiết bị | Kết quả mong đợi |
|---|---|---|---|---|
| 1 | Personal | 0 | 3 | 120.000 + 2×30.000 = 180.000 |
| 2 | Personal | 4 | 1 | Hạ cấp, 0 |
| 3 | Family | 4 | 4 | Hạ cấp, 0 (không phạt) |
| 4 | Free | 5 | 2 | Giữ Free, 0 (không tính là hạ cấp) |

Tổng = **180.000 VND**, số tài khoản bị hạ cấp = **2**.
Biên dịch: `gcc -Wall -Wextra -pedantic -std=c11 main.c` → 0 warning.
