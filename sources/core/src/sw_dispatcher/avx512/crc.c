/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT

 ******************************************************************************/

#include "../dml_kernels.h"

#if defined(_MSC_BUILD)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#else
#error "Unsupported compiler"
#endif

#define D_POLINOM_1 0x1EDC6F41

 /**
 *  @todo
 */
static const uint8_t opt_poly_1_ptr[128] = {
    0x41, 0x6f, 0xdc, 0x1e, 0x01, 0x00, 0x00, 0x00, 0xf6, 0xca, 0x91, 0x1f, 0x01, 0x00, 0x00, 0x00,
    0x76, 0x45, 0xab, 0x3a, 0x65, 0x16, 0xa0, 0xd7, 0x9e, 0x60, 0x60, 0xff, 0xba, 0xe4, 0x87, 0xe2,
    0x39, 0xa4, 0xca, 0xb8, 0x37, 0x68, 0x8c, 0xb7, 0x70, 0x28, 0x71, 0x41, 0xf2, 0xa4, 0x76, 0x3f,
    0x5c, 0xe0, 0xee, 0x81, 0x38, 0x97, 0x9b, 0xd2, 0x20, 0xb8, 0xec, 0x6e, 0x81, 0x3e, 0x3b, 0x8a,
    0x07, 0x9d, 0x39, 0xd3, 0x85, 0xd8, 0x5f, 0xcf, 0xa8, 0x36, 0x71, 0x18, 0x60, 0xb9, 0x0d, 0x92,
    0x2c, 0x93, 0xe6, 0x8a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/**
*  @todo
*/
static inline uint32_t getCRCSize(uint64_t poly)
{
    uint32_t crcSize;
    crcSize = 63 - (uint32_t)_lzcnt_u64(poly);
    return crcSize;
}

/**
*  @todo
*/
static void own_CRC_8u_opt_k0(const uint8_t* src_ptr, uint32_t init_crc, int len0, int crc_size, uint32_t* koeff_ptr, uint32_t* crc_ptr);
/**
*  @todo
*/
static void own_CRC_reflected_8u_opt_k0(const uint8_t* src_ptr, uint32_t init_crc, int len0, int crc_size, uint32_t* koeff_ptr, uint32_t* crc_ptr);


static uint64_t own_crc32_long_le_k0(const uint8_t* src_ptr, uint32_t length, uint32_t crc);

static uint32_t own_crc32_long_be_k0(const uint8_t* src_ptr, uint32_t length, uint32_t crc);
/**
*  @todo
*/
static void own_CRC_8u_k0(const uint8_t* src_ptr, uint32_t len, uint64_t poly, const uint8_t optPoly[128], uint32_t init, uint32_t* crc_ptr)
{
    if (((((uint64_t)1 << 32) | D_POLINOM_1) == poly)) {
        if (len < 1024) {
            own_CRC_8u_opt_k0(src_ptr, init, len, 32, (uint32_t*)optPoly, crc_ptr);
        }
        else {
            *crc_ptr = (uint32_t)own_crc32_long_le_k0(src_ptr, len, *crc_ptr);
        }
    }
    else {
        int crc_size = getCRCSize(poly);
        own_CRC_8u_opt_k0(src_ptr, init, len, crc_size, (uint32_t*)optPoly, crc_ptr);
    }
}
/**
*  @todo
*/
static void own_CRC_reflected_8u_k0(const uint8_t* src_ptr, uint32_t len, uint64_t poly, const uint8_t optPoly[128], uint32_t init, uint32_t* crc_ptr)
{
    if (((((uint64_t)1 << 32) | D_POLINOM_1) == poly)) {
        if (len < 1024) {
            own_CRC_reflected_8u_opt_k0(src_ptr, init, len, 32, (uint32_t*)optPoly, crc_ptr);
        }
        else {
            *crc_ptr = (uint32_t)own_crc32_long_be_k0(src_ptr, len, *crc_ptr);
        }
    }
    else {
        int crc_size = getCRCSize(poly);
        own_CRC_reflected_8u_opt_k0(src_ptr, init, len, crc_size, (uint32_t*)optPoly, crc_ptr);
    }
}

/**
*  @todo
*/
static void poly1x64_32_div(uint64_t poly, uint64_t* quit_ptr, uint32_t* tail_ptr)
{
    int      i, j;
    uint64_t tail = 0;
    uint64_t quot = 0;
    uint8_t  byte = 0x01;
    for (i = 0; i < 9; i++)
    {
        uint8_t  bit;
        uint64_t hbit;
        for (j = 0; j < 8; j++)
        {
            bit = (byte & 0x80) >> 7;
            byte <<= 1;
            hbit = tail & 0x80000000;
            tail <<= 1;
            tail |= bit;
            quot <<= 1;
            if (hbit)
            {
                tail = tail ^ poly;
                quot |= 1;
            }
            tail = tail & 0xffffffff;
        }
    }
    *quit_ptr = quot;
    *tail_ptr = (uint32_t)tail;
    return;
}

/**
*  @todo
*/
static inline void own_gen_crc_opt_poly_8u(uint64_t poly, uint8_t optPoly[128])
{
    uint64_t u;
    uint32_t i, k, crc_size;
    uint32_t t;

    crc_size        = getCRCSize(poly);
    uint32_t* opt   = (uint32_t*)optPoly;
    *(uint64_t*)opt = poly; /*copy poly*/
    uint64_t poly32 = poly << (32 - crc_size);
    poly1x64_32_div(poly32, &u, &t); /*for 1^64 and U*/
    *(uint64_t*)(opt + 2) = u;
    int      bits[]       = { 64, 96, 160, 224, 288, 352, 416, 480, 544, 608, 672, 736, 800, 864, 928, 992, 1056, 2016, 2080 };
    uint32_t tail         = 0;
    uint32_t poly_32      = (uint32_t)poly;
    int      j;

    k    = bits[0] + 8;
    tail = poly_32;
    for (j = 40; (uint32_t)j < k; j++)
    {
        uint32_t mask;
        mask = (tail & 0x80000000) ? poly_32 : 0;
        tail += tail;
        tail ^= mask;
    }
    opt[4 + 0] = (uint32_t)tail;

    for (i = 1; i < ((sizeof(bits) / sizeof(bits[0])) - 2); i++)
    {
        k = bits[i] + 8;
        for (; (uint32_t)j < k; j++)
        {
            uint32_t mask;
            mask = (tail & 0x80000000) ? poly_32 : 0;
            tail += tail;
            tail ^= mask;
        }
        opt[4 + i] = (uint32_t)tail;
    }
}

static inline void dmlc_own_calculate_crc_32u(const uint8_t* const memory_region_ptr,
    uint32_t             bytes_to_hash,
    uint32_t* const      crc_ptr,
    uint32_t             polynomial)
{
    uint64_t    poly = (uint64_t)polynomial | ((uint64_t)1u << (uint64_t)32u);

    if (D_POLINOM_1 == polynomial) {
        own_CRC_8u_k0(memory_region_ptr, bytes_to_hash, poly, opt_poly_1_ptr, *crc_ptr, crc_ptr);
    }
    else {
        uint8_t opt_poly_ptr[128];
        own_gen_crc_opt_poly_8u(poly, opt_poly_ptr);
        own_CRC_8u_k0(memory_region_ptr, bytes_to_hash, poly, opt_poly_ptr, *crc_ptr, crc_ptr);
    }
}

static inline void dmlc_own_calculate_crc_reflected_32u(const uint8_t* const memory_region_ptr,
    uint32_t             bytes_to_hash,
    uint32_t* const      crc_ptr,
    uint32_t             polynomial)
{
    uint64_t    poly = (uint64_t)polynomial | ((uint64_t)1u << (uint64_t)32u);

    if (D_POLINOM_1 == polynomial) {
        own_CRC_reflected_8u_k0(memory_region_ptr, bytes_to_hash, poly, opt_poly_1_ptr, *crc_ptr, crc_ptr);
    }
    else {
        uint8_t opt_poly_ptr[128];
        own_gen_crc_opt_poly_8u(poly, opt_poly_ptr);
        own_CRC_reflected_8u_k0(memory_region_ptr, bytes_to_hash, poly, opt_poly_ptr, *crc_ptr, crc_ptr);
    }
}

#if defined(_MSC_VER)
#pragma optimize("", off)
#pragma optimize("O3", on)
#endif

/**
*  @todo
*/
#define _MM_XOR_PS(A, B) _mm_castps_si128(_mm_xor_ps(_mm_castsi128_ps(A), _mm_castsi128_ps(B)))
/**
*  @todo
*/
#define arg1_low32 ecx

/**
*  @todo
*/
static void own_CRC_8u_opt_k0(const uint8_t* src_ptr, uint32_t init_crc, int len0, int crc_size, uint32_t* koeff, uint32_t* crc_ptr)
{
    uint64_t pshufb_shf_table[] = { 0x8786858483828100, 0x8f8e8d8c8b8a8988, 0x0706050403020100, 0x000e0d0c0b0a0908 };

    int      len = len0;
    uint8_t  ttt[128];
    uint8_t* r11 = ttt;
    uint8_t* ptr;

    __m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm12, xmm13;

    int     eax, ecx, r9;
    __m128i ENDIA_SHUF_MASK = _mm_set_epi8(0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F);
    __m128i mask2           = _mm_set_epi64x(0x00000000FFFFFFFF, 0xFFFFFFFFFFFFFFFF);
    __m128i mask1           = _mm_set_epi64x(0x8080808080808080, 0x8080808080808080);

    uint64_t q = *(uint64_t*)(koeff + 0);
    q <<= (32 - crc_size);
    uint64_t u      = *(uint64_t*)(koeff + 2);
    uint64_t k_64   = ((uint64_t)koeff[4]) << 32;
    uint64_t k_96   = ((uint64_t)koeff[5]) << 32;
    uint64_t k_160  = ((uint64_t)koeff[6]) << 32;
    uint64_t k_224  = ((uint64_t)koeff[7]) << 32;
    uint64_t k_288  = ((uint64_t)koeff[8]) << 32;
    uint64_t k_352  = ((uint64_t)koeff[9]) << 32;
    uint64_t k_416  = ((uint64_t)koeff[10]) << 32;
    uint64_t k_480  = ((uint64_t)koeff[11]) << 32;
    uint64_t k_544  = ((uint64_t)koeff[12]) << 32;
    uint64_t k_608  = ((uint64_t)koeff[13]) << 32;
    uint64_t k_672  = ((uint64_t)koeff[14]) << 32;
    uint64_t k_736  = ((uint64_t)koeff[15]) << 32;
    uint64_t k_800  = ((uint64_t)koeff[16]) << 32;
    uint64_t k_864  = ((uint64_t)koeff[17]) << 32;
    uint64_t k_928  = ((uint64_t)koeff[18]) << 32;
    uint64_t k_992  = ((uint64_t)koeff[19]) << 32;
    uint64_t k_1056 = ((uint64_t)koeff[20]) << 32;

    ecx = init_crc;
    //crc16_t10dif_01:
    ecx = ecx << (32 - crc_size);

    if (len < 256)
    {
        goto _less_than_256;
    }

    //; load the initial crc value
    xmm10 = _mm_cvtsi32_si128(arg1_low32);  //movd	xmm10, arg1_low32; initial crc
    //; crc value does not need to be byte - reflected, but it needs to be moved to the high part of the register.
    //; because data will be byte - reflected and will align with initial crc at correct place.
    xmm10 = _mm_slli_si128(xmm10, 12);
    //; receive the initial 128B data, xor the initial crc value
    xmm0 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 0));
    xmm1 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 1));
    xmm2 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 2));
    xmm3 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 3));
    xmm4 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 4));
    xmm5 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 5));
    xmm6 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 6));
    xmm7 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 7));
    xmm0 = _mm_shuffle_epi8(xmm0, ENDIA_SHUF_MASK);
    //; XOR the initial_crc value
    xmm0  = _mm_xor_si128(xmm0, xmm10);
    xmm1  = _mm_shuffle_epi8(xmm1, ENDIA_SHUF_MASK);
    xmm2  = _mm_shuffle_epi8(xmm2, ENDIA_SHUF_MASK);
    xmm3  = _mm_shuffle_epi8(xmm3, ENDIA_SHUF_MASK);
    xmm4  = _mm_shuffle_epi8(xmm4, ENDIA_SHUF_MASK);
    xmm5  = _mm_shuffle_epi8(xmm5, ENDIA_SHUF_MASK);
    xmm6  = _mm_shuffle_epi8(xmm6, ENDIA_SHUF_MASK);
    xmm7  = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm10 = _mm_set_epi64x(k_1056 /*rk4*/, k_992 /*rk3*/);
    //; imm value of pclmulqdq instruction will determine which constant to use
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //; we subtract 256 instead of 128 to save one instruction from the loop
    len -= 256;
