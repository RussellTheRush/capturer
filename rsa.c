#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>

#define TEST 1

#define N_BYTE_SIZE	(1024/8)
#define N2_BYTE_SIZE	(2048/8)

#define ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))

#define STR_N       "26DD2D997696AA52EDDF7CA1C9979DF355179EAA98D76D736F3C7EC4C22E96BBDE029672EDA467544A1F1678E901A24A67FE4CE413BE7AAEA6328E397E7E15AF"
#define STR_e       "10001"
#define STR_d       "186D0EC9D334E29BF8916C1DAB3C35B3C8196E49BE82377243C97BCB625831B81DF64BA29B057A333359FB55736727B6EA9C50261B7B6AE44C319F3F134C1481"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

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

#ifdef TEST
void dump_bytes(u8 *a, u32 l);
u8 bignum[] = {0xab, 0xcc, 0xfb, 0x19, 0xaa, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
u8 bignum_e[] = {0x3c, 0x69, 0xfb, 0x75, 0x9b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
u8 bignum_N[] = {0xab, 0xea, 0xfb, 0x19, 0xd9, 0x33, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
void test_bignumber_BitShiftSub(u8 *ia, u8 *ib);
void test_bignumber_mul();
void test_bignumber_modPow();
void test_bignumber_modPow(u8 *ia, u8 *ib, u8 *iN);
#endif


static u8 rsa_N[N_BYTE_SIZE];
static u8 rsa_e[N_BYTE_SIZE];
static u8 rsa_d[N_BYTE_SIZE];

static u32 rsa_Nbytes, rsa_ebytes, rsa_dbytes;

void rsa_init();
u32 rsa_encrypt(u8 *M, u32 Mbyte, u8 *out);
u32 rsa_decrypt(u8 *M, u32 Mbyte, u8 *out);


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
    u8 msg[] = {0x11, 0x72, 0x31, 0x38, 0x91, 0x11};
    u8 enc[N_BYTE_SIZE];
    u8 str[N2_BYTE_SIZE];
    u32 enc_size, str_size;
    rsa_init();
    enc_size = rsa_encrypt(msg, sizeof(msg), enc);
    str_size = bytes2string(enc, enc_size, str);
    string_reverse(str);
    printf("#enc:\n%s\n", str);
    enc_size = rsa_decrypt(enc, enc_size, enc);
    dump_bytes(enc, enc_size);
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

u32 bignumber_modMul(u8 *a, u8 *b, u8 *N, u32 la, u32 lb, u32 lN, u8 *out) {
	u32 i;
	u32 t;
	u8 ta[N2_BYTE_SIZE + 16] = {0};
	u8 sum[N2_BYTE_SIZE] = {0};
	u32 tsize;
	u32 sum_size = 0;
    u8 strta[N2_BYTE_SIZE*2];
    u8 strsum[N2_BYTE_SIZE*2];

	for (i=0; i<lb; i+=2) {
		if (i == lb-1) {
			t = (*(u32 *)(b+i)) & 0xff;
		} else {
			t = (*(u32 *)(b+i)) & 0xffff;
		}

		tsize = bignumber_mul(a, t, la, ta);
		sum_size = bignumber_shiftAdd(sum, ta, sum_size, tsize, i);

        //bytes2string(sum, sum_size, strsum);
        //string_reverse(strsum);
        //printf("raw sum: %s\n", strsum);

        //sum_size = bignumber_mod(sum, N, sum_size, lN);


        //bytes2string(ta, tsize, strta);
        //string_reverse(strta);
        //bytes2string(sum, sum_size, strsum);
        //string_reverse(strsum);
        //printf("mod sum: %s\n", strsum);
        //printf("ta: %s\n", strta);
	}
    sum_size = bignumber_mod(sum, N, sum_size, lN);

    memcpy(out, sum, sum_size);
	//dump_bytes(sum, sum_size);
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
	u32 i;
	for (i=0; i<tsize; i+=2) {
		if (i == tsize-1) {
			t = (*(u32 *)(ta + i)) & 0xff;
		}
		else  {
			t = (*(u32 *)(ta + i)) & 0xffff;
		}

		if (i+n < sum_size-1) {
			f = (*(u32 *)(sum + i + n)) & 0xffff;
		} else if (i+n == sum_size-1){
			f = (*(u32 *)(sum + i + n)) & 0xff;
		} else {
			f = 0;
		}

        //printf("t: %x, f: %x ", t, f);
		t = f + t + carry;
		carry = (t & (~0xffff)) >> 16;
        //printf("carry: %x t+f:%x\n", carry, t);
		(*(u16 *)(sum + i + n)) = t & 0xffff;
	}
    if (i+n > ret_size) {
        ret_size = i+n;
        while (!*(u8 *)(sum + ret_size -1)) ret_size--;
    }

	if (carry != 0) {
		(*(u16 *)(sum + i + n)) = t & 0xffff;
		if ((carry & 0xff00) != 0) ret_size = ret_size + 2;
		else ret_size = ret_size + 1;
	}
	return ret_size;
}

u32 bignumber_getBitSize(u8 *a, u32 la) {
	u32 res = (la - 1) * 8;
	u8 byte = *(a + la - 1);
	int i;
	for (i=7; i>=0; i--) {
		if ((byte & (1<<i))) {
			res += i + 1;
			break;
		}
	}
	return res;
}

u32 bignumber_BitShiftCmp(u8 *a, u8 *b, u32 abits, u32 bbits, u32 shift) {
    u32 t, f;
    u32 units = bbits / 16;
    u32 rem = bbits % 16;
    u32 i, ai, bi;

    //printf("abits: %d, bbits+shift: %d\n", abits, bbits+shift);
    if (abits > bbits + shift) return 1;
    if (abits < bbits + shift) return -1;

    //printf("rem: %d\n", rem);
   
    ai = bbits + shift;
    bi = bbits;
    for (i=0; i<units; i++) {
        ai -= 16;
        bi -= 16;
        t = bignumber_bitShiftGetBits(a, abits, ai, 16);
        f = bignumber_bitShiftGetBits(b, bbits, bi, 16);
        //printf("t: %x\n", t);
        //printf("f: %x\n", f);
        if (t > f) {
            return 1;
        } else if (t < f) {
            return -1;
        } 
    }

    if (rem) {
        t = bignumber_bitShiftGetBits(a, abits, ai - rem, rem);
        f = bignumber_bitShiftGetBits(b, abits, bi - rem, rem);
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
    u32 units = bbits / 16;
    u32 rem = bbits % 16;
    int i, ai, bi;
    u32 f, t, ts;
    u32 carry = 1;
    u32 res = 0;
    u32 zflag = 0;


    assert(bbits + shift <= abits);

    ai = shift;
    bi = 0;

#ifdef TEST
    u8 a_cpy[N2_BYTE_SIZE];
    u8 b_cpy[N2_BYTE_SIZE];
    u32 abytes_cpy = abytes;
    u32 bbytes_cpy = bbytes;
    memcpy(a_cpy, a, abytes);
    memcpy(b_cpy, b, bbytes);
#endif


    for (i=0; i<units; i++) {
        t = bignumber_bitShiftGetBits(a, abits, ai, 16);
        f = bignumber_bitShiftGetBits(b, bbits, bi, 16);
        f = (~f) & 0xffff;
        t = t + f + carry;
        carry = t >> 16;
        bignumber_bitShiftSetBits(a, abits, ai, 16, t);
        ai = ai + 16;
        bi = bi + 16;
    }

    if (rem) {
        t = bignumber_bitShiftGetBits(a, abits, ai, rem);
        f = bignumber_bitShiftGetBits(b, bbits, bi, rem);
        //printf("t: %x, f: %x, carry: %x\n", t, f, carry);
        f = (~f) & ((1<<rem)-1);
        t = t + f + carry;
        //printf("t: %x, f: %x\n", t, f);
        carry = t >> rem;
        //printf("rem: %x, t: %x, carry: %x\n", rem, t, carry);
        bignumber_bitShiftSetBits(a, abits, ai, rem, t);
        ai = ai + rem;
        res = ai;
    }

//    if (carry && (res < abits)) {
//        ts = bignumber_getBitSize((u8 *)&carry, 1);
//        f = bignumber_bitShiftGetBits(a, abits, res, ts);
//        t = carry + f + ((1<<ts)-1);
//        
//        bignumber_bitShiftSetBits(a, abits, ai, ts, t);
//        ai = ai + t;
//        res = ai;
//    }
    
    if (res<abits) {
        //carry = 0;
        units = (abits-res) / 16;
        rem = (abits-res) % 16;
        for (i=0; i<units; i++) {
            f = bignumber_bitShiftGetBits(a, abits, res, 16);
            t = f + 0xffff + carry;
            carry = t >> 16;
            bignumber_bitShiftSetBits(a, abits, res, 16, t);
            res = res + 16;
        }
        if (rem) {
            f = bignumber_bitShiftGetBits(a, abits, res, rem);
            t = f + ((1<<rem)-1) + carry;
            t = t & ((1<<rem)-1);
            bignumber_bitShiftSetBits(a, abits, res, 16, t);
            res = res + rem;
        }
    }

    rem = res % 8;
    units = res/8;
    zflag = 1;

    if (rem) {
       f = bignumber_bitShiftGetBits(a, abits, res-rem, rem);
       //printf("res: %d, f: %x, rem: %d\n", res, f, rem);
       if (!f) {
           zflag = 1;
           res = res - rem;
       } else {
           zflag = 0;
           res = bignumber_getBitSize(a, (res+7)/8);
       }
    }

    if (zflag) {
        for (i=units-1; i>=0; i--) {
            f = *(u8 *)(a+i);
            //printf("AAf: %x, res: %d\n", f, res);
            if (!f) {
                res = res - 8;
            }
            else {
                break;
            }
        }
    }

    res = bignumber_getBitSize(a, (res+7)/8);
    //printf("ai: %d\nt: ", ai);
    //printf("res: %d\n", res);
    //bytes2string(a, (res+7)/8, stra);
    //string_reverse(stra);
    //printf("res: %s\n", stra);
    //printf("bits: %d\n", res);
//    exit(0);
    return res;
}

inline u32 bignumber_bitShiftGetBits(u8 *a, u32 abits, u32 bitShift, u32 bits) {
    //assert(bitShift < abits);
    assert(bits < 32);
    u32 bytes = bitShift / 8;
    u32 rem = bitShift % 8;
    u32 res = 0;
    res = (*(u32 *)(a+bytes) >> rem) & ((1 << (bits)) - 1);
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

    if (sum_bits < Nbits) return sum_size;

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


#ifdef TEST
void dump_bytes(u8 *a, u32 l) {
	u32 i;
	printf("#size: %d", l);
	for (i=0; i<l; i++) {
		if(i % 16 == 0) printf("\n#");
		printf("%02x ", (*(u32 *)(a+i)) & 0xff);
	}
	printf("\n#");
}
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
    pid_t pid;
    u8 stra[N_BYTE_SIZE*2];
    u8 strb[N_BYTE_SIZE*2];
    u8 a[N_BYTE_SIZE];
    u8 b[N_BYTE_SIZE];
    u8 res[N2_BYTE_SIZE];
    u32 as, bs, abits, bbits;

    strcpy(stra, ia);
    strcpy(strb, ib);

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
    as = bignumber_BitShiftSub(a, b, abits, bbits, 1);
    //dump_bytes(res, rs);

    printf("#output:\n#");
    printBytesAsHexString(a, (as+7)/8);
}

void printBytesAsHexString(u8 *bytes, u32 size) {
    u8 str[N2_BYTE_SIZE];
    bytes2string(bytes, size, str);
    string_reverse(str);
    printf("%s\n", str);
}
#endif
