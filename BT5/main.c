/*
 * Demo quan ly thue bao SaaS - Array of Structs
 * Nhap N goi dich vu, in bang can cot chuan, tinh tong doanh thu hang thang.
 * Rang buoc: toan bo xu ly nam trong main(), khong dung ham tu dinh nghia.
 */
#include <stdio.h>

#define MAX_PLANS 100
#define PLAN_NAME_LEN 30

/* Mot goi dich vu dang ky SaaS */
struct Subscription {
    int sub_id;                         /* Ma goi dang ky (so nguyen duong) */
    char plan_name[PLAN_NAME_LEN + 1];  /* Ten goi (khong chua khoang trang) */
    double monthly_fee;                 /* Cuoc phi thang (USD) */
    int user_limit;                     /* Gioi han so nguoi dung (so nguyen duong) */
};

int main() {
    struct Subscription plans[MAX_PLANS];
    int n = 0;
    int i;
    double total_revenue = 0.0;

    /* ---------- 1. Nhap N va kiem tra hop le (1 <= N <= 100) ---------- */
    /* scanf tra ve != 1 (vd: nhap chu) cung duoc xem la khong hop le */
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PLANS) {
        printf("So luong goi dich vu khong hop le!\n");
        return 0;
    }

    /* ---------- 2. Nhap danh sach goi dich vu ---------- */
    for (i = 0; i < n; i++) {
        if (scanf("%d %30s %lf %d",
                  &plans[i].sub_id,
                  plans[i].plan_name,
                  &plans[i].monthly_fee,
                  &plans[i].user_limit) != 4
            || plans[i].sub_id <= 0
            || plans[i].monthly_fee < 0
            || plans[i].user_limit <= 0) {
            printf("Du lieu goi dich vu thu %d khong hop le!\n", i + 1);
            return 0;
        }

        /* Tich luy cuoc phi cua tung phan tu vao tong doanh thu */
        total_revenue += plans[i].monthly_fee;
    }

    /* ---------- 3. In bang tong hop ---------- */
    printf("%-10s %-20s %-16s %-15s\n", "MA GOI", "TEN GOI", "CUOC PHI (USD)", "GIOI HAN USER");
    printf("---------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%-10d %-20s %-16.2f %-15d\n",
               plans[i].sub_id,
               plans[i].plan_name,
               plans[i].monthly_fee,
               plans[i].user_limit);
    }
    printf("---------------------------------------------------------------\n");

    /* ---------- 4. In tong doanh thu (lam tron 2 chu so thap phan) ---------- */
    printf("TONG DOANH THU HANG THANG: %.2f USD\n", total_revenue);

    return 0;
}
