#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
//Dosyadan sat�r sat�r okumak i�in ara�t�rmalar sonucunda buldu�umuz fonksiyondur
//Ancak fonksiyonun orijinali Linux & Posix sistemlerde olup windowsta olmamas�ndan dolay� �al��mam��t�r ve
//ara�t�rmam�z sonucu c de yaz�lm�� fonksiyon koda entegre edilmi�tir.
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
//kendi veri tipimiz 'ogrenci' yi olu�turduk
//bu veri tipi bir ��rencinin ad soyad kay�t s�ras� ��renci no ve ��retim tipini depolayabilir.
typedef struct sinif
{
    Ogrenci* ogrenciler;
    int ogrencisayisi;
}Sinif;

//burada ise 'ogrenci' ve 'ogrencisayisi' ndan olu�an s�n�f yap�m�z� olu�turduk.
//bu yap�,ogrenci sayisi kadar ��rencinin bilgisini i�inde ta��yacak
void ogrencileriyazdir (Ogrenci ogrenci)
{
    printf("��renci : %s %s %s %s %s",ogrenci.no, ogrenci.ad,ogrenci.soyad,ogrenci.kayitsirasi,ogrenci.ogrtipi);

};

//bu fonksiyon char dizisi i�inde verilen ��renci bilgisini par�a par�a al�p yeni ��renci struct� olu�turduk.
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

// Verilen ��renci i�in yeni numara �rettik.
char* ogrencinumarasiolustur (Ogrenci ogrenci)
{
    int yil = 1700000;
    int birinciogretim = 1000; //1.��retim i�in
    int ikinciogretim = 2000; //2.��retim i�in
    int yeniogrencino = 0;
    char* str = strstr(ogrenci.ogrtipi,"II");
    //i�inde II yok ise null d�necek.null ise birinci ��retimdir
    if (str == NULL)
	{
        yeniogrencino = yil + birinciogretim + atoi(ogrenci.kayitsirasi);
    }
	else
	{
        yeniogrencino = yil + ikinciogretim + atoi(ogrenci.kayitsirasi);
    }
    // int olarak toplad���m�z ��renci nosunu 7 haneli stringe �evirdik.
    char* ogrno = malloc(sizeof(char)*7);
    sprintf(ogrno, "%d", yeniogrencino);
    return ogrno;
}
Ogrenci satir_veri_cek(char* satir)
{
    char* tmp = malloc(sizeof(char)*strlen(satir));
    strcpy(tmp, satir);
    //sat�r uzunlu�unda bir string olu�turduk ve tmp e atad�k
    char* bilgi = strtok(tmp," \t");
    //bunu tab bo�luklar�na g�re b�ld�k ve her bir par�ay� ogrencibilgisi dizisine eleman olarak att�k ( ��nk� txt dosyas�nda bilgiler aras�nda tab var.)
    char* ogrencibilgisi[5];
    int i = 0;
    while (bilgi != NULL)
    {
        ogrencibilgisi[i] = bilgi;
        i++;
        bilgi = strtok (NULL, " \t");
    }
    // al�nan ��renci bilgileri ile  yeni bir ��renci struct'� yaratt�k.
    Ogrenci yeniogrenci = yeniogrenciolustur(ogrencibilgisi);
    return yeniogrenci;
}

