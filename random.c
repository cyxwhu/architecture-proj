#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <tss/tss_error.h>
#include <tss/platform.h>
#include <tss/tss_defines.h>
#include <tss/tss_typedef.h>
#include <tss/tss_structs.h>
#include <tss/tspi.h>
#include <trousers/trousers.h>
#define DBG(message, tResult) printf("Line%d, %s)%s returned 0x%08x. %s.\n",__LINE__,__func__,message, tResult,(char *)Trspi_Error_String(tResult))
BYTE *randomBytes;
int main(int argc,char **argv){
 TSS_HCONTEXT hContext;
 TSS_HTPM hTPM;
 TSS_RESULT result;
 //TSS_HKEY hSRK=0;
 //TSS_HPOLICY hSRKPolicy=0;
 //TSS_UUID SRK_UUID=TSS_UUID_SRK;
 FILE *fp,*record;
 int j,i;
 clock_t start,finish;
 double duration;
 result = Tspi_Context_Create(&hContext);
 DBG("Create Context",result);
 result = Tspi_Context_Connect(hContext,NULL);
 DBG("Context Connect",result);
 result = Tspi_Context_GetTpmObject(hContext,&hTPM);
 DBG("Get TPM Handle",result);
 fp = fopen("random.out","wb");
 record = fopen("record.out","wb");
 for (j = 100;j < 1000;j++){
   start=clock();
   if((randomBytes=(BYTE*)malloc(j))==NULL){
    printf("ERROR AllOCATING randomBytes\n");
    return 1;
   }
   result=Tspi_TPM_GetRandom(hTPM,j,&randomBytes);
   DBG("Get Random number from TPM",result);
   printf("length:%d",j);
   //fp = fopen("random.out","ab+");
   for(i=0;i<j;i++){
     //printf("%02x",randomBytes[i]);
     fprintf(fp,"%02x",randomBytes[i]);
    }
   //printf("\n");
   //write(fileno(fp),randomBytes,j);
   fprintf(fp,"end\n");
   //fp = fopen("random.out","ab+");
  //fclose(fp);
   finish = clock();
   duration = (double)(finish-start)/CLOCKS_PER_SEC;
   fprintf(record,"%d:%lf\n",j,duration);
   //fputs(":",record);
   //fprintf(record,"%lf\n",duration);
 }
  Tspi_Context_FreeMemory(hContext,NULL);
  Tspi_Context_Close(hContext);
  fclose(fp);
 return 0;
}
