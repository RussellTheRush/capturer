#include <stdio.h>
#include <string.h>
#include "libperm_rsa.h"

#define STR_N       "596E33249DF3F94D0F97C2DE767572EB06A62D084D80F6334EA449505B9FC11C17D2EA2BA4BB7B712DEF1A91E031001530C9AA5F17ECDB9729BE992CFFD8CC55"
#define STR_e       "10001"
#define STR_d       "18D367AD259001C7EBA934766F2A0181CEF1B894927757613CD68C679B57C8D2808DCADC80E708958EAF2B2F82CA55B9979B6E0EAB2284CE2ADE9C45E5E2EC2D"

int main(int argc, char *argv[]) {
    char msg[] = {0x21,0x11};
    char res[N_BYTE_SIZE];
    char strN[] = STR_N;
    char strE[] = STR_e;
    char strD[] = STR_d;

    int res_size;

    int strNLen = strlen(strN);
    int strELen = strlen(strE);
    int strDLen = strlen(strD);
    u32 i;

    //for (i=0; i<0xffffff; i++) {

        res_size = libperm_rsa_modPow((u8 *)msg, sizeof(msg), strE, strELen, strN, strNLen, res);
        dump_bytes(res, res_size);
        res_size = libperm_rsa_modPow(res, res_size, strD, strDLen, strN, strNLen, res);


        dump_bytes(res, res_size);

        if (memcmp((void *)msg, res, res_size) == 0) {
            printf("#successed.\n");
        } else {
            fprintf(stderr, "#failed, i: %d\n", i);
            printf("#failed, i: %d\n", i);
        }
    //}

    return 0;
}