// tekrar eden ��rencileri ekrana yazacakt�r.
void tekrar_edenleri_yazdir(Sinif tekraredenogrenciler)
{
    printf("\n* �smi Tekrar eden ��renciler:\n");
    printf("\n* �smi tekrar eden ��renciler silinmi�tir!\n");
    if (tekraredenogrenciler.ogrencisayisi != 0)
	{
        int i;
        for ( i = 0; i < tekraredenogrenciler.ogrencisayisi; i++)
		{
            Ogrenci tekraredenogrenci = tekraredenogrenciler.ogrenciler[i];
            int j , tekrarsayi = 2;
            //tekrar say� 2 den ba�lar ��nk� bir �eyin tekrar etmi� olabilmesi i�in min 2 olmal�d�r
            //listenin kalan�nda ayn� ��renci varm� diye arar.��nk� tekrar eden t�m ��renciler buraya eklenir.
			//Bu y�zden ka� kez eklendi bulmas� gerekir.
            for ( j = i+ 1; j< tekraredenogrenciler.ogrencisayisi; j++)
			{
                Ogrenci tmp = tekraredenogrenciler.ogrenciler[j];
                //D�� for'da 0 dan t�m ��rencilere kadar de�er d�n�yor.Biz bunun her birini tutup i+1 ile kendinden
                //sonrakilere bakt�r�yoruz.e�er kendinden sonraki var ise direkt tekrar ediyordur.
				//if ile ka� defa tekrar etti�ini anl�yoruz.
                if ((strcmp(tmp.ad, tekraredenogrenci.ad) == 0) &&(strcmp(tmp.soyad, tekraredenogrenci.soyad) == 0))
				{
                    tekraredenogrenciler.ogrenciler[j].ad = "";
                    tekraredenogrenciler.ogrenciler[j].soyad = "";
                    tekrarsayi++;
                }
            }
            //Burada silinmi� ��renci de�ilse yazacakt�r.Yoksa s�rekli bo�luk buldu�u i�in sonsuz defa yazd�r�r.
            if (strcmp(tekraredenogrenci.ad, "" ) != 0)
			{
                printf("\n%s %s : %d kez tekrar etti.\n",tekraredenogrenci.ad,tekraredenogrenci.soyad, tekrarsayi);
            }
        }
    }
    else
    {
        printf("\nTekrar eden ��renci yoktur!\n");
    }
}