//
//; at this section of the code, there is 128 * x + y(0 <= y < 128) bytes of buffer.The _fold_128_B_loop
//; loop will fold 128B at a time until we have 128 + y Bytes of buffer
//
//
//; fold 128B at a time.This section of the code folds 8 xmm registers in parallel
_fold_128_B_loop:
    //
    //; update the buffer pointer
    src_ptr += 128;
    xmm9  = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 0));
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 1));
    xmm9  = _mm_shuffle_epi8(xmm9, ENDIA_SHUF_MASK);
    xmm12 = _mm_shuffle_epi8(xmm12, ENDIA_SHUF_MASK);
    xmm8  = xmm0;
    xmm13 = xmm1;
    xmm0  = _mm_clmulepi64_si128(xmm0, xmm10, 0x0);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x11);
    xmm1  = _mm_clmulepi64_si128(xmm1, xmm10, 0x0);
    xmm13 = _mm_clmulepi64_si128(xmm13, xmm10, 0x11);
    xmm0  = _mm_xor_si128(xmm0, xmm9);
    xmm0  = _MM_XOR_PS(xmm0, xmm8);
    xmm1  = _mm_xor_si128(xmm1, xmm12);
    xmm1  = _MM_XOR_PS(xmm1, xmm13);
    xmm9  = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 2));
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 3));
    xmm9  = _mm_shuffle_epi8(xmm9, ENDIA_SHUF_MASK);
    xmm12 = _mm_shuffle_epi8(xmm12, ENDIA_SHUF_MASK);
    xmm8  = xmm2;
    xmm13 = xmm3;
    xmm2  = _mm_clmulepi64_si128(xmm2, xmm10, 0x0);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x11);
    xmm3  = _mm_clmulepi64_si128(xmm3, xmm10, 0x0);
    xmm13 = _mm_clmulepi64_si128(xmm13, xmm10, 0x11);
    xmm2  = _mm_xor_si128(xmm2, xmm9);
    xmm2  = _MM_XOR_PS(xmm2, xmm8);
    xmm3  = _mm_xor_si128(xmm3, xmm12);
    xmm3  = _MM_XOR_PS(xmm3, xmm13);

    xmm9  = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 4));
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 5));
    xmm9  = _mm_shuffle_epi8(xmm9, ENDIA_SHUF_MASK);
    xmm12 = _mm_shuffle_epi8(xmm12, ENDIA_SHUF_MASK);
    xmm8  = xmm4;
    xmm13 = xmm5;
    xmm4  = _mm_clmulepi64_si128(xmm4, xmm10, 0x0);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x11);
    xmm5  = _mm_clmulepi64_si128(xmm5, xmm10, 0x0);
    xmm13 = _mm_clmulepi64_si128(xmm13, xmm10, 0x11);
    xmm4  = _mm_xor_si128(xmm4, xmm9);
    xmm4  = _MM_XOR_PS(xmm4, xmm8);
    xmm5  = _mm_xor_si128(xmm5, xmm12);
    xmm5  = _MM_XOR_PS(xmm5, xmm13);

    xmm9  = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 6));
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 7));
    xmm9  = _mm_shuffle_epi8(xmm9, ENDIA_SHUF_MASK);
    xmm12 = _mm_shuffle_epi8(xmm12, ENDIA_SHUF_MASK);
    xmm8  = xmm6;
    xmm13 = xmm7;
    xmm6  = _mm_clmulepi64_si128(xmm6, xmm10, 0x0);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x11);
    xmm7  = _mm_clmulepi64_si128(xmm7, xmm10, 0x0);
    xmm13 = _mm_clmulepi64_si128(xmm13, xmm10, 0x11);
    xmm6  = _mm_xor_si128(xmm6, xmm9);
    xmm6  = _MM_XOR_PS(xmm6, xmm8);
    xmm7  = _mm_xor_si128(xmm7, xmm12);
    xmm7  = _MM_XOR_PS(xmm7, xmm13);

    len -= 128;
    //; check if there is another 128B in the buffer to be able to fold
    if (len >= 0)
        goto _fold_128_B_loop;  //jge	_fold_128_B_loop
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    src_ptr += 128;
    //; at this point, the buffer pointer is pointing at the last y Bytes of the buffer
    //; fold the 8 xmm registers to 1 xmm register with different constants
    //
    xmm10 = _mm_set_epi64x(k_928 /*rk10*/, k_864 /*rk9*/);
    xmm8  = xmm0;
    xmm0  = _mm_clmulepi64_si128(xmm0, xmm10, 0x11);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7  = _mm_xor_si128(xmm7, xmm8);
    xmm7  = _MM_XOR_PS(xmm7, xmm0);

    xmm10 = _mm_set_epi64x(k_800 /*rk12*/, k_736 /*rk11*/);
    xmm8  = xmm1;
    xmm1  = _mm_clmulepi64_si128(xmm1, xmm10, 0x11);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7  = _mm_xor_si128(xmm7, xmm8);
    xmm7  = _MM_XOR_PS(xmm7, xmm1);
    xmm10 = _mm_set_epi64x(k_672 /*rk14*/, k_608 /*rk13*/);
    xmm8  = xmm2;
    xmm2  = _mm_clmulepi64_si128(xmm2, xmm10, 0x11);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7  = _mm_xor_si128(xmm7, xmm8);
    xmm7  = _mm_xor_si128(xmm7, xmm2);
    xmm10 = _mm_set_epi64x(k_544 /*rk16*/, k_480 /*rk15*/);
    xmm8  = xmm3;
    xmm3  = _mm_clmulepi64_si128(xmm3, xmm10, 0x11);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7  = _mm_xor_si128(xmm7, xmm8);
    xmm7  = _MM_XOR_PS(xmm7, xmm3);
    xmm10 = _mm_set_epi64x(k_416 /*rk18*/, k_352 /*rk17*/);
    xmm8  = xmm4;
    xmm4  = _mm_clmulepi64_si128(xmm4, xmm10, 0x11);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7  = _mm_xor_si128(xmm7, xmm8);
    xmm7  = _mm_xor_si128(xmm7, xmm4);
    xmm10 = _mm_set_epi64x(k_288 /*rk20*/, k_224 /*rk19*/);
    xmm8  = xmm5;
    xmm5  = _mm_clmulepi64_si128(xmm5, xmm10, 0x11);
    xmm8  = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7  = _mm_xor_si128(xmm7, xmm8);
    xmm7  = _MM_XOR_PS(xmm7, xmm5);
    xmm10 = _mm_set_epi64x(k_160 /*rk2*/, k_96 /*rk1*/);
    //        ; imm value of pclmulqdq instruction will determine which constant to use
    xmm8 = xmm6;
    xmm6 = _mm_clmulepi64_si128(xmm6, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _mm_xor_si128(xmm7, xmm6);
    //
    //        ; instead of 128, we add 112 to the loop counter to save 1 instruction from the loop
    //        ; instead of a cmp instruction, we use the negative flag with the jl instruction
    len += (128 - 16);
    if (len < 0)
        goto _final_reduction_for_128;  //        jl	_final_reduction_for_128
//        ; now we have 16 + y bytes left to reduce. 16 Bytes is in register xmm7 and the rest is in memory
//        ; we can fold 16 bytes at a time if y >= 16
//        ; continue folding 16B at a time
_16B_reduction_loop:
    xmm8 = xmm7;
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm0 = _mm_loadu_si128((const __m128i*)src_ptr);
    xmm0 = _mm_shuffle_epi8(xmm0, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    src_ptr += 16;
    len -= 16;
    //        ; instead of a cmp instruction, we utilize the flags with the jge instruction
    //        ; equivalent of : cmp arg3, 16 - 16
    //        ; check if there is any more 16B in the buffer to be able to fold

    if (len >= 0)
        goto _16B_reduction_loop;  //        jge	_16B_reduction_loop
//        ; now we have 16 + z bytes left to reduce, where 0 <= z < 16.
//        ; first, we reduce the data in the xmm7 register
_final_reduction_for_128:
    //    ; check if any more data to fold.If not, compute the CRC of the final 128 bits
    len += 16;
    if (len == 0)
        goto _128_done;  //        je	_128_done
                         //        ; here we are getting data that is less than 16 bytes.
        //        ; since we know that there was data before the pointer, we can offset the input pointer before the actual point, to receive exactly 16 bytes.
        //        ; after that the registers need to be adjusted.

_get_last_two_xmms:
    xmm2 = xmm7;
    xmm1 = _mm_loadu_si128((const __m128i*)(src_ptr - 16 + len));
    xmm1 = _mm_shuffle_epi8(xmm1, ENDIA_SHUF_MASK);
    //        ; get rid of the extra data that was loaded before
    //        ; load the shift constant
    //        lea	rax, [pshufb_shf_table + 16]
    //        sub	rax, arg3
    ptr  = (uint8_t*)pshufb_shf_table + 16 - len;
    xmm0 = _mm_loadu_si128((const __m128i*)ptr);
    //
    //        ; shift xmm2 to the left by arg3 bytes
    xmm2 = _mm_shuffle_epi8(xmm2, xmm0);
    xmm0 = _mm_xor_si128(xmm0, mask1);
    xmm7 = _mm_shuffle_epi8(xmm7, xmm0);
    xmm1 = _mm_blendv_epi8(xmm1, xmm2, xmm0);
    //        ; fold 16 Bytes
    xmm2 = xmm1;
    xmm8 = xmm7;
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _mm_xor_si128(xmm7, xmm2);
_128_done:
    //    ; compute crc of a 128 - bit value
    xmm10 = _mm_set_epi64x(k_64 /*rk6*/, k_96 /*rk5*/);
    xmm0  = xmm7;
    //        ; 64b fold
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x1);
    xmm0 = _mm_slli_si128(xmm0, 8);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    //        ; 32b fold
    xmm0 = xmm7;
    xmm0 = _mm_and_si128(xmm0, mask2);
    xmm7 = _mm_srli_si128(xmm7, 12);
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x10);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
//        ; barrett reduction
_barrett:
    xmm10 = _mm_set_epi64x(q /*rk8*/, u /*rk7*/);
    xmm0  = xmm7;
    xmm7  = _mm_clmulepi64_si128(xmm7, xmm10, 0x01);
    xmm7  = _mm_slli_si128(xmm7, 4);
    xmm7  = _mm_clmulepi64_si128(xmm7, xmm10, 0x11);

    xmm7 = _mm_slli_si128(xmm7, 4);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    eax  = _mm_extract_epi32(xmm7, 1);

