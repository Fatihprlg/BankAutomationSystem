#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

time_t t;
struct tm *zaman;

struct musteri
{
    char *isim;
    char *soyisim;
    struct tarih
    {
        int gun, ay, yil;
    } dogumTarihi, limitTarihi;
    int musteriID, sifre, limit;
};

struct ozhesap
{
    int gun, ay, yil;
    int hesapNo, havaleHesapNo;
    char *islemTuru;
    float bakiye;
};

struct hesap
{
    int hesapID, musteriID;
    struct tarih acilmaTarihi;
    float bakiye;
};

int musteri_id_cookie = 0;
int hesap_id_cookie = 0;

int havaleHesapKontrol(int hesapNo)
{
    FILE *hesap;
    struct hesap kontrol;
    int hesapBulundu = 0;
    hesap = fopen("hesaplar.txt", "r");
    while (fscanf(hesap, "%d %d %d/%d/%d %f", &kontrol.musteriID, &kontrol.hesapID, &kontrol.acilmaTarihi.gun, &kontrol.acilmaTarihi.ay, &kontrol.acilmaTarihi.yil, &kontrol.bakiye) != EOF)
        if (hesapNo == kontrol.hesapID)
            hesapBulundu = 1;
    return hesapBulundu;
}
int paraUstuTamamlama(int musteriNo, int hesapNo, float miktar)
{
    FILE *hesap;
    struct hesap tamamlama;

    hesap = fopen("hesaplar.txt", "r");
    while (fscanf(hesap, "%d %d %d/%d/%d %f", &tamamlama.musteriID, &tamamlama.hesapID, &tamamlama.acilmaTarihi.gun, &tamamlama.acilmaTarihi.ay, &tamamlama.acilmaTarihi.yil, &tamamlama.bakiye) != EOF)
    {
        if (tamamlama.musteriID == musteriNo || tamamlama.hesapID != hesapNo)
            miktar -= tamamlama.bakiye;
    }
    fclose(hesap);
    if (miktar <= 0)
        return 1;
    else
        return 0;
}

