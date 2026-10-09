#include <stdio.h>

#define MAX_USERS 100
#define FEE_FREE 0
#define FEE_PERSONAL 120000
#define FEE_FAMILY 250000
#define PENALTY_PER_DEVICE 30000
#define OVERDUE_LIMIT 3

struct UserAccount {
    int user_id;
    char username[31];
    int plan_type;      /* 1: Free, 2: Personal, 3: Family */
    int days_overdue;
    int active_devices;
    int fee;            /* Phi dich vu thuc thu (da gom phi phat) */
};

int main() {
    struct UserAccount users[MAX_USERS];
    int n = 0;
    int i;
    int ok;
    int c;
    int total_revenue = 0;
    int downgraded_count = 0;
    int extra_devices;

    printf("=== HE THONG RA SOAT & HA CAP TAI KHOAN (STREAMFLOW SAAS) ===\n");

    /* ---------- 1. NHAP N (1 <= N <= 100) ---------- */
    do {
        printf("Nhap so luong tai khoan N (1-%d): ", MAX_USERS);
        ok = scanf("%d", &n);
        if (ok == EOF) {
            return 1;
        }
        if (ok != 1) {
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            n = 0;
        }
        if (n < 1 || n > MAX_USERS) {
            printf("[LOI] N phai la so nguyen tu 1 den %d. Vui long nhap lai!\n", MAX_USERS);
        }
    } while (n < 1 || n > MAX_USERS);

    /* ---------- 2. NHAP DU LIEU + VALIDATION ---------- */
    for (i = 0; i < n; i++) {
        printf("\n--- Tai khoan thu %d/%d ---\n", i + 1, n);

        /* Ma tai khoan: so nguyen duong */
        do {
            printf("Ma tai khoan (user_id > 0): ");
            ok = scanf("%d", &users[i].user_id);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                users[i].user_id = 0;
            }
            if (users[i].user_id <= 0) {
                printf("[LOI] Ma tai khoan phai la so nguyen duong. Nhap lai!\n");
            }
        } while (users[i].user_id <= 0);

        /* Ten tai khoan: chuoi khong dau cach, toi da 30 ky tu */
        do {
            printf("Ten tai khoan (khong chua dau cach): ");
            ok = scanf("%30s", users[i].username);
            if (ok == EOF) {
                return 1;
            }
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            if (ok != 1) {
                printf("[LOI] Ten tai khoan khong hop le. Nhap lai!\n");
            }
        } while (ok != 1);

        /* Loai goi: 1, 2 hoac 3 */
        do {
            printf("Loai goi (1: Free, 2: Personal, 3: Family): ");
            ok = scanf("%d", &users[i].plan_type);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                users[i].plan_type = 0;
            }
            if (users[i].plan_type < 1 || users[i].plan_type > 3) {
                printf("[LOI] Loai goi phai thuoc khoang [1, 3]. Nhap lai!\n");
            }
        } while (users[i].plan_type < 1 || users[i].plan_type > 3);

        /* So ngay no cuoc: >= 0 */
        do {
            printf("So ngay no cuoc (>= 0): ");
            ok = scanf("%d", &users[i].days_overdue);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                users[i].days_overdue = -1;
            }
            if (users[i].days_overdue < 0) {
                printf("[LOI] So ngay no cuoc khong duoc am. Nhap lai!\n");
            }
        } while (users[i].days_overdue < 0);

        /* So thiet bi dang ket noi: >= 0 */
        do {
            printf("So thiet bi dang ket noi (>= 0): ");
            ok = scanf("%d", &users[i].active_devices);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                users[i].active_devices = -1;
            }
            if (users[i].active_devices < 0) {
                printf("[LOI] So thiet bi khong duoc am. Nhap lai!\n");
            }
        } while (users[i].active_devices < 0);
    }

    /* ---------- 3. XU LY NGHIEP VU ---------- */
    for (i = 0; i < n; i++) {
        /* Quy tac 1: ha cap do no cuoc >= 3 ngay (chi dem tai khoan dang o goi tra phi) */
        if (users[i].days_overdue >= OVERDUE_LIMIT && users[i].plan_type != 1) {
            users[i].plan_type = 1;
            downgraded_count++;
        }

        /* Phi co ban theo goi (sau khi da ap dung Quy tac 1) */
        if (users[i].plan_type == 1) {
            users[i].fee = FEE_FREE;
        } else if (users[i].plan_type == 2) {
            users[i].fee = FEE_PERSONAL;
        } else {
            users[i].fee = FEE_FAMILY;
        }

        /* Quy tac 2: phi phat thiet bi vuot muc - chi ap dung goi Personal */
        if (users[i].plan_type == 2 && users[i].active_devices > 1) {
            extra_devices = users[i].active_devices - 1;
            users[i].fee += extra_devices * PENALTY_PER_DEVICE;
        }

        total_revenue += users[i].fee;
    }

    /* ---------- 4. XUAT BAO CAO ---------- */
    printf("\n=== BAO CAO RA SOAT TAI KHOAN SAU XU LY ===\n");
    printf("%-8s %-20s %-8s %-12s %-10s %-18s\n",
           "MA TK", "TEN TK", "MA GOI", "SO THIET BI", "SO NGAY NO", "PHI THUC THU (VND)");
    printf("--------------------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%-8d %-20s %-8d %-12d %-10d %-18d\n",
               users[i].user_id,
               users[i].username,
               users[i].plan_type,
               users[i].active_devices,
               users[i].days_overdue,
               users[i].fee);
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("Tong doanh thu thuc te ky billing : %d VND\n", total_revenue);
    printf("So tai khoan bi ha cap ve Free     : %d\n", downgraded_count);

    return 0;
}