_cleanup:
    //    ; scale the result back to 16 bits
    eax      = ((uint32_t)eax) >> (32 - crc_size);
    *crc_ptr = eax;
    return;  //        ret

    //align 16

_less_than_256:
    //
    //; check if there is enough buffer to be able to fold 16B at a time
    //cmp	arg3, 32
    //jl	_less_than_32
    if (len < 32)
    {
        goto _less_than_32;
    }
    //; if there is, load the constants
    xmm10 = _mm_set_epi64x(k_160 /*rk2*/, k_96 /*rk1*/);
    xmm0  = _mm_cvtsi32_si128(arg1_low32);
    xmm0  = _mm_slli_si128(xmm0, 12);
    xmm7  = _mm_loadu_si128((const __m128i*)src_ptr);
    xmm7  = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7  = _mm_xor_si128(xmm7, xmm0);
    //
    //; update the buffer pointer
    src_ptr += 16;
    //
    //; update the counter.subtract 32 instead of 16 to save one instruction from the loop
    len -= 32;
    goto _16B_reduction_loop;  //jmp	_16B_reduction_loop

//align 16
_less_than_32:
    //; mov initial crc to the return value. this is necessary for zero - length buffers.
    eax = arg1_low32;  //mov	eax, arg1_low32
    //test	arg3, arg3
    if (len == 0)
        goto _cleanup;  //je	_cleanup
    //

    xmm0 = _mm_cvtsi32_si128(arg1_low32);
    xmm0 = _mm_slli_si128(xmm0, 12);
    //cmp	arg3, 16
    if (len == 16)
        goto _exact_16_left;  //je	_exact_16_left
    if (len < 16)
        goto _less_than_16_left;  //jl	_less_than_16_left

    xmm7 = _mm_loadu_si128((const __m128i*)src_ptr);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    src_ptr += 16;
    len -= 16;
    xmm10 = _mm_set_epi64x(k_160 /*rk2*/, k_96 /*rk1*/);
    goto _get_last_two_xmms;  //jmp	_get_last_two_xmms
//align 16
_less_than_16_left:
    //; use stack space to load data less than 16 bytes, zero - out the 16B in memory first.
    //
    xmm1 = _mm_setzero_si128();

    _mm_storeu_si128((__m128i*)r11, xmm1);
    //cmp	arg3, 4
    if (len < 4)
        goto _only_less_than_4;  //jl	_only_less_than_4
    //;	backup the counter value
    r9 = len;
    //cmp	arg3, 8
    if (len < 8)
        goto _less_than_8_left;  //jl	_less_than_8_left
    //; load 8 Bytes
    *(int64_t*)r11 = *(int64_t*)src_ptr;
    r11 += 8;
    len -= 8;
    src_ptr += 8;
_less_than_8_left:
    //cmp	arg3, 4
    if (len < 4)
        goto _less_than_4_left;  //jl	_less_than_4_left
    //; load 4 Bytes
    *(int*)r11 = *(int*)src_ptr;
    r11 += 4;
    len -= 4;
    src_ptr += 4;  //add	arg2, 4
_less_than_4_left:
    //
    //cmp	arg3, 2
    if (len < 2)
        goto _less_than_2_left;  //jl	_less_than_2_left
    //
    //; load 2 Bytes
    *(short*)r11 = *(short*)src_ptr;
    r11 += 2;
    len -= 2;
    src_ptr += 2;
_less_than_2_left:
    //cmp     arg3, 1
    if (len < 1)
        goto _zero_left;  //jl      _zero_left
    //; load 1 Byte
    *r11 = *src_ptr;
_zero_left:
    xmm7 = _mm_loadu_si128((const __m128i*)ttt);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);

    ptr  = (uint8_t*)pshufb_shf_table + 16 - r9;
    xmm0 = _mm_loadu_si128((const __m128i*)ptr);
    xmm0 = _mm_xor_si128(xmm0, mask1);
    //
    xmm7 = _mm_shuffle_epi8(xmm7, xmm0);
    goto _128_done;  //jmp	_128_done
    //align 16

