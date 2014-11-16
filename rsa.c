#include <stdio.h>
#include <string.h>

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

u32 bignumber_bitShiftCmp(u8 *a, u8 *b, u32 bitShift);
u32 bignumber_bitShift(u8 *a, u8 *out, u32 bitShift);

u32 bignumber_bitShiftAdd(u8 *a, u8 *b, u32 abits, u32 bbits, u32 bitShift);

u32 bignumber_bitShiftGet2Bytes(u8 *a, u32 abits, u32 bitShift);

void dump_bytes(u8 *a, u32 l);

#ifdef TEST
u8 bignum[] = {0xab, 0xcc, 0xfb, 0x19, 0xaa, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
u8 bignum_e[] = {0x3c, 0x69, 0xfb, 0x75, 0x9b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
u8 bignum_N[] = {0xab, 0xea, 0xfb, 0x19, 0xd9, 0x33, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x00}; //little endian
#endif

int main(int argc, char *argv[]) {
#ifdef TEST
	//u32 size = bignumber_mul(bignum, 0xfa3c, 6);
	//dump_bytes(bignum, size);
	u32 sum_size = bignumber_modMul(bignum, bignum_e, bignum_N, ARRAY_SIZE(bignum), ARRAY_SIZE(bignum_e), ARRAY_SIZE(bignum_N));
	//dump_bytes(bignum, );
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

u32 bignumber_bitCmp(u8 *a, u8 *b, u32 abits, u32 bbits) {
	u32 abytes = abits / 8;
	u32 aremainder = abits % 8;
	u32 bbytes = bbits / 8;
	u32 bremainder = bbits % 8;
	u32 ta, tb;
	u32 i;
	if (bbits <= 8) {
		ta = (*(u32 *)(a+abytes)) & ((1<<(aremainder+9))-1);
	}
	ta = (*(u32 *)(a+abytes)) & ((1<<(aremainder+9))-1);
	tb = (*(u32 *)(b+bbytes)) & ((1<<(bremainder+9))-1);
	if (ta > tb) {
		return 1;
	} else if (ta < tb) {
		return -1;
	} else {
		return 0;
	}
}

u32 bignumber_bitShift(u8 *a, u8 *out, u32 abits, u32 bitShift) {

	return abits + bitShift;
}

u32 bignumber_bitShiftAdd(u8 *a, u8 *b, u32 abits, u32 bbits, u32 bitShift) {

}

u32 bignumber_bitShiftGet2Bytes(u8 *a, u32 abits, u32 bitShift) {

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
