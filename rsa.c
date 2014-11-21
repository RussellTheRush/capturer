#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include "libperm_rsa.h"

//#define TEST 1
#define IS_LIB

#define ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))

//a,b,N little endian big number, result in a return size
u32 bignumber_modMul(u8 *a, u8 *b, u8 *N, u32 la, u32 lb, u32 lN, u8 *out);
//a, little endian big number, b less then 0xffff, result in out, return size
u32 bignumber_mul(u8 *a, u32 b, u32 la, u8 *out);
//sum add ta after shit n, result in sum, ruturn size
u32 bignumber_shiftAdd(u8 *sum, u8 *ta, u32 sum_size, u32 tsize, u32 n);

u32 bignumber_getBitSize(u8 *a, u32 la);

u32 bignumber_mod(u8 *sum, u8 *N, u32 sum_size, u32 lN);

u32 bignumber_BitShiftCmp(u8 *a, u8 *b, u32 abits, u32 bbits, u32 shift);

u32 bignumber_BitShiftSub(u8 *a, u8 *b, u32 abits, u32 bbits, u32 shift);
u32 bignumber_modPow(u8 *a, u8 *b, u8 *N, u32 abytes, u32 bbytes, u32 Nbytes, u8 *out);

u32 string2bytes(u8 *string, u8 *bytes);
u32 bytes2string(u8 *bytes, u32 byte_len, u8 *string);

u32 bytes_reverse(u8 *src, u8 *des, u32 len);

inline u32 bignumber_bitShiftGetBits(u8 *a, u32 abits, u32 bitShift, u32 bits);

inline void bignumber_bitShiftSetBits(u8 *a, u32 abits, u32 bitShift, u32 bits, u32 n);

void string_reverse(u8 *string);


#ifndef IS_LIB
//#define STR_N       "26DD2D997696AA52EDDF7CA1C9979DF355179EAA98D76D736F3C7EC4C22E96BBDE029672EDA467544A1F1678E901A24A67FE4CE413BE7AAEA6328E397E7E15AF"
//#define STR_N       "9628686253532F6A07852BE7744E20B1880737AA4FACB5B44E6EB91749CB64D99879724308C928449633238D5F245E10AC01A6D57D5513D7EC67EC726B41C3E7"
#define STR_N       "95DA32B04A160337F88B11882EFBDB7A3B4633BC80E1FF19AA5064728992E80BF75ED11473D2E9DAE200C9DF3A283CB32D143C83880F65C02C432F850A79FBD1"
#define STR_e       "10001"
//#define STR_d       "186D0EC9D334E29BF8916C1DAB3C35B3C8196E49BE82377243C97BCB625831B81DF64BA29B057A333359FB55736727B6EA9C50261B7B6AE44C319F3F134C1481"
//#define STR_d       "25276A6CA9E17361F0EA6AAAA28492625B1E2E0FDE1705FECF496652F4D776E45F21356CDFAE8B9DBA6925D0762CBE3842B2B44CF814EA413BEEACCA60E9DE81"
#define STR_d       "4E5A3FA4B3A81CD8B82926EA98FE717CD0DF5D0FD427D6B5B9BABD3AB23A50FF9B31999789AE0099CA48E20594D8538A61F1E41477348DEA28513474AA50CD4D"

static u8 rsa_N[N_BYTE_SIZE];
static u8 rsa_e[N_BYTE_SIZE];
static u8 rsa_d[N_BYTE_SIZE];

static u32 rsa_Nbytes, rsa_ebytes, rsa_dbytes;

void rsa_init();
u32 rsa_encrypt(u8 *M, u32 Mbyte, u8 *out);
u32 rsa_decrypt(u8 *M, u32 Mbyte, u8 *out);
#endif //IS_LIB