void paraCekme(int musteriNo, int hesapNo)
{
    int hesapVar = 0, limit, tercih = 0, paraUstuKontrol = 0;
    float islemMiktari, dekontaYazilacak;
    FILE *musteri, *hesap, *yeniHesap, *dekont;
    struct hesap islem1;
    struct musteri okunan;

    okunan.isim = malloc(sizeof(char) * 25);
    okunan.soyisim = malloc(sizeof(char) * 25);

    time(&t);
    zaman = localtime(&t);
    if (musteriNo < 55555)
        musteri = fopen("ticariMusteri.txt", "r");
    else
        musteri = fopen("bireyselMusteri.txt", "r");
    while (fscanf(musteri, "%s\t%s\t%d.%d.%d\t%d\t%d\t%d\t%d", okunan.isim, okunan.soyisim, &okunan.dogumTarihi.gun, &okunan.dogumTarihi.ay, &okunan.dogumTarihi.yil, &okunan.musteriID, &okunan.sifre, &okunan.limit, &okunan.limitTarihi.gun) != EOF)
    {
        if (musteriNo == okunan.musteriID)
        {
            limit = okunan.limit;
        }
    }
    fclose(musteri);

    hesap = fopen("hesaplar.txt", "r");
    yeniHesap = fopen("islem1.txt", "w");

    while (fscanf(hesap, "%d %d %d/%d/%d %f", &islem1.musteriID, &islem1.hesapID, &islem1.acilmaTarihi.gun, &islem1.acilmaTarihi.ay, &islem1.acilmaTarihi.yil, &islem1.bakiye) != EOF)
    {
        if (islem1.hesapID == hesapNo)
        {
            printf("\nCekmek istediginiz tutari giriniz-->");
            scanf("%f", &islemMiktari);
            if (islemMiktari <= limit)
            {
                if (islemMiktari <= islem1.bakiye)
                {
                    hesapVar = 1;
                    dekontaYazilacak = islemMiktari;
                    islem1.bakiye -= islemMiktari;
                    printf("\nPara cekme islemi basarili");
                    fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem1.musteriID, islem1.hesapID, islem1.acilmaTarihi.gun, islem1.acilmaTarihi.ay, islem1.acilmaTarihi.yil, islem1.bakiye);
                }
                else
                {
                    printf("\nHesabinizda yeterli bakiye bulunmamaktadir!");
                    printf("\nBaska bir hesabinizdan ustunu tamamlamak icin 1, ana menuye donmek icin herhangi bir tusa basiniz-> ");
                    scanf("%d", &tercih);
                    if (tercih == 1)
                    {
                        int tamamlama = paraUstuTamamlama(musteriNo, hesapNo, (islemMiktari - islem1.bakiye));
                        if (tamamlama)
                        {
                            paraUstuKontrol = 1;
                            dekontaYazilacak = islemMiktari;
                            islemMiktari -= islem1.bakiye;
                            islem1.bakiye = 0;
                            hesapVar = 1;
                            fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem1.musteriID, islem1.hesapID, islem1.acilmaTarihi.gun, islem1.acilmaTarihi.ay, islem1.acilmaTarihi.yil, islem1.bakiye);
                        }
                        else
                        {
                            printf("\nHesaplarinizda yeterli bakiye bulunmamaktadır!!");
                            fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem1.musteriID, islem1.hesapID, islem1.acilmaTarihi.gun, islem1.acilmaTarihi.ay, islem1.acilmaTarihi.yil, islem1.bakiye);
                        }
                    }
                    else
                        fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem1.musteriID, islem1.hesapID, islem1.acilmaTarihi.gun, islem1.acilmaTarihi.ay, islem1.acilmaTarihi.yil, islem1.bakiye);
                }
            }
            else
            {
                printf("\nGunluk islem limitini astiniz!!!");
                fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem1.musteriID, islem1.hesapID, islem1.acilmaTarihi.gun, islem1.acilmaTarihi.ay, islem1.acilmaTarihi.yil, islem1.bakiye);
            }
        }
        else
        {
            fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem1.musteriID, islem1.hesapID, islem1.acilmaTarihi.gun, islem1.acilmaTarihi.ay, islem1.acilmaTarihi.yil, islem1.bakiye);
        }
    }

    _fcloseall();

    remove("hesaplar.txt");
    rename("islem1.txt", "hesaplar.txt");

    if (paraUstuKontrol)
    {
        hesap = fopen("hesaplar.txt", "r");
        yeniHesap = fopen("yeniHesap.txt", "w");
        while (fscanf(hesap, "%d %d %d/%d/%d %f", &islem1.musteriID, &islem1.hesapID, &islem1.acilmaTarihi.gun, &islem1.acilmaTarihi.ay, &islem1.acilmaTarihi.yil, &islem1.bakiye) != EOF)
        {
            if (islem1.musteriID == musteriNo && islem1.hesapID != hesapNo && islemMiktari != 0)
            {
                if (islem1.bakiye < islemMiktari)
                {
                    islemMiktari -= islem1.bakiye;
                    islem1.bakiye = 0;
                    fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem1.musteriID, islem1.hesapID, islem1.acilmaTarihi.gun, islem1.acilmaTarihi.ay, islem1.acilmaTarihi.yil, islem1.bakiye);
                }
                else
                {
                    islem1.bakiye -= islemMiktari;
                    islemMiktari = 0;
                    fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem1.musteriID, islem1.hesapID, islem1.acilmaTarihi.gun, islem1.acilmaTarihi.ay, islem1.acilmaTarihi.yil, islem1.bakiye);
                }
            }
            else
            {
                fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem1.musteriID, islem1.hesapID, islem1.acilmaTarihi.gun, islem1.acilmaTarihi.ay, islem1.acilmaTarihi.yil, islem1.bakiye);
            }
        }
        printf("\nPara cekme islemi basarili");
        fclose(hesap);
        fclose(yeniHesap);
        remove("hesaplar.txt");
        rename("yeniHesap.txt", "hesaplar.txt");
    }

    if (hesapVar)
    {
        time(&t);
        zaman = localtime(&t);
        dekont = fopen("dekont.txt", "a");
        fprintf(dekont, "%d/%d/%d %d %s %f %d\n", zaman->tm_mday, zaman->tm_mon, zaman->tm_year + 1900, hesapNo, "Para_cekme", 0 - dekontaYazilacak, 0);
        fclose(dekont);
    }
    else
        printf("\nBir hata olustu!");

    int anaMenu;
    printf("\nTekrar denemek icin 1, ana menuye donmek icin herhangi bir tusa basiniz->");
    scanf("%d", &anaMenu);
    if (anaMenu == 1)
        paraCekme(musteriNo, hesapNo);
}

