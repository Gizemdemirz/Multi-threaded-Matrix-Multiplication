#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include"sys/time.h"
#include<time.h>

//Girdiye bagli olarak olusturulacak matrislerimin tutuldugu degiskenler
int **A,**B,**C;

//Kullanici tarafindan girilen boyutlari kullanmak icin 3 adet degisken 
int row,rowCol,column;
//Calisan thread'i gozlemleyebilmek icin bir degisken 
int part =0;
//Threadlerin calisma zamanlarini olcmek icin 2 adet struct yapisi 
struct timeval time_start;
struct timeval time_end;

void randMat(int **Matrix,int Row,int Column);
void printMat(int **Matrix,int Row,int Column);
void detailMat(int **Matrix,int Row,int Column);

//Iki matrisin carpimi
void* mut_matrix(void* arg){
 int Ctemp ;
//Calisan thread'in belirlenebilmesi icin metot calistiginda part degiskenini 1 arttirip yeni degiskenime atadim.
 int thread_part  = part++;
//Her satir islemini bir thread gerceklestirsin diye satir numarami thread numarama esitledim.
 for(int i = thread_part;i<row;i++){
   for (int j  = 0 ;j<column;j++){
        Ctemp =0;
        for(int k = 0; k<rowCol;k++){
           Ctemp += A[i][k] * B[k][j];
         }
        C[i][j] = Ctemp;
    }
}
}


int main(int argc,char *argv[]){
//Komut satirina girilen argumanlar istenilenden az ise ekrana uyari mesaji yazdirdim.
if(argc != 4){
   printf("Kullanım: %s <row> <rowCol> <column>\n",argv[0]);
   return 1;
}
//Girilen arguman degerleri matris olusumuna engel ise ekrana uyari mesaji yazdirdim.
else if(atoi(argv[1]) <1 || atoi(argv[2])<1 || atoi(argv[3])<1){
   printf("Argüman değerleri 1'den küçük olamaz!!\n");
   return 1;
}

//Girilen arguman degerlerini degiskenlere atadim.
row = atoi(argv[1]);
rowCol = atoi(argv[2]);
column = atoi(argv[3]);

//Bellekte matrisler icin yer tahsis ettim.
A = (int **) malloc(sizeof(int*) *row);
for(int i=0;i<row;i++)
  A[i] = (int*) malloc(sizeof(int)*rowCol);

B = (int **) malloc(sizeof(int*) *rowCol);
for(int i=0;i<rowCol;i++)
  B[i] = (int*) malloc(sizeof(int)*column);

C = (int **) malloc(sizeof(int*) *row);
for(int i=0;i<row;i++)
  C[i] = (int*) malloc(sizeof(int)*column);


printf("İlk matrisin boyutları: %dx%d\n",row,rowCol);
printf("İkinci matrisin boyutları: %dx%d\n",rowCol,column);
printf("Çarpım matrisinin boyutları: %dx%d\n",row,column);
printf("\n");

randMat(A,row,rowCol);
randMat(B,rowCol,column);


//Kullanicinin belirledigi satir sayisi kadar thread olusturdum
pthread_t threads[row];

for(int i = 0;i<row;i++){
  pthread_create(&threads[i],NULL,mut_matrix,(void*)NULL);
//Calisma zamani hesabi icin sureyi baslattim.
  gettimeofday(&time_start,NULL);
}
//Olasi sorunlara karsı join ifadesiyle her thread'in birbirini beklemesini sagladim.
for(int i = 0;i<row;i++){
  pthread_join(threads[i],NULL);
//Her thread gorevini yaptiktan sonra sureyi bitirdim.
  gettimeofday(&time_end,NULL);
//Baslangic ve bitis zamanini kullanarak gecen sureyi hesapladim ve yazdirdim.
  float time =(float) ((1000000*time_end.tv_sec+time_end.tv_usec)-(1000000*time_start.tv_sec+time_start.tv_usec))/1000000;
  printf("%d.thread çalışma süresi= %f\n",i+1,time);
}
printf("\n");
printf("A matrisi\n");
printMat(A,row,rowCol);
printf("B matrisi\n");
printMat(B,rowCol,column);
detailMat(C,row,column);

//Bellekte ayrilan alanlari serbest biraktim
for(int i=0;i<row;i++){free(A[i]);free(C[i]);}
free(A);free(C);
for(int i=0;i<rowCol;i++){free(B[i]);}
free(B);
return 0;
}



//Matrislerin icine rastgele sayi atan bir metot
void randMat(int **Matrix,int Row,int Column){
 for(int i = 0;i<Row;i++){
    for(int j= 0;j<Column;j++){
       Matrix[i][j] = rand()%10;
    } 
 }
}

//Olusan matrisin satir ve sutun degerlerini detayli gorebilmek icin bir metot
void detailMat(int **Matrix,int Row,int Column){
 for(int i = 0; i<Row;i++){
   for(int j = 0; j<Column;j++){
       printf("C[%d][%d] = %d \n",i,j,Matrix[i][j]);
     
   }
   printf("\n");
 }
}

//Matrislerin degerleri random atildigi icin baslangicta olusan matrisleri gorebilmek icin bir metot
void printMat(int **Matrix,int Row,int Column){
printf("\n");
  for(int i = 0; i<Row;i++){
    for(int j = 0; j<Column;j++){
       printf(" %d ",Matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");

}

