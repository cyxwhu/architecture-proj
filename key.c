#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
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
#define BACKUP_KEY_UUID {0, 0, 0, 0, 0, {0, 0, 0, 0, 2, 19}}
#define DBG(message, tResult) printf("Line%d, %s)%s returned 0x%08x. %s.\n",__LINE__,__func__,message, tResult,(char *)Trspi_Error_String(tResult))
int main(){
    TSS_RESULT result;
    TSS_HTPM hTPM;
    TSS_HCONTEXT hContext;
    TSS_HPOLICY hownerpolicy,hSRKPolicy;
    TSS_HKEY hSRK;
    TSS_HKEY hkey;
    TSS_UUID SRK_UUID=TSS_UUID_SRK;
    TSS_FLAG initFlags;            
    TSS_UUID bindkeyUUID=BACKUP_KEY_UUID;    
    BYTE *pubKey;
    UINT32 pubKeySize;
    struct timeval tpstart,tpend;
    double duration;
    FILE *fp;
    int i;
    fp = fopen("2048key.out","wb");
    for(i=0;i<50;i++){

   // bindkeyUUID = {0, 0, 0, 0, 0, {0, 0, 0, 0, 2, i}};
    gettimeofday(&tpstart,NULL);
    result=Tspi_Context_Create(&hContext);
    DBG("create context",result);
    result=Tspi_Context_Connect(hContext,NULL);
    DBG("connect to local tpm",result);
    result=Tspi_Context_GetTpmObject(hContext,&hTPM);
    DBG("creat tpm object",result); 
    result=Tspi_GetPolicyObject(hTPM,TSS_POLICY_USAGE,&hownerpolicy);
    DBG("get tpm policy",result);
    result=Tspi_Policy_SetSecret(hownerpolicy,TSS_SECRET_MODE_NONE,0,NULL);
    DBG("set tpm policy secret",result);
    result=Tspi_Context_LoadKeyByUUID(hContext,TSS_PS_TYPE_SYSTEM,SRK_UUID,&hSRK);
    DBG("load srk",result);
    result=Tspi_GetPolicyObject(hSRK,TSS_POLICY_USAGE,&hSRKPolicy);
    DBG("get srk policy",result);
    result=Tspi_Policy_SetSecret(hSRKPolicy,TSS_SECRET_MODE_NONE,0,NULL);
    DBG("set srk secret",result);
    initFlags=TSS_KEY_TYPE_BIND|TSS_KEY_SIZE_2048|TSS_KEY_NO_AUTHORIZATION;                
    result=Tspi_Context_CreateObject(hContext,TSS_OBJECT_TYPE_RSAKEY,initFlags,&hkey);        
    DBG("set bind key",result);
    result=Tspi_SetAttribUint32(hkey,TSS_TSPATTRIB_KEY_INFO,TSS_TSPATTRIB_KEYINFO_ENCSCHEME,TSS_ES_RSAESPKCSV15);
    DBG("padding key",result);
    result=Tspi_Key_CreateKey(hkey,hSRK,0);
    DBG("create key",result);
    result=Tspi_Context_RegisterKey(hContext,hkey,TSS_PS_TYPE_USER,bindkeyUUID,TSS_PS_TYPE_SYSTEM,SRK_UUID);
    DBG("register key",result);
    result = Tspi_Key_LoadKey(hkey,hSRK);
    DBG("load key",result);
    result = Tspi_Key_GetPubKey(hkey,&pubKeySize,&pubKey);
    DBG("get key",result);
    Tspi_Context_FreeMemory(hContext,NULL);
    Tspi_Context_Close(hContext);
    gettimeofday(&tpend,NULL);
    duration = 1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    printf("%lf\n",duration/1000000);
    printf("SUCCESS!\n");
    fprintf(fp,"%d:%lf\n",i,duration);
    }
    fclose(fp);
    return 0;
}
