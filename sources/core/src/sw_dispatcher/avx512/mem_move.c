/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "../dml_cpuid.h"
#include "../dml_kernels.h"

#if defined(_MSC_BUILD)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#else
#error "Unsupported compiler"
#endif

#if defined(_MSC_VER)
#define OWN_ALIGNED_64_ARRAY(array_declaration) __declspec(align(64u)) array_declaration
#elif defined(__GNUC__)
#define OWN_ALIGNED_64_ARRAY(array_declaration) array_declaration __attribute__((aligned(64u)))
#endif

static inline void dmlc_own_px_copy_8u_unrolled(const uint8_t *src, uint8_t *dst, uint32_t length)
{
    uint32_t align_dst = 64u - ((uint64_t)dst & 0x3F);
    uint32_t align_src = 64u - ((uint64_t)src & 0x3F);

    if (align_dst < 64u)
    {
        if (length < align_dst)
        {
            align_dst = length;
            for (uint32_t i = 0u; i < align_dst; ++i)
            {
                dst[i] = src[i];
            }
            return;
        }
        for (uint32_t i = 0u; i < align_dst; ++i)
        {
            dst[i] = src[i];
        }
        length -= align_dst;
        src += align_dst;
        dst += align_dst;
    }

    if (align_dst == align_src)
    {
        const uint64_t *src_64u_ptr = (uint64_t *)src;
        uint64_t       *dst_64u_ptr = (uint64_t *)dst;

        uint32_t length_64u = length / sizeof(uint64_t);
        uint32_t tail_start = length_64u * sizeof(uint64_t);

        while (length_64u > 3u)
        {
            dst_64u_ptr[0] = src_64u_ptr[0];
            dst_64u_ptr[1] = src_64u_ptr[1];
            dst_64u_ptr[2] = src_64u_ptr[2];
            dst_64u_ptr[3] = src_64u_ptr[3];
            dst_64u_ptr += 4u;
            src_64u_ptr += 4u;
            length_64u -= 4u;
        }

        for (uint32_t i = 0u; i < length_64u; ++i)
        {
            dst_64u_ptr[i] = src_64u_ptr[i];
        }

        for (uint32_t i = tail_start; i < length; ++i)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        while (length > 7u)
        {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            dst[3] = src[3];
            dst[4] = src[4];
            dst[5] = src[5];
            dst[6] = src[6];
            dst[7] = src[7];

            dst += 8u;
            src += 8u;
            length -= 8;
        }

        for (uint32_t i = 0u; i < length; ++i)
        {
            dst[i] = src[i];
        }
    }
}

OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_2u[32])  = { 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,
                                                              17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_4u[32])  = { 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17,
                                                              18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_6u[32])  = { 3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18,
                                                              19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_8u[32])  = { 4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                                                              20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_10u[32]) = { 5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                                               21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_12u[32]) = { 6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                                                               22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_14u[32]) = { 7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                                                               23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_16u[32]) = { 8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                                                               24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_18u[32]) = { 9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                                                               25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_20u[32]) = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                                                               26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_22u[32]) = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                                                               27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_24u[32]) = { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                                                               28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_26u[32]) = { 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                                                               29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_28u[32]) = { 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                                                               30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_30u[32]) = { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                                                               31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_32u[32]) = { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                                                               32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_34u[32]) = { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
                                                               33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_36u[32]) = { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                                                               34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_38u[32]) = { 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
                                                               35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_40u[32]) = { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
                                                               36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_42u[32]) = { 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
                                                               37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_44u[32]) = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
                                                               38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_46u[32]) = { 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                                                               39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_48u[32]) = { 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                                                               40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_50u[32]) = { 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                                               41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_52u[32]) = { 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                                                               42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_54u[32]) = { 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                                                               43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_56u[32]) = { 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
                                                               44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_58u[32]) = { 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                                                               45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_60u[32]) = { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
                                                               46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61 };
OWN_ALIGNED_64_ARRAY(static uint16_t permutex_idx_62u[32]) = { 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
                                                               47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62 };

