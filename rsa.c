#include <stdio.h>
#include <string.h>
#include <assert.h>

#define TEST 1

#define N_BYTE_SIZE	(1024/8)
#define N2_BYTE_SIZE	(2048/8)

#define ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

//a,b,N little endian big number, result in a return size
u32 bignumber_modMul(u8 *a, u8 *b, u8 *N, u32 la, u32 lb, u32 lN);
//a, little endian big number, b less then 0xffff, result in out, return size
u32 bignumber_mul(u8 *a, u32 b, u32 la, u8 *out);
//sum add ta after shit n, result in sum, ruturn size
u32 bignumber_shiftAdd(u8 *sum, u8 *ta, u32 sum_size, u32 tsize, u32 n);

u32 bignumber_getBitSize(u8 *a, u32 la);

u32 bignumber_BitShiftCmp(u8 *a, u8 *b, u32 abits, u32 bbits, u32 shift);


inline u32 bignumber_bitShiftGetBits(u8 *a, u32 abits, u32 bitShift, u32 bits);

inline void bignumber_bitShiftSetBits(u8 *a, u32 abits, u32 bitShift, u32 bits, u32 n);

void dump_bytes(u8 *a, u32 l);

#ifdef TEST
u8 bignum[] = {0xab, 0xcc, 0xfb, 0x19, 0xaa, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
u8 bignum_e[] = {0x3c, 0x69, 0xfb, 0x75, 0x9b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
u8 bignum_N[] = {0xab, 0xea, 0xfb, 0x19, 0xd9, 0x33, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
#endif

int main(int argc, char *argv[]) {
#ifdef TEST
	//u32 size = bignumber_mul(bignum, 0xfa3c, 6);
	//dump_bytes(bignum, size);
	u32 sum_size = bignumber_modMul(bignum, bignum_e, bignum_N, ARRAY_SIZE(bignum), ARRAY_SIZE(bignum_e), ARRAY_SIZE(bignum_N));
    printf("test get bit size:\n");
    u32 bit_size = bignumber_getBitSize(bignum_N, 7);
    printf("bit size: %d\n", bit_size);
    printf("test bignumber_bitShiftGetBits:\n");
    u32 n = bignumber_bitShiftGetBits(bignum_N, bit_size, 17, 15);
    printf("n: %x\n", n);

    printf("test bignumber_bitShiftSetBits:\n");
	dump_bytes(bignum_N, ARRAY_SIZE(bignum_N));
    bignumber_bitShiftSetBits(bignum_N, bit_size, 17, 16, 0x56c7);
	dump_bytes(bignum_N, ARRAY_SIZE(bignum_N));
    printf("test bignumber_BitShiftCmp:\n");
    u8 a[] = {0xaa, 0xba, 0xcd, 0x17};
    u8 b[] = {0xba, 0xcd, 0x17};
	dump_bytes(a, ARRAY_SIZE(a));
	dump_bytes(b, ARRAY_SIZE(b));
    u32 sa = bignumber_getBitSize(a, ARRAY_SIZE(a));
    printf("sa: %d\n", sa);
    u32 sb = bignumber_getBitSize(b, ARRAY_SIZE(b));
    printf("sb: %d\n", sb);
    u32 cmp_res = bignumber_BitShiftCmp(a, b, sa, sb, sa - sb);
    printf("cmp_res %d\n", cmp_res);
#endif
	return 0;
}

u32 bignumber_modMul(u8 *a, u8 *b, u8 *N, u32 la, u32 lb, u32 lN) {
	u32 i;
	u32 t;
	u8 ta[N2_BYTE_SIZE] = {0};
	u8 sum[N2_BYTE_SIZE] = {0};
	u32 tsize;
	u32 sum_size = 0;
	for (i=0; i<lb; i+=2) {
		if (i == lb-1) {
			t = (*(u32 *)(b+i)) & 0xff;
		} else {
			t = (*(u32 *)(b+i)) & 0xffff;
		}
		tsize = bignumber_mul(a, t, la, ta);
		sum_size = bignumber_shiftAdd(sum, ta, sum_size, tsize, i);
	}
	dump_bytes(sum, sum_size);
	return 0;
}

u32 bignumber_mul(u8 *a, u32 b, u32 la, u8 *out) {
	u32 i;
	u32 t;
	u32 carry = 0;
	u32 ret_size = la;
	for (i=0; i<la; i+=2) {
		if (i == la-1) {
			t = (*(u16 *)(a+i)) & 0xff;
		} else {
			t = (*(u16 *)(a+i)) & 0xffff;
		}
		t = t * b + carry;
		carry = (t & (~0xffff)) >> 16;
		t = t & 0xffff;
		*(u16 *)(out + i) = t;
	}
	if (carry != 0) {
		//printf("i: %d,carry: %04x\n", i, carry & 0xffff);
		*(u16 *)(out + i) = carry;
		if ((carry & 0xff00) != 0) ret_size = i + 2;
		else ret_size = i + 1;
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
		t = f + t + carry;
		carry = (t & (~0xffff)) >> 16;
		(*(u16 *)(sum + i + n)) = t & 0xffff;
	}
	if (carry != 0) {
		(*(u16 *)(sum + i + n)) = t & 0xffff;
		if ((carry & 0xff00) != 0) ret_size = i + n + 2;
		else ret_size = i + n + 1;
	}
	return ret_size;
}

u32 bignumber_getBitSize(u8 *a, u32 la) {
	u32 res = (la - 1) * 8;
	u8 byte = *(a + la - 1);
	u32 i;
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

inline u32 bignumber_bitShiftGetBits(u8 *a, u32 abits, u32 bitShift, u32 bits) {
    assert(bitShift < abits);
    assert(bits < 32);
    u32 bytes = bitShift / 8;
    u32 rem = bitShift % 8;
    u32 res = 0;
    res = (*(u32 *)(a+bytes) >> rem) & ((1 << (bits)) - 1);
    return res;
}

inline void bignumber_bitShiftSetBits(u8 *a, u32 abits, u32 bitShift, u32 bits, u32 n) {
    assert(bitShift < abits);
    assert(bits <= 16);
    u32 bytes = bitShift / 8;
    u32 rem = bitShift % 8;
    u32 t = 0;
    t = *(u32 *)(a+bytes);
    t = (t & (~(((1<<bits)-1) << rem))) | ((n & ((1<<bits)-1)) << rem);
    *(u32 *)(a+bytes) = t;
}

void dump_bytes(u8 *a, u32 l) {
	u32 i;
	printf("size: %d", l);
	for (i=0; i<l; i++) {
		if(i % 16 == 0) printf("\n");
		printf("%02x ", (*(u32 *)(a+i)) & 0xff);
	}
	printf("\n");
}
