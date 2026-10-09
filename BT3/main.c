/*
 * CloudSync Studio - Kiem toan & Tai cau truc mang tai khoan dang ky
 * Giai phap duoc chon: GIAI PHAP A (In-place Processing)
 *   - Chuan hoa/kiem toan tung ban ghi ngay tren mang goc.
 *   - Sap xep tai cho bang INSERTION SORT (on dinh) theo monthly_fee giam dan.
 * Rang buoc: chi dung ham main(), khong ham tu dinh nghia, khong mang phu.
 *
 * Y tuong then chot: sau kiem toan, tai khoan tra phi luon co monthly_fee > 0
 * (120000 hoac 300000) con tai khoan Free co monthly_fee = 0. Do do chi can
 * sap xep on dinh theo monthly_fee GIAM DAN la dong thoi:
 *   (1) dua nhom tra phi len dau, sap theo phi giam dan,
 *   (2) day nhom Free/ha cap xuong cuoi,
 *   (3) giu nguyen thu tu xuat hien ban dau khi bang phi (insertion sort
 *       chi dich chuyen khi so sanh LON HON NGHIEM NGAT nen khong doi cho
 *       cac phan tu bang nhau).
 */
#include <stdio.h>

#define MAX_N 100

#define FEE_FREE 0
#define FEE_STANDARD 120000
#define FEE_PREMIUM 300000

#define DEV_FREE 1
#define DEV_STANDARD 2
#define DEV_PREMIUM 5

struct UserAccount {
    int account_id;
    int plan_type;       /* 0: Free, 1: Standard, 2: Premium */
    int monthly_fee;
    int remaining_days;
    int active_devices;
    int was_downgraded;  /* co: bi ha cap do het han */
    int device_cut;      /* co: bi cat giam thiet bi */
    int fee_fixed;       /* co: phi bi chuan hoa lai */
};