_exact_16_left:
    xmm7 = _mm_loadu_si128((const __m128i*)src_ptr);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);

    goto _128_done;  //jmp	_128_done
_only_less_than_4:
    //cmp	arg3, 3
    if (len < 3)
        goto _only_less_than_3;  //jl	_only_less_than_3
    //; load 3 Bytes
    r11[0] = src_ptr[0];

    r11[1] = src_ptr[1];

    r11[2] = src_ptr[2];
    xmm7   = _mm_loadu_si128((const __m128i*)r11);
    xmm7   = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7   = _mm_xor_si128(xmm7, xmm0);
    xmm7   = _mm_srli_si128(xmm7, 5);
    goto _barrett;  //jmp	_barrett
_only_less_than_3:
    //cmp	arg3, 2
    if (len < 2)
        goto _only_less_than_2;  //jl	_only_less_than_2
    //; load 2 Bytes
    r11[0] = src_ptr[0];

    r11[1] = src_ptr[1];
    xmm7   = _mm_loadu_si128((const __m128i*)r11);
    xmm7   = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7   = _mm_xor_si128(xmm7, xmm0);
    xmm7   = _mm_srli_si128(xmm7, 6);
    goto _barrett;  //jmp	_barrett
_only_less_than_2:
    //
    //; load 1 Byte
    eax    = src_ptr[0];
    r11[0] = eax;

    xmm7 = _mm_loadu_si128((const __m128i*)r11);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);

    xmm7 = _mm_srli_si128(xmm7, 7);

    goto _barrett;  //jmp	_barrett
}

#define D_REFL(xmmREFL) \
    xmm11 = _mm_srli_epi16(xmmREFL, 4); \
    xmmREFL = _mm_and_si128(xmmREFL, xmm_F); \
    xmm11 = _mm_and_si128(xmm11, xmm_F); \
    xmmREFL = _mm_shuffle_epi8(xmmSH_L, xmmREFL); \
    xmm11 = _mm_shuffle_epi8(xmmSH_H, xmm11); \
    xmmREFL = _mm_or_si128(xmmREFL, xmm11);

/**
*  @todo
*/
static void own_CRC_reflected_8u_opt_k0(const uint8_t* src_ptr, uint32_t init_crc, int len0, int crc_size, uint32_t* koeff, uint32_t* crc_ptr)
{
    uint64_t pshufb_shf_table[] = {0x8786858483828100, 0x8f8e8d8c8b8a8988, 0x0706050403020100, 0x000e0d0c0b0a0908};

    __m128i xmmSH_L = _mm_set_epi8((char)0xf0, (char)0x70, (char)0xb0, (char)0x30,
                                   (char)0xd0, (char)0x50, (char)0x90, (char)0x10,
                                   (char)0xe0, (char)0x60, (char)0xa0, (char)0x20,
                                   (char)0xc0, (char)0x40, (char)0x80, (char)0x00);
    __m128i xmmSH_H = _mm_set_epi8(0x0f, 0x07, 0x0b, 0x03, 0x0d, 0x05, 0x09, 0x01, 0x0e, 0x06, 0x0a, 0x02, 0x0c, 0x04, 0x08, 0x00);
    __m128i xmm_F = _mm_set1_epi8(0x0f);

    int      len = len0;
    uint8_t  ttt[128];
    uint8_t* r11 = ttt;
    uint8_t* ptr;

    __m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm12, xmm13;
    __m128i xmm11;

    int     eax, ecx, r9;
    __m128i ENDIA_SHUF_MASK = _mm_set_epi8(0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F);
    __m128i mask2 = _mm_set_epi64x(0x00000000FFFFFFFF, 0xFFFFFFFFFFFFFFFF);
    __m128i mask1 = _mm_set_epi64x(0x8080808080808080, 0x8080808080808080);

    uint64_t q = *(uint64_t*)(koeff + 0);
    q <<= (32 - crc_size);
    uint64_t u = *(uint64_t*)(koeff + 2);
    uint64_t k_64 = ((uint64_t)koeff[4]) << 32;
    uint64_t k_96 = ((uint64_t)koeff[5]) << 32;
    uint64_t k_160 = ((uint64_t)koeff[6]) << 32;
    uint64_t k_224 = ((uint64_t)koeff[7]) << 32;
    uint64_t k_288 = ((uint64_t)koeff[8]) << 32;
    uint64_t k_352 = ((uint64_t)koeff[9]) << 32;
    uint64_t k_416 = ((uint64_t)koeff[10]) << 32;
    uint64_t k_480 = ((uint64_t)koeff[11]) << 32;
    uint64_t k_544 = ((uint64_t)koeff[12]) << 32;
    uint64_t k_608 = ((uint64_t)koeff[13]) << 32;
    uint64_t k_672 = ((uint64_t)koeff[14]) << 32;
    uint64_t k_736 = ((uint64_t)koeff[15]) << 32;
    uint64_t k_800 = ((uint64_t)koeff[16]) << 32;
    uint64_t k_864 = ((uint64_t)koeff[17]) << 32;
    uint64_t k_928 = ((uint64_t)koeff[18]) << 32;
    uint64_t k_992 = ((uint64_t)koeff[19]) << 32;
    uint64_t k_1056 = ((uint64_t)koeff[20]) << 32;

    ecx = init_crc;
    //crc16_t10dif_01:
    ecx = ecx << (32 - crc_size);

    if (len < 256)
    {
        goto _less_than_256;
    }
    //; load the initial crc value
    xmm10 = _mm_cvtsi32_si128(arg1_low32);  //movd	xmm10, arg1_low32; initial crc
    //; crc value does not need to be byte - reflected, but it needs to be moved to the high part of the register.
    //; because data will be byte - reflected and will align with initial crc at correct place.
    xmm10 = _mm_slli_si128(xmm10, 12);
    //; receive the initial 128B data, xor the initial crc value
    xmm0 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 0));
    D_REFL(xmm0)
    xmm1 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 1));
    D_REFL(xmm1)
    xmm2 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 2));
    D_REFL(xmm2)
    xmm3 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 3));
    D_REFL(xmm3)
    xmm4 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 4));
    D_REFL(xmm4)
    xmm5 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 5));
    D_REFL(xmm5)
    xmm6 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 6));
    D_REFL(xmm6)
    xmm7 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 7));
    D_REFL(xmm7)
    xmm0 = _mm_shuffle_epi8(xmm0, ENDIA_SHUF_MASK);
    //; XOR the initial_crc value
    xmm0 = _mm_xor_si128(xmm0, xmm10);
    xmm1 = _mm_shuffle_epi8(xmm1, ENDIA_SHUF_MASK);
    xmm2 = _mm_shuffle_epi8(xmm2, ENDIA_SHUF_MASK);
    xmm3 = _mm_shuffle_epi8(xmm3, ENDIA_SHUF_MASK);
    xmm4 = _mm_shuffle_epi8(xmm4, ENDIA_SHUF_MASK);
    xmm5 = _mm_shuffle_epi8(xmm5, ENDIA_SHUF_MASK);
    xmm6 = _mm_shuffle_epi8(xmm6, ENDIA_SHUF_MASK);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm10 = _mm_set_epi64x(k_1056 /*rk4*/, k_992 /*rk3*/);
    //; imm value of pclmulqdq instruction will determine which constant to use
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //; we subtract 256 instead of 128 to save one instruction from the loop
    len -= 256;
    //
    //; at this section of the code, there is 128 * x + y(0 <= y < 128) bytes of buffer.The _fold_128_B_loop
    //; loop will fold 128B at a time until we have 128 + y Bytes of buffer
    //
    //
    //; fold 128B at a time.This section of the code folds 8 xmm registers in parallel