//Bize verilen sinifta bu for ile ��renciyi ara. bulursan 1 bulamazsan 0 d�nd�recektir.
int ogrenci_listede_varmi(Ogrenci ogrenci, Sinif sinif)
{
    int tekrarsayisi = 0; // ��rencinin listeye eklenip eklenmedi�ini tutacakt�r.
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

// Yeni ��renciler i�in numara atamas� yapaca��z.
void yeni_ogrenciler_numara_ata (Sinif *sinif) 
{
    printf("\nYeni ��renciler i�in numara atamas� yap�ld�.\n");
    int i;
    for (i = 0; i < (*sinif).ogrencisayisi; i++) 
	{
        Ogrenci tmp = (*sinif).ogrenciler[i];
        // e�er ��renci numaras� alan�nda "-" varsa �nce 0 yap�l�r sonra numara verilir.
        if (strcmp(tmp.no,"-") == 0)
        {
            char* yeniogrencino = ogrencinumarasiolustur(tmp);
            strcpy((*sinif).ogrenciler[i].no, yeniogrencino);
        }
    }
}

// ��rencileri okul numaralar�na g�re s�ralan�r.
void ogrencileri_sirala_okulno(Sinif *sinif)
{
    printf("\n��renciler okul numaralar�na g�re s�raland�.\n");
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
                // ��rencileri yer de�i�tirilir.
                Ogrenci s = (*sinif).ogrenciler[j];
                (*sinif).ogrenciler[j] = (*sinif).ogrenciler[j-1];
                (*sinif).ogrenciler[j-1] = s;
                // bir �ndeki ��e i�in tekrar sorgulan�r.
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

// ��rencileri kay�t s�ras�na g�re s�ralan�r.
void ogrencileri_sirala_kayitsirasi(Sinif *sinif) 
{
    printf("\n��renciler kay�t numaralar�na g�re s�raland�.\n");
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
            // ��rencileri yer de�i�tirilir.
            Ogrenci s = (*sinif).ogrenciler[j];
            (*sinif).ogrenciler[j] = (*sinif).ogrenciler[j-1];
            (*sinif).ogrenciler[j-1] = s;

            // bir �ndeki ��e i�in tekrar sorgulan�r.
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

// ��rencilerin kay�t numalar� g�ncellenir.
void kayit_sirasi_guncelle(Sinif* sinif) {

    printf("\nKay�t numaralar� g�ncellendi.\n");
    // g�ncellemeden �nce ��rencileri s�ralan�r.
    ogrencileri_sirala_kayitsirasi(sinif);
    int sayac = 0;
    int i;
    for (i = 0; i < (*sinif).ogrencisayisi; i++)
        {
        Ogrenci tmp = (*sinif).ogrenciler[i];
        // E�er ��renci numaras� alan�nda "-" varsa kay�t numaras� g�ncellenmesi gerekiyordur.
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

// 1. ��retimde okuyan ��renci say�s� d�nd�r�l�r.
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
    //Dosyadan ilk sat�r� okuyacakt�r.�lk sat�r a�a��daki ba�l�klar� i�erir, atlanabilir.
    ssize_t okuyucu = getline(&str, &uzunluk, oku);
    if (okuyucu != -1)
        {
            printf("\n ��renci Bilgileri Okundu.\n");
            // ��renci bilgilerini sat�r halinde okur.
            while ((okur = getline(&str, &uzunluk, oku)) != -1)
            {
                // al�nan sat�rdan ��renci bilgilerini �ekecektir.
                Ogrenci yeniogrenci = satir_veri_cek(str);
                if (ogrenci_listede_varmi(yeniogrenci, *sinif) == 0)
                    {
                        //��renci daha �nce eklenmemi�tir.
                        //yeni ��renciyi listenin sonuna ekler ve ��renci say�s�n� art�r�r.
                        (*sinif).ogrenciler[(*sinif).ogrencisayisi] = yeniogrenci;
                        (*sinif).ogrencisayisi += 1;
                    }
                    else
                    {
                        // ��renci tekrar� var, tekrar listesine ekler.
                        (*tekrarlayanogrenciler).ogrenciler[(*tekrarlayanogrenciler).ogrencisayisi] = yeniogrenci;
                        (*tekrarlayanogrenciler).ogrencisayisi++;
                    }
            }
        }
    free(str);
    fclose(oku);
    return 1;
}

// verilen ��rencileri dosyaya yazd�r�r.
void ogrencileri_dosyaya_yaz(Sinif sinif,char* dosya,int birinciogretimsayi,int basla,int son)
{
    FILE * oku;
    oku = fopen(dosya, "w+");
    if (oku == NULL)
    {
        printf("\n\n\n***Dosya a�arken hata olu�tu!!!\n\n");
        return;
    }
    const char *title = "OgrenciNo\tAd\tSoyad\tKayitSirasi\tOgretimTuru";
    fprintf(oku, "%s", title);
    int listebasi = 0; // listeba�� : listede ka� numaral� ��renciden itibaren yazmaya ba�lancak.
    int listesonu = 0; // listesonu : toplamda ka� ��renci yazd�r�lacak.
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
                    //Ba�lang�� de�erine gelinceye kadar atlar.
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
                // pozisyon ba�lang�� de�erine gelinceye kadar atlar.
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

//Verilen ders listesinin kontenjan�na g�re az dan �o�a do�ru s�ralan�r.
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
    Sinif sinif; // ismi tekrars�z ��rencilerin listesi
    sinif.ogrencisayisi = 0;
    sinif.ogrenciler = malloc(sizeof(Ogrenci)*1000);
    Sinif tekraredenler; // tekrar eden ��rencilerin listesi
    tekraredenler.ogrencisayisi = 0;
    tekraredenler.ogrenciler = malloc(sizeof(Ogrenci)*1000);
    
    // ��renci bilgilerini dosyadan oku
    dosyadan_oku(&sinif, &tekraredenler);

    // tekrar eden ��rencileri ekrana yazd�r
    tekrar_edenleri_yazdir(tekraredenler);

    //��rencilerin kay�t numaralar�n� g�ncelle,
    //silinen ��rencilerden sonra kay�t numaralar�n�n g�ncellenmesi gerekir.
    kayit_sirasi_guncelle(&sinif);//KARE

    // yeni ��renciler i�in yeni okul numaras� olu�tur
    yeni_ogrenciler_numara_ata(&sinif);//KARE

    // ��rencileri okul numaralar�na g�re s�rala- azdan �o�a do�ru
    ogrencileri_sirala_okulno(&sinif);//KARE

    // birinci ve ikinci ��retim ��renci say�lar�n� hesapla
    int birinci_ogretim_sayi = birinci_ogretim_ogrenci_sayisi_bul(sinif);
    int ikinci_ogretim_sayisi = sinif.ogrencisayisi - birinci_ogretim_sayi;

    printf("\nDersi alan ��renci say�s� 1. ��retim: %d , 2. ��retim:%d 'd�r.!\n\n",birinci_ogretim_sayi,ikinci_ogretim_sayisi);

    // s�n�f say�s� ve kontenjan bilgilerini kullan�c�dan al
    int start = 0;
    int sinifsayisi;
    int* ders_kontenjanlar;
    do {
        sinifsayisi= 0;
        printf("\n\nS�n�f Say�s�n� giriniz : ");
        scanf("%d", &sinifsayisi);
        ders_kontenjanlar = malloc(sizeof(int)*sinifsayisi);
        int totalkontenjan = 0;
        int i;
        for (i = 0; i < sinifsayisi; i++)
		{
            printf("\n%d.S�n�f i�in kapasite giriniz: ",i+1);
            scanf("%d", (ders_kontenjanlar + i));
            totalkontenjan += *(ders_kontenjanlar + i);
        }
        //E�er kontenjan say�lar� ��renci say�s�ndan az ise,tekrar kontenjan istenir.
        if ((totalkontenjan >= birinci_ogretim_sayi) && (totalkontenjan >= ikinci_ogretim_sayisi))
		{
            start = 1;
        }
        else
		{
            printf("\n\n\nS�n�flara atanan kontenjanlar t�m ��rencileri alacak kadar yeterli de�il!!\n\n\n");
        }
    } while (start != 1);
    
    // ders listesi olu�turulur.Ders bilgileri listeye eklenir.
    DerslikStruct* derslikler = malloc(sizeof(DerslikStruct)*sinifsayisi);
    int i;
    for (i = 0; i < sinifsayisi; i++)
	{
        DerslikStruct yeniderslik;
        yeniderslik.derslikno = i+1;
        yeniderslik.kontenjan =  ders_kontenjanlar[i];
        derslikler[i] = yeniderslik;
    }
    
    // ders listesini kontenjana g�re azdan �o�a s�ralan�r.
    derslikler = kontenjana_gore_sirala(sinifsayisi,derslikler);
    
    //E�it Da��l�m:1. ��retim
    //kontenjan ortalamas�
    int birinci_ogretim_esit_dagilim = birinci_ogretim_sayi/sinifsayisi;
    int kalan_ogrenci_sayisi = birinci_ogretim_sayi;
    
    // dosyaya yazd�rma da kullan�lacak,ba�lang�� say�s�
    int dosya_baslangic = 0;
    
    // yazd�r�lacak dosya ismi
    char* dosya_ad = malloc(sizeof(char)*30);
    
    // ��retim tipi- 1. veya 2. ��retim
    int formalType = 1;
    for (i = 0; i < sinifsayisi; i++)
	{
        //E�er ortalama, en az kotal� dersin kontenjan�ndan az ise ��renciler 4 e�it par�aya b�l�nebilir.
        if (birinci_ogretim_esit_dagilim < derslikler[i].kontenjan)
		{
            for (; i < sinifsayisi; i++)
			{
                if(i == sinifsayisi -1 )
				{
                    // ��renci say�s� ders say�s�na e�it b�l�nemezse uygulan�r.
                    //dosya ismi ayarlan�r.
                    sprintf(dosya_ad, "EsitDagilim_sinif%dogr1.txt", derslikler[i].derslikno);
                    
                    // verilen ba�lang�� noktas�ndan itibaren ��renciler dosyaya yazd�r�l�r.
                    ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,kalan_ogrenci_sayisi);
                    
                    // ba�lang�� noktas� g�ncellenir.
                    dosya_baslangic +=kalan_ogrenci_sayisi;
                    printf("\nE�it da��l�m 1. ��retim: %d. s�n�f : %d", derslikler[i].derslikno,kalan_ogrenci_sayisi);
                }
                else
				{
                    sprintf(dosya_ad, "EsitDagilim_sinif%dogr1.txt", derslikler[i].derslikno);
                    ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,birinci_ogretim_esit_dagilim);
                    dosya_baslangic +=birinci_ogretim_esit_dagilim;
                    printf("\nE�it da��l�m 1. ��retim: %d. s�n�f : %d", derslikler[i].derslikno,birinci_ogretim_esit_dagilim);
                }
                kalan_ogrenci_sayisi = kalan_ogrenci_sayisi - birinci_ogretim_esit_dagilim;
            }
        }
        else
		{
            // dersin kontenjan� ortalamadan daha az ise uygulan�r.
            // ��rencileri dosyaya yazd�r.
            sprintf(dosya_ad, "EsitDagilim_sinif%dogr1.txt", derslikler[i].derslikno);
            ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,derslikler[i].kontenjan);
            dosya_baslangic += derslikler[i].kontenjan;
            
            // ortalama de�eri g�ncellenir.
            printf("\nE�it da��l�m 1. ��retim: %d. s�n�f : %d", derslikler[i].derslikno,derslikler[i].kontenjan);
            int kalan_derslik_sayisi = sinifsayisi - (i + 1);
            kalan_ogrenci_sayisi = kalan_ogrenci_sayisi - derslikler[i].kontenjan;
            birinci_ogretim_esit_dagilim = kalan_ogrenci_sayisi / kalan_derslik_sayisi;
        }
    }
    printf("\n");
    
    //E�it Da��l�m:2.��retim
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
                    printf("\nE�it da��l�m 2. ��retim: %d. s�n�f : %d", derslikler[i].derslikno,kalan_ogrenci_sayisi);
                }
				else
                {
                    sprintf(dosya_ad, "EsitDagilim_sinif%dogr2.txt", derslikler[i].derslikno);
                    ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,birinci_ogretim_esit_dagilim);
                    dosya_baslangic +=birinci_ogretim_esit_dagilim;
                    printf("\nE�it da��l�m 2. ��retim: %d. s�n�f : %d", derslikler[i].derslikno,birinci_ogretim_esit_dagilim);
                }
                kalan_ogrenci_sayisi = kalan_ogrenci_sayisi - birinci_ogretim_esit_dagilim;
            }
        }
        else
		{
            sprintf(dosya_ad, "EsitDagilim_sinif%dogr2.txt", derslikler[i].derslikno);
            ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,derslikler[i].kontenjan);
            dosya_baslangic += derslikler[i].kontenjan;
            printf("\nE�it da��l�m 2. ��retim: %d. s�n�f : %d", derslikler[i].derslikno,derslikler[i].kontenjan);
            int kalan_derslik_sayisi = sinifsayisi - (i + 1);
            kalan_ogrenci_sayisi = kalan_ogrenci_sayisi - derslikler[i].kontenjan;
            birinci_ogretim_esit_dagilim = kalan_ogrenci_sayisi / kalan_derslik_sayisi;
        }
    }
    printf("\n\n");
    //en az s�n�f da��l�m�:1. ��retim

    // en az da��l�m i�in yeni bir liste olu�turulur.
    DerslikStruct* enaz_dagilim_derslik = malloc(sizeof(DerslikStruct)*sinifsayisi);
    int kalan_ogrenci_kontenjan = birinci_ogretim_sayi;
    
    // ders listesi en �ok kota olandan az olana do�ru i�lenir.
    // �nce kotas� �ok olan derslere ��renciler yerle�tirilir.
    for (i = sinifsayisi -1 ; i >= 0; i--)
	{
        DerslikStruct drs;
        drs.derslikno = derslikler[i].derslikno;
        int kontenjan = derslikler[i].kontenjan;
        
        //E�er kalan ��renci kotadan daha az ise t�m ��rencileri o derse yerle�tirilir.
        if (kalan_ogrenci_kontenjan < kontenjan)
		{
            kontenjan = kalan_ogrenci_kontenjan;
            kalan_ogrenci_kontenjan = 0;
        }
        else
		{
            // ders kontenjan� kalan ��renci listesinden ��kar�l�r.
            kalan_ogrenci_kontenjan -= kontenjan;
        }
        drs.kontenjan = kontenjan;
        
        //listeye eklenir.
        enaz_dagilim_derslik[i] = drs;
    }
    
    // listeyi dosyaya yazd�r�l�r.
    dosya_baslangic = 0;
    formalType = 1;
    for (i = 0; i < sinifsayisi; i++)
	{
        sprintf(dosya_ad, "En Az Dagilim Sinif%dogr1.txt", enaz_dagilim_derslik[i].derslikno);
        ogrencileri_dosyaya_yaz(sinif,dosya_ad,formalType,dosya_baslangic,enaz_dagilim_derslik[i].kontenjan);
        dosya_baslangic += enaz_dagilim_derslik[i].kontenjan;
        printf("\nEn az s�n�f da��l�m�: 1. ��retim:  %d. s�n�f : %d",enaz_dagilim_derslik[i].derslikno,enaz_dagilim_derslik[i].kontenjan);
    }
    printf("\n\n");
    
    //en az s�n�f da��l�m�:2. ��retim
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
        printf("En az s�n�f da��l�m�: 2. ��retim:  %d. s�n�f : %d\n",enaz_dagilim_derslik[i].derslikno,enaz_dagilim_derslik[i].kontenjan);
    }
    free(dosya_ad);
    free(derslikler);
    free(enaz_dagilim_derslik);
    free(tekraredenler.ogrenciler);
    free(sinif.ogrenciler);
    time (&zaman_son);
    printf ("\nProgram�n �al��mas� %.2lf saniye s�rd�.\n", difftime (zaman_son,zaman_basla));
    return 0;
}