void paraYatirma(int hesapNo)
{
    FILE *hesap, *yeniHesap, *dekont;
    int hesapVar = 0;
    float islemMiktari;
    struct hesap islem2;
    time(&t);
    zaman = localtime(&t);
    hesap = fopen("hesaplar.txt", "r");
    yeniHesap = fopen("yeniHesap.txt", "w");
    while (fscanf(hesap, "%d %d %d/%d/%d %f", &islem2.musteriID, &islem2.hesapID, &islem2.acilmaTarihi.gun, &islem2.acilmaTarihi.ay, &islem2.acilmaTarihi.yil, &islem2.bakiye) != EOF)
    {
        if (islem2.hesapID == hesapNo)
        {
            hesapVar = 1;
            printf("\nYatirmak istediginiz tutari giriniz-> ");
            scanf("%f", &islemMiktari);
            islem2.bakiye += islemMiktari;
            printf("\nPara yatirma islemi basarili!");
            fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem2.musteriID, islem2.hesapID, islem2.acilmaTarihi.gun, islem2.acilmaTarihi.ay, islem2.acilmaTarihi.yil, islem2.bakiye);
        }
        else
        {
            fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem2.musteriID, islem2.hesapID, islem2.acilmaTarihi.gun, islem2.acilmaTarihi.ay, islem2.acilmaTarihi.yil, islem2.bakiye);
        }
    }
    fclose(hesap);
    fclose(yeniHesap);
    remove("hesaplar.txt");
    rename("yeniHesap.txt", "hesaplar.txt");
    if (hesapVar)
    {
        time(&t);
        zaman = localtime(&t);
        dekont = fopen("dekont.txt", "a");
        fprintf(dekont, "%d/%d/%d %d %s %f %d\n", zaman->tm_mday, zaman->tm_mon, zaman->tm_year + 1900, hesapNo, "Para_Yatirma", islemMiktari, 0);
        fclose(dekont);
    }
    else
        printf("\nHesap bulunamadi!");
    int anaMenu;
    printf("\nTekrar denemek icin 1, ana menuye donmek icin herhangi bir tusa basiniz->");
    scanf("%d", &anaMenu);
    if (anaMenu == 1)
        paraYatirma(hesapNo);
}

