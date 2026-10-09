# Bài tập 03 – Trade-off: Sắp xếp tại chỗ vs Mảng trung gian (CloudSync Studio)

## 1. Phân tích Input / Output
**Input:** `N` (1..100) và N bản ghi `UserAccount{account_id, plan_type, monthly_fee, remaining_days, active_devices}`; dữ liệu có thể "bẩn" (ngày âm, phí sai, thiết bị ≤ 0 hoặc rất lớn).
**Output:** bảng kiểm toán sau khi chuẩn hóa và sắp xếp (gói trả phí lên đầu, phí giảm dần, ổn định; Free/hạ cấp xuống cuối); thống kê số TK hạ cấp / cắt thiết bị / chuẩn hóa phí; MRR trước – sau – chênh lệch.

Bảng quy chuẩn: Free (0): 0đ, 1 thiết bị · Standard (1): 120.000đ, 2 thiết bị · Premium (2): 300.000đ, 5 thiết bị.

## 2. Hai giải pháp
**A – In-place:** kiểm toán trực tiếp trên mảng gốc, rồi sắp xếp bằng *insertion sort* (ổn định, dịch phần tử bằng phép gán struct, chỉ cần 1 biến struct tạm `key`).
**B – Mảng trung gian:** kiểm toán xong, tách thành các mảng phụ (`premium[]`, `standard[]`, `free[]`) theo thứ tự xuất hiện, rồi ghép lại vào mảng chính.

Nhận xét then chốt: sau chuẩn hóa, TK trả phí luôn có phí > 0, TK Free có phí = 0. Vì vậy **một lần sắp xếp ổn định theo `monthly_fee` giảm dần** đã thỏa cả 3 yêu cầu (trả phí lên đầu, phí giảm dần, Free xuống cuối, giữ thứ tự gốc khi bằng phí).

## 3. Bảng so sánh
| Tiêu chí | Giải pháp A (In-place, insertion sort) | Giải pháp B (Mảng trung gian) |
|---|---|---|
| Time complexity | O(N²) trường hợp xấu nhất (N ≤ 100 → tối đa ~5.000 phép dịch, không đáng kể); O(N) nếu mảng gần như đã sắp | O(N) (duyệt tách nhóm + ghép) |
| Space complexity | O(1) – chỉ 1 struct tạm | O(N) – 3 mảng phụ × 100 struct (~3 × 100 × 32 byte ≈ 9,6 KB, hoặc phải khai báo tối đa mỗi mảng cho N) |
| Độ phức tạp mã (không dùng hàm) | Trung bình: 1 vòng `for` + 1 vòng `while`, ngắn gọn | Cao hơn: 3 mảng, 3 bộ đếm, 3 khối `if` tách + 3 vòng ghép; nhân bản code |
| Rủi ro lỗi logic | Cần chú ý điều kiện `j >= 0` đặt trước `acc[j]` và dấu `<` nghiêm ngặt (giữ ổn định). Rủi ro thấp, ít biến | Nhiều chỉ số/bộ đếm dễ lệch (quên cộng, ghép sai thứ tự, vượt chỉ số khi nhóm rỗng); thêm phí mới (gói mới) phải sửa nhiều nơi |
| Tính tổng quát | Sắp theo phí bất kỳ, thêm gói mới không cần sửa | Phụ thuộc cố định 3 mức phí |

## 4. Lựa chọn: Giải pháp A
- Bài toán nhấn mạnh **môi trường hạn chế, cấm hàm phụ** → ưu tiên ít mã, ít biến, bộ nhớ O(1).
- Với N ≤ 100, chênh lệch thời gian O(N²) so với O(N) là không đáng kể; trade-off nghiêng về giảm bộ nhớ và độ phức tạp mã.
- Insertion sort tự nhiên ổn định, không cần mảng phụ để bảo toàn thứ tự gốc.
- Trường hợp biên (toàn bộ hết hạn / toàn bộ còn hạn) không phát sinh lỗi vì `while` kiểm tra `j >= 0` trước khi truy cập `acc[j]`.
- (Nếu N lên hàng triệu, nên chuyển sang B hoặc counting/bucket vì O(N).)

## 5. Mã giả (Giải pháp A)
```
đọc N (lặp đến khi 1 ≤ N ≤ 100)
đọc từng tài khoản; plan_type phải ∈ {0,1,2}
mrr_before = 0; mrr_after = 0

FOR i = 0..N-1:                         # BƯỚC KIỂM TOÁN
    mrr_before += fee[i]
    IF devices[i] <= 0: devices[i] = 1
    IF days[i] <= 0:                    # Quy tắc hết hạn
        IF plan[i] != 0: plan[i] = 0; đếm hạ cấp
        days[i] = 0
    (std_fee, max_dev) = tra bảng theo plan[i]
    IF fee[i] != std_fee: fee[i] = std_fee; đếm chuẩn hóa
    IF days[i] > 0 AND devices[i] > max_dev:   # Quy tắc vượt thiết bị
        devices[i] = max_dev; đếm cắt
    mrr_after += fee[i]

FOR i = 1..N-1:                         # BƯỚC SẮP XẾP (insertion sort ổn định)
    key = acc[i]; j = i - 1
    WHILE j >= 0 AND acc[j].fee < key.fee:
        acc[j+1] = acc[j]; j = j - 1
    acc[j+1] = key

in bảng, thống kê, MRR trước / sau / chênh lệch
```

## 6. Quyết định xử lý các điểm đề chưa nêu rõ
- **MRR trước kiểm toán** = tổng `monthly_fee` đang ghi nhận trong dữ liệu gốc (kể cả TK hết hạn, phí sai). **MRR sau** = tổng phí sau chuẩn hóa.
- TK đã là Free mà hết hạn: không tính là "bị hạ cấp".
- TK hết hạn: `remaining_days` đưa về 0; số thiết bị giữ nguyên (quy tắc vượt thiết bị chỉ áp dụng cho TK *chưa hết hạn*, đúng đề). Thiết bị ≤ 0 luôn được nâng về 1.
- Chuẩn hóa phí thực hiện **sau** khi hạ cấp.
- Chỉ chặn nhập sai cấu trúc (không phải số, `N` ngoài [1,100], `plan_type` ngoài {0,1,2}); các giá trị "bẩn" khác được nhận vào để kiểm toán xử lý.

## 7. Test đã chạy (`gcc -Wall -Wextra -pedantic -std=c11`, 0 warning)
| Tình huống | Kết quả |
|---|---|
| 7 TK hỗn hợp (ngày âm, 99 thiết bị, thiết bị âm, phí sai, bằng phí) | Premium 3→7, Standard 1→4, Free 2→5→6 (ổn định); MRR 1.140.000 → 840.000 (−300.000) |
| Toàn bộ hết hạn | 3 TK Free, MRR sau = 0, không lỗi chỉ số |
| Toàn bộ còn hạn | Giữ nguyên MRR, chỉ sắp xếp Premium lên đầu |
| N = 0, nhập chữ, plan_type = 9 | Báo lỗi và yêu cầu nhập lại |