#ifdef TEST
u8 bignum[] = {0xab, 0xcc, 0xfb, 0x19, 0xaa, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
u8 bignum_e[] = {0x3c, 0x69, 0xfb, 0x75, 0x9b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
u8 bignum_N[] = {0xab, 0xea, 0xfb, 0x19, 0xd9, 0x33, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
void test_bignumber_BitShiftSub(u8 *ia, u8 *ib);
void test_bignumber_mul();
void test_bignumber_modPow();
void test_bignumber_modPow(u8 *ia, u8 *ib, u8 *iN);
void printBytesAsHexString(u8 *bytes, u32 size);
#endif


int libperm_rsa_modPow(u8 *msg, u32 msgBytes, u8 *strE, u32 strELen, u8 *strN, u32 strNLen, u8 *out) {
    if (msgBytes > N_BYTE_SIZE || strELen > N_BYTE_SIZE*2 || strNLen > N_BYTE_SIZE*2) {
        fprintf(stderr, "arguments error.\n");
        return -1;
    }

    u8 e[N_BYTE_SIZE];
    u8 N[N_BYTE_SIZE];  
    u8 strrN[N_BYTE_SIZE*2];
    u8 strrE[N_BYTE_SIZE*2];
    u8 msg_cpy[N_BYTE_SIZE];
    u8 sum[N2_BYTE_SIZE];
    u32 e_bytes, N_bytes, sum_bytes;

    while ((msgBytes > 0) && (msg[msgBytes-1] == 0)) {
        msgBytes--;
    }
    if (msgBytes == 0) msgBytes = 1;
    
    strcpy(strrE, strE);
    strcpy(strrN, strN);
    string_reverse(strrE);
    string_reverse(strrN);

    memcpy(msg_cpy, msg, msgBytes);

    e_bytes = string2bytes(strrE, e);
    N_bytes = string2bytes(strrN, N);
    sum_bytes = bignumber_modPow(msg, e, N, msgBytes, e_bytes, N_bytes, sum);
    memcpy(out, sum, sum_bytes);
    return (int)sum_bytes;
}

#ifndef IS_LIB

int main(int argc, char *argv[]) {
#ifdef TEST
    //test_bignumber_mul(argv[1], argv[2], argv[3]);
    //test_bignumber_modPow(argv[1], argv[2], argv[3]);
    //test_bignumber_BitShiftSub(argv[1], argv[2]);
	//u32 size = bignumber_mul(bignum, 0xfa3c, 6);
	//dump_bytes(bignum, size);
    //u8 string[N_BYTE_SIZE * 2];
    //scanf("%s", string);
    //u8 bs[N_BYTE_SIZE]; 
    //u8 bsr[N_BYTE_SIZE]; 
    //u32 bss =  string2bytes(string, bs);
    //dump_bytes(bs, bss);
    //bytes2string(bs, bss, string);
    //bytes_reverse(bs, bsr, bss);
    //bytes_reverse(bsr, bs, bss);
    //bytes2string(bs, bss, string);
    //printf("string: %s\n", string);
	//u32 sum_size = bignumber_modMul(bignum, bignum_e, bignum_N, ARRAY_SIZE(bignum), ARRAY_SIZE(bignum_e), ARRAY_SIZE(bignum_N));
    //printf("test get bit size:\n");
    //u32 bit_size = bignumber_getBitSize(bignum_N, 7);
    //printf("bit size: %d\n", bit_size);
    //printf("test bignumber_bitShiftGetBits:\n");
    //u32 n = bignumber_bitShiftGetBits(bignum_N, bit_size, 17, 15);
    //printf("n: %x\n", n);

    //printf("test bignumber_bitShiftSetBits:\n");
	//dump_bytes(bignum_N, ARRAY_SIZE(bignum_N));
    //bignumber_bitShiftSetBits(bignum_N, bit_size, 17, 16, 0x56c7);
	//dump_bytes(bignum_N, ARRAY_SIZE(bignum_N));
    //printf("test bignumber_BitShiftCmp:\n");
    //u8 a[] = {0xaa, 0xba, 0xcd, 0x17};
    //u8 b[] = {0xba, 0xcd, 0x17};
	//dump_bytes(a, ARRAY_SIZE(a));
	//dump_bytes(b, ARRAY_SIZE(b));
    //u32 sa = bignumber_getBitSize(a, ARRAY_SIZE(a));
    //printf("sa: %d\n", sa);
    //u32 sb = bignumber_getBitSize(b, ARRAY_SIZE(b));
    //printf("sb: %d\n", sb);
    //u32 cmp_res = bignumber_BitShiftCmp(a, b, sa, sb, sa - sb);
    //printf("cmp_res %d\n", cmp_res);
#endif
    //u8 msg[] = {0x18, 0x78, 0x31, 0x38, 0x91, 0x11, 0x3f, 0xee, 0x78, 0xcd, 0x96, 0xab, 0x98, 0x33};
    u8 msg[] = {0x4d, 0x9c, 0x03, 0x11, 0x3e, 0x78, 0x77, 0x12, 0x13, 0x1a};
    u8 enc[N_BYTE_SIZE];
    u8 enc_cpy[N_BYTE_SIZE];
    u32 enc_size, enc_size_cpy;
    rsa_init();
    enc_size = rsa_encrypt(msg, sizeof(msg), enc);
    memcpy(enc_cpy, enc, enc_size);
    enc_size_cpy = enc_size;
    enc_size = rsa_decrypt(enc, enc_size, enc);

    printf("#RSA N: ");
    printBytesAsHexString(rsa_N, rsa_Nbytes);
    printf("#RSA e: ");
    printBytesAsHexString(rsa_e, rsa_ebytes);
    printf("#RSA d: ");
    printBytesAsHexString(rsa_d, rsa_dbytes);
    printf("#RSA msg: ");
    printBytesAsHexString(msg, sizeof(msg));
    printf("#RSA enc(msg): ");
    printBytesAsHexString(enc_cpy, enc_size_cpy);
    printf("#RSA dec(enc(msg)): ");
    printBytesAsHexString(enc, enc_size);
    //dump_bytes(msg, ARRAY_SIZE(msg));
    //dump_bytes(enc, enc_size);
    if (memcmp(msg, enc, enc_size) == 0) {
        printf("#successed.\n");
    } else {
        printf("#failed.\n");
    }
	return 0;
}


void rsa_init() {
    u8 rsa_str_N[] = STR_N;
    u8 rsa_str_e[] = STR_e;
    u8 rsa_str_d[] = STR_d;
    string_reverse(rsa_str_N);
    string_reverse(rsa_str_e);
    string_reverse(rsa_str_d);
    rsa_Nbytes = string2bytes(rsa_str_N, rsa_N);
    rsa_ebytes = string2bytes(rsa_str_e, rsa_e);
    rsa_dbytes = string2bytes(rsa_str_d, rsa_d);
}

u32 rsa_encrypt(u8 *M, u32 Mbytes, u8 *out) {
    return bignumber_modPow(M, rsa_d, rsa_N, Mbytes, rsa_dbytes, rsa_Nbytes, out);
}

u32 rsa_decrypt(u8 *M, u32 Mbytes, u8 *out) {
    return bignumber_modPow(M, rsa_e, rsa_N, Mbytes, rsa_ebytes, rsa_Nbytes, out);
}
#endif // IS_LIB

u32 bignumber_modMul(u8 *a, u8 *b, u8 *N, u32 la, u32 lb, u32 lN, u8 *out) {
	u32 i;
	u32 t;
	u8 ta[N2_BYTE_SIZE + 16] = {0};
	u8 sum[N2_BYTE_SIZE] = {0};
	u32 tsize;
	u32 sum_size = 0;

#ifdef TEST
    u8 a_cpy[N2_BYTE_SIZE];
    u8 b_cpy[N2_BYTE_SIZE];
    u8 N_cpy[N2_BYTE_SIZE];
    u8 a_mul_b[N2_BYTE_SIZE];
    u32 la_cpy = la;
    u32 lb_cpy = lb;
    u32 lN_cpy = lN;
    u32 a_mul_b_size;

    memcpy(a_cpy, a, la_cpy);
    memcpy(b_cpy, b, lb_cpy);
    memcpy(N_cpy, N, lN_cpy);
#endif

	for (i=0; i<lb; i+=2) {
		if (i == lb-1) {
			t = (*(u32 *)(b+i)) & 0xff;
		} else {
			t = (*(u32 *)(b+i)) & 0xffff;
		}

		tsize = bignumber_mul(a, t, la, ta);
		sum_size = bignumber_shiftAdd(sum, ta, sum_size, tsize, i);

	}
#ifdef TEST
    memcpy(a_mul_b, sum, sum_size);
    a_mul_b_size = sum_size;
#endif
    sum_size = bignumber_mod(sum, N, sum_size, lN);

    memcpy(out, sum, sum_size);
#ifdef TEST
    printf("test modMul\n");
    printBytesAsHexString(a_cpy, la_cpy);
    printBytesAsHexString(b_cpy, lb_cpy);
    printBytesAsHexString(N_cpy, lN_cpy);
    printBytesAsHexString(a_mul_b, a_mul_b_size);
    printBytesAsHexString(out, sum_size);
#endif
	return sum_size;
}

u32 bignumber_mul(u8 *a, u32 b, u32 la, u8 *out) {
	u32 i;
	u32 t;
	u32 carry = 0;
	u32 ret_size = la;
    u32 end=0;

	for (i=0; i<la; i+=2) {
		if (i == la-1) {
			t = (*(u8 *)(a+i)) & 0xff;
            t = t * b + carry;
            carry = (t & (~0xff)) >> 8;
            t = t & 0xff;
            *(u8 *)(out + i) = t;
            end = i + 1;
		} else {
			t = (*(u16 *)(a+i)) & 0xffff;
            t = t * b + carry;
            carry = (t & (~0xffff)) >> 16;
            t = t & 0xffff;
            *(u16 *)(out + i) = t;
            end = i + 2;
		}
	}

	if (carry != 0) {
		//printf("i: %d,carry: %04x\n", i, carry & 0xffff);
		*(u16 *)(out + end) = carry;
		if ((carry & 0xff00) != 0) ret_size = end + 2;
		else ret_size = end + 1;
	}
	return ret_size;
}

u32 bignumber_shiftAdd(u8 *sum, u8 *ta, u32 sum_size, u32 tsize, u32 n) {
	u32 ret_size = sum_size > tsize ? sum_size : tsize;
	u32 carry = 0;
	u32 t;
	u32 f;
    int i,j;

	for (i=0,j=n; (i<tsize-1) && (j<sum_size-1); i+=2, j+=2) {
        t = (*(u32 *)(ta + i)) & 0xffff;
        f = (*(u32 *)(sum + j)) & 0xffff;

		t = f + t + carry;

        carry = (t >> 16) & 0xffff;
        (*(u16 *)(sum + j)) = t & 0xffff;
	}

    for (; i < tsize; i++) {
        t = *(u8 *)(ta+i);
        if (j < sum_size) {
            f = *(u8 *)(sum + j);
        } else {
            f = 0;
        }
        t = t + f + carry;
        carry = (t >> 8) & 0xff;
        (*(u8 *)(sum + j)) = t;
        j++;
    }

    while (carry) {
        if (j < sum_size) {
            f = *(u8 *)(sum + j);
        } else {
            f = 0;
        }
        t = f + carry;
        carry = (t >> 8) & 0xff;
        (*(u8 *)(sum + j)) = t;
        j++;
    }

    ret_size = j;
    if (ret_size < sum_size) ret_size = sum_size;

	return ret_size;
}

u32 bignumber_getBitSize(u8 *a, u32 la) {
	u32 res = (la - 1) * 8;
	u8 byte = *(a + la - 1);
	//printf("#byte: %x, la-1: %d\n", byte, la-1);
	int i;
	for (i=7; i>=0; i--) {
		if ((byte & (1<<i))) {
			res = res + i + 1;
			break;
		}
	}
	return res;
}

u32 bignumber_BitShiftCmp(u8 *a, u8 *b, u32 abits, u32 bbits, u32 shift) {
    u32 t, f;
    u32 units = bbits / 16;
    u32 rem = bbits % 16;
    int i, ai, bi;

    //printf("abits: %d, bbits+shift: %d\n", abits, bbits+shift);
    if (abits > bbits + shift) return 1;
    if (abits < bbits + shift) return -1;

    //printf("rem: %d\n", rem);
   
    ai = bbits + shift - 1;
    bi = bbits - 1;
    for (i=0; i<units; i++) {
        ai -= 16;
        bi -= 16;
        t = bignumber_bitShiftGetBits(a, abits, ai, 16);
        //printf("#cmp t: %x\n", t);
        f = bignumber_bitShiftGetBits(b, bbits, bi, 16);
        if (t > f) {
            return 1;
        } else if (t < f) {
            return -1;
        } 
    }


    if (rem) {
        t = bignumber_bitShiftGetBits(a, abits, ai - rem + 1, rem);
        f = bignumber_bitShiftGetBits(b, abits, bi - rem + 1, rem);
        //printf("rem t: %x\n", t);
        //printf("rem f: %x\n", f);
        if (t > f) {
            return 1;
        } else if (t < f) {
            return -1;
        }
    }

    return 0;
}

u32 bignumber_BitShiftSub(u8 *a, u8 *b, u32 abits, u32 bbits, u32 shift) {

    u32 units;
    u32 rem;
    u32 f, t;
    u32 carry = 1;
    u32 res = 0;
    u32 abytes = (abits+7)/8;
    u32 bbytes = (bbits+7)/8;
    int i, j, k;
    int needTrim = 0;


    assert(bbits + shift <= abits);

#ifdef TEST
    u8 a_cpy[N2_BYTE_SIZE];
    u8 b_cpy[N2_BYTE_SIZE];

    u32 abytes_cpy = abytes;
    u32 bbytes_cpy = bbytes;
    u32 shift_cpy = shift;
    memcpy(a_cpy, a, abytes_cpy);
    memcpy(b_cpy, b, bbytes_cpy);
#endif
    

    i = shift;
    j = 0;
    rem = 16 - (shift % 16);


    //printf("#rem %d\n", rem);

    if (rem) {
        t = bignumber_bitShiftGetBits(a, abits, i, rem);
        f = bignumber_bitShiftGetBits(b, bbits, j, rem);
        //printf("#A:\n#");
        //printBytesAsHexString(a, abytes);
        //printf("#B:\n#");
        //printBytesAsHexString(b, bbytes);
        //printf("#t: %x, f: %x, carry: %x\n", t, f, carry);
        f = (~f) & ((1<<rem)-1);
        t = t + f + carry;
        //printf("#t: %x, f: %x\n", t, f);
        carry = t >> rem;
        t = t & ((1<<rem)-1);
        //printf("#rem: %x, t: %x, carry: %x, t<<rem: %x\n", rem, t, carry, t<<(i%8));
        bignumber_bitShiftSetBits(a, abits, i, rem, t);
        //printf("#C:\n#");
        //printBytesAsHexString(a, abytes);
        i = i + rem;
        j = j + rem;
    }

    //printf("#i %d, units: %d\n", i, (abits-i)%16);
    //
    if (i<abits) {
    rem = (abits - i) % 16;
    units = (abits - i)/16;

    for (k=0; k<units; k++) {
        t = *(u16 *)(a+(i/8));
        f = bignumber_bitShiftGetBits(b, bbits, j, 16);
        f = (~f) & 0xffff;
        t = t + f + carry;
        carry = t >> 16;
        *(u16 *)(a+(i/8)) = t;
        //if (k == units-1) {
        //    printf("#final units t: %x\n#", t);
        //    printBytesAsHexString(a, abytes);
        //}
        i = i + 16;
        j = j + 16;
    }
    //printf("#i: %d\n", i);

    if (rem) {
        t = bignumber_bitShiftGetBits(a, abits, i, rem);
        f = bignumber_bitShiftGetBits(b, bbits, j, rem);
        //printf("#t: %x, f: %x, carry: %x, f<<%x\n", t, f, carry, f<<(j%8));
        f = (~f) & ((1<<rem)-1);
        t = t + f + carry;
        //printf("t: %x, f: %x\n", t, f);
        carry = t >> rem;
        t = t & ((1<<rem)-1);
        //printf("#rem: %x, t: %x, f: %x, carry: %x, t<<%x\n", rem, t, f, carry, t<<(i%8));
        bignumber_bitShiftSetBits(a, abits, i, rem, t);
        //printf("#AAA:\n#");
        //printBytesAsHexString(a, abytes);
        i = i + rem;
        j = j + rem;
    }
    }


    i--;
    rem = (i + 1) % 8;
    needTrim = 1;
    //printf("#MMMM i: %d, abits: %d\n", i, abits);

    if (rem) {
        t = *(u8 *)(a+(i/8));
        t = t & ((1<<rem)-1);
        //printf("#ZZZZt: %x, rem: %d, i: %d\n", t, rem, i);
        if (!t) {
            i = i - rem;
            needTrim = 1;
        } else {
            for (k=rem-1; k>=0; k--) {
                if ((t & (1<<k))) {
                    i = i - (rem-1-k);
                    break;
                }
            }
            needTrim = 0;
        }
    }

    //printf("#KKKK i: %d\n", i);

    for (; (i>=0) && (needTrim==1); ) {
        t = *(u8 *)(a+(i/8));
        //printf("#FFFt: %x, i: %d\n", t, i);
        if (t) {
            needTrim = 0;
            for (k=7; k>=0; k--) {
                if ((t & (1<<k))) {
                    i = i - (7-k);
                    break;
                }
            }
            break;
        } else {
            i = i - 8;
        }
        //printf("#LLL: %d\n", i);
    }

    if (i < 0) i = 0;

    res = i + 1;
    t = *(u8 *)(a+res/8);
    t = t & ((1<<(res%8))-1);
    *(u8 *)(a+res/8) = t;


#ifdef TEST
    printf("test shiftSub\n");
    printf("#res: %d, abits: %d, bbits: %d\n", res, abits, bbits);
    printf("%d\n", shift_cpy);
    printBytesAsHexString(a_cpy, abytes_cpy);
    printBytesAsHexString(b_cpy, bbytes_cpy);
    printBytesAsHexString(a, (res+7)/8);
#endif
    return res;
}

inline u32 bignumber_bitShiftGetBits(u8 *a, u32 abits, u32 bitShift, u32 bits) {
    //assert(bitShift < abits);
    assert(bits < 32);
    u32 bytes = bitShift / 8;
    u32 rem = bitShift % 8;
    u32 res = 0;

    if (bitShift + bits < abits) {
        //printf("#AgetBits: a+bytes: %x, shift: %d, abits: %d, bits: %d\n", *(u32*)(a+bytes), bitShift, abits, bits);
        res = (*(u32 *)(a+bytes) >> rem) & ((1 << (bits)) - 1);
    } else if (bitShift < abits) {
        rem = abits - bitShift;
        //printf("#BgetBits: a+bytes: %x, shift: %d, abits: %d, bits: %d\n", *(u32*)(a+bytes), bitShift, abits, bits);
        res = ((*(u32 *)(a+bytes))>>(bitShift%8) )& ((1<<rem)-1);
    } else {
        res = 0;
    }
    return res;
}

inline void bignumber_bitShiftSetBits(u8 *a, u32 abits, u32 bitShift, u32 bits, u32 n) {
    //assert(bitShift < abits);
    assert(bits <= 16);
    u32 bytes = bitShift / 8;
    u32 rem = bitShift % 8;
    u32 t = 0;
    t = *(u32 *)(a+bytes);
    t = (t & (~(((1<<bits)-1) << rem))) | ((n & ((1<<bits)-1)) << rem);
    *(u32 *)(a+bytes) = t;
}

inline u8 ascii2byte(u8 c) {
    if (c>='0' && c<='9') {
        c = c - '0';
    } else if (c>='a' && c<='f') {
        c = c - 'a' + 10;
    } else if (c>='A' && c<='F') {
        c = c - 'A' + 10;
    } else {
        assert(0);
    }
    return c;
}

inline u8 byte2ascii(u8 c) {
    if (c>=0 && c<=9) {
        c = c + '0';
    } else if (c>=10 && c<=15) {
        c = c - 10 + 'a';
    } else {
        assert(0);
    }
    return c;
}

u32 string2bytes(u8 *string, u8 *bytes) {
    u32 i,j;
    u32 len = strlen(string);
    u8  t;
    u8  h, l;

    for (i=0,j=0; i<len; i+=2,j++) {
        if (i == len - 1) {
            h = *(string + i);
            h = ascii2byte(h);
            *(bytes + j) = h;
        } else {
            h = *(string + i);
            h = ascii2byte(h);
            l = *(string + i + 1);
            l = ascii2byte(l);
            t = (l << 4) | h;
            *(bytes + j) = t;
        }
    }
    return j;
}

u32 bytes2string(u8 *bytes, u32 byte_len, u8 *string) {
    u32 i,j;
    u32 len;
    u8  t;
    u8  h, l;
    for (i=0,j=0; i<byte_len; i++) {
        t = *(bytes+i);
        h = t >> 4;
        l = t & 0xf;
        l = byte2ascii(l);
        *(string+j++) = l;
        if (!(i==byte_len-1 && h==0)) {
            h = byte2ascii(h);
            *(string+j++) = h;
        }
    }
    *(string+j++) = '\0';
    return j;
}

void string_reverse(u8 *string) {
    u32 string_len = strlen(string);
    u8 string_rev[N2_BYTE_SIZE*2];
    u32 i,j;
    for (i=string_len-1,j=0; j<string_len; i--,j++) {
        string_rev[j] = string[i];
    }
    string_rev[j++] = '\0';
    memcpy(string, string_rev, string_len);
}

u32 bytes_reverse(u8 *src, u8 *des, u32 len) {
    u8 string[N_BYTE_SIZE*2];
    u32 string_len,i,j;

    string_len = bytes2string(src, len, string);
    string_reverse(string);
    return string2bytes(string, des);
}

u32 bignumber_mod(u8 *sum, u8 *N, u32 sum_size, u32 lN) {
    u32 Nbits = bignumber_getBitSize(N, lN);
    u32 sum_bits = bignumber_getBitSize(sum, sum_size);
    int shift;

	//printf("#Nbits: %d, sum_bits: %d\n", Nbits, sum_bits);
    //printf("#");
    //printBytesAsHexString(sum, sum_size);
    //printf("#");
    //printBytesAsHexString(N, lN);

    if (sum_bits < Nbits) return sum_size;

#ifdef TEST
    u8 sum_cpy[N2_BYTE_SIZE];
    u8 N_cpy[N2_BYTE_SIZE];
    u32 sum_size_cpy = sum_size;
    u32 lN_cpy = lN;

    memcpy(sum_cpy, sum, sum_size_cpy);
    memcpy(N_cpy, N, lN_cpy);
#endif

    shift = sum_bits - Nbits;

    while (shift > 0) {
        if (bignumber_BitShiftCmp(sum, N, sum_bits, Nbits, shift) == -1) {
            shift--;
            if (shift == 0) {
                break;
            }
        }
        sum_bits = bignumber_BitShiftSub(sum, N, sum_bits, Nbits, shift);
        if (sum_bits > Nbits) {
            shift = sum_bits - Nbits;
        } else {
            break;
        }
    }

out:
    if (bignumber_BitShiftCmp(sum, N, sum_bits, Nbits, 0) == 1) {
        sum_bits = bignumber_BitShiftSub(sum, N, sum_bits, Nbits, 0);
    }
#ifdef TEST
    printf("test mode\n");
    printBytesAsHexString(sum_cpy, sum_size_cpy);
    printBytesAsHexString(N_cpy, lN_cpy);
    printBytesAsHexString(sum, (sum_bits+7)/8);
    printf("#sum_bts/8: %d\n", (sum_bits+7)/8);
#endif
    return (sum_bits+7)/8;
}

u32 bignumber_modPow(u8 *a, u8 *b, u8 *N, u32 abytes, u32 bbytes, u32 Nbytes, u8 *out) {
    u32 t;
    u32 bbits;
    u8 ta[N2_BYTE_SIZE] = {1};
    u8 sum[N2_BYTE_SIZE] = {1};
    u32 ta_size, sum_size;
    int i;

#ifdef TEST
    u8 a_cpy[N2_BYTE_SIZE];
    u8 b_cpy[N2_BYTE_SIZE];
    u8 N_cpy[N2_BYTE_SIZE];
    u32 abytes_cpy = abytes;
    u32 bbytes_cpy = bbytes;
    u32 Nbytes_cpy = Nbytes;
    memcpy(a_cpy, a, abytes);
    memcpy(b_cpy, b, bbytes);
    memcpy(N_cpy, N, Nbytes);
#endif
    
    bbits = bignumber_getBitSize(b, bbytes);
    memcpy(ta, a, abytes);
    ta_size = abytes;
    ta_size = bignumber_mod(ta, N, ta_size, Nbytes);
    if (bignumber_bitShiftGetBits(b, bbits, 0, 1)) {
        memcpy(sum, ta, ta_size);
        sum_size = ta_size;
    }

    for (i=1; i<bbits; i++) {
        //bytes2string(ta, ta_size, strta);
        //string_reverse(strta);
        //printf("ta*ta:\n%s\n", strta);
        ta_size = bignumber_modMul(ta, ta, N, ta_size, ta_size, Nbytes, ta);
        //bytes2string(ta, ta_size, strta);
        //string_reverse(strta);
        //printf("%s\n", strta);
        if (bignumber_bitShiftGetBits(b, bbits, i, 1)) {
            //bytes2string(ta, ta_size, strta);
            //string_reverse(strta);
            //bytes2string(sum, sum_size, strsum);

            sum_size = bignumber_modMul(sum, ta, N, sum_size, ta_size, Nbytes, sum);

            //string_reverse(strsum);
            //printf("ta*sum:\n%s\n%s\n", strta, strsum);
            //bytes2string(sum, sum_size, strsum);
            //string_reverse(strsum);
            //printf("%s\n", strsum);
        }
    }
    memcpy(out, sum, sum_size);
#ifdef TEST
    printf("test modPow\n");
    printBytesAsHexString(a_cpy, abytes_cpy);
    printBytesAsHexString(b_cpy, bbytes_cpy);
    printBytesAsHexString(N_cpy, Nbytes_cpy);
    printBytesAsHexString(out, sum_size);
#endif
    return sum_size;
}

void printBytesAsHexString(u8 *bytes, u32 size) {
    u8 str[N2_BYTE_SIZE*2];
    bytes2string(bytes, size, str);
    string_reverse(str);
    printf("%s\n", str);
}

void dump_bytes(u8 *a, u32 l) {
	u32 i;
	printf("#size: %d", l);
	for (i=0; i<l; i++) {
		if(i % 16 == 0) printf("\n#");
		printf("%02x ", (*(u32 *)(a+i)) & 0xff);
	}
	printf("\n#");
}
#ifdef TEST
void test_bignumber_modPow(u8 *ia, u8 *ib, u8 *iN) {

    pid_t pid;
    u8 stra[N_BYTE_SIZE*2];
    u8 strb[N_BYTE_SIZE*2];
    u8 a[N_BYTE_SIZE];
    u8 b[N_BYTE_SIZE];
    u8 N[N_BYTE_SIZE];
    u8 res[N2_BYTE_SIZE];
    u8 strres[N2_BYTE_SIZE*2];
    u8 strN[N2_BYTE_SIZE*2];
    u32 as, bs, Ns, rs;

    strcpy(stra, ia);
    strcpy(strb, ib);
    strcpy(strN, iN);
    printf("#a:\n");
    printf("#%s\n", stra);
    printf("#b:\n");
    printf("#%s\n", strb);
    printf("#N:\n");
    printf("#%s\n", strN);
    
    string_reverse(stra);
    string_reverse(strb);
    string_reverse(strN);

    as = string2bytes(stra, a);
    bs = string2bytes(strb, b);
    Ns = string2bytes(strN, N);
    
    //dump_bytes(a, as);
    //dump_bytes(b, bs);
    rs = bignumber_modPow(a, b, N, as, bs, Ns, res);
    //dump_bytes(res, rs);

    bytes2string(res, rs, strres);
    string_reverse(strres);
    printf("#output:\n");
    printf("#%s\n", strres);
}

void test_bignumber_mul(u8 *ia, u8 *ib, u8 *iN) {

    pid_t pid;
    u8 stra[N_BYTE_SIZE*2];
    u8 strb[N_BYTE_SIZE*2];
    u8 a[N_BYTE_SIZE];
    u8 b[N_BYTE_SIZE];
    u8 N[N_BYTE_SIZE];
    u8 res[N2_BYTE_SIZE];
    u8 strres[N2_BYTE_SIZE*2];
    u8 strN[N2_BYTE_SIZE*2];
    u32 as, bs, Ns, rs;

    strcpy(stra, ia);
    strcpy(strb, ib);
    strcpy(strN, iN);
    printf("#a:\n");
    printf("#%s\n", stra);
    printf("#b:\n");
    printf("#%s\n", strb);
    printf("#N:\n");
    printf("#%s\n", strN);
    
    string_reverse(stra);
    string_reverse(strb);
    string_reverse(strN);

    as = string2bytes(stra, a);
    bs = string2bytes(strb, b);
    Ns = string2bytes(strN, N);
    
    //dump_bytes(a, as);
    //dump_bytes(b, bs);
    rs = bignumber_modMul(a, b, N, as, bs, Ns, res);
    //dump_bytes(res, rs);

    bytes2string(res, rs, strres);
    string_reverse(strres);
    printf("#output:\n");
    printf("#%s\n", strres);
}

void test_bignumber_BitShiftSub(u8 *ia, u8 *ib) {
    u8 stra[N_BYTE_SIZE*2] = "10526e6c0ba918c25ec5c80ed208e43f744d04234d43e08569b4ad99f6f72d756454a2dd60f529456c33d3e483e12cf76a521e17a48c15d3d8b1050628e50d677b7e2e2ecda3fb57b76acc4e214f327b15f322cc9d4b59aefdacd02d20808f9e125152cedb1a6039cb28c172f7b90081";
    u8 strb[N_BYTE_SIZE*2] = "9628686253532f6a07852be7744e20b1880737aa4facb5b44e6eb91749cb64d99879724308c928449633238d5f245e10ac01a6d57d5513d7ec67ec726b41c3e7";
    u8 a[N_BYTE_SIZE];
    u8 b[N_BYTE_SIZE];
    u8 res[N2_BYTE_SIZE];
    u32 as, bs, abits, bbits;
    u32 shift;

    //strcpy(stra, ia);
    //strcpy(strb, ib);
    //
    shift = 379;

    printf("#a:\n");
    printf("#%s\n", stra);
    printf("#b:\n");
    printf("#%s\n", strb);
    
    string_reverse(stra);
    string_reverse(strb);

    as = string2bytes(stra, a);
    bs = string2bytes(strb, b);

    abits = bignumber_getBitSize(a, as);
    bbits = bignumber_getBitSize(b, bs);

    
    //dump_bytes(a, as);
    //dump_bytes(b, bs);
    as = bignumber_BitShiftSub(a, b, abits, bbits, shift);
    //dump_bytes(res, rs);

    printf("#output:\n#");
    printBytesAsHexString(a, (as)/8);
}

#endif