static uint16_t *permutex_idx_pptr[31] = { permutex_idx_2u,  permutex_idx_4u,  permutex_idx_6u,  permutex_idx_8u,  permutex_idx_10u,
                                           permutex_idx_12u, permutex_idx_14u, permutex_idx_16u, permutex_idx_18u, permutex_idx_20u,
                                           permutex_idx_22u, permutex_idx_24u, permutex_idx_26u, permutex_idx_28u, permutex_idx_30u,
                                           permutex_idx_32u, permutex_idx_34u, permutex_idx_36u, permutex_idx_38u, permutex_idx_40u,
                                           permutex_idx_42u, permutex_idx_44u, permutex_idx_46u, permutex_idx_48u, permutex_idx_50u,
                                           permutex_idx_52u, permutex_idx_54u, permutex_idx_56u, permutex_idx_58u, permutex_idx_60u,
                                           permutex_idx_62u };

static inline __m512i dmlc_own_mm512_bsrli_epi128(__m512i a, uint32_t shift)
{
    switch (shift)
    {
        case 1:
            {
                return _mm512_bsrli_epi128(a, 1);
            }
        case 2:
            {
                return _mm512_bsrli_epi128(a, 2);
            }
        case 3:
            {
                return _mm512_bsrli_epi128(a, 3);
            }
        case 4:
            {
                return _mm512_bsrli_epi128(a, 4);
            }
        case 5:
            {
                return _mm512_bsrli_epi128(a, 5);
            }
        case 6:
            {
                return _mm512_bsrli_epi128(a, 6);
            }
        case 7:
            {
                return _mm512_bsrli_epi128(a, 7);
            }
        case 8:
            {
                return _mm512_bsrli_epi128(a, 8);
            }
        case 9:
            {
                return _mm512_bsrli_epi128(a, 9);
            }
        case 10:
            {
                return _mm512_bsrli_epi128(a, 10);
            }
        case 11:
            {
                return _mm512_bsrli_epi128(a, 11);
            }
        case 12:
            {
                return _mm512_bsrli_epi128(a, 12);
            }
        case 13:
            {
                return _mm512_bsrli_epi128(a, 13);
            }
        case 14:
            {
                return _mm512_bsrli_epi128(a, 14);
            }
        case 15:
            {
                return _mm512_bsrli_epi128(a, 15);
            }
        default:
            return _mm512_setzero_si512();
    }
}

static inline __m512i dmlc_own_mm512_bslli_epi128(__m512i a, uint32_t shift)
{
    switch (shift)
    {
        case 1:
            {
                return _mm512_bslli_epi128(a, 1);
            }
        case 2:
            {
                return _mm512_bslli_epi128(a, 2);
            }
        case 3:
            {
                return _mm512_bslli_epi128(a, 3);
            }
        case 4:
            {
                return _mm512_bslli_epi128(a, 4);
            }
        case 5:
            {
                return _mm512_bslli_epi128(a, 5);
            }
        case 6:
            {
                return _mm512_bslli_epi128(a, 6);
            }
        case 7:
            {
                return _mm512_bslli_epi128(a, 7);
            }
        case 8:
            {
                return _mm512_bslli_epi128(a, 8);
            }
        case 9:
            {
                return _mm512_bslli_epi128(a, 9);
            }
        case 10:
            {
                return _mm512_bslli_epi128(a, 10);
            }
        case 11:
            {
                return _mm512_bslli_epi128(a, 11);
            }
        case 12:
            {
                return _mm512_bslli_epi128(a, 12);
            }
        case 13:
            {
                return _mm512_bslli_epi128(a, 13);
            }
        case 14:
            {
                return _mm512_bslli_epi128(a, 14);
            }
        case 15:
            {
                return _mm512_bslli_epi128(a, 15);
            }
        default:
            return _mm512_setzero_si512();
    }
}

static inline __m512i dmlc_own_mm512_alignr_epi8(__m512i a, __m512i b, uint32_t shift)
{
    switch (shift)
    {
        case 0:
            {
                return b;
            }
        case 4:
            {
                return _mm512_alignr_epi32(a, b, 1);
            }
        case 8:
            {
                return _mm512_alignr_epi32(a, b, 2);
            }
        case 12:
            {
                return _mm512_alignr_epi32(a, b, 3);
            }
        case 16:
            {
                return _mm512_alignr_epi32(a, b, 4);
            }
        case 20:
            {
                return _mm512_alignr_epi32(a, b, 5);
            }
        case 24:
            {
                return _mm512_alignr_epi32(a, b, 6);
            }
        case 28:
            {
                return _mm512_alignr_epi32(a, b, 7);
            }
        case 32:
            {
                return _mm512_alignr_epi32(a, b, 8);
            }
        case 36:
            {
                return _mm512_alignr_epi32(a, b, 9);
            }
        case 40:
            {
                return _mm512_alignr_epi32(a, b, 10);
            }
        case 44:
            {
                return _mm512_alignr_epi32(a, b, 11);
            }
        case 48:
            {
                return _mm512_alignr_epi32(a, b, 12);
            }
        case 52:
            {
                return _mm512_alignr_epi32(a, b, 13);
            }
        case 56:
            {
                return _mm512_alignr_epi32(a, b, 14);
            }
        case 60:
            {
                return _mm512_alignr_epi32(a, b, 15);
            }
        default:
            return _mm512_setzero_si512();
    }
}