_fold_128_B_loop:
    //
    //; update the buffer pointer
    src_ptr += 128;
    xmm9 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 0));
    D_REFL(xmm9)
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 1));
    D_REFL(xmm12)
    xmm9 = _mm_shuffle_epi8(xmm9, ENDIA_SHUF_MASK);
    xmm12 = _mm_shuffle_epi8(xmm12, ENDIA_SHUF_MASK);
    xmm8 = xmm0;
    xmm13 = xmm1;
    xmm0 = _mm_clmulepi64_si128(xmm0, xmm10, 0x0);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x11);
    xmm1 = _mm_clmulepi64_si128(xmm1, xmm10, 0x0);
    xmm13 = _mm_clmulepi64_si128(xmm13, xmm10, 0x11);
    xmm0 = _mm_xor_si128(xmm0, xmm9);
    xmm0 = _MM_XOR_PS(xmm0, xmm8);
    xmm1 = _mm_xor_si128(xmm1, xmm12);
    xmm1 = _MM_XOR_PS(xmm1, xmm13);
    xmm9 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 2));
    D_REFL(xmm9)
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 3));
    D_REFL(xmm12)
    xmm9 = _mm_shuffle_epi8(xmm9, ENDIA_SHUF_MASK);
    xmm12 = _mm_shuffle_epi8(xmm12, ENDIA_SHUF_MASK);
    xmm8 = xmm2;
    xmm13 = xmm3;
    xmm2 = _mm_clmulepi64_si128(xmm2, xmm10, 0x0);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x11);
    xmm3 = _mm_clmulepi64_si128(xmm3, xmm10, 0x0);
    xmm13 = _mm_clmulepi64_si128(xmm13, xmm10, 0x11);
    xmm2 = _mm_xor_si128(xmm2, xmm9);
    xmm2 = _MM_XOR_PS(xmm2, xmm8);
    xmm3 = _mm_xor_si128(xmm3, xmm12);
    xmm3 = _MM_XOR_PS(xmm3, xmm13);

    xmm9 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 4));
    D_REFL(xmm9)
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 5));
    D_REFL(xmm12)
    xmm9 = _mm_shuffle_epi8(xmm9, ENDIA_SHUF_MASK);
    xmm12 = _mm_shuffle_epi8(xmm12, ENDIA_SHUF_MASK);
    xmm8 = xmm4;
    xmm13 = xmm5;
    xmm4 = _mm_clmulepi64_si128(xmm4, xmm10, 0x0);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x11);
    xmm5 = _mm_clmulepi64_si128(xmm5, xmm10, 0x0);
    xmm13 = _mm_clmulepi64_si128(xmm13, xmm10, 0x11);
    xmm4 = _mm_xor_si128(xmm4, xmm9);
    xmm4 = _MM_XOR_PS(xmm4, xmm8);
    xmm5 = _mm_xor_si128(xmm5, xmm12);
    xmm5 = _MM_XOR_PS(xmm5, xmm13);

    xmm9 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 6));
    D_REFL(xmm9)
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 7));
    D_REFL(xmm12)
    xmm9 = _mm_shuffle_epi8(xmm9, ENDIA_SHUF_MASK);
    xmm12 = _mm_shuffle_epi8(xmm12, ENDIA_SHUF_MASK);
    xmm8 = xmm6;
    xmm13 = xmm7;
    xmm6 = _mm_clmulepi64_si128(xmm6, xmm10, 0x0);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x11);
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x0);
    xmm13 = _mm_clmulepi64_si128(xmm13, xmm10, 0x11);
    xmm6 = _mm_xor_si128(xmm6, xmm9);
    xmm6 = _MM_XOR_PS(xmm6, xmm8);
    xmm7 = _mm_xor_si128(xmm7, xmm12);
    xmm7 = _MM_XOR_PS(xmm7, xmm13);

    len -= 128;
    //; check if there is another 128B in the buffer to be able to fold
    if (len >= 0)
        goto _fold_128_B_loop;  //jge	_fold_128_B_loop
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    src_ptr += 128;
    //; at this point, the buffer pointer is pointing at the last y Bytes of the buffer
    //; fold the 8 xmm registers to 1 xmm register with different constants
    //
    xmm10 = _mm_set_epi64x(k_928 /*rk10*/, k_864 /*rk9*/);
    xmm8 = xmm0;
    xmm0 = _mm_clmulepi64_si128(xmm0, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _MM_XOR_PS(xmm7, xmm0);

    xmm10 = _mm_set_epi64x(k_800 /*rk12*/, k_736 /*rk11*/);
    xmm8 = xmm1;
    xmm1 = _mm_clmulepi64_si128(xmm1, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _MM_XOR_PS(xmm7, xmm1);
    xmm10 = _mm_set_epi64x(k_672 /*rk14*/, k_608 /*rk13*/);
    xmm8 = xmm2;
    xmm2 = _mm_clmulepi64_si128(xmm2, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _mm_xor_si128(xmm7, xmm2);
    xmm10 = _mm_set_epi64x(k_544 /*rk16*/, k_480 /*rk15*/);
    xmm8 = xmm3;
    xmm3 = _mm_clmulepi64_si128(xmm3, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _MM_XOR_PS(xmm7, xmm3);
    xmm10 = _mm_set_epi64x(k_416 /*rk18*/, k_352 /*rk17*/);
    xmm8 = xmm4;
    xmm4 = _mm_clmulepi64_si128(xmm4, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _mm_xor_si128(xmm7, xmm4);
    xmm10 = _mm_set_epi64x(k_288 /*rk20*/, k_224 /*rk19*/);
    xmm8 = xmm5;
    xmm5 = _mm_clmulepi64_si128(xmm5, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _MM_XOR_PS(xmm7, xmm5);
    xmm10 = _mm_set_epi64x(k_160 /*rk2*/, k_96 /*rk1*/);
    //        ; imm value of pclmulqdq instruction will determine which constant to use
    xmm8 = xmm6;
    xmm6 = _mm_clmulepi64_si128(xmm6, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _mm_xor_si128(xmm7, xmm6);
    //
    //        ; instead of 128, we add 112 to the loop counter to save 1 instruction from the loop
    //        ; instead of a cmp instruction, we use the negative flag with the jl instruction
    len += (128 - 16);
    if (len < 0)
        goto _final_reduction_for_128;  //        jl	_final_reduction_for_128
//        ; now we have 16 + y bytes left to reduce. 16 Bytes is in register xmm7 and the rest is in memory
//        ; we can fold 16 bytes at a time if y >= 16
//        ; continue folding 16B at a time
_16B_reduction_loop:
    xmm8 = xmm7;
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm0 = _mm_loadu_si128((const __m128i*)src_ptr);
    D_REFL(xmm0)
    xmm0 = _mm_shuffle_epi8(xmm0, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    src_ptr += 16;
    len -= 16;
    //        ; instead of a cmp instruction, we utilize the flags with the jge instruction
    //        ; equivalent of : cmp arg3, 16 - 16
    //        ; check if there is any more 16B in the buffer to be able to fold

    if (len >= 0)
        goto _16B_reduction_loop;  //        jge	_16B_reduction_loop
//        ; now we have 16 + z bytes left to reduce, where 0 <= z < 16.
//        ; first, we reduce the data in the xmm7 register
_final_reduction_for_128:
    //    ; check if any more data to fold.If not, compute the CRC of the final 128 bits
    len += 16;
    if (len == 0)
        goto _128_done;  //        je	_128_done
                         //        ; here we are getting data that is less than 16 bytes.
        //        ; since we know that there was data before the pointer, we can offset the input pointer before the actual point, to receive exactly 16 bytes.
        //        ; after that the registers need to be adjusted.

    _get_last_two_xmms :
    xmm2 = xmm7;
    xmm1 = _mm_loadu_si128((const __m128i*)(src_ptr - 16 + len));
    D_REFL(xmm1)
    xmm1 = _mm_shuffle_epi8(xmm1, ENDIA_SHUF_MASK);
    //        ; get rid of the extra data that was loaded before
    //        ; load the shift constant
    //        lea	rax, [pshufb_shf_table + 16]
    //        sub	rax, arg3
    ptr = (uint8_t*)pshufb_shf_table + 16 - len;
    xmm0 = _mm_loadu_si128((const __m128i*)ptr);
    //
    //        ; shift xmm2 to the left by arg3 bytes
    xmm2 = _mm_shuffle_epi8(xmm2, xmm0);
    xmm0 = _mm_xor_si128(xmm0, mask1);
    xmm7 = _mm_shuffle_epi8(xmm7, xmm0);
    xmm1 = _mm_blendv_epi8(xmm1, xmm2, xmm0);
    //        ; fold 16 Bytes
    xmm2 = xmm1;
    xmm8 = xmm7;
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _mm_xor_si128(xmm7, xmm2);
_128_done:
    //    ; compute crc of a 128 - bit value
    xmm10 = _mm_set_epi64x(k_64 /*rk6*/, k_96 /*rk5*/);
    xmm0 = xmm7;
    //        ; 64b fold
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x1);
    xmm0 = _mm_slli_si128(xmm0, 8);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    //        ; 32b fold
    xmm0 = xmm7;
    xmm0 = _mm_and_si128(xmm0, mask2);
    xmm7 = _mm_srli_si128(xmm7, 12);
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x10);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    //        ; barrett reduction
_barrett:
    xmm10 = _mm_set_epi64x(q /*rk8*/, u /*rk7*/);
    xmm0 = xmm7;
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x01);
    xmm7 = _mm_slli_si128(xmm7, 4);
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x11);

    xmm7 = _mm_slli_si128(xmm7, 4);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    eax = _mm_extract_epi32(xmm7, 1);

    _cleanup :
    //    ; scale the result back to 16 bits
    eax = ((uint32_t)eax) >> (32 - crc_size);
    *crc_ptr = eax;
    return;  //        ret

    //align 16

    _less_than_256 :
    //
    //; check if there is enough buffer to be able to fold 16B at a time
    //cmp	arg3, 32
    //jl	_less_than_32
    if (len < 32)
    {
        goto _less_than_32;
    }
    //; if there is, load the constants
    xmm10 = _mm_set_epi64x(k_160 /*rk2*/, k_96 /*rk1*/);
    xmm0 = _mm_cvtsi32_si128(arg1_low32);
    xmm0 = _mm_slli_si128(xmm0, 12);
    xmm7 = _mm_loadu_si128((const __m128i*)src_ptr);
    D_REFL(xmm7)
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    //
    //; update the buffer pointer
    src_ptr += 16;
    //
    //; update the counter.subtract 32 instead of 16 to save one instruction from the loop
    len -= 32;
    goto _16B_reduction_loop;  //jmp	_16B_reduction_loop

//align 16
_less_than_32:
    //; mov initial crc to the return value. this is necessary for zero - length buffers.
    eax = arg1_low32;  //mov	eax, arg1_low32
    //test	arg3, arg3
    if (len == 0)
        goto _cleanup;  //je	_cleanup
    //

    xmm0 = _mm_cvtsi32_si128(arg1_low32);
    xmm0 = _mm_slli_si128(xmm0, 12);
    //cmp	arg3, 16
    if (len == 16)
        goto _exact_16_left;  //je	_exact_16_left
    if (len < 16)
        goto _less_than_16_left;  //jl	_less_than_16_left

    xmm7 = _mm_loadu_si128((const __m128i*)src_ptr);
    D_REFL(xmm7)
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    src_ptr += 16;
    len -= 16;
    xmm10 = _mm_set_epi64x(k_160 /*rk2*/, k_96 /*rk1*/);
    goto _get_last_two_xmms;  //jmp	_get_last_two_xmms
//align 16
_less_than_16_left:
    //; use stack space to load data less than 16 bytes, zero - out the 16B in memory first.
    //
    xmm1 = _mm_setzero_si128();

    _mm_storeu_si128((__m128i*)r11, xmm1);
    //cmp	arg3, 4
    if (len < 4)
        goto _only_less_than_4;  //jl	_only_less_than_4
    //;	backup the counter value
    r9 = len;
    //cmp	arg3, 8
    if (len < 8)
        goto _less_than_8_left;  //jl	_less_than_8_left
    //; load 8 Bytes
    *(int64_t*)r11 = *(int64_t*)src_ptr;
    r11 += 8;
    len -= 8;
    src_ptr += 8;
_less_than_8_left:
    //cmp	arg3, 4
    if (len < 4)
        goto _less_than_4_left;  //jl	_less_than_4_left
    //; load 4 Bytes
    *(int*)r11 = *(int*)src_ptr;
    r11 += 4;
    len -= 4;
    src_ptr += 4;  //add	arg2, 4
_less_than_4_left:
    //
    //cmp	arg3, 2
    if (len < 2)
        goto _less_than_2_left;  //jl	_less_than_2_left
    //
    //; load 2 Bytes
    *(short*)r11 = *(short*)src_ptr;
    r11 += 2;
    len -= 2;
    src_ptr += 2;
_less_than_2_left:
    //cmp     arg3, 1
    if (len < 1)
        goto _zero_left;  //jl      _zero_left
    //; load 1 Byte
    *r11 = *src_ptr;
_zero_left:
    xmm7 = _mm_loadu_si128((const __m128i*)ttt);
    D_REFL(xmm7)
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);

    ptr = (uint8_t*)pshufb_shf_table + 16 - r9;
    xmm0 = _mm_loadu_si128((const __m128i*)ptr);
    xmm0 = _mm_xor_si128(xmm0, mask1);
    //
    xmm7 = _mm_shuffle_epi8(xmm7, xmm0);
    goto _128_done;  //jmp	_128_done
    //align 16

    _exact_16_left :
    xmm7 = _mm_loadu_si128((const __m128i*)src_ptr);
    D_REFL(xmm7)
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);

    goto _128_done;  //jmp	_128_done
_only_less_than_4:
    //cmp	arg3, 3
    if (len < 3)
        goto _only_less_than_3;  //jl	_only_less_than_3
    //; load 3 Bytes
    r11[0] = src_ptr[0];

    r11[1] = src_ptr[1];

    r11[2] = src_ptr[2];
    xmm7 = _mm_loadu_si128((const __m128i*)r11);
    D_REFL(xmm7)
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    xmm7 = _mm_srli_si128(xmm7, 5);
    goto _barrett;  //jmp	_barrett
_only_less_than_3:
    //cmp	arg3, 2
    if (len < 2)
        goto _only_less_than_2;  //jl	_only_less_than_2
    //; load 2 Bytes
    r11[0] = src_ptr[0];

    r11[1] = src_ptr[1];
    xmm7 = _mm_loadu_si128((const __m128i*)r11);
    D_REFL(xmm7)
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    xmm7 = _mm_srli_si128(xmm7, 6);
    goto _barrett;  //jmp	_barrett
_only_less_than_2:
    //
    //; load 1 Byte
    eax = src_ptr[0];
    r11[0] = eax;

    xmm7 = _mm_loadu_si128((const __m128i*)r11);
    D_REFL(xmm7)
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);

    xmm7 = _mm_srli_si128(xmm7, 7);

    goto _barrett;  //jmp	_barrett
}