int main() {
    struct UserAccount acc[MAX_N];
    struct UserAccount key;   /* bien tam phuc vu chen (insertion) */
    int n = 0;
    int i, j;
    int ok, c;
    int max_devices;
    int std_fee;
    int mrr_before = 0;
    int mrr_after = 0;
    int downgraded_count = 0;
    int device_cut_count = 0;
    int fee_fixed_count = 0;
    int paid_count = 0;
    char note[64];
    const char *plan_name;

    printf("=== KIEM TOAN & TAI CAU TRUC TAI KHOAN - CLOUDSYNC STUDIO ===\n");

    /* ---------- 1. NHAP N (1..100) ---------- */
    do {
        printf("Nhap so luong tai khoan N (1-%d): ", MAX_N);
        ok = scanf("%d", &n);
        if (ok == EOF) {
            return 1;
        }
        if (ok != 1) {
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            n = 0;
        }
        if (n < 1 || n > MAX_N) {
            printf("[LOI] N phai la so nguyen tu 1 den %d. Nhap lai!\n", MAX_N);
        }
    } while (n < 1 || n > MAX_N);

    /* ---------- 2. NHAP DU LIEU ---------- */
    /* Chi chan cac loi lam hong cau truc (khong phai so, plan_type ngoai [0,2]).
       Cac gia tri "ban" (ngay am, thiet bi am/lon, phi sai) duoc GIU NGUYEN de
       buoc kiem toan phia sau xu ly va de tinh MRR truoc kiem toan. */
    for (i = 0; i < n; i++) {
        printf("\n--- Tai khoan thu %d/%d ---\n", i + 1, n);

        do {
            printf("account_id: ");
            ok = scanf("%d", &acc[i].account_id);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                printf("[LOI] Vui long nhap so nguyen!\n");
            }
        } while (ok != 1);

        do {
            printf("plan_type (0: Free, 1: Standard, 2: Premium): ");
            ok = scanf("%d", &acc[i].plan_type);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                acc[i].plan_type = -1;
            }
            if (acc[i].plan_type < 0 || acc[i].plan_type > 2) {
                printf("[LOI] plan_type phai thuoc {0, 1, 2}. Nhap lai!\n");
            }
        } while (acc[i].plan_type < 0 || acc[i].plan_type > 2);

        do {
            printf("monthly_fee (VND): ");
            ok = scanf("%d", &acc[i].monthly_fee);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                printf("[LOI] Vui long nhap so nguyen!\n");
            }
        } while (ok != 1);

        do {
            printf("remaining_days (co the am): ");
            ok = scanf("%d", &acc[i].remaining_days);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                printf("[LOI] Vui long nhap so nguyen!\n");
            }
        } while (ok != 1);

        do {
            printf("active_devices: ");
            ok = scanf("%d", &acc[i].active_devices);
            if (ok == EOF) {
                return 1;
            }
            if (ok != 1) {
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                printf("[LOI] Vui long nhap so nguyen!\n");
            }
        } while (ok != 1);

        acc[i].was_downgraded = 0;
        acc[i].device_cut = 0;
        acc[i].fee_fixed = 0;
    }

    /* ---------- 3. KIEM TOAN (chuan hoa tung ban ghi tai cho) ---------- */
    for (i = 0; i < n; i++) {
        /* MRR truoc kiem toan: tong phi dang duoc ghi nhan trong du lieu goc */
        mrr_before += acc[i].monthly_fee;

        /* Bien: thiet bi <= 0 la du lieu rac -> toi thieu 1 */
        if (acc[i].active_devices <= 0) {
            acc[i].active_devices = 1;
        }

        /* Quy tac Het han: remaining_days <= 0 (ke ca am) -> ha ve Free.
           Tai khoan da la Free khong tinh la "bi ha cap". */
        if (acc[i].remaining_days <= 0) {
            if (acc[i].plan_type != 0) {
                acc[i].plan_type = 0;
                acc[i].was_downgraded = 1;
                downgraded_count++;
            }
            acc[i].remaining_days = 0; /* ghi nhan ve 0, tranh gia tri am */
        }

        /* Chuan hoa phi + gioi han thiet bi theo plan_type SAU khi ha cap */
        if (acc[i].plan_type == 0) {
            std_fee = FEE_FREE;
            max_devices = DEV_FREE;
        } else if (acc[i].plan_type == 1) {
            std_fee = FEE_STANDARD;
            max_devices = DEV_STANDARD;
        } else {
            std_fee = FEE_PREMIUM;
            max_devices = DEV_PREMIUM;
        }

        if (acc[i].monthly_fee != std_fee) {
            acc[i].monthly_fee = std_fee;
            acc[i].fee_fixed = 1;
            fee_fixed_count++;
        }

        /* Quy tac Vuot han thiet bi: chi ap dung cho tai khoan CHUA het han
           (remaining_days > 0). Tai khoan vua bi ha cap giu nguyen so thiet bi
           de ghi nhan hien trang vi pham. */
        if (acc[i].remaining_days > 0 && acc[i].active_devices > max_devices) {
            acc[i].active_devices = max_devices;
            acc[i].device_cut = 1;
            device_cut_count++;
        }

        mrr_after += acc[i].monthly_fee;
        if (acc[i].plan_type > 0) {
            paid_count++;
        }
    }

    /* ---------- 4. SAP XEP TAI CHO: INSERTION SORT ON DINH ---------- */
    /* Sap theo monthly_fee giam dan. Dieu kien dich chuyen la '<' (nghiem ngat)
       nen hai phan tu bang phi KHONG bi doi cho -> on dinh.
       Kiem tra j >= 0 DUNG TRUOC acc[j] (short-circuit) -> khong vuot chi so,
       dung ca khi toan bo da het han hoac toan bo con han. */
    for (i = 1; i < n; i++) {
        key = acc[i];
        j = i - 1;
        while (j >= 0 && acc[j].monthly_fee < key.monthly_fee) {
            acc[j + 1] = acc[j];
            j--;
        }
        acc[j + 1] = key;
    }

    /* ---------- 5. XUAT BAO CAO ---------- */
    printf("\n=== KET QUA KIEM TOAN (da sap xep) ===\n");
    printf("%-4s %-10s %-10s %-14s %-12s %-10s %s\n",
           "STT", "MA TK", "GOI", "PHI (VND)", "NGAY CON", "THIET BI", "GHI CHU");
    printf("-----------------------------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        if (acc[i].plan_type == 0) {
            plan_name = "Free";
        } else if (acc[i].plan_type == 1) {
            plan_name = "Standard";
        } else {
            plan_name = "Premium";
        }

        note[0] = '\0';
        if (acc[i].was_downgraded) {
            sprintf(note, "Ha cap (het han)");
        } else if (acc[i].device_cut) {
            sprintf(note, "Cat thiet bi");
        } else if (acc[i].fee_fixed) {
            sprintf(note, "Chuan hoa phi");
        } else {
            sprintf(note, "Hop le");
        }
        if (acc[i].was_downgraded && acc[i].device_cut) {
            sprintf(note, "Ha cap + cat thiet bi");
        }

        printf("%-4d %-10d %-10s %-14d %-12d %-10d %s\n",
               i + 1, acc[i].account_id, plan_name, acc[i].monthly_fee,
               acc[i].remaining_days, acc[i].active_devices, note);
    }
    printf("-----------------------------------------------------------------------------------------\n");
    printf("So tai khoan tra phi (dau mang)      : %d\n", paid_count);
    printf("So tai khoan Free/ha cap (cuoi mang) : %d\n", n - paid_count);
    printf("So tai khoan bi ha cap do het han    : %d\n", downgraded_count);
    printf("So tai khoan bi cat giam thiet bi    : %d\n", device_cut_count);
    printf("So tai khoan duoc chuan hoa lai phi  : %d\n", fee_fixed_count);

    printf("\n=== BAO CAO MRR ===\n");
    printf("MRR truoc kiem toan (ghi nhan goc) : %d VND\n", mrr_before);
    printf("MRR sau kiem toan (thuc te)        : %d VND\n", mrr_after);
    printf("Chenh lech (sau - truoc)           : %d VND\n", mrr_after - mrr_before);

    return 0;
}