static void copy_avx512(const uint8_t *src, uint8_t *dst, uint32_t transfer_size)
{
    const size_t kilobyte = 1024;

    if (transfer_size < kilobyte)
    {
        dmlc_own_px_copy_8u_unrolled(src, dst, transfer_size);
        return;
    }

    if (transfer_size > (32 * kilobyte))
    {
        size_t cache_size = dml_core_get_cache_size();
        if ((cache_size > 0) && (transfer_size > cache_size))
        {
            dmlc_own_px_copy_8u_unrolled(src, dst, transfer_size);
            return;
        }
    }

    const uint32_t alignment    = 64;
    const uint32_t magic_number = 0x3f;

    uint32_t align_dst = alignment - ((uintptr_t)dst & magic_number);
    uint32_t align_src = alignment - ((uintptr_t)src & magic_number);

    if (align_dst < alignment)
    {
        if (transfer_size < 4 * kilobyte)
        {
            dmlc_own_px_copy_8u_unrolled(src, dst, transfer_size);
            return;
        }

        dmlc_own_px_copy_8u_unrolled(src, dst, align_dst);

        transfer_size -= align_dst;
        dst += align_dst;
        src += align_dst;

        uint32_t length_512u = transfer_size / sizeof(__m512i);
        uint32_t tail        = transfer_size % sizeof(__m512i);

        if (0u != ((align_src - align_dst) & 15u))
        {
            uint32_t shift = (align_dst > align_src) ? (align_dst - align_src) : (64u + align_dst - align_src);

            if (0u == (shift & 3u))
            {
                src -= shift;
                __mmask64 skip_mask = ~((1llu << shift) - 1u);
                __m512i   zmm0      = _mm512_maskz_loadu_epi8(skip_mask, (const __m512i *)src);
                src += 64u;

                while (length_512u > 4u)
                {
                    __m512i zmm1 = _mm512_load_si512((const __m512i *)src);
                    __m512i zmm2 = dmlc_own_mm512_alignr_epi8(zmm1, zmm0, shift);
                    __m512i zmm3 = _mm512_load_si512((const __m512i *)(src + 64u));
                    __m512i zmm4 = dmlc_own_mm512_alignr_epi8(zmm3, zmm1, shift);
                    __m512i zmm5 = _mm512_load_si512((const __m512i *)(src + 128u));
                    __m512i zmm6 = dmlc_own_mm512_alignr_epi8(zmm5, zmm3, shift);
                    zmm0         = _mm512_load_si512((const __m512i *)(src + 192u));
                    __m512i zmm7 = dmlc_own_mm512_alignr_epi8(zmm0, zmm5, shift);
                    _mm512_store_si512((__m512i *)dst, zmm2);
                    _mm512_store_si512((__m512i *)(dst + 64u), zmm4);
                    _mm512_store_si512((__m512i *)(dst + 128u), zmm6);
                    _mm512_store_si512((__m512i *)(dst + 192u), zmm7);
                    src += 256u;
                    dst += 256u;
                    length_512u -= 4u;
                }

                src -= 64u - shift;
            }
            else if (0u == (shift & 1u))
            {
                src -= shift;
                __mmask64 skip_mask = ~((1llu << shift) - 1u);
                __m512i   zmm0      = _mm512_maskz_loadu_epi8(skip_mask, (const __m512i *)src);
                src += 64u;

                __m512i permutex_idxmm = _mm512_load_si512(permutex_idx_pptr[(shift - 2) / 2]);

                while (length_512u > 4u)
                {
                    __m512i zmm1 = _mm512_load_si512((const __m512i *)src);
                    __m512i zmm2 = _mm512_permutex2var_epi16(zmm0, permutex_idxmm, zmm1);
                    __m512i zmm3 = _mm512_load_si512((const __m512i *)(src + 64u));
                    __m512i zmm4 = _mm512_permutex2var_epi16(zmm1, permutex_idxmm, zmm3);
                    __m512i zmm5 = _mm512_load_si512((const __m512i *)(src + 128u));
                    __m512i zmm6 = _mm512_permutex2var_epi16(zmm3, permutex_idxmm, zmm5);
                    zmm0         = _mm512_load_si512((const __m512i *)(src + 192u));
                    __m512i zmm7 = _mm512_permutex2var_epi16(zmm5, permutex_idxmm, zmm0);
                    _mm512_store_si512((__m512i *)dst, zmm2);
                    _mm512_store_si512((__m512i *)(dst + 64u), zmm4);
                    _mm512_store_si512((__m512i *)(dst + 128u), zmm6);
                    _mm512_store_si512((__m512i *)(dst + 192u), zmm7);
                    src += 256u;
                    dst += 256u;
                    length_512u -= 4u;
                }

                src -= 64u - shift;
            }
            else if (shift < 16u)
            {
                if (transfer_size < 16 * kilobyte)
                {
                    dmlc_own_px_copy_8u_unrolled(src, dst, transfer_size);
                    return;
                }

                src -= shift;
                __mmask64 skip_mask = ~((1llu << shift) - 1u);
                __m512i   zmm0      = _mm512_maskz_loadu_epi8(skip_mask, (const __m512i *)src);
                src += 64u;

                __m512i permutex_idxmm_higher = _mm512_load_si512(permutex_idx_pptr[(shift - 1) / 2]);

                while (length_512u > 4u)
                {
                    __m512i zmm1        = _mm512_load_si512((const __m512i *)src);
                    __m512i zmm2_lower  = dmlc_own_mm512_bsrli_epi128(zmm0, shift);
                    __m512i zmm2_higher = _mm512_permutex2var_epi16(zmm0, permutex_idxmm_higher, zmm1);
                    zmm2_higher         = dmlc_own_mm512_bslli_epi128(zmm2_higher, 1u);
                    zmm2_higher         = _mm512_mask_mov_epi8(zmm2_higher, 0x0001000100010001, zmm2_lower);
                    __m512i zmm3        = _mm512_load_si512((const __m512i *)(src + 64u));
                    __m512i zmm4_lower  = dmlc_own_mm512_bsrli_epi128(zmm1, shift);
                    __m512i zmm4_higher = _mm512_permutex2var_epi16(zmm1, permutex_idxmm_higher, zmm3);
                    zmm4_higher         = dmlc_own_mm512_bslli_epi128(zmm4_higher, 1u);
                    zmm4_higher         = _mm512_mask_mov_epi8(zmm4_higher, 0x0001000100010001, zmm4_lower);
                    __m512i zmm5        = _mm512_load_si512((const __m512i *)(src + 128u));
                    __m512i zmm6_lower  = dmlc_own_mm512_bsrli_epi128(zmm3, shift);
                    __m512i zmm6_higher = _mm512_permutex2var_epi16(zmm3, permutex_idxmm_higher, zmm5);
                    zmm6_higher         = dmlc_own_mm512_bslli_epi128(zmm6_higher, 1u);
                    zmm6_higher         = _mm512_mask_mov_epi8(zmm6_higher, 0x0001000100010001, zmm6_lower);
                    zmm0                = _mm512_load_si512((const __m512i *)(src + 192u));
                    __m512i zmm7_lower  = dmlc_own_mm512_bsrli_epi128(zmm5, shift);
                    __m512i zmm7_higher = _mm512_permutex2var_epi16(zmm5, permutex_idxmm_higher, zmm0);
                    zmm7_higher         = dmlc_own_mm512_bslli_epi128(zmm7_higher, 1u);
                    zmm7_higher         = _mm512_mask_mov_epi8(zmm7_higher, 0x0001000100010001, zmm7_lower);
                    _mm512_store_si512((__m512i *)dst, zmm2_higher);
                    _mm512_store_si512((__m512i *)(dst + 64u), zmm4_higher);
                    _mm512_store_si512((__m512i *)(dst + 128u), zmm6_higher);
                    _mm512_store_si512((__m512i *)(dst + 192u), zmm7_higher);
                    src += 256u;
                    dst += 256u;
                    length_512u -= 4u;
                }

                src -= 64u - shift;
            }
            else if (shift > 48u)
            {
                if (transfer_size < 16 * kilobyte)
                {
                    dmlc_own_px_copy_8u_unrolled(src, dst, transfer_size);
                    return;
                }

                src -= shift;
                __mmask64 skip_mask = ~((1llu << shift) - 1u);
                __m512i   zmm0      = _mm512_maskz_loadu_epi8(skip_mask, (const __m512i *)src);
                src += 64u;

                __m512i  permutex_idxmm_lower = _mm512_load_si512(permutex_idx_pptr[(shift - 3) / 2]);
                uint32_t shift_higher         = 64u - shift;

                while (length_512u > 4u)
                {
                    __m512i zmm1        = _mm512_load_si512((const __m512i *)src);
                    __m512i zmm2_lower  = _mm512_permutex2var_epi16(zmm0, permutex_idxmm_lower, zmm1);
                    zmm2_lower          = dmlc_own_mm512_bsrli_epi128(zmm2_lower, 1u);
                    __m512i zmm2_higher = dmlc_own_mm512_bslli_epi128(zmm1, shift_higher);
                    zmm2_higher         = _mm512_mask_mov_epi8(zmm2_higher, 0x7FFF7FFF7FFF7FFF, zmm2_lower);
                    __m512i zmm3        = _mm512_load_si512((const __m512i *)(src + 64u));
                    __m512i zmm4_lower  = _mm512_permutex2var_epi16(zmm1, permutex_idxmm_lower, zmm3);
                    zmm4_lower          = dmlc_own_mm512_bsrli_epi128(zmm4_lower, 1u);
                    __m512i zmm4_higher = dmlc_own_mm512_bslli_epi128(zmm3, shift_higher);
                    zmm4_higher         = _mm512_mask_mov_epi8(zmm4_higher, 0x7FFF7FFF7FFF7FFF, zmm4_lower);
                    __m512i zmm5        = _mm512_load_si512((const __m512i *)(src + 128u));
                    __m512i zmm6_lower  = _mm512_permutex2var_epi16(zmm3, permutex_idxmm_lower, zmm5);
                    zmm6_lower          = dmlc_own_mm512_bsrli_epi128(zmm6_lower, 1u);
                    __m512i zmm6_higher = dmlc_own_mm512_bslli_epi128(zmm5, shift_higher);
                    zmm6_higher         = _mm512_mask_mov_epi8(zmm6_higher, 0x7FFF7FFF7FFF7FFF, zmm6_lower);
                    zmm0                = _mm512_load_si512((const __m512i *)(src + 192u));
                    __m512i zmm7_lower  = _mm512_permutex2var_epi16(zmm5, permutex_idxmm_lower, zmm0);
                    zmm7_lower          = dmlc_own_mm512_bsrli_epi128(zmm7_lower, 1u);
                    __m512i zmm7_higher = dmlc_own_mm512_bslli_epi128(zmm0, shift_higher);
                    zmm7_higher         = _mm512_mask_mov_epi8(zmm7_higher, 0x7FFF7FFF7FFF7FFF, zmm7_lower);
                    _mm512_store_si512((__m512i *)dst, zmm2_higher);
                    _mm512_store_si512((__m512i *)(dst + 64u), zmm4_higher);
                    _mm512_store_si512((__m512i *)(dst + 128u), zmm6_higher);
                    _mm512_store_si512((__m512i *)(dst + 192u), zmm7_higher);
                    src += 256u;
                    dst += 256u;
                    length_512u -= 4u;
                }

                src -= 64u - shift;
            }
            else
            {
                dmlc_own_px_copy_8u_unrolled(src, dst, transfer_size);
                return;
            }
        }
        else
        {
            while (length_512u > 3u)
            {
                __m512i zmm0 = _mm512_loadu_si512((const __m512i *)src);
                __m512i zmm1 = _mm512_loadu_si512((const __m512i *)(src + 64u));
                __m512i zmm2 = _mm512_loadu_si512((const __m512i *)(src + 128u));
                __m512i zmm3 = _mm512_loadu_si512((const __m512i *)(src + 192u));
                _mm512_store_si512((__m512i *)dst, zmm0);
                _mm512_store_si512((__m512i *)(dst + 64u), zmm1);
                _mm512_store_si512((__m512i *)(dst + 128u), zmm2);
                _mm512_store_si512((__m512i *)(dst + 192u), zmm3);
                src += 256u;
                dst += 256u;
                length_512u -= 4;
            }
        }
        while (length_512u > 0u)
        {
            __m512i zmm0 = _mm512_loadu_si512((const __m512i *)src);
            _mm512_store_si512((__m512i *)dst, zmm0);
            src += 64u;
            dst += 64u;
            --length_512u;
        }

        dmlc_own_px_copy_8u_unrolled(src, dst, tail);

        return;
    }

    uint32_t length_512u = transfer_size / sizeof(__m512i);
    uint32_t tail        = transfer_size % sizeof(__m512i);

    if (align_src < 64u)
    {
        if (transfer_size < 32 * kilobyte)
        {
            dmlc_own_px_copy_8u_unrolled(src, dst, transfer_size);
            return;
        }

        uint32_t shift = 64 - align_src;

        if (0u == (shift & 3u))
        {
            src -= shift;
            __mmask64 skip_mask = ~((1llu << shift) - 1u);
            __m512i   zmm0      = _mm512_maskz_loadu_epi8(skip_mask, (const __m512i *)src);
            src += 64u;

            while (length_512u > 4u)
            {
                __m512i zmm1 = _mm512_load_si512((const __m512i *)src);
                __m512i zmm2 = dmlc_own_mm512_alignr_epi8(zmm1, zmm0, shift);
                __m512i zmm3 = _mm512_load_si512((const __m512i *)(src + 64u));
                __m512i zmm4 = dmlc_own_mm512_alignr_epi8(zmm3, zmm1, shift);
                __m512i zmm5 = _mm512_load_si512((const __m512i *)(src + 128u));
                __m512i zmm6 = dmlc_own_mm512_alignr_epi8(zmm5, zmm3, shift);
                zmm0         = _mm512_load_si512((const __m512i *)(src + 192u));
                __m512i zmm7 = dmlc_own_mm512_alignr_epi8(zmm0, zmm5, shift);
                _mm512_store_si512((__m512i *)dst, zmm2);
                _mm512_store_si512((__m512i *)(dst + 64u), zmm4);
                _mm512_store_si512((__m512i *)(dst + 128u), zmm6);
                _mm512_store_si512((__m512i *)(dst + 192u), zmm7);
                src += 256u;
                dst += 256u;
                length_512u -= 4u;
            }

            src -= 64u - shift;
        }
        else if (0u == (shift & 1u))
        {
            src -= shift;
            __mmask64 skip_mask = ~((1llu << shift) - 1u);
            __m512i   zmm0      = _mm512_maskz_loadu_epi8(skip_mask, (const __m512i *)src);
            src += 64u;

            __m512i permutex_idxmm = _mm512_load_si512(permutex_idx_pptr[(shift - 2) / 2]);

            while (length_512u > 4u)
            {
                __m512i zmm1 = _mm512_load_si512((const __m512i *)src);
                __m512i zmm2 = _mm512_permutex2var_epi16(zmm0, permutex_idxmm, zmm1);
                __m512i zmm3 = _mm512_load_si512((const __m512i *)(src + 64u));
                __m512i zmm4 = _mm512_permutex2var_epi16(zmm1, permutex_idxmm, zmm3);
                __m512i zmm5 = _mm512_load_si512((const __m512i *)(src + 128u));
                __m512i zmm6 = _mm512_permutex2var_epi16(zmm3, permutex_idxmm, zmm5);
                zmm0         = _mm512_load_si512((const __m512i *)(src + 192u));
                __m512i zmm7 = _mm512_permutex2var_epi16(zmm5, permutex_idxmm, zmm0);
                _mm512_store_si512((__m512i *)dst, zmm2);
                _mm512_store_si512((__m512i *)(dst + 64u), zmm4);
                _mm512_store_si512((__m512i *)(dst + 128u), zmm6);
                _mm512_store_si512((__m512i *)(dst + 192u), zmm7);
                src += 256u;
                dst += 256u;
                length_512u -= 4u;
            }

            src -= 64u - shift;
        }
        else if (shift < 16u)
        {
            src -= shift;
            __mmask64 skip_mask = ~((1llu << shift) - 1u);
            __m512i   zmm0      = _mm512_maskz_loadu_epi8(skip_mask, (const __m512i *)src);
            src += 64u;

            __m512i permutex_idxmm_higher = _mm512_load_si512(permutex_idx_pptr[(shift - 1) / 2]);

            while (length_512u > 4u)
            {
                __m512i zmm1        = _mm512_load_si512((const __m512i *)src);
                __m512i zmm2_lower  = dmlc_own_mm512_bsrli_epi128(zmm0, shift);
                __m512i zmm2_higher = _mm512_permutex2var_epi16(zmm0, permutex_idxmm_higher, zmm1);
                zmm2_higher         = dmlc_own_mm512_bslli_epi128(zmm2_higher, 1u);
                zmm2_higher         = _mm512_mask_mov_epi8(zmm2_higher, 0x0001000100010001, zmm2_lower);
                __m512i zmm3        = _mm512_load_si512((const __m512i *)(src + 64u));
                __m512i zmm4_lower  = dmlc_own_mm512_bsrli_epi128(zmm1, shift);
                __m512i zmm4_higher = _mm512_permutex2var_epi16(zmm1, permutex_idxmm_higher, zmm3);
                zmm4_higher         = dmlc_own_mm512_bslli_epi128(zmm4_higher, 1u);
                zmm4_higher         = _mm512_mask_mov_epi8(zmm4_higher, 0x0001000100010001, zmm4_lower);
                __m512i zmm5        = _mm512_load_si512((const __m512i *)(src + 128u));
                __m512i zmm6_lower  = dmlc_own_mm512_bsrli_epi128(zmm3, shift);
                __m512i zmm6_higher = _mm512_permutex2var_epi16(zmm3, permutex_idxmm_higher, zmm5);
                zmm6_higher         = dmlc_own_mm512_bslli_epi128(zmm6_higher, 1u);
                zmm6_higher         = _mm512_mask_mov_epi8(zmm6_higher, 0x0001000100010001, zmm6_lower);
                zmm0                = _mm512_load_si512((const __m512i *)(src + 192u));
                __m512i zmm7_lower  = dmlc_own_mm512_bsrli_epi128(zmm5, shift);
                __m512i zmm7_higher = _mm512_permutex2var_epi16(zmm5, permutex_idxmm_higher, zmm0);
                zmm7_higher         = dmlc_own_mm512_bslli_epi128(zmm7_higher, 1u);
                zmm7_higher         = _mm512_mask_mov_epi8(zmm7_higher, 0x0001000100010001, zmm7_lower);
                _mm512_store_si512((__m512i *)dst, zmm2_higher);
                _mm512_store_si512((__m512i *)(dst + 64u), zmm4_higher);
                _mm512_store_si512((__m512i *)(dst + 128u), zmm6_higher);
                _mm512_store_si512((__m512i *)(dst + 192u), zmm7_higher);
                src += 256u;
                dst += 256u;
                length_512u -= 4u;
            }

            src -= 64u - shift;
        }
        else if (shift > 48u)
        {
            src -= shift;
            __mmask64 skip_mask = ~((1llu << shift) - 1u);
            __m512i   zmm0      = _mm512_maskz_loadu_epi8(skip_mask, (const __m512i *)src);
            src += 64u;

            __m512i  permutex_idxmm_lower = _mm512_load_si512(permutex_idx_pptr[(shift - 3) / 2]);
            uint32_t shift_higher         = 64u - shift;

            while (length_512u > 4u)
            {
                __m512i zmm1        = _mm512_load_si512((const __m512i *)src);
                __m512i zmm2_lower  = _mm512_permutex2var_epi16(zmm0, permutex_idxmm_lower, zmm1);
                zmm2_lower          = dmlc_own_mm512_bsrli_epi128(zmm2_lower, 1u);
                __m512i zmm2_higher = dmlc_own_mm512_bslli_epi128(zmm1, shift_higher);
                zmm2_higher         = _mm512_mask_mov_epi8(zmm2_higher, 0x7FFF7FFF7FFF7FFF, zmm2_lower);
                __m512i zmm3        = _mm512_load_si512((const __m512i *)(src + 64u));
                __m512i zmm4_lower  = _mm512_permutex2var_epi16(zmm1, permutex_idxmm_lower, zmm3);
                zmm4_lower          = dmlc_own_mm512_bsrli_epi128(zmm4_lower, 1u);
                __m512i zmm4_higher = dmlc_own_mm512_bslli_epi128(zmm3, shift_higher);
                zmm4_higher         = _mm512_mask_mov_epi8(zmm4_higher, 0x7FFF7FFF7FFF7FFF, zmm4_lower);
                __m512i zmm5        = _mm512_load_si512((const __m512i *)(src + 128u));
                __m512i zmm6_lower  = _mm512_permutex2var_epi16(zmm3, permutex_idxmm_lower, zmm5);
                zmm6_lower          = dmlc_own_mm512_bsrli_epi128(zmm6_lower, 1u);
                __m512i zmm6_higher = dmlc_own_mm512_bslli_epi128(zmm5, shift_higher);
                zmm6_higher         = _mm512_mask_mov_epi8(zmm6_higher, 0x7FFF7FFF7FFF7FFF, zmm6_lower);
                zmm0                = _mm512_load_si512((const __m512i *)(src + 192u));
                __m512i zmm7_lower  = _mm512_permutex2var_epi16(zmm5, permutex_idxmm_lower, zmm0);
                zmm7_lower          = dmlc_own_mm512_bsrli_epi128(zmm7_lower, 1u);
                __m512i zmm7_higher = dmlc_own_mm512_bslli_epi128(zmm0, shift_higher);
                zmm7_higher         = _mm512_mask_mov_epi8(zmm7_higher, 0x7FFF7FFF7FFF7FFF, zmm7_lower);
                _mm512_store_si512((__m512i *)dst, zmm2_higher);
                _mm512_store_si512((__m512i *)(dst + 64u), zmm4_higher);
                _mm512_store_si512((__m512i *)(dst + 128u), zmm6_higher);
                _mm512_store_si512((__m512i *)(dst + 192u), zmm7_higher);
                src += 256u;
                dst += 256u;
                length_512u -= 4u;
            }

            src -= 64u - shift;
        }
        else
        {
            dmlc_own_px_copy_8u_unrolled(src, dst, transfer_size);
            return;
        }
    }
    else
    {
        if (((12 * kilobyte) < transfer_size) && (transfer_size < (32 * kilobyte)))
        {
            dmlc_own_px_copy_8u_unrolled(src, dst, transfer_size);
            return;
        }
        while (length_512u > 3u)
        {
            __m512i zmm0 = _mm512_load_si512((const __m512i *)src);
            __m512i zmm1 = _mm512_load_si512((const __m512i *)(src + 64u));
            __m512i zmm2 = _mm512_load_si512((const __m512i *)(src + 128u));
            __m512i zmm3 = _mm512_load_si512((const __m512i *)(src + 192u));
            _mm512_store_si512((__m512i *)dst, zmm0);
            _mm512_store_si512((__m512i *)(dst + 64u), zmm1);
            _mm512_store_si512((__m512i *)(dst + 128u), zmm2);
            _mm512_store_si512((__m512i *)(dst + 192u), zmm3);
            src += 256u;
            dst += 256u;
            length_512u -= 4;
        }
    }

    while (length_512u > 0u)
    {
        __m512i zmm0 = _mm512_loadu_si512((const __m512i *)src);
        _mm512_store_si512((__m512i *)dst, zmm0);
        src += 64u;
        dst += 64u;
        --length_512u;
    }

    dmlc_own_px_copy_8u_unrolled(src, dst, tail);
}

void dml_avx512_mem_move(const uint8_t *src, uint8_t *dst, uint32_t transfer_size)
{
    const uint8_t *const src_begin = src;
    const uint8_t *const src_end   = src + transfer_size;
    const uint8_t *const dst_begin = dst;
    const uint8_t *const dst_end   = dst + transfer_size;

    /*
     * Either:
     * src: |-------|
     * dst:          |-------|
     *
     * OR:
     * src:          |-------|
     * dst: |-------|
     *
     * Assume ranges are exclusive, then equality is taken into account
     *
     * Copy is safe
     */
    if (src_end <= dst_begin || src_begin >= dst_end)
    {
        copy_avx512(src, dst, transfer_size);
    }
    /*
     * Fallback to move
     */
    else
    {
        dml_ref_mem_move(src, dst, transfer_size);
    }
}
