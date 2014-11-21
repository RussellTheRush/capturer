#include <stdio.h>
#include <string.h>
#include "libperm_rsa.h"

#define STR_N       "95DA32B04A160337F88B11882EFBDB7A3B4633BC80E1FF19AA5064728992E80BF75ED11473D2E9DAE200C9DF3A283CB32D143C83880F65C02C432F850A79FBD1"
#define STR_e       "10001"
#define STR_d       "4E5A3FA4B3A81CD8B82926EA98FE717CD0DF5D0FD427D6B5B9BABD3AB23A50FF9B31999789AE0099CA48E20594D8538A61F1E41477348DEA28513474AA50CD4D"

int main(int argc, char *argv[]) {
    char msg[] = {0x12, 0x34, 0x56, 0xaa, 0xbb, 0xcc};
    char res[N_BYTE_SIZE];
    char strN[] = STR_N;
    char strE[] = STR_e;
    char strD[] = STR_d;

    int res_size;

    int strNLen = strlen(strN);
    int strELen = strlen(strE);
    int strDLen = strlen(strD);

    res_size = libperm_rsa_modPow(msg, sizeof(msg), strE, strELen, strN, strNLen, res);
    res_size = libperm_rsa_modPow(res, res_size, strD, strDLen, strN, strNLen, res);

    if (memcmp(msg, res, res_size) == 0) {
        printf("#successed.\n");
    } else {
        printf("#failed.\n");
    }

    return 0;
}