void havale(int musteriNo, int hesapNo)
{
    FILE *hesap, *yeniHesap, *dekont;
    int hesapNoKontrol = 0, havaleHesapNo, havaleBasarili = 0, hesapVar = 0;
    float islemMiktari;
    struct hesap islem3;
    time(&t);
    zaman = localtime(&t);
    hesap = fopen("hesaplar.txt", "r");
    yeniHesap = fopen("yeniHesap.txt", "w");
    while (fscanf(hesap, "%d %d %d/%d/%d %f", &islem3.musteriID, &islem3.hesapID, &islem3.acilmaTarihi.gun, &islem3.acilmaTarihi.ay, &islem3.acilmaTarihi.yil, &islem3.bakiye) != EOF)
    {
        if (islem3.hesapID == hesapNo)
        {
            hesapVar = 1;
            printf("\nHavale yapilacak hesap numarasini giriniz->");
            scanf("%d", &havaleHesapNo);
            hesapNoKontrol = havaleHesapKontrol(havaleHesapNo);
            if (hesapNoKontrol)
            {
                printf("\nGondermek istediginiz tutari giriniz-> ");
                scanf("%f", &islemMiktari);
                if (islemMiktari <= islem3.bakiye)
                {
                    havaleBasarili = 1;
                    if (musteriNo < 55555)
                        islem3.bakiye -= islemMiktari;
                    else
                        islem3.bakiye -= (islemMiktari + ((islemMiktari * 2) / 100));
                }
                else
                    printf("\nYetersiz bakiye!");
            }
            else
            {
                printf("\nGecersiz hesap numarasi!!");
            }
        }
        fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem3.musteriID, islem3.hesapID, islem3.acilmaTarihi.gun, islem3.acilmaTarihi.ay, islem3.acilmaTarihi.yil, islem3.bakiye);
    }
    _fcloseall();
    remove("hesaplar.txt");
    rename("yeniHesap.txt", "hesaplar.txt");

    if (havaleBasarili)
    {
        hesap = fopen("hesaplar.txt", "r");
        yeniHesap = fopen("yeniHesap.txt", "w");
        while (fscanf(hesap, "%d %d %d/%d/%d %f", &islem3.musteriID, &islem3.hesapID, &islem3.acilmaTarihi.gun, &islem3.acilmaTarihi.ay, &islem3.acilmaTarihi.yil, &islem3.bakiye) != EOF)
        {
            if (havaleHesapNo == islem3.hesapID)
            {
                islem3.bakiye += islemMiktari;
                printf("\nHavale islemi basarili!");
            }
            fprintf(yeniHesap, "%d %d %d/%d/%d %f\n", islem3.musteriID, islem3.hesapID, islem3.acilmaTarihi.gun, islem3.acilmaTarihi.ay, islem3.acilmaTarihi.yil, islem3.bakiye);
        }
        fclose(hesap);
        fclose(yeniHesap);
        remove("hesaplar.txt");
        rename("yeniHesap.txt", "hesaplar.txt");
    }

    fclose(hesap);
    fclose(yeniHesap);
    remove("hesaplar.txt");
    rename("yeniHesap.txt", "hesaplar.txt");

    if (hesapVar)
    {
        time(&t);
        zaman = localtime(&t);
        dekont = fopen("dekont.txt", "a");
        fprintf(dekont, "%d/%d/%d %d %s %f %d\n", zaman->tm_mday, zaman->tm_mon, zaman->tm_year + 1900, hesapNo, "Havale", islemMiktari, havaleHesapNo);
        fclose(dekont);
    }
    else
        printf("\nHesap bulunamadi!");
    int anaMenu;
    printf("\nTekrar denemek icin 1, ana menuye donmek icin herhangi bir tusa basiniz->");
    scanf("%d", &anaMenu);
    if (anaMenu == 1)
        havale(musteriNo, hesapNo);
}

