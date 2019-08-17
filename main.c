#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
//Dosyadan satýr satýr okumak için araþtýrmalar sonucunda bulduðumuz fonksiyondur
//Ancak fonksiyonun orijinali Linux & Posix sistemlerde olup windowsta olmamasýndan dolayý çalýþmamýþtýr ve
//araþtýrmamýz sonucu c de yazýlmýþ fonksiyon koda entegre edilmiþtir.
//https://stackoverflow.com/questions/735126/are-there-alternate-implementations-of-gnu-getline-interface/735472#735472
size_t getline(char **lineptr, size_t *n, FILE *stream)
{
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;
    if (lineptr == NULL)
	{
        return -1;
    }
    if (stream == NULL) 
	{
        return -1;
    }
    if (n == NULL) 
	{
        return -1;
    }
    bufptr = *lineptr;
    size = *n;
    c = fgetc(stream);
    if (c == EOF)
	{
        return -1;
    }
    if (bufptr == NULL) 
	{
        bufptr = malloc(128);
        if (bufptr == NULL) 
		{
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) 
	{
        if ((p - bufptr) > (size - 1)) 
		{
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) 
			{
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') 
		{
            break;
        }
        c = fgetc(stream);
    }
    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;
    return p - bufptr - 1;
}
typedef struct ogrenci
{
    char* no;
    char* ad;
    char* soyad;
    char* kayitsirasi;
    char* ogrtipi;
}Ogrenci;
//kendi veri tipimiz 'ogrenci' yi oluþturduk
//bu veri tipi bir öðrencinin ad soyad kayýt sýrasý öðrenci no ve öðretim tipini depolayabilir.
typedef struct sinif
{
    Ogrenci* ogrenciler;
    int ogrencisayisi;
}Sinif;

//burada ise 'ogrenci' ve 'ogrencisayisi' ndan oluþan sýnýf yapýmýzý oluþturduk.
//bu yapý,ogrenci sayisi kadar öðrencinin bilgisini içinde taþýyacak
void ogrencileriyazdir (Ogrenci ogrenci)
{
    printf("Öðrenci : %s %s %s %s %s",ogrenci.no, ogrenci.ad,ogrenci.soyad,ogrenci.kayitsirasi,ogrenci.ogrtipi);

};

//bu fonksiyon char dizisi içinde verilen öðrenci bilgisini parça parça alýp yeni öðrenci structý oluþturduk.
Ogrenci yeniogrenciolustur(char* ogrencibilgisi[])
{
    Ogrenci yeniogrenci;
    yeniogrenci.no = malloc(sizeof(char)*7);
    yeniogrenci.ad = malloc(sizeof(char)*20);
    yeniogrenci.soyad = malloc(sizeof(char)*20);
    yeniogrenci.kayitsirasi = malloc(sizeof(char)*5);
    yeniogrenci.ogrtipi = malloc(sizeof(char)*5);
    strcpy(yeniogrenci.no, ogrencibilgisi[0]);
    strcpy(yeniogrenci.ad, ogrencibilgisi[1]);
    strcpy(yeniogrenci.soyad, ogrencibilgisi[2]);
    strcpy(yeniogrenci.kayitsirasi, ogrencibilgisi[3]);
    strcpy(yeniogrenci.ogrtipi, ogrencibilgisi[4]);
    return yeniogrenci;
}

// Verilen öðrenci için yeni numara ürettik.
char* ogrencinumarasiolustur (Ogrenci ogrenci)
{
    int yil = 1700000;
    int birinciogretim = 1000; //1.öðretim için
    int ikinciogretim = 2000; //2.öðretim için
    int yeniogrencino = 0;
    char* str = strstr(ogrenci.ogrtipi,"II");
    //içinde II yok ise null dönecek.null ise birinci öðretimdir
    if (str == NULL)
	{
        yeniogrencino = yil + birinciogretim + atoi(ogrenci.kayitsirasi);
    }
	else
	{
        yeniogrencino = yil + ikinciogretim + atoi(ogrenci.kayitsirasi);
    }
    // int olarak topladýðýmýz öðrenci nosunu 7 haneli stringe çevirdik.
    char* ogrno = malloc(sizeof(char)*7);
    sprintf(ogrno, "%d", yeniogrencino);
    return ogrno;
}
Ogrenci satir_veri_cek(char* satir)
{
    char* tmp = malloc(sizeof(char)*strlen(satir));
    strcpy(tmp, satir);
    //satýr uzunluðunda bir string oluþturduk ve tmp e atadýk
    char* bilgi = strtok(tmp," \t");
    //bunu tab boþluklarýna göre böldük ve her bir parçayý ogrencibilgisi dizisine eleman olarak attýk ( çünkü txt dosyasýnda bilgiler arasýnda tab var.)
    char* ogrencibilgisi[5];
    int i = 0;
    while (bilgi != NULL)
    {
        ogrencibilgisi[i] = bilgi;
        i++;
        bilgi = strtok (NULL, " \t");
    }
    // alýnan öðrenci bilgileri ile  yeni bir öðrenci struct'ý yarattýk.
    Ogrenci yeniogrenci = yeniogrenciolustur(ogrencibilgisi);
    return yeniogrenci;
}

// tekrar eden öðrencileri ekrana yazacaktýr.
void tekrar_edenleri_yazdir(Sinif tekraredenogrenciler)
{
    printf("\n* Ýsmi Tekrar eden öðrenciler:\n");
    printf("\n* Ýsmi tekrar eden öðrenciler silinmiþtir!\n");
    if (tekraredenogrenciler.ogrencisayisi != 0)
	{
        int i;
        for ( i = 0; i < tekraredenogrenciler.ogrencisayisi; i++)
		{
            Ogrenci tekraredenogrenci = tekraredenogrenciler.ogrenciler[i];
            int j , tekrarsayi = 2;
            //tekrar sayý 2 den baþlar çünkü bir þeyin tekrar etmiþ olabilmesi için min 2 olmalýdýr
            //listenin kalanýnda ayný öðrenci varmý diye arar.Çünkü tekrar eden tüm öðrenciler buraya eklenir.
			//Bu yüzden kaç kez eklendi bulmasý gerekir.
            for ( j = i+ 1; j< tekraredenogrenciler.ogrencisayisi; j++)
			{
                Ogrenci tmp = tekraredenogrenciler.ogrenciler[j];
                //Dýþ for'da 0 dan tüm öðrencilere kadar deðer dönüyor.Biz bunun her birini tutup i+1 ile kendinden
                //sonrakilere baktýrýyoruz.eðer kendinden sonraki var ise direkt tekrar ediyordur.
				//if ile kaç defa tekrar ettiðini anlýyoruz.
                if ((strcmp(tmp.ad, tekraredenogrenci.ad) == 0) &&(strcmp(tmp.soyad, tekraredenogrenci.soyad) == 0))
				{
                    tekraredenogrenciler.ogrenciler[j].ad = "";
                    tekraredenogrenciler.ogrenciler[j].soyad = "";
                    tekrarsayi++;
                }
            }
            //Burada silinmiþ öðrenci deðilse yazacaktýr.Yoksa sürekli boþluk bulduðu için sonsuz defa yazdýrýr.
            if (strcmp(tekraredenogrenci.ad, "" ) != 0)
			{
                printf("\n%s %s : %d kez tekrar etti.\n",tekraredenogrenci.ad,tekraredenogrenci.soyad, tekrarsayi);
            }
        }
    }
    else
    {
        printf("\nTekrar eden öðrenci yoktur!\n");
    }
}

//Bize verilen sinifta bu for ile öðrenciyi ara. bulursan 1 bulamazsan 0 döndürecektir.
int ogrenci_listede_varmi(Ogrenci ogrenci, Sinif sinif)
{
    int tekrarsayisi = 0; // öðrencinin listeye eklenip eklenmediðini tutacaktýr.
    int i;
    for (i= 0; i< sinif.ogrencisayisi; i++) {

        Ogrenci tmp = sinif.ogrenciler[i];
        if ((strcmp(tmp.ad, ogrenci.ad) == 0) && (strcmp(tmp.soyad, ogrenci.soyad) == 0))
         {
            tekrarsayisi++;
            break;
        }
    }
    return tekrarsayisi;

}

// Yeni öðrenciler için numara atamasý yapacaðýz.
void yeni_ogrenciler_numara_ata (Sinif *sinif) 
{
    printf("\nYeni öðrenciler için numara atamasý yapýldý.\n");
    int i;
    for (i = 0; i < (*sinif).ogrencisayisi; i++) 
	{
        Ogrenci tmp = (*sinif).ogrenciler[i];
        // eðer öðrenci numarasý alanýnda "-" varsa önce 0 yapýlýr sonra numara verilir.
        if (strcmp(tmp.no,"-") == 0)
        {
            char* yeniogrencino = ogrencinumarasiolustur(tmp);
            strcpy((*sinif).ogrenciler[i].no, yeniogrencino);
        }
    }
}

// Öðrencileri okul numaralarýna göre sýralanýr.
void ogrencileri_sirala_okulno(Sinif *sinif)
{
    printf("\nÖðrenciler okul numaralarýna göre sýralandý.\n");
    int j;
    Ogrenci tmp;
    Ogrenci ogrenci;
    int i;
    for (i = 0; i < (*sinif).ogrencisayisi; i++)
        {
            j = i;
            int siniftakideger = 0;
            int ogrencidegeri = 0;
            if (j > 0)
            {
                ogrenci = (*sinif).ogrenciler[j];
                tmp = (*sinif).ogrenciler[j-1];
                siniftakideger = atoi(tmp.no);
                ogrencidegeri = atoi(ogrenci.no);
            }
            while (j > 0 && ogrencidegeri < siniftakideger)
            {
                // öðrencileri yer deðiþtirilir.
                Ogrenci s = (*sinif).ogrenciler[j];
                (*sinif).ogrenciler[j] = (*sinif).ogrenciler[j-1];
                (*sinif).ogrenciler[j-1] = s;
                // bir öndeki öðe için tekrar sorgulanýr.
                j--;
                if (j > 0)
                {
                    ogrenci = (*sinif).ogrenciler[j];
                    tmp = (*sinif).ogrenciler[j-1];
                    siniftakideger = atoi(tmp.no);
                    ogrencidegeri = atoi(ogrenci.no);
                }
            }
        }
}

// Öðrencileri kayýt sýrasýna göre sýralanýr.
void ogrencileri_sirala_kayitsirasi(Sinif *sinif) 
{
    printf("\nÖðrenciler kayýt numaralarýna göre sýralandý.\n");
    int j;
    Ogrenci tmp;
    Ogrenci ogrenci;
    int i;
    for (i = 0; i < (*sinif).ogrencisayisi; i++)
        {
        j = i;
        int siniftakideger = 0;
        int ogrencidegeri = 0;
        if (j > 0)
        {
            ogrenci = (*sinif).ogrenciler[j];
            tmp = (*sinif).ogrenciler[j-1];
            siniftakideger = atoi(tmp.kayitsirasi);
            ogrencidegeri = atoi(ogrenci.kayitsirasi);
        }
        while (j > 0 && ogrencidegeri < siniftakideger)
        {
            // öðrencileri yer deðiþtirilir.
            Ogrenci s = (*sinif).ogrenciler[j];
            (*sinif).ogrenciler[j] = (*sinif).ogrenciler[j-1];
            (*sinif).ogrenciler[j-1] = s;

            // bir öndeki öðe için tekrar sorgulanýr.
            j--;
            if (j > 0)
             {
                ogrenci = (*sinif).ogrenciler[j];
                tmp = (*sinif).ogrenciler[j-1];
                siniftakideger = atoi(tmp.kayitsirasi);
                ogrencidegeri = atoi(ogrenci.kayitsirasi);
            }
        }
    }
}

// öðrencilerin kayýt numalarý güncellenir.
void kayit_sirasi_guncelle(Sinif* sinif) {

    printf("\nKayýt numaralarý güncellendi.\n");
    // güncellemeden önce öðrencileri sýralanýr.
    ogrencileri_sirala_kayitsirasi(sinif);
    int sayac = 0;
    int i;
    for (i = 0; i < (*sinif).ogrencisayisi; i++)
        {
        Ogrenci tmp = (*sinif).ogrenciler[i];
        // Eðer öðrenci numarasý alanýnda "-" varsa kayýt numarasý güncellenmesi gerekiyordur.
        if (strcmp(tmp.no,"-") == 0)
            {
            char* yenikayitno =  malloc(sizeof(char)*5);
            sprintf(yenikayitno, "%d", sayac);
            strcpy(tmp.kayitsirasi, yenikayitno);
            strcpy((*sinif).ogrenciler[i].kayitsirasi, yenikayitno);
            sayac++;
            }
        }
}

// 1. öðretimde okuyan öðrenci sayýsý döndürülür.
int birinci_ogretim_ogrenci_sayisi_bul(Sinif sinif)
{
    int ogrenci_sayi = 0;
    int i;
    for (i = 0; i < sinif.ogrencisayisi; i++)
        {
        Ogrenci ogrenci = sinif.ogrenciler[i];
        char* ara = strstr(ogrenci.ogrtipi,"II");
        if (ara == NULL)
        {
            ogrenci_sayi++;
        }
    }
    return ogrenci_sayi;
}
int dosyadan_oku (Sinif* sinif,Sinif* tekrarlayanogrenciler) 
{
    FILE * oku;
    char * str = NULL;
    size_t uzunluk = 0;
    ssize_t okur;
    oku = fopen("odev.txt", "r+");
    if (oku == NULL)
    {
       return 0;
    }
    //Dosyadan ilk satýrý okuyacaktýr.Ýlk satýr aþaðýdaki baþlýklarý içerir, atlanabilir.
    ssize_t okuyucu = getline(&str, &uzunluk, oku);
    if (okuyucu != -1)
        {
            printf("\n Öðrenci Bilgileri Okundu.\n");
            // öðrenci bilgilerini satýr halinde okur.
            while ((okur = getline(&str, &uzunluk, oku)) != -1)
            {
                // alýnan satýrdan öðrenci bilgilerini çekecektir.
                Ogrenci yeniogrenci = satir_veri_cek(str);
                if (ogrenci_listede_varmi(yeniogrenci, *sinif) == 0)
                    {
                        //Öðrenci daha önce eklenmemiþtir.
                        //yeni öðrenciyi listenin sonuna ekler ve öðrenci sayýsýný artýrýr.
                        (*sinif).ogrenciler[(*sinif).ogrencisayisi] = yeniogrenci;
                        (*sinif).ogrencisayisi += 1;
                    }
                    else
                    {
                        // öðrenci tekrarý var, tekrar listesine ekler.
                        (*tekrarlayanogrenciler).ogrenciler[(*tekrarlayanogrenciler).ogrencisayisi] = yeniogrenci;
                        (*tekrarlayanogrenciler).ogrencisayisi++;
                    }
            }
        }
    free(str);
    fclose(oku);
    return 1;
}

// verilen öðrencileri dosyaya yazdýrýr.
void ogrencileri_dosyaya_yaz(Sinif sinif,char* dosya,int birinciogretimsayi,int basla,int son)
{
    FILE * oku;
    oku = fopen(dosya, "w+");
    if (oku == NULL)
    {
        printf("\n\n\n***Dosya açarken hata oluþtu!!!\n\n");
        return;
    }
    const char *title = "OgrenciNo\tAd\tSoyad\tKayitSirasi\tOgretimTuru";
    fprintf(oku, "%s", title);
    int listebasi = 0; // listebaþý : listede kaç numaralý öðrenciden itibaren yazmaya baþlancak.
    int listesonu = 0; // listesonu : toplamda kaç öðrenci yazdýrýlacak.
    int i;
    for (i = 0; (i < sinif.ogrencisayisi) && (listesonu <= son) ; i++)
        {
        char* ara = strstr(sinif.ogrenciler[i].ogrtipi,"II");
        if (birinciogretimsayi == 1 && ara == NULL)
            {
                if (listebasi == basla)
                 {
                    fprintf(oku, "\n%s\t%s\t%s\t%s\t%s",sinif.ogrenciler[i].no,sinif.ogrenciler[i].ad,sinif.ogrenciler[i].soyad,sinif.ogrenciler[i].kayitsirasi,sinif.ogrenciler[i].ogrtipi);
                    listesonu++;
                 }
                else
                 {
                    //Baþlangýç deðerine gelinceye kadar atlar.
                    listebasi++;
                 }
            }
            else if(birinciogretimsayi == 2 && ara != NULL)
            {
            if (basla == listebasi)
            {
                fprintf(oku, "\n%s\t%s\t%s\t%s\t%s",sinif.ogrenciler[i].no,sinif.ogrenciler[i].ad,sinif.ogrenciler[i].soyad,sinif.ogrenciler[i].kayitsirasi, sinif.ogrenciler[i].ogrtipi);
                listesonu++;
            }
            else
            {
                // pozisyon baþlangýç deðerine gelinceye kadar atlar.
                listebasi++;
            }
        }
    }
    fclose(oku);
}
typedef struct derslikStruct 
{
    int derslikno;
    int kontenjan;
}DerslikStruct;
char* string_birlestir(char *s1, char *s2)
{
    char *tmp = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(tmp, s1);
    strcat(tmp, s2);
    return tmp;
}

//Verilen ders listesinin kontenjanýna göre az dan çoða doðru sýralanýr.
DerslikStruct* kontenjana_gore_sirala (int dersliksayisi, DerslikStruct* derslikler) 
{
    int j;
    int i;
    DerslikStruct tmp;
    for (i = 0; i < dersliksayisi; i++)
        {
            j = i;
            while (j > 0 && derslikler[j].kontenjan < derslikler[j-1].kontenjan)
            {
                tmp = derslikler[j];
                derslikler[j] = derslikler[j-1];
                derslikler[j-1] = tmp;
                j--;
            }
        }
    return derslikler;
}

int main(int argc, const char * argv[])
{
	 time_t zaman_basla,zaman_son;
  	 time (&zaman_basla);
    setlocale(LC_ALL,"TURKISH");
    Sinif sinif; // ismi tekrarsýz öðrencilerin listesi
    sinif.ogrencisayisi = 0;
    sinif.ogrenciler = malloc(sizeof(Ogrenci)*1000);
    Sinif tekraredenler; // tekrar eden öðrencilerin listesi
    tekraredenler.ogrencisayisi = 0;
    tekraredenler.ogrenciler = malloc(sizeof(Ogrenci)*1000);
    
    // öðrenci bilgilerini dosyadan oku
    dosyadan_oku(&sinif, &tekraredenler);

    // tekrar eden öðrencileri ekrana yazdýr
    tekrar_edenleri_yazdir(tekraredenler);

    //Öðrencilerin kayýt numaralarýný güncelle,
    //silinen öðrencilerden sonra kayýt numaralarýnýn güncellenmesi gerekir.
    kayit_sirasi_guncelle(&sinif);//KARE

    // yeni öðrenciler için yeni okul numarasý oluþtur
    yeni_ogrenciler_numara_ata(&sinif);//KARE

    // öðrencileri okul numaralarýna göre sýrala- azdan çoða doðru
    ogrencileri_sirala_okulno(&sinif);//KARE

    // birinci ve ikinci öðretim öðrenci sayýlarýný hesapla
    int birinci_ogretim_sayi = birinci_ogretim_ogrenci_sayisi_bul(sinif);
    int ikinci_ogretim_sayisi = sinif.ogrencisayisi - birinci_ogretim_sayi;

    printf("\nDersi alan öðrenci sayýsý 1. öðretim: %d , 2. öðretim:%d 'dýr.!\n\n",birinci_ogretim_sayi,ikinci_ogretim_sayisi);

    // sýnýf sayýsý ve kontenjan bilgilerini kullanýcýdan al
    int start = 0;
    int sinifsayisi;
    int* ders_kontenjanlar;
    do {
        sinifsayisi= 0;
        printf("\n\nSýnýf Sayýsýný giriniz : ");
        scanf("%d", &sinifsayisi);
        ders_kontenjanlar = malloc(sizeof(int)*sinifsayisi);
        int totalkontenjan = 0;
        int i;
        for (i = 0; i < sinifsayisi; i++)
		{
            printf("\n%d.Sýnýf için kapasite giriniz: ",i+1);
            scanf("%d", (ders_kontenjanlar + i));
            totalkontenjan += *(ders_kontenjanlar + i);
        }
        //Eðer kontenjan sayýlarý öðrenci sayýsýndan az ise,tekrar kontenjan istenir.
        if ((totalkontenjan >= birinci_ogretim_sayi) && (totalkontenjan >= ikinci_ogretim_sayisi))
		{
            start = 1;
        }
        else
		{
            printf("\n\n\nSýnýflara atanan kontenjanlar tüm öðrencileri alacak kadar yeterli deðil!!\n\n\n");
        }
    } while (start != 1);
    
    // ders listesi oluþturulur.Ders bilgileri listeye eklenir.
    DerslikStruct* derslikler = malloc(sizeof(DerslikStruct)*sinifsayisi);
    int i;
    for (i = 0; i < sinifsayisi; i++)
	{
        DerslikStruct yeniderslik;
        yeniderslik.derslikno = i+1;
        yeniderslik.kontenjan =  ders_kontenjanlar[i];
        derslikler[i] = yeniderslik;
    }
    
    // ders listesini kontenjana göre azdan çoða sýralanýr.
    derslikler = kontenjana_gore_sirala(sinifsayisi,derslikler);
    
    //Eþit Daðýlým:1. öðretim
    //kontenjan ortalamasý
    int birinci_ogretim_esit_dagilim = birinci_ogretim_sayi/sinifsayisi;
    int kalan_ogrenci_sayisi = birinci_ogretim_sayi;
    
    // dosyaya yazdýrma da kullanýlacak,baþlangýç sayýsý
    int dosya_baslangic = 0;
    
    // yazdýrýlacak dosya ismi
    char* dosya_ad = malloc(sizeof(char)*30);
    
    // öðretim tipi- 1. veya 2. öðretim
    int formalType = 1;
    for (i = 0; i < sinifsayisi; i++)
	{
        //Eðer ortalama, en az kotalý dersin kontenjanýndan az ise öðrenciler 4 eþit parçaya bölünebilir.
        if (birinci_ogretim_esit_dagilim < derslikler[i].kontenjan)
		{
            for (; i < sinifsayisi; i++)
			{
                if(i == sinifsayisi -1 )
				{
                    // öðrenci sayýsý ders sayýsýna eþit bölünemezse uygulanýr.
                    //dosya ismi ayarlanýr.
                    sprintf(dosya_ad, "EsitDagilim_sinif%dogr1.txt", derslikler[i].derslikno);
                    
                    // verilen baþlangýç noktasýndan itibaren öðrenciler dosyaya yazdýrýlýr.
                    ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,kalan_ogrenci_sayisi);
                    
                    // baþlangýç noktasý güncellenir.
                    dosya_baslangic +=kalan_ogrenci_sayisi;
                    printf("\nEþit daðýlým 1. Öðretim: %d. sýnýf : %d", derslikler[i].derslikno,kalan_ogrenci_sayisi);
                }
                else
				{
                    sprintf(dosya_ad, "EsitDagilim_sinif%dogr1.txt", derslikler[i].derslikno);
                    ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,birinci_ogretim_esit_dagilim);
                    dosya_baslangic +=birinci_ogretim_esit_dagilim;
                    printf("\nEþit daðýlým 1. Öðretim: %d. sýnýf : %d", derslikler[i].derslikno,birinci_ogretim_esit_dagilim);
                }
                kalan_ogrenci_sayisi = kalan_ogrenci_sayisi - birinci_ogretim_esit_dagilim;
            }
        }
        else
		{
            // dersin kontenjaný ortalamadan daha az ise uygulanýr.
            // öðrencileri dosyaya yazdýr.
            sprintf(dosya_ad, "EsitDagilim_sinif%dogr1.txt", derslikler[i].derslikno);
            ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,derslikler[i].kontenjan);
            dosya_baslangic += derslikler[i].kontenjan;
            
            // ortalama deðeri güncellenir.
            printf("\nEþit daðýlým 1. Öðretim: %d. sýnýf : %d", derslikler[i].derslikno,derslikler[i].kontenjan);
            int kalan_derslik_sayisi = sinifsayisi - (i + 1);
            kalan_ogrenci_sayisi = kalan_ogrenci_sayisi - derslikler[i].kontenjan;
            birinci_ogretim_esit_dagilim = kalan_ogrenci_sayisi / kalan_derslik_sayisi;
        }
    }
    printf("\n");
    
    //Eþit Daðýlým:2.Öðretim
    birinci_ogretim_esit_dagilim = ikinci_ogretim_sayisi/sinifsayisi;
    kalan_ogrenci_sayisi = ikinci_ogretim_sayisi;
    formalType = 2;
    dosya_baslangic = 0;
    for (i = 0; i < sinifsayisi; i++)
	{
        if (birinci_ogretim_esit_dagilim < derslikler[i].kontenjan)
		{
            for (; i < sinifsayisi; i++)
			{
                if(i == sinifsayisi -1 )
				{
                    sprintf(dosya_ad, "EsitDagilim_sinif%dogr2.txt", derslikler[i].derslikno);
                    ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,kalan_ogrenci_sayisi);
                    dosya_baslangic +=kalan_ogrenci_sayisi;
                    printf("\nEþit daðýlým 2. Öðretim: %d. sýnýf : %d", derslikler[i].derslikno,kalan_ogrenci_sayisi);
                }
				else
                {
                    sprintf(dosya_ad, "EsitDagilim_sinif%dogr2.txt", derslikler[i].derslikno);
                    ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,birinci_ogretim_esit_dagilim);
                    dosya_baslangic +=birinci_ogretim_esit_dagilim;
                    printf("\nEþit daðýlým 2. Öðretim: %d. sýnýf : %d", derslikler[i].derslikno,birinci_ogretim_esit_dagilim);
                }
                kalan_ogrenci_sayisi = kalan_ogrenci_sayisi - birinci_ogretim_esit_dagilim;
            }
        }
        else
		{
            sprintf(dosya_ad, "EsitDagilim_sinif%dogr2.txt", derslikler[i].derslikno);
            ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,derslikler[i].kontenjan);
            dosya_baslangic += derslikler[i].kontenjan;
            printf("\nEþit daðýlým 2. Öðretim: %d. sýnýf : %d", derslikler[i].derslikno,derslikler[i].kontenjan);
            int kalan_derslik_sayisi = sinifsayisi - (i + 1);
            kalan_ogrenci_sayisi = kalan_ogrenci_sayisi - derslikler[i].kontenjan;
            birinci_ogretim_esit_dagilim = kalan_ogrenci_sayisi / kalan_derslik_sayisi;
        }
    }
    printf("\n\n");
    //en az sýnýf daðýlýmý:1. Öðretim

    // en az daðýlým için yeni bir liste oluþturulur.
    DerslikStruct* enaz_dagilim_derslik = malloc(sizeof(DerslikStruct)*sinifsayisi);
    int kalan_ogrenci_kontenjan = birinci_ogretim_sayi;
    
    // ders listesi en çok kota olandan az olana doðru iþlenir.
    // önce kotasý çok olan derslere öðrenciler yerleþtirilir.
    for (i = sinifsayisi -1 ; i >= 0; i--)
	{
        DerslikStruct drs;
        drs.derslikno = derslikler[i].derslikno;
        int kontenjan = derslikler[i].kontenjan;
        
        //Eðer kalan öðrenci kotadan daha az ise tüm öðrencileri o derse yerleþtirilir.
        if (kalan_ogrenci_kontenjan < kontenjan)
		{
            kontenjan = kalan_ogrenci_kontenjan;
            kalan_ogrenci_kontenjan = 0;
        }
        else
		{
            // ders kontenjaný kalan öðrenci listesinden çýkarýlýr.
            kalan_ogrenci_kontenjan -= kontenjan;
        }
        drs.kontenjan = kontenjan;
        
        //listeye eklenir.
        enaz_dagilim_derslik[i] = drs;
    }
    
    // listeyi dosyaya yazdýrýlýr.
    dosya_baslangic = 0;
    formalType = 1;
    for (i = 0; i < sinifsayisi; i++)
	{
        sprintf(dosya_ad, "En Az Dagilim Sinif%dogr1.txt", enaz_dagilim_derslik[i].derslikno);
        ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,enaz_dagilim_derslik[i].kontenjan);
        dosya_baslangic += enaz_dagilim_derslik[i].kontenjan;
        printf("\nEn az sýnýf daðýlýmý: 1. öðretim:  %d. sýnýf : %d",enaz_dagilim_derslik[i].derslikno,enaz_dagilim_derslik[i].kontenjan);
    }
    printf("\n\n");
    
    //en az sýnýf daðýlýmý:2. Öðretim
    enaz_dagilim_derslik = NULL;
    free(enaz_dagilim_derslik);
    enaz_dagilim_derslik = malloc(sizeof(DerslikStruct)*sinifsayisi);
    kalan_ogrenci_kontenjan = ikinci_ogretim_sayisi;
    for (i = sinifsayisi -1 ; i >= 0; i--)
	{
        DerslikStruct drs;
        drs.derslikno = derslikler[i].derslikno;
        int kontenjan = derslikler[i].kontenjan;
        if (kalan_ogrenci_kontenjan < kontenjan)
		{
            kontenjan = kalan_ogrenci_kontenjan;
            kalan_ogrenci_kontenjan = 0;
        }
        else
		{
            kalan_ogrenci_kontenjan -= kontenjan;
        }
        drs.kontenjan = kontenjan;
        enaz_dagilim_derslik[i] = drs;
    }
    dosya_baslangic = 0;
    formalType = 2;
    for (i = 0; i < sinifsayisi; i++)
	{
        sprintf(dosya_ad, "EnAzDagilim_sinif%dogr2.txt", enaz_dagilim_derslik[i].derslikno);
        ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,enaz_dagilim_derslik[i].kontenjan);
        dosya_baslangic += enaz_dagilim_derslik[i].kontenjan;
        printf("En az sýnýf daðýlýmý: 2. öðretim:  %d. sýnýf : %d\n",enaz_dagilim_derslik[i].derslikno,enaz_dagilim_derslik[i].kontenjan);
    }
    free(dosya_ad);
    free(derslikler);
    free(enaz_dagilim_derslik);
    free(tekraredenler.ogrenciler);
    free(sinif.ogrenciler);
    time (&zaman_son);
    printf ("\nProgramýn çalýþmasý %.2lf saniye sürdü.\n", difftime (zaman_son,zaman_basla));
    return 0;
}

