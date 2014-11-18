#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void change2Hex(int a,FILE *b){
	if(0<=a&&a<=9)
		fprintf(b,"%d",a);
	if(a==10)
		fprintf(b,"a");
	if(a==11)
		fprintf(b,"b");
	if(a==12)
		fprintf(b,"c");
	if(a==13)
		fprintf(b,"d");
	if(a==14)
		fprintf(b,"e");
	if(a==15)
		fprintf(b,"f");
}
int main(){
  int i,j;
  clock_t start,finish;
  double duration;
  FILE *fp,*record;
  fp = fopen("Crandom.out","wb");
  record = fopen("Crecord.out","wb");
  srand((int)time(0)); 
  for(i=100;i<1000;i++){
  start = clock();
  printf("%d:",i);    
  for(j=0;j<i*2;j++){
     printf("%d ",(int)(random()%16));
     change2Hex((int)(random()%16),fp);
  }  
   printf("\n");
   fprintf(fp,"\n");
   finish = clock();
   duration = (double)(finish-start)/CLOCKS_PER_SEC;
   fprintf(record,"%d:%lf\n",i,duration);
  }
  fclose(fp);
  fclose(record);
  return 0;
}
