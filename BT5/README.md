# Bài tập 05 – Demo Quản lý Thuê bao SaaS (Array of Structs)

## Giải pháp
- `struct Subscription { int sub_id; char plan_name[31]; double monthly_fee; int user_limit; }`
- Mảng `plans[100]`; toàn bộ nhập – tính – xuất nằm trong `main()`, không có hàm tự định nghĩa.
- Đọc `N`: nếu `N <= 0`, `N > 100` hoặc nhập không phải số → in `So luong goi dich vu khong hop le!` và kết thúc.
- Chương trình không in lời nhắc nhập để đầu ra khớp chính xác với mẫu của đề.
- Mỗi bản ghi đọc bằng `scanf("%d %30s %lf %d")`; `%30s` chặn tràn bộ đệm. Bản ghi lỗi (không đủ 4 giá trị, mã ≤ 0, phí < 0, giới hạn ≤ 0) → báo lỗi và dừng an toàn.
- Tổng doanh thu cộng dồn `monthly_fee` trong vòng lặp nhập, in bằng `%.2f`.
- In bảng bằng `%-10d %-20s %-16.2f %-15d`.

## Nghiệm thu
| Hạng mục | Kết quả |
|---|---|
| Biên dịch `gcc -Wall -Wextra -pedantic -std=c11` | 0 warning, 0 lỗi |
| Case 1: `3 / 101 Starter 29.99 5 / 102 Pro 99.50 25 / 103 Enterprise 299.00 100` | Đúng mẫu, `TONG DOANH THU HANG THANG: 428.49 USD` |
| Case 2: `0` | `So luong goi dich vu khong hop le!` |
| `N = -4`, `N = 101`, `N = abc` | Cùng thông báo lỗi, không crash |
| Bản ghi lỗi (`x` thay cho phí) | Báo lỗi dòng thứ 2, dừng an toàn |
