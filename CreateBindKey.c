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
#define BACKUP_KEY_UUID {0,0,0,0,0,{0,0,0,2,12}}
#define DBG(message, tResult) printf("Line%d, %s)%s returned 0x%08x. %s.\n",__LINE__,__func__,message, tResult,(char *)Trspi_Error_String(tResult))
int main(int argc,char **argv){
TSS_HCONTEXT hContext;
TSS_HKEY hSRK, hKey;
TSS_RESULT result;
TSS_HPOLICY hPolicy;
FILE *fp;
TSS_UUID SRK_UUID=TSS_UUID_SRK;
BYTE *secret = "cyx123";
BYTE *pubKey;
UINT32 pubKeySize;
int i=0;
Tspi_Context_Create(&hContext);
/* Connect to the local TCS provider */
Tspi_Context_Connect(hContext, NULL);
/* Load the new key’s parent key, the Storage Root Key */
result = Tspi_Context_LoadKeyByUUID(hContext, TSS_PS_TYPE_SYSTEM,SRK_UUID,&hSRK);
DBG("load parentKey",result);
/* Create the software key object */
Tspi_Context_CreateObject(hContext, TSS_OBJECT_TYPE_RSAKEY,TSS_KEY_TYPE_BIND|TSS_KEY_SIZE_2048 |TSS_KEY_AUTHORIZATION|TSS_KEY_NOT_MIGRATABLE,&hKey);
/* Create the usage policy object for the key */
Tspi_Context_CreateObject(hContext, TSS_OBJECT_TYPE_POLICY,TSS_POLICY_USAGE, &hPolicy);
/* Set the secret in the policy */
Tspi_Policy_SetSecret(hPolicy, TSS_SECRET_MODE_PLAIN, strlen(secret), secret);
/* Assign the policy object to the key */
result = Tspi_Policy_AssignToObject(hPolicy, hKey);
//DBG("assign policy",result);
/* Call down to the TPM to generate the key */
result = Tspi_SetAttribUint32(hKey,TSS_TSPATTRIB_KEY_INFO,TSS_TSPATTRIB_KEYINFO_ENCSCHEME,TSS_ES_RSAESPKCSV15);
DBG("set attribute",result);
result = Tspi_Key_CreateKey(hKey, hSRK, 0);
DBG("creat a key",result);
result = Tspi_Key_LoadKey(hKey,hSRK);
DBG("load public Key",result);
result = Tspi_Key_GetPubKey(hKey,&pubKeySize,&pubKey);
DBG("get the public key",result);
return 0;
}
