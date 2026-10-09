/*
 * StreamFlix - SaaS Subscription Auditor Module
 * Mo-dun ra soat & ha cap tai khoan dang ky.
 * Luong du lieu: NHAP + KIEM TRA -> RULE ENGINE (3 quy tac) -> BANG + THONG KE.
 * Rang buoc: chi dung ham main(), mang struct toi da 50 phan tu.
 */
#include <stdio.h>

#define MAX_USERS 50
#define NAME_LEN 30

#define PLAN_FREE 1
#define PLAN_INDIVIDUAL 2
#define PLAN_FAMILY 3

#define OVERDUE_LIMIT 3  /* qua han > 3 ngay thi ha cap */

#define STATUS_ACTIVE 1
#define STATUS_DOWNGRADED 0

struct Subscription {
    int user_id;
    char account_name[NAME_LEN + 1];
    int plan_type;       /* 1: Free, 2: Individual, 3: Family */
    int active_devices;
    int max_devices;
    int days_overdue;
    int account_status;  /* 1: Active, 0: Downgraded */
    int violation;       /* 1: vuot gioi han thiet bi (ket qua Quy tac 3) */
};

int main() {
    struct Subscription subs[MAX_USERS];
    int n = 0;
    int i, k;
    int ok, c;
    int dup;
    int downgraded_count = 0;
    int violation_count = 0;
    const char *plan_name;
    const char *status_name;
    char dev_text[24];

    printf("==============================================================\n");
    printf("   STREAMFLIX - SAAS SUBSCRIPTION AUDITOR MODULE\n");
    printf("==============================================================\n");

    /* ================= 1. NHAP N (1..50) ================= */
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
            printf("[LOI] N phai la so nguyen tu 1 den %d. Nhap lai!\n", MAX_USERS);
        }
    } while (n < 1 || n > MAX_USERS);

    /* ================= 2. NHAP & KIEM TRA TUNG TAI KHOAN ================= */
    for (i = 0; i < n; i++) {
        printf("\n--- Tai khoan thu %d/%d ---\n", i + 1, n);

        /* user_id: so nguyen duong va KHONG TRUNG voi cac tai khoan truoc */
        do {
            dup = 0;
            printf("user_id (so nguyen duong, duy nhat): ");
            ok = scanf("%d", &subs[i].user_id);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                subs[i].user_id = 0;
            }
            for (k = 0; k < i; k++) {
                if (subs[k].user_id == subs[i].user_id) {
                    dup = 1;
                }
            }
            if (subs[i].user_id <= 0) {
                printf("[LOI] user_id phai la so nguyen duong. Nhap lai!\n");
            } else if (dup) {
                printf("[LOI] user_id da ton tai. Nhap ma khac!\n");
            }
        } while (subs[i].user_id <= 0 || dup);

        /* account_name: toi da 30 ky tu, khong chua dau cach (vd: email) */
        do {
            printf("account_name (toi da %d ky tu, khong dau cach): ", NAME_LEN);
            ok = scanf("%30s", subs[i].account_name);
            if (ok == EOF) {
                return 1;
            }
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            if (ok != 1) {
                printf("[LOI] Ten tai khoan khong hop le. Nhap lai!\n");
            }
        } while (ok != 1);

        /* plan_type: 1, 2 hoac 3 */
        do {
            printf("plan_type (1: Free, 2: Individual, 3: Family): ");
            ok = scanf("%d", &subs[i].plan_type);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                subs[i].plan_type = 0;
            }
            if (subs[i].plan_type < 1 || subs[i].plan_type > 3) {
                printf("[LOI] plan_type phai la 1, 2 hoac 3. Nhap lai!\n");
            }
        } while (subs[i].plan_type < 1 || subs[i].plan_type > 3);

        /* active_devices: >= 0 */
        do {
            printf("active_devices (>= 0): ");
            ok = scanf("%d", &subs[i].active_devices);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                subs[i].active_devices = -1;
            }
            if (subs[i].active_devices < 0) {
                printf("[LOI] So thiet bi khong duoc am. Nhap lai!\n");
            }
        } while (subs[i].active_devices < 0);

        /* days_overdue: >= 0 */
        do {
            printf("days_overdue (>= 0): ");
            ok = scanf("%d", &subs[i].days_overdue);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                subs[i].days_overdue = -1;
            }
            if (subs[i].days_overdue < 0) {
                printf("[LOI] So ngay no khong duoc am. Nhap lai!\n");
            }
        } while (subs[i].days_overdue < 0);
    }

    /* ================= 3. BUSINESS RULES ENGINE ================= */
    for (i = 0; i < n; i++) {
        subs[i].account_status = STATUS_ACTIVE;
        subs[i].violation = 0;

        /* Quy tac 1: han muc thiet bi mac dinh theo goi */
        if (subs[i].plan_type == PLAN_FAMILY) {
            subs[i].max_devices = 5;
        } else {
            subs[i].max_devices = 1; /* Free va Individual */
        }

        /* Quy tac 2: no cuoc > 3 ngay -> ha cap ve Free.
           Tai khoan da o goi Free thi khong co goi nao de ha cap nen khong
           tinh la "bi ha cap". */
        if (subs[i].days_overdue > OVERDUE_LIMIT && subs[i].plan_type != PLAN_FREE) {
            subs[i].plan_type = PLAN_FREE;
            subs[i].max_devices = 1;
            subs[i].account_status = STATUS_DOWNGRADED;
            downgraded_count++;
        }

        /* Quy tac 3: so sanh thiet bi voi han muc SAU khi da ha cap */
        if (subs[i].active_devices > subs[i].max_devices) {
            subs[i].violation = 1;
            violation_count++;
        }
    }

    /* ================= 4. BANG KET QUA ================= */
    printf("\n==================== DANH SACH TAI KHOAN SAU RA SOAT ====================\n");
    printf("%-9s %-31s %-11s %-10s %-9s %-11s %s\n",
           "USER_ID", "ACCOUNT_NAME", "GOI", "THIET BI", "NGAY NO", "TRANG THAI", "CANH BAO");
    printf("---------------------------------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        if (subs[i].plan_type == PLAN_FREE) {
            plan_name = "Free";
        } else if (subs[i].plan_type == PLAN_INDIVIDUAL) {
            plan_name = "Individual";
        } else {
            plan_name = "Family";
        }

        if (subs[i].account_status == STATUS_ACTIVE) {
            status_name = "Active";
        } else {
            status_name = "Downgraded";
        }

        sprintf(dev_text, "%d/%d", subs[i].active_devices, subs[i].max_devices);

        printf("%-9d %-31s %-11s %-10s %-9d %-11s %s\n",
               subs[i].user_id,
               subs[i].account_name,
               plan_name,
               dev_text,
               subs[i].days_overdue,
               status_name,
               subs[i].violation ? "VUOT THIET BI" : "-");
    }
    printf("---------------------------------------------------------------------------------------------\n");

    /* ================= 5. BAO CAO THONG KE VAN HANH ================= */
    printf("\n================ BAO CAO THONG KE VAN HANH ================\n");
    printf("Tong so tai khoan da ra soat                 : %d\n", n);
    printf("So tai khoan bi ha cap ve Free (no > %d ngay) : %d\n", OVERDUE_LIMIT, downgraded_count);
    printf("So tai khoan vi pham gioi han thiet bi       : %d\n", violation_count);
    printf("===========================================================\n");

    return 0;
}