/*********************************/
static void own_shift_two_lanes(int offset, __m128i* _xmm0, __m128i* _xmm1) {
    __m128i xmm0 = *_xmm0;
    __m128i xmm1 = *_xmm1;

    switch (offset) {
    case 15:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 1);
        xmm1 = _mm_srli_si128(xmm1, 1);
        break;
    case 14:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 2);
        xmm1 = _mm_srli_si128(xmm1, 2);
        break;
    case 13:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 3);
        xmm1 = _mm_srli_si128(xmm1, 3);
        break;
    case 12:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 4);
        xmm1 = _mm_srli_si128(xmm1, 4);
        break;
    case 11:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 5);
        xmm1 = _mm_srli_si128(xmm1, 5);
        break;
    case 10:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 6);
        xmm1 = _mm_srli_si128(xmm1, 6);
        break;
    case 9:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 7);
        xmm1 = _mm_srli_si128(xmm1, 7);
        break;
    case 8:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 8);
        xmm1 = _mm_srli_si128(xmm1, 8);
        break;
    case 7:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 9);
        xmm1 = _mm_srli_si128(xmm1, 9);
        break;
    case 6:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 10);
        xmm1 = _mm_srli_si128(xmm1, 10);
        break;
    case 5:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 11);
        xmm1 = _mm_srli_si128(xmm1, 11);
        break;
    case 4:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 12);
        xmm1 = _mm_srli_si128(xmm1, 12);
        break;
    case 3:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 13);
        xmm1 = _mm_srli_si128(xmm1, 13);
        break;
    case 2:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 14);
        xmm1 = _mm_srli_si128(xmm1, 14);
        break;
    case 1:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 15);
        xmm1 = _mm_srli_si128(xmm1, 15);
        break;
    default:
        xmm0 = _mm_alignr_epi8(xmm1, xmm0, 16);
        xmm1 = _mm_srli_si128(xmm1, 16);
    }

    *_xmm0 = xmm0;
    *_xmm1 = xmm1;
}