int oturum_ac(int tip_no)
{

    FILE *hesap;
    struct musteri oturum;

    oturum.isim = malloc(sizeof(char) * 25);
    oturum.soyisim = malloc(sizeof(char) * 25);

    int musteri_no, musteri_no_sifre;
    int musteri_no_onay = 0;
    if (tip_no == 1)
    {
        do
        {
            printf("Lutfen musteri numaranizi giriniz: ");
            scanf("%d", &musteri_no);

            hesap = fopen("bireyselMusteri.txt", "r");

            while (fscanf(hesap, "%s\t%s\t%d.%d.%d\t%d\t%d\t%d\t%d", oturum.isim, oturum.soyisim, &oturum.dogumTarihi.gun, &oturum.dogumTarihi.ay, &oturum.dogumTarihi.yil, &oturum.musteriID, &oturum.sifre, &oturum.limit, &oturum.limitTarihi.gun) != EOF)
            {

                if (musteri_no == oturum.musteriID)
                {
                    musteri_no_onay = 1;
                }
            }
            fclose(hesap);

            if (musteri_no_onay != 1)
            {
                printf("Girdiginiz musteri no kayitli degil.\n");
            }
        } while (musteri_no_onay != 1);

        printf("Girdiginiz musteri numarasinin sifresini giriniz: ");
        scanf("%d", &musteri_no_sifre);

        hesap = fopen("bireyselMusteri.txt", "r");

        while (fscanf(hesap, "%s\t%s\t%d.%d.%d\t%d\t%d\t%d\t%d", oturum.isim, oturum.soyisim, &oturum.dogumTarihi.gun, &oturum.dogumTarihi.ay, &oturum.dogumTarihi.yil, &oturum.musteriID, &oturum.sifre, &oturum.limit, &oturum.limitTarihi.gun) != EOF)
        {
            if (oturum.musteriID == musteri_no)
            {
                if (musteri_no_sifre == oturum.sifre)
                {
                    musteri_no_onay = 1;
                }
                else
                {
                    musteri_no_onay = 0;
                }
            }
        }
        fclose(hesap);

        if (musteri_no_onay != 1)
        {
            printf("Girdiginiz sifre yanlis. Tekrar oturum acmak uzere yonlendiriliyorsunuz...\n");
            oturum_ac(1);
        }
    }

    if (tip_no == 2)
    {
        do
        {
            printf("Lutfen musteri numaranizi giriniz: ");
            scanf("%d", &musteri_no);

            hesap = fopen("ticariMusteri.txt", "r");

            while (fscanf(hesap, "%s\t%s\t%d.%d.%d\t%d\t%d\t%d\t%d", oturum.isim, oturum.soyisim, &oturum.dogumTarihi.gun, &oturum.dogumTarihi.ay, &oturum.dogumTarihi.yil, &oturum.musteriID, &oturum.sifre, &oturum.limit, &oturum.limitTarihi.gun) != EOF)
            {

                if (musteri_no == oturum.musteriID)
                {
                    musteri_no_onay = 1;
                }
            }
            fclose(hesap);

            if (musteri_no_onay != 1)
            {
                printf("Girdiginiz musteri no kayitli degil.\n");
            }
        } while (musteri_no_onay != 1);

        printf("Girdiginiz musteri numarasinin sifresini giriniz: ");
        scanf("%d", &musteri_no_sifre);

        hesap = fopen("ticariMusteri.txt", "r");

        while (fscanf(hesap, "%s\t%s\t%d.%d.%d\t%d\t%d\t%d\t%d", oturum.isim, oturum.soyisim, &oturum.dogumTarihi.gun, &oturum.dogumTarihi.ay, &oturum.dogumTarihi.yil, &oturum.musteriID, &oturum.sifre, &oturum.limit, &oturum.limitTarihi.gun) != EOF)
        {
            if (oturum.musteriID == musteri_no)
            {
                if (musteri_no_sifre == oturum.sifre)
                {
                    musteri_no_onay = 1;
                }
                else
                {
                    musteri_no_onay = 0;
                }
            }
        }
        fclose(hesap);

        if (musteri_no_onay != 1)
        {
            printf("Girdiginiz sifre yanlis. Tekrar oturum acmak uzere yonlendiriliyorsunuz...\n");
            oturum_ac(1);
        }
    }

    printf("\n\nGiris basarili.. Aktariliyorsunuz..\n\n");
    return musteri_no;
}

void hesap_ac(int musteriId)
{

    struct hesap hesap_ac;
    FILE *hesap;

    hesap = fopen("hesaplar.txt", "r+");
    if (hesap == NULL)
    {

        hesap_ac.hesapID = 1001;

        hesap = fopen("hesaplar.txt", "w+");

        printf("Hesap ID'niz otomatik olarak olusturuldu. Hesap ID: 1001\n");
        printf("Bakiye --> 0 TL");

        time(&t);
        zaman = localtime(&t);
        hesap_ac.bakiye = 0; //yeni acilan hesap bakiye 0
        fprintf(hesap, "%d %d %d/%d/%d %f\n", musteriId, hesap_ac.hesapID, zaman->tm_mday, zaman->tm_mon, zaman->tm_year + 1900, hesap_ac.bakiye);
    }
    else
    {

        int tmp;
        while (fscanf(hesap, "%d %d %d/%d/%d %f", &hesap_ac.musteriID, &hesap_ac.hesapID, &hesap_ac.acilmaTarihi.gun, &hesap_ac.acilmaTarihi.ay, &hesap_ac.acilmaTarihi.yil, &hesap_ac.bakiye) != EOF)
        {
            tmp = hesap_ac.hesapID;
        }

        tmp++;
        printf("\nHesap ID'niz otomatik olarak olusturuldu. Hesap ID: %d\n", tmp);
        printf("Bakiye --> 0 TL\n\n");
        time(&t);
        zaman = localtime(&t);
        hesap_ac.bakiye = 0; //yeni acilan hesap bakiye 0
        fprintf(hesap, "%d %d %d/%d/%d %f\n", musteriId, tmp, zaman->tm_mday, zaman->tm_mon, zaman->tm_year + 1900, hesap_ac.bakiye);
    }
    fclose(hesap);
}

