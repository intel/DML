/*
 * Copyright 2020-2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no
 * express or implied warranties, other than those that are expressly
 * stated in the License.
 *
 */

/**
 * @brief Contain implementation of the follow functions:
 *      - @ref dmlc_calculate_crc_32u()
 *
 * @date 7/20/2021
 *
 */

#define DML_DISABLE_OPTIMIZATION_

/**
*  @todo
*/
static inline uint32_t getCRCSize(uint64_t poly)
{
    uint32_t    crcSize;
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
static void own_CRC_8u_k0(const uint8_t* src_ptr, uint32_t len, uint64_t poly, const uint8_t optPoly[128], uint32_t init, uint32_t* crc_ptr)
{
    int crc_size = getCRCSize(poly);
    own_CRC_8u_opt_k0(src_ptr, init, len, crc_size, (uint32_t*)optPoly, crc_ptr);
}

/**
*  @todo
*/
static void poly1x64_32_div(uint64_t poly, uint64_t* quit_ptr, uint32_t* tail_ptr)
{
    int i, j;
    uint64_t tail = 0;
    uint64_t quot = 0;
    uint8_t  byte = 0x01;
    for (i = 0; i < 9; i++) {
        uint8_t bit;
        uint64_t hbit;
        for (j = 0; j < 8; j++) {
            bit = (byte & 0x80) >> 7;
            byte <<= 1;
            hbit = tail & 0x80000000;
            tail <<= 1;
            tail |= bit;
            quot <<= 1;
            if (hbit) {
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

    crc_size = getCRCSize(poly);
    uint32_t* opt = (uint32_t*)optPoly;
    *(uint64_t*)opt = poly;     /*copy poly*/
    uint64_t poly32 = poly << (32 - crc_size);
    poly1x64_32_div(poly32, &u, &t); /*for 1^64 and U*/
    *(uint64_t*)(opt + 2) = u;
    int bits[] = {64, 96, 160, 224, 288, 352, 416, 480, 544, 608, 672, 736, 800, 864, 928, 992, 1056, 2016, 2080};
    uint32_t tail = 0;
    uint32_t poly_32 = (uint32_t)poly;
    int j;

    k = bits[0] + 8;
    tail = poly_32;
    for (j = 40; (uint32_t)j < k; j++) {
        uint32_t mask;
        mask = (tail & 0x80000000) ? poly_32 : 0;
        tail += tail;
        tail ^= mask;
    }
    opt[4 + 0] = (uint32_t)tail;

    for (i = 1; i < ((sizeof(bits) / sizeof(bits[0])) - 2); i++) {
        k = bits[i] + 8;
        for (; (uint32_t)j < k; j++) {
            uint32_t mask;
            mask = (tail & 0x80000000) ? poly_32 : 0;
            tail += tail;
            tail ^= mask;
        }
        opt[4 + i] = (uint32_t)tail;
    }
}

DML_CORE_OWN_INLINE(dmlc_status_t, calculate_crc_32u, (const uint8_t* const memory_region_ptr,
        uint32_t bytes_to_hash,
        uint32_t* const crc_ptr,
        uint32_t polynomial))
{
    uint64_t poly = (uint64_t)polynomial | ((uint64_t)1u << (uint64_t)32u);
    uint8_t  opt_poly_ptr[128];

    own_gen_crc_opt_poly_8u(poly, opt_poly_ptr);
    own_CRC_8u_k0(memory_region_ptr, bytes_to_hash, poly, opt_poly_ptr, *crc_ptr, crc_ptr);
    return DML_STATUS_OK;
}

#if defined(_MSC_VER)
#pragma optimize("", off)
#pragma optimize("O3", on)
#endif

/**
*  @todo
*/
#define _MM_XOR_PS(A,B) _mm_castps_si128(_mm_xor_ps(_mm_castsi128_ps(A),_mm_castsi128_ps(B)))
/**
*  @todo
*/
#define arg1_low32 ecx
/**
*  @todo
*/
static void own_CRC_8u_opt_k0(const uint8_t* src_ptr, uint32_t init_crc, int len0, int crc_size, uint32_t* koeff, uint32_t* crc_ptr)
{
    uint64_t pshufb_shf_table[] = {
            0x8786858483828100, 0x8f8e8d8c8b8a8988,
            0x0706050403020100, 0x000e0d0c0b0a0908};

    int len = len0;
    uint8_t ttt[128];
    uint8_t* r11 = ttt;
    uint8_t* ptr;

    __m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm12, xmm13;
#ifndef DML_DISABLE_OPTIMIZATION_
    __m128i xmm11;
#endif // DML_DISABLE_OPTIMIZATION_

    int eax, ecx, r9;
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
#ifndef DML_DISABLE_OPTIMIZATION_
    if (len < 256) {
        goto _less_than_256;
    }
#endif // DML_DISABLE_OPTIMIZATION_
    //; load the initial crc value
    xmm10 = _mm_cvtsi32_si128(arg1_low32);               //movd	xmm10, arg1_low32; initial crc
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
    xmm0 = _mm_xor_si128(xmm0, xmm10);
    xmm1 = _mm_shuffle_epi8(xmm1, ENDIA_SHUF_MASK);
    xmm2 = _mm_shuffle_epi8(xmm2, ENDIA_SHUF_MASK);
    xmm3 = _mm_shuffle_epi8(xmm3, ENDIA_SHUF_MASK);
    xmm4 = _mm_shuffle_epi8(xmm4, ENDIA_SHUF_MASK);
    xmm5 = _mm_shuffle_epi8(xmm5, ENDIA_SHUF_MASK);
    xmm6 = _mm_shuffle_epi8(xmm6, ENDIA_SHUF_MASK);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm10 = _mm_set_epi64x(k_1056/*rk4*/, k_992/*rk3*/);
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
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 1));
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
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 3));
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
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 5));
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
    xmm12 = _mm_loadu_si128((const __m128i*)(src_ptr + 16 * 7));
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
    if (len >= 0) goto _fold_128_B_loop;                 //jge	_fold_128_B_loop
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    src_ptr += 128;
    //; at this point, the buffer pointer is pointing at the last y Bytes of the buffer
    //; fold the 8 xmm registers to 1 xmm register with different constants
    //
    xmm10 = _mm_set_epi64x(k_928/*rk10*/, k_864/*rk9*/);
    xmm8 = xmm0;
    xmm0 = _mm_clmulepi64_si128(xmm0, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _MM_XOR_PS(xmm7, xmm0);

    xmm10 = _mm_set_epi64x(k_800/*rk12*/, k_736/*rk11*/);
    xmm8 = xmm1;
    xmm1 = _mm_clmulepi64_si128(xmm1, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _MM_XOR_PS(xmm7, xmm1);
    xmm10 = _mm_set_epi64x(k_672/*rk14*/, k_608/*rk13*/);
    xmm8 = xmm2;
    xmm2 = _mm_clmulepi64_si128(xmm2, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _mm_xor_si128(xmm7, xmm2);
    xmm10 = _mm_set_epi64x(k_544/*rk16*/, k_480/*rk15*/);
    xmm8 = xmm3;
    xmm3 = _mm_clmulepi64_si128(xmm3, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _MM_XOR_PS(xmm7, xmm3);
    xmm10 = _mm_set_epi64x(k_416/*rk18*/, k_352/*rk17*/);
    xmm8 = xmm4;
    xmm4 = _mm_clmulepi64_si128(xmm4, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _mm_xor_si128(xmm7, xmm4);
    xmm10 = _mm_set_epi64x(k_288/*rk20*/, k_224/*rk19*/);
    xmm8 = xmm5;
    xmm5 = _mm_clmulepi64_si128(xmm5, xmm10, 0x11);
    xmm8 = _mm_clmulepi64_si128(xmm8, xmm10, 0x0);
    xmm7 = _mm_xor_si128(xmm7, xmm8);
    xmm7 = _MM_XOR_PS(xmm7, xmm5);
    xmm10 = _mm_set_epi64x(k_160/*rk2*/, k_96/*rk1*/);
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
    if (len < 0) goto _final_reduction_for_128;          //        jl	_final_reduction_for_128
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

    if (len >= 0) goto _16B_reduction_loop;              //        jge	_16B_reduction_loop
    //        ; now we have 16 + z bytes left to reduce, where 0 <= z < 16.
    //        ; first, we reduce the data in the xmm7 register
    _final_reduction_for_128:
    //    ; check if any more data to fold.If not, compute the CRC of the final 128 bits
    len += 16;
    if (len == 0) goto _128_done;                        //        je	_128_done
    //        ; here we are getting data that is less than 16 bytes.
    //        ; since we know that there was data before the pointer, we can offset the input pointer before the actual point, to receive exactly 16 bytes.
    //        ; after that the registers need to be adjusted.
#ifndef DML_DISABLE_OPTIMIZATION_
    _get_last_two_xmms:
#endif // DML_DISABLE_OPTIMIZATION_
    xmm2 = xmm7;
    xmm1 = _mm_loadu_si128((const __m128i*)(src_ptr - 16 + len));
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
    xmm10 = _mm_set_epi64x(k_64/*rk6*/, k_96/*rk5*/);
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
    xmm10 = _mm_set_epi64x(q/*rk8*/, u/*rk7*/);
    xmm0 = xmm7;
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x01);
    xmm7 = _mm_slli_si128(xmm7, 4);
    xmm7 = _mm_clmulepi64_si128(xmm7, xmm10, 0x11);

    xmm7 = _mm_slli_si128(xmm7, 4);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    eax = _mm_extract_epi32(xmm7, 1);
#ifndef DML_DISABLE_OPTIMIZATION_
    _cleanup:
#endif // DML_DISABLE_OPTIMIZATION_
    //    ; scale the result back to 16 bits
    eax = ((uint32_t)eax) >> (32 - crc_size);
    *crc_ptr = eax;
    return;                                             //        ret

    //align 16
#ifndef DML_DISABLE_OPTIMIZATION_
    _less_than_256:
    //
    //; check if there is enough buffer to be able to fold 16B at a time
    //cmp	arg3, 32
    //jl	_less_than_32
    if (len < 32) {
        goto _less_than_32;
    }
    xmm11 = ENDIA_SHUF_MASK;
    //; if there is, load the constants
    xmm10 = _mm_set_epi64x(k_160/*rk2*/, k_96/*rk1*/);
    xmm0 = _mm_cvtsi32_si128(arg1_low32);
    xmm0 = _mm_slli_si128(xmm0, 12);
    xmm7 = _mm_loadu_si128((const __m128i*)src_ptr);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    //
    //; update the buffer pointer
    src_ptr += 16;
    //
    //; update the counter.subtract 32 instead of 16 to save one instruction from the loop
    len -= 32;
    goto _16B_reduction_loop;                            //jmp	_16B_reduction_loop

    //align 16
    _less_than_32:
    //; mov initial crc to the return value. this is necessary for zero - length buffers.
    eax = arg1_low32;                                    //mov	eax, arg1_low32
    //test	arg3, arg3
    if (len == 0) goto _cleanup;                         //je	_cleanup
    //
    xmm11 = ENDIA_SHUF_MASK;

    xmm0 = _mm_cvtsi32_si128(arg1_low32);
    xmm0 = _mm_slli_si128(xmm0, 12);
    //cmp	arg3, 16
    if (len == 16) goto _exact_16_left;                  //je	_exact_16_left
    if (len < 16) goto _less_than_16_left;               //jl	_less_than_16_left

    xmm7 = _mm_loadu_si128((const __m128i*)src_ptr);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    src_ptr += 16;
    len -= 16;
    xmm10 = _mm_set_epi64x(k_160/*rk2*/, k_96/*rk1*/);
    goto _get_last_two_xmms;                             //jmp	_get_last_two_xmms
    //align 16
    _less_than_16_left:
#endif // DML_DISABLE_OPTIMIZATION_
    //; use stack space to load data less than 16 bytes, zero - out the 16B in memory first.
    //
    xmm1 = _mm_setzero_si128();

    _mm_storeu_si128((__m128i*)r11, xmm1);
    //cmp	arg3, 4
    if (len < 4) goto _only_less_than_4;                 //jl	_only_less_than_4
    //;	backup the counter value
    r9 = len;
    //cmp	arg3, 8
    if (len < 8) goto _less_than_8_left;                 //jl	_less_than_8_left
    //; load 8 Bytes
    *(int64_t*)r11 = *(int64_t*)src_ptr;
    r11 += 8;
    len -= 8;
    src_ptr += 8;
    _less_than_8_left:
    //cmp	arg3, 4
    if (len < 4) goto _less_than_4_left;                 //jl	_less_than_4_left
    //; load 4 Bytes
    *(int*)r11 = *(int*)src_ptr;
    r11 += 4;
    len -= 4;
    src_ptr += 4;                                        //add	arg2, 4
    _less_than_4_left:
    //
    //cmp	arg3, 2
    if (len < 2) goto _less_than_2_left;                 //jl	_less_than_2_left
    //
    //; load 2 Bytes
    *(short*)r11 = *(short*)src_ptr;
    r11 += 2;
    len -= 2;
    src_ptr += 2;
    _less_than_2_left:
    //cmp     arg3, 1
    if (len < 1) goto _zero_left;                        //jl      _zero_left
    //; load 1 Byte
    *r11 = *src_ptr;
    _zero_left:
    xmm7 = _mm_loadu_si128((const __m128i*)ttt);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);

    ptr = (uint8_t*)pshufb_shf_table + 16 - r9;
    xmm0 = _mm_loadu_si128((const __m128i*)ptr);
    xmm0 = _mm_xor_si128(xmm0, mask1);
    //
    xmm7 = _mm_shuffle_epi8(xmm7, xmm0);
    goto _128_done;                                      //jmp	_128_done
    //align 16
#ifndef DML_DISABLE_OPTIMIZATION_
    _exact_16_left:
#endif // DML_DISABLE_OPTIMIZATION_
    xmm7 = _mm_loadu_si128((const __m128i*)src_ptr);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);

    goto _128_done;                                      //jmp	_128_done
    _only_less_than_4:
    //cmp	arg3, 3
    if (len < 3) goto _only_less_than_3;                 //jl	_only_less_than_3
    //; load 3 Bytes
    r11[0] = src_ptr[0];

    r11[1] = src_ptr[1];

    r11[2] = src_ptr[2];
    xmm7 = _mm_loadu_si128((const __m128i*)r11);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    xmm7 = _mm_srli_si128(xmm7, 5);
    goto _barrett;                                       //jmp	_barrett
    _only_less_than_3:
    //cmp	arg3, 2
    if (len < 2) goto _only_less_than_2;                 //jl	_only_less_than_2
    //; load 2 Bytes
    r11[0] = src_ptr[0];

    r11[1] = src_ptr[1];
    xmm7 = _mm_loadu_si128((const __m128i*)r11);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);
    xmm7 = _mm_srli_si128(xmm7, 6);
    goto _barrett;                                       //jmp	_barrett
    _only_less_than_2:
    //
    //; load 1 Byte
    eax = src_ptr[0];
    r11[0] = eax;

    xmm7 = _mm_loadu_si128((const __m128i*)r11);
    xmm7 = _mm_shuffle_epi8(xmm7, ENDIA_SHUF_MASK);
    xmm7 = _mm_xor_si128(xmm7, xmm0);

    xmm7 = _mm_srli_si128(xmm7, 7);

    goto _barrett;                                       //jmp	_barrett
}

/**
*  @todo
*/
dmlc_status_t dmlc_own_calculate_crc_32u_noopt(const uint8_t* const memory_region_ptr,
                                               uint32_t bytes_to_hash,
                                               uint32_t* const crc_ptr,
                                               uint32_t polynomial)
{
    // Current crc value
    uint32_t current_crc = (*crc_ptr);

    // Through all bytes
    for (uint32_t i = 0u; i < bytes_to_hash; ++i)
    {
        // Calculate crc for current byte
        current_crc = dmlc_own_crc_byte_32u(current_crc, memory_region_ptr[i], polynomial);
    }

    // Store result
    (*crc_ptr) = current_crc;

    return DML_STATUS_OK;
}