#if defined(_MSC_VER)
#pragma optimize("", off)
#endif
static uint64_t own_crc32_long_le_k0(const uint8_t* src_ptr, uint32_t length, uint32_t crc) {
    const uint32_t polynomial = D_POLINOM_1;
    if (length >= 16u) {
        __m128i     xmm0, xmm1, xmm2;
        __m128i     xCRC, k_koef;
        __m128i shuffle_le_mask = _mm_set_epi8(
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F);
        __m128i mask2 = _mm_set_epi64x(0x00000000FFFFFFFF, 0xFFFFFFFFFFFFFFFF);

                                                        /* For poly = 0x11edc6f41           */
        uint64_t k_q = 0x11edc6f41;                     /* k_q = P(x)                       */
        uint64_t k_u = 0x11f91caf6;                     /* k_u = (x**64)/P(x)               */
        uint64_t k_64 = (uint64_t)0x3aab4576 << 32;     /* k_64 = (x**64) mod P(x)          */
        uint64_t k_96 = (uint64_t)0xd7a01665 << 32;     /* k_96 = (x**96) mod P(x)          */
        uint64_t k_128 = (uint64_t)0xd7a01665 << 32 ;   /* k_128 = (x**(128-32)) mod P(x)   */
        uint64_t k_192 = (uint64_t)0xff60609e << 32;    /* k_192 = (x**(192-32)) mod P(x)   */

        uint32_t tail = length & 0xf;

        xCRC = _mm_cvtsi32_si128(crc);
        xCRC = _mm_slli_si128(xCRC, 12);

        xmm0 = _mm_loadu_si128((const __m128i*)src_ptr);
        src_ptr += 16u;
        xmm0 = _mm_shuffle_epi8(xmm0, shuffle_le_mask);
        xmm0 = _mm_xor_si128(xmm0, xCRC);

        // 1. fold by 512bit until remaining length < 2 * 512bits.

        if (length > 512u) {
            __m128i xmm3, xmm4, xmm5, xmm6, xmm7;
            __m128i srcmm0, srcmm1, srcmm2, srcmm3;
                                                            /* For poly = 0x11edc6f41       */
            uint64_t k_512 = (uint64_t)0x3f76a4f2 << 32;    /* k_512 = (x**512) mod P(x)    */
            uint64_t k_576 = (uint64_t)0x81eee05c << 32;    /* k_574 = (x**576) mod P(x)    */
            k_koef = _mm_set_epi64x(k_576, k_512);

            xmm2 = _mm_loadu_si128((const __m128i*)src_ptr);
            xmm2 = _mm_shuffle_epi8(xmm2, shuffle_le_mask);
            xmm4 = _mm_loadu_si128((const __m128i*)(src_ptr + 16u));
            xmm4 = _mm_shuffle_epi8(xmm4, shuffle_le_mask);
            xmm6 = _mm_loadu_si128((const __m128i*)(src_ptr + 32u));
            xmm6 = _mm_shuffle_epi8(xmm6, shuffle_le_mask);
            src_ptr += 48u;

            while (length >= 128u) {
                srcmm0 = _mm_loadu_si128((const __m128i*)src_ptr);
                srcmm0 = _mm_shuffle_epi8(srcmm0, shuffle_le_mask);
                srcmm1 = _mm_loadu_si128((const __m128i*)(src_ptr + 16u));
                srcmm1 = _mm_shuffle_epi8(srcmm1, shuffle_le_mask);
                srcmm2 = _mm_loadu_si128((const __m128i*)(src_ptr + 32u));
                srcmm2 = _mm_shuffle_epi8(srcmm2, shuffle_le_mask);
                srcmm3 = _mm_loadu_si128((const __m128i*)(src_ptr + 48u));
                srcmm3 = _mm_shuffle_epi8(srcmm3, shuffle_le_mask);

                xmm1 = xmm0;
                xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x00);
                xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x11);
                xmm0 = _mm_xor_si128(xmm0, srcmm0);
                xmm0 = _mm_xor_si128(xmm0, xmm1);

                xmm3 = xmm2;
                xmm2 = _mm_clmulepi64_si128(xmm2, k_koef, 0x00);
                xmm3 = _mm_clmulepi64_si128(xmm3, k_koef, 0x11);
                xmm2 = _mm_xor_si128(xmm2, srcmm1);
                xmm2 = _mm_xor_si128(xmm2, xmm3);

                xmm5 = xmm4;
                xmm4 = _mm_clmulepi64_si128(xmm4, k_koef, 0x00);
                xmm5 = _mm_clmulepi64_si128(xmm5, k_koef, 0x11);
                xmm4 = _mm_xor_si128(xmm4, srcmm2);
                xmm4 = _mm_xor_si128(xmm4, xmm5);

                xmm7 = xmm6;
                xmm6 = _mm_clmulepi64_si128(xmm6, k_koef, 0x00);
                xmm7 = _mm_clmulepi64_si128(xmm7, k_koef, 0x11);
                xmm6 = _mm_xor_si128(xmm6, srcmm3);
                xmm6 = _mm_xor_si128(xmm6, xmm7);

                src_ptr += 64u;
                length -= 64u;
            }

            k_koef = _mm_set_epi64x(k_192, k_128);

            xmm1 = xmm0;
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x00);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x11);
            xmm0 = _mm_xor_si128(xmm0, xmm2);
            xmm0 = _mm_xor_si128(xmm0, xmm1);

            xmm1 = xmm0;
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x00);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x11);
            xmm0 = _mm_xor_si128(xmm0, xmm4);
            xmm0 = _mm_xor_si128(xmm0, xmm1);

            xmm1 = xmm0;
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x00);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x11);
            xmm0 = _mm_xor_si128(xmm0, xmm6);
            xmm0 = _mm_xor_si128(xmm0, xmm1);

            length -= 48u;
        }

        k_koef = _mm_set_epi64x(k_192, k_128);

        while (length >= 32u) {
            xmm1 = xmm0;
            xmm2 = _mm_loadu_si128((const __m128i*)src_ptr);
            xmm2 = _mm_shuffle_epi8(xmm2, shuffle_le_mask);
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x00);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x11);
            xmm0 = _mm_xor_si128(xmm0, xmm2);
            xmm0 = _mm_xor_si128(xmm0, xmm1);
            src_ptr += 16u;
            length -= 16u;
        }

        if (tail) {
            __mmask16 tail_mask = (1 << tail) - 1;

            xmm2 = _mm_maskz_loadu_epi8(tail_mask, (__m128i*)src_ptr);
            xmm2 = _mm_shuffle_epi8(xmm2, shuffle_le_mask);

            own_shift_two_lanes(tail, &xmm2, &xmm0);

            xmm1 = xmm0;
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x11);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x00);
            xmm0 = _mm_xor_si128(xmm0, xmm1);
            xmm0 = _mm_xor_si128(xmm0, xmm2);
        }

        k_koef = _mm_set_epi64x(k_64, k_96);

        xmm1 = xmm0;
        xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x1);
        xmm1 = _mm_slli_si128(xmm1, 8);
        xmm0 = _mm_xor_si128(xmm0, xmm1);

        xmm1 = xmm0;
        xmm1 = _mm_and_si128(xmm1, mask2);
        xmm0 = _mm_srli_si128(xmm0, 12);
        xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x10);
        xmm0 = _mm_xor_si128(xmm0, xmm1);

        /* barrett */
        k_koef = _mm_set_epi64x(k_q, k_u);
        xmm1 = xmm0;
        xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x01);
        xmm0 = _mm_slli_si128(xmm0, 4);
        xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x11);

        xmm0 = _mm_slli_si128(xmm0, 4);
        xmm0 = _mm_xor_si128(xmm0, xmm1);
        crc = _mm_extract_epi32(xmm0, 1);

    }
    else {
        for (uint32_t i = 0; i < length; ++i) {
            crc ^= (uint32_t)(*src_ptr++) << 24u;
            for (uint32_t j = 0; j < 8u; ++j) {
                crc = (crc << 1) ^ (-(int64_t)(crc >> 31u) & polynomial);
            }
        }
    }

    return crc;
}

#if defined(_MSC_VER)
#pragma optimize("O3", on)
#endif

static uint32_t bit_reflect_32(uint32_t x) {
    __m128i shuffle_mask = _mm_cvtsi32_si128(0x00010203);
    __m128i xmmSH_L = _mm_set_epi8((char)0xf0, (char)0x70, (char)0xb0, (char)0x30,
                                   (char)0xd0, (char)0x50, (char)0x90, (char)0x10,
                                   (char)0xe0, (char)0x60, (char)0xa0, (char)0x20,
                                   (char)0xc0, (char)0x40, (char)0x80, (char)0x00);
    __m128i xmmSH_H = _mm_set_epi8(0x0f, 0x07, 0x0b, 0x03, 0x0d, 0x05, 0x09, 0x01, 0x0e, 0x06, 0x0a, 0x02, 0x0c, 0x04, 0x08, 0x00);
    __m128i xmm_F = _mm_set1_epi8(0x0f);
    __m128i xmm0 = _mm_cvtsi32_si128((int)x);
    __m128i xmm1 = _mm_srli_epi16(xmm0, 4);
    xmm0 = _mm_and_si128(xmm0, xmm_F);
    xmm1 = _mm_and_si128(xmm1, xmm_F);
    xmm0 = _mm_shuffle_epi8(xmmSH_L, xmm0);
    xmm1 = _mm_shuffle_epi8(xmmSH_H, xmm1);
    xmm0 = _mm_or_si128(xmm0, xmm1);
    xmm0 = _mm_shuffle_epi8(xmm0, shuffle_mask);
    return (uint32_t)_mm_cvtsi128_si32(xmm0);
}

static void own_shift_two_lanes_be(int offset, __m128i* _xmm0, __m128i* _xmm1) {
    __m128i xmm0 = *_xmm0;
    __m128i xmm1 = *_xmm1;

    switch (offset) {
    case 15:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 15);
        xmm1 = _mm_slli_si128(xmm1, 1);
        break;
    case 14:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 14);
        xmm1 = _mm_slli_si128(xmm1, 2);
        break;
    case 13:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 13);
        xmm1 = _mm_slli_si128(xmm1, 3);
        break;
    case 12:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 12);
        xmm1 = _mm_slli_si128(xmm1, 4);
        break;
    case 11:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 11);
        xmm1 = _mm_slli_si128(xmm1, 5);
        break;
    case 10:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 10);
        xmm1 = _mm_slli_si128(xmm1, 6);
        break;
    case 9:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 9);
        xmm1 = _mm_slli_si128(xmm1, 7);
        break;
    case 8:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 8);
        xmm1 = _mm_slli_si128(xmm1, 8);
        break;
    case 7:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 7);
        xmm1 = _mm_slli_si128(xmm1, 9);
        break;
    case 6:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 6);
        xmm1 = _mm_slli_si128(xmm1, 10);
        break;
    case 5:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 5);
        xmm1 = _mm_slli_si128(xmm1, 11);
        break;
    case 4:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 4);
        xmm1 = _mm_slli_si128(xmm1, 12);
        break;
    case 3:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 3);
        xmm1 = _mm_slli_si128(xmm1, 13);
        break;
    case 2:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 2);
        xmm1 = _mm_slli_si128(xmm1, 14);
        break;
    case 1:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 1);
        xmm1 = _mm_slli_si128(xmm1, 15);
        break;
    default:
        xmm0 = _mm_alignr_epi8(xmm0, xmm1, 0);
        xmm1 = _mm_slli_si128(xmm1, 16);
    }

    *_xmm0 = xmm0;
    *_xmm1 = xmm1;
}