int hesap_no_kontrol(int musteriID)
{
    FILE *hesap;
    hesap = fopen("hesaplar.txt", "r+");
    struct hesap hesap_ac;
    int hesapID;
    printf("\nLutfen islem yapmak istediginiz hesap no'yu giriniz:");
    scanf("%d", &hesapID);

    int boom = 0;
    if (hesap == NULL)
    {
        printf("Böyle bir hesap bulunamadı.");
    }
    else
    {
        while (fscanf(hesap, "%d %d %d/%d/%d %f", &hesap_ac.musteriID, &hesap_ac.hesapID, &hesap_ac.acilmaTarihi.gun, &hesap_ac.acilmaTarihi.ay, &hesap_ac.acilmaTarihi.yil, &hesap_ac.bakiye) != EOF)
        {
            if (hesap_ac.musteriID == musteriID)
            {
                if (hesap_ac.hesapID == hesapID)
                {
                    printf("\nDogrulama basarili...");
                    boom = 1;
                    break;
                }
                else
                {
                    continue;
                }
            }
        }

        if (boom != 1)
        {
            printf("Girdiginiz hesap no hatali. Lutfen tekrar deneyin.");
            hesap_no_kontrol(musteriID);
        }
    }
    fclose(hesap);
    return hesapID;
}

void hesap_sil(int hesapID)
{
    FILE *hesap, *gecicihesap;
    struct hesap hesapsil;
    hesap = fopen("hesaplar.txt", "r+");
    gecicihesap = fopen("hesaplartmp.txt", "w+");
    if (hesap == NULL)
    {
        printf("Dosyayı acilamadi...");
        exit(1);
    }
    while (fscanf(hesap, "%d %d %d/%d/%d %f", &hesapsil.musteriID, &hesapsil.hesapID, &hesapsil.acilmaTarihi.gun, &hesapsil.acilmaTarihi.ay, &hesapsil.acilmaTarihi.yil, &hesapsil.bakiye) != EOF)
    {
        if (hesapsil.hesapID == hesapID)
        {
            continue;
        }
        fprintf(gecicihesap, "%d %d %d/%d/%d %f\n", hesapsil.musteriID, hesapsil.hesapID, hesapsil.acilmaTarihi.gun, hesapsil.acilmaTarihi.ay, hesapsil.acilmaTarihi.yil, hesapsil.bakiye);
    }
    fclose(hesap);
    fclose(gecicihesap);
    remove("hesaplar.txt");
    rename("hesaplartmp.txt", "hesaplar.txt");
    printf("\nHesap basariyla silindi!");
    printf("\nAna menuye yonlendiriliyorsunuz...");
    Sleep(4000);

}

void hesap_ozet(int hesapID)
{
    FILE *dekontac;
    struct ozhesap ozethesap;
    char menu_secimi;
    ozethesap.islemTuru = malloc(sizeof(char) * 25);
    dekontac = fopen("dekont.txt", "r");

    printf("\t\t\tHESAP OZETI");
    if (dekontac == NULL)
    {
        printf("Hesap ozetiniz bulunmamaktadir.");
        exit(1);
    }

    while (fscanf(dekontac, "%d/%d/%d %d %s %f %d\n", &ozethesap.gun, &ozethesap.ay, &ozethesap.yil, &ozethesap.hesapNo, ozethesap.islemTuru, &ozethesap.bakiye, &ozethesap.havaleHesapNo) != EOF)
    {
        if (ozethesap.hesapNo == hesapID)
        {
            printf("\n\n%d.%d.%d\t%d\t%s\t%f\t%d\n", ozethesap.gun, ozethesap.ay, ozethesap.yil, ozethesap.hesapNo, ozethesap.islemTuru, ozethesap.bakiye, ozethesap.havaleHesapNo);
        }
    }
    fflush(stdin);
    printf("\nAna menuye donmek icin herhangi bir tusa basiniz->");
    scanf("%c", &menu_secimi);
}

void musteriEkleme()
{
    typedef struct musteri musteri;
    int musteriTipi;
    time(&t);
    zaman = localtime(&t);
    while (1)
    {
        Sleep(1000);
        system("cls");
        printf("\n\n\tLutfen sahip olmak istediginiz musteri tipini seciniz!\n\n");
        printf("\t~~~~~~~~~~~~~~~~~~\n");
        printf("\t~~    Ticari Musteri icin bir(1)                  ~~\n");
        printf("\t~~    Bireysel Musteri icin ikiye(2) basiniz      ~~\n");
        printf("\t~~    Cikis (3)                                   ~~\n");
        printf("\t~~~~~~~~~~~~~~~~~~\n");

        printf("-->");
        scanf("%d", &musteriTipi);

        if (musteriTipi == 1)
        {
            musteri okunan;
            okunan.isim = (char *)malloc(sizeof(char) * 25);
            printf("\nIsminizi giriniz:");
            scanf("%s", okunan.isim);
            okunan.soyisim = (char *)malloc(sizeof(char) * 25);
            printf("\nSoy isminizi giriniz:");
            scanf("%s", okunan.soyisim);
            printf("\nDogum Gununuzu giriniz:");
            scanf("%d", &okunan.dogumTarihi.gun);
            printf("\nDogum Ayinizi giriniz:");
            scanf("%d", &okunan.dogumTarihi.ay);
            printf("\nDogum Yilinizi giriniz:");
            scanf("%d", &okunan.dogumTarihi.yil);

            srand(time(NULL));
            okunan.musteriID = 10000 + rand() % 45555;
            okunan.limit = 1500;
            okunan.limitTarihi.gun = zaman->tm_mday;
            printf("\nMusteri No: %d dir.", okunan.musteriID);
            printf("\nLutfen islem yapabilmek icin musteri no unutmayiniz!!!");
            while (1)
            {
                printf("\n\nLutfen profilniz icin dort haneli bir sifre belirleyiniz!!!");
                printf("\n-->");
                scanf("%d", &okunan.sifre);
                if (!(okunan.sifre > 9999 || okunan.sifre < 1000))
                    break;
                else
                    printf("Sifre kurallara uymuyor!! Lutfen tekrar deneyiniz!");
            }

            FILE *ticMus;

            ticMus = fopen("ticariMusteri.txt", "a");
            if (ticMus == NULL)
            {
                printf("Ticari Musteri dosyasi acilamadi!");
                exit(0);
            }
            fprintf(ticMus, "%s\t%s\t%d.%d.%d\t%d\t%d\t%d\t%d\n", okunan.isim, okunan.soyisim, okunan.dogumTarihi.gun, okunan.dogumTarihi.ay, okunan.dogumTarihi.yil, okunan.musteriID, okunan.sifre, okunan.limit, okunan.limitTarihi.gun);
            printf("\n\n.....Musteri ekleme islemi basarili bir sekilde gerceklesmistir.....\n\n");

            fclose(ticMus);
            free(okunan.isim);
            free(okunan.soyisim);

            break;
        }
        if (musteriTipi == 2)
        {
            musteri okunan;
            okunan.isim = (char *)malloc(sizeof(char) * 25);
            printf("\nIsminizi giriniz:");
            scanf("%s", okunan.isim);
            okunan.soyisim = (char *)malloc(sizeof(char) * 25);
            printf("\nSoy isminizi giriniz:");
            scanf("%s", okunan.soyisim);
            printf("\nDogum Gununuzu giriniz:");
            scanf("%d", &okunan.dogumTarihi.gun);
            printf("\nDogum Ayinizi giriniz:");
            scanf("%d", &okunan.dogumTarihi.ay);
            printf("\nDogum Yilinizi giriniz:");
            scanf("%d", &okunan.dogumTarihi.yil);

            srand(time(NULL));
            okunan.musteriID = 55555 + rand() % 44444;
            okunan.limit = 750;
            okunan.limitTarihi.gun = zaman->tm_mday;
            printf("\nMusteri No: %d dir.", okunan.musteriID);
            printf("\nLutfen islem yapabilmek icin musteri no unutmayiniz!!!");

            while (1)
            {
                printf("\n\nLutfen profilniz icin dort haneli bir sifre belirleyiniz!!!");
                printf("\n-->");
                scanf("%d", &okunan.sifre);
                if (!(okunan.sifre > 9999 || okunan.sifre < 1000))
                    break;
                else
                    printf("Sifre kurallara uymuyor!! Lutfen tekrar deneyiniz!");
            }

            FILE *BirMus;

            BirMus = fopen("bireyselMusteri.txt", "a");
            if (BirMus == NULL)
            {
                printf("Bireysel Musteri dosyasi acilamadi!");
                exit(0);
            }
            fprintf(BirMus, "%s\t%s\t%d.%d.%d\t%d\t%d\t%d\t%d\n", okunan.isim, okunan.soyisim, okunan.dogumTarihi.gun, okunan.dogumTarihi.ay, okunan.dogumTarihi.yil, okunan.musteriID, okunan.sifre, okunan.limit, okunan.limitTarihi.gun);
            fclose(BirMus);
            free(okunan.isim);
            free(okunan.soyisim);
            break;
        }
        if (musteriTipi == 3)
            break;
    }
}