uint32_t own_crc32_long_be_k0(const uint8_t* src_ptr, uint32_t length, uint32_t crc) {
    const uint32_t polynomial = D_POLINOM_1;
    crc = bit_reflect_32(crc);
    if (length >= 16u) {
        __m128i     xmm0, xmm1, xmm2;
        __m128i     xCRC, k_koef;
        __m128i shuffle_le_mask = _mm_set_epi8(
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F);
        __m128i mask2 = _mm_set_epi64x(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFF00000000);

                                                        /* For poly = 0x11edc6f41                   */
        uint64_t k_q = 0x11edc6f41;                     /* k_q = P(x)                               */
        uint64_t k_u = 0x11f91caf6;                     /* k_u = (x**64)/P(x)                       */
        uint64_t k_64 = (uint64_t)0x6ea2d55c << 1;      /* k_64 = Reflect((x**64)/P(x)) << 1        */
        uint64_t k_96 = (uint64_t)0xa66805eb << 1;      /* k_96 = Reflect((x**96)/P(x)) << 1        */
        uint64_t k_128 = (uint64_t)0xa66805eb << 1;     /* k_128 = Reflect((x**(128-32)) mod P(x)   */
        uint64_t k_192 = (uint64_t)0x790606ff << 1;     /* k_192 = Reflect((x**(192-32)) mod P(x)   */

        uint32_t tail = length & 0xf;

        xCRC = _mm_cvtsi32_si128(crc);

        xmm0 = _mm_loadu_si128((const __m128i*)src_ptr);
        src_ptr += 16u;
        xmm0 = _mm_xor_si128(xmm0, xCRC);

        // 1. fold by 512bit until remaining length < 2 * 512bits.

        if (length > 512u) {
            __m128i xmm3, xmm4, xmm5, xmm6, xmm7;
            __m128i srcmm0, srcmm1, srcmm2, srcmm3;
            uint64_t k_512 = (uint64_t)0x4f256efc << 1; /* k_128 = Reflect((x**512) mod P(x)   */
            uint64_t k_576 = (uint64_t)0x3a077781 << 1; /* k_192 = Reflect((x**576) mod P(x)   */
            k_koef = _mm_set_epi64x(k_576, k_512);

            xmm2 = _mm_loadu_si128((const __m128i*)src_ptr);
            xmm4 = _mm_loadu_si128((const __m128i*)(src_ptr + 16u));
            xmm6 = _mm_loadu_si128((const __m128i*)(src_ptr + 32u));
            src_ptr += 48u;

            while (length >= 128u) {
                srcmm0 = _mm_loadu_si128((const __m128i*)src_ptr);
                srcmm1 = _mm_loadu_si128((const __m128i*)(src_ptr + 16u));
                srcmm2 = _mm_loadu_si128((const __m128i*)(src_ptr + 32u));
                srcmm3 = _mm_loadu_si128((const __m128i*)(src_ptr + 48u));

                xmm1 = xmm0;
                xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x01);
                xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x10);
                xmm0 = _mm_xor_si128(xmm0, srcmm0);
                xmm0 = _mm_xor_si128(xmm0, xmm1);

                xmm3 = xmm2;
                xmm2 = _mm_clmulepi64_si128(xmm2, k_koef, 0x01);
                xmm3 = _mm_clmulepi64_si128(xmm3, k_koef, 0x10);
                xmm2 = _mm_xor_si128(xmm2, srcmm1);
                xmm2 = _mm_xor_si128(xmm2, xmm3);

                xmm5 = xmm4;
                xmm4 = _mm_clmulepi64_si128(xmm4, k_koef, 0x01);
                xmm5 = _mm_clmulepi64_si128(xmm5, k_koef, 0x10);
                xmm4 = _mm_xor_si128(xmm4, srcmm2);
                xmm4 = _mm_xor_si128(xmm4, xmm5);

                xmm7 = xmm6;
                xmm6 = _mm_clmulepi64_si128(xmm6, k_koef, 0x01);
                xmm7 = _mm_clmulepi64_si128(xmm7, k_koef, 0x10);
                xmm6 = _mm_xor_si128(xmm6, srcmm3);
                xmm6 = _mm_xor_si128(xmm6, xmm7);

                src_ptr += 64u;
                length -= 64u;
            }

            k_koef = _mm_set_epi64x(k_192, k_128);

            xmm1 = xmm0;
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x01);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x10);
            xmm0 = _mm_xor_si128(xmm0, xmm2);
            xmm0 = _mm_xor_si128(xmm0, xmm1);

            xmm1 = xmm0;
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x01);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x10);
            xmm0 = _mm_xor_si128(xmm0, xmm4);
            xmm0 = _mm_xor_si128(xmm0, xmm1);

            xmm1 = xmm0;
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x01);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x10);
            xmm0 = _mm_xor_si128(xmm0, xmm6);
            xmm0 = _mm_xor_si128(xmm0, xmm1);

            length -= 48u;
        }

        k_koef = _mm_set_epi64x(k_192, k_128);

        while (length >= 32u) {
            xmm1 = xmm0;
            xmm2 = _mm_loadu_si128((const __m128i*)src_ptr);
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x01);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x10);
            xmm0 = _mm_xor_si128(xmm0, xmm2);
            xmm0 = _mm_xor_si128(xmm0, xmm1);
            src_ptr += 16u;
            length -= 16u;
        }

        if (tail) {
            __mmask16 tail_mask = (1 << tail) - 1;

            xmm2 = _mm_maskz_loadu_epi8(tail_mask, (__m128i*)src_ptr);

            own_shift_two_lanes_be(tail, &xmm2, &xmm0);

            xmm1 = xmm0;
            xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x01);
            xmm1 = _mm_clmulepi64_si128(xmm1, k_koef, 0x10);
            xmm0 = _mm_xor_si128(xmm0, xmm1);
            xmm0 = _mm_xor_si128(xmm0, xmm2);
        }

        k_koef = _mm_set_epi64x(k_96, k_64);

        xmm1 = xmm0;
        xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x10);
        xmm1 = _mm_srli_si128(xmm1, 8);
        xmm0 = _mm_xor_si128(xmm0, xmm1);

        xmm1 = xmm0;
        xmm1 = _mm_and_si128(xmm1, mask2);
        xmm0 = _mm_slli_si128(xmm0, 12);
        xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x01);
        xmm0 = _mm_xor_si128(xmm0, xmm1);

        /* go to LE */
        {
            __m128i xmmSH_L = _mm_set_epi8((char)0xf0, (char)0x70, (char)0xb0, (char)0x30,
                                           (char)0xd0, (char)0x50, (char)0x90, (char)0x10,
                                           (char)0xe0, (char)0x60, (char)0xa0, (char)0x20,
                                           (char)0xc0, (char)0x40, (char)0x80, (char)0x00);
            __m128i xmmSH_H = _mm_set_epi8(0x0f, 0x07, 0x0b, 0x03, 0x0d, 0x05, 0x09, 0x01, 0x0e, 0x06, 0x0a, 0x02, 0x0c, 0x04, 0x08, 0x00);
            __m128i xmm_F = _mm_set1_epi8(0x0f);

            xmm1 = _mm_srli_epi16(xmm0, 4);
            xmm0 = _mm_and_si128(xmm0, xmm_F);
            xmm1 = _mm_and_si128(xmm1, xmm_F);
            xmm0 = _mm_shuffle_epi8(xmmSH_L, xmm0);
            xmm1 = _mm_shuffle_epi8(xmmSH_H, xmm1);
            xmm0 = _mm_or_si128(xmm0, xmm1);
            xmm0 = _mm_shuffle_epi8(xmm0, shuffle_le_mask);
        }

        /* barrett */
        k_koef = _mm_set_epi64x(k_q, k_u);
        xmm1 = xmm0;
        xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x01);
        xmm0 = _mm_slli_si128(xmm0, 4);
        xmm0 = _mm_clmulepi64_si128(xmm0, k_koef, 0x11);

        xmm0 = _mm_slli_si128(xmm0, 4);
        xmm0 = _mm_xor_si128(xmm0, xmm1);
        crc = _mm_extract_epi32(xmm0, 1);
    }
    else {
        const uint32_t polynomial_s = bit_reflect_32(polynomial);
        for (uint32_t i = 0; i < length; ++i) {
            crc ^= (uint64_t)(*src_ptr++);
            for (uint32_t j = 0; j < 8u; ++j) {
                crc = (crc >> 1) ^ (-(int32_t)(crc & 1u) & polynomial_s);
            }
        }
        crc = bit_reflect_32(crc);
    }
    return crc;
}

/*********************************/


uint32_t dml_avx512_crc_u32(const uint8_t* src, uint32_t transfer_size, uint32_t crc_value, uint32_t polynomial)
{
    dmlc_own_calculate_crc_32u(src, transfer_size, &crc_value, polynomial);
    return crc_value;
}

uint32_t dml_avx512_crc_reflected_u32(const uint8_t* src, uint32_t transfer_size, uint32_t crc_value, uint32_t polynomial)
{
    dmlc_own_calculate_crc_reflected_32u(src, transfer_size, &crc_value, polynomial);
    return crc_value;
}