int main()
{
    printf("\n\t\t BANKA UYGULAMAMIZA HOSGELDINIZ\n\n\n");

    while (1)
    {
        system("cls");
        int secim1;

        printf("\n\t=================================================");
        printf("\n\t=\t 1.Yeni Musteri Ekleme  \t\t=\n\t=\t 2.Musteri Profiline Giris  \t\t=\n\t=\t 3.Cikis  \t\t\t\t=  ");
        printf("\n\t=================================================");
        printf("\n\n\tLutfen yukarida bulunan seceklerden yapmak istediginizi seciniz:\n");
        printf("-->");
        scanf("%d", &secim1);

        if (secim1 == 1)
        {
            musteriEkleme();
        }
        if (secim1 == 2)
        {
            int oturum_tip_secimi;
            system("cls");
            printf("Musteri hesabinizin kayitli oldugu oldugu tipi seciniz.\n");
            printf("\n(bireysel hesap icin 1, ticari hesap icin 2)\n");
            scanf("%d", &oturum_tip_secimi);

            musteri_id_cookie = oturum_ac(oturum_tip_secimi);

            printf(".... Hosgeldiniz");

            int secim2;
            int musteri_no;
            printf("\n\nYeni hesap acmak icin bir(1)");
            printf("\n Var olan bir hesaba girmek icin ikiye(2) basiniz!");
            scanf("%d", &secim2);

            if (secim2 == 1)
            {
                system("cls");
                hesap_ac(musteri_id_cookie);
                printf("\nYonlendiriliyorsunuz...");
                Sleep(3000);
                secim2 = 2;
            }
            if (secim2 == 2)
            {
                int secim3;
                system("cls");
                hesap_id_cookie = hesap_no_kontrol(musteri_id_cookie);

                printf("\n\nLutfen yapmak istediginiz islemi seciniz!!!");
                printf("\n\nHesabinizdan para cekme islemi icin (1)");
                printf("\nHesabiniza para yatirmak icin (2)");
                printf("\nBaska bir hesaba havale icin (3)");
                printf("\nHesap ozetini gormek icin (4)");
                printf("\nHesabinizi kapatmak icin lutfen (5)'e basiniz");
                printf("\n-->");
                scanf("%d", &secim3);

                if (secim3 == 1)
                {
                    system("cls");
                    paraCekme(musteri_id_cookie, hesap_id_cookie);
                }
                if (secim3 == 2)
                {
                    system("cls");
                    paraYatirma(hesap_id_cookie);
                }
                if (secim3 == 3)
                {
                    system("cls");
                    havale(musteri_id_cookie, hesap_id_cookie);
                }
                if (secim3 == 4)
                {
                    system("cls");
                    hesap_ozet(hesap_id_cookie);
                }
                if (secim3 == 5)
                {
                    system("cls");
                    hesap_sil(hesap_id_cookie);
                }
            }

            if (secim1 == 3)
                break;
        }
    }

    return 0;
}
