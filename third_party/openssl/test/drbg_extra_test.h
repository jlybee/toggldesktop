/*
 * Copyright 2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * Self generated known answer test (KATs) for NIST SP800-90A AES CTR_DRBG.
 * Test overflow of V after |ngen| generate operation.
 */

#include <stddef.h>

#ifndef DRBG_EXTRA_TEST_H
# define DRBG_EXTRA_TEST_H

# define BUFFSIZE        512
# define RESEEDINTERVAL  ((1 << 24) - 1)

struct drbg_extra_kat {
    int nid;
    const unsigned char *expected;
    unsigned long long ngen;
};

static const unsigned char expected128[BUFFSIZE] = {
    0x39, 0x10, 0x06, 0x64, 0x68, 0x7d, 0x77, 0x64, 0xf9, 0xb8, 0xe8, 0x3c,
    0x55, 0xb0, 0xf1, 0x8c, 0xeb, 0x11, 0x23, 0x50, 0x15, 0x28, 0x44, 0x38,
    0x84, 0x47, 0x7b, 0xc2, 0x78, 0x16, 0x73, 0x57, 0x9f, 0x01, 0x57, 0xf0,
    0x17, 0x14, 0xa3, 0xa8, 0xdf, 0x3b, 0x87, 0x78, 0x37, 0xf8, 0x53, 0x23,
    0x18, 0xd2, 0xbc, 0x82, 0xc5, 0xed, 0x51, 0x84, 0x30, 0x14, 0xaa, 0x01,
    0xb8, 0xb5, 0xed, 0x33, 0x14, 0xe7, 0x6c, 0xa7, 0x38, 0x02, 0x60, 0x69,
    0x52, 0x0f, 0x5b, 0x7f, 0x4c, 0x82, 0x25, 0x05, 0x10, 0xaa, 0xd5, 0xc6,
    0xac, 0x44, 0x7c, 0xef, 0x0f, 0x69, 0xc7, 0x4d, 0x07, 0x54, 0x83, 0xb5,
    0xf2, 0x05, 0x0f, 0x69, 0xfd, 0x85, 0x8f, 0x63, 0x27, 0xe6, 0xff, 0xa5,
    0x26, 0xd9, 0xf0, 0x8b, 0x31, 0x89, 0xbc, 0x51, 0xb8, 0x62, 0x06, 0x88,
    0xe7, 0x95, 0xa7, 0x69, 0x7d, 0x43, 0x9c, 0x2b, 0x3c, 0x59, 0x9f, 0xa9,
    0x63, 0xa3, 0x80, 0x1b, 0xe9, 0x96, 0x6b, 0xde, 0xf2, 0xdb, 0x0c, 0x07,
    0x22, 0xb1, 0x30, 0xd0, 0x08, 0x66, 0x23, 0x31, 0x63, 0xd2, 0x86, 0xbf,
    0x6e, 0xcb, 0x44, 0xb7, 0x6a, 0x44, 0xac, 0xe6, 0x53, 0x0b, 0x32, 0x20,
    0xb6, 0xcb, 0x10, 0x22, 0x57, 0xa5, 0x7f, 0x7d, 0x9e, 0x83, 0xb9, 0xdb,
    0x27, 0x1b, 0x04, 0xf5, 0x67, 0x94, 0x6e, 0x44, 0xba, 0x77, 0xc2, 0xf6,
    0x6a, 0xcd, 0xfa, 0x71, 0x44, 0x07, 0x5e, 0x09, 0x6f, 0x42, 0x5c, 0x06,
    0x55, 0x4f, 0xae, 0xc7, 0x53, 0x73, 0x89, 0x80, 0x2d, 0x5a, 0x73, 0x3c,
    0x11, 0x8f, 0x76, 0xa1, 0x3b, 0xb6, 0xbe, 0x6b, 0xd2, 0x1f, 0xa7, 0x63,
    0x7c, 0x99, 0x46, 0x3d, 0x6b, 0x0d, 0x2a, 0xfe, 0x10, 0x00, 0x77, 0x43,
    0x5b, 0xd0, 0x52, 0x79, 0x8d, 0x1d, 0xfd, 0xe2, 0x9a, 0xac, 0x5c, 0x77,
    0x69, 0x68, 0xec, 0xf1, 0x92, 0xa8, 0xb7, 0x26, 0xcd, 0x64, 0xef, 0x8f,
    0x55, 0xa0, 0x83, 0x3f, 0x27, 0xf0, 0x34, 0xc3, 0x61, 0x74, 0xf9, 0x44,
    0x2d, 0xe1, 0xb3, 0xad, 0x02, 0xc2, 0x45, 0x40, 0x40, 0xc0, 0x0f, 0x05,
    0x72, 0xdb, 0x71, 0xae, 0x84, 0x4c, 0x3d, 0xd0, 0x61, 0x5d, 0x79, 0x52,
    0x16, 0xb1, 0x02, 0x07, 0xaf, 0xfa, 0x9a, 0xe0, 0x21, 0xbf, 0x1c, 0xc4,
    0x21, 0xf9, 0x76, 0x95, 0xc4, 0x92, 0x57, 0xb7, 0x1e, 0xce, 0xb8, 0xd5,
    0x1c, 0x68, 0x7c, 0xed, 0x8f, 0x45, 0x09, 0x0b, 0xef, 0x30, 0x68, 0xab,
    0x5f, 0x26, 0x4a, 0xf2, 0x8a, 0x55, 0x97, 0x3d, 0x1c, 0xa0, 0x33, 0x20,
    0x10, 0x9e, 0x10, 0x77, 0x93, 0xa2, 0xcb, 0x72, 0x33, 0xf3, 0x6f, 0xdc,
    0x41, 0x46, 0x83, 0xb1, 0x59, 0x22, 0xf0, 0x9b, 0x9f, 0x50, 0x9f, 0x14,
    0x2e, 0xe0, 0xb8, 0xa3, 0xf7, 0x40, 0x9d, 0xe0, 0x76, 0x78, 0xa6, 0xd4,
    0xf6, 0xd4, 0x29, 0x33, 0x97, 0x76, 0x54, 0xdf, 0x3e, 0xf0, 0x82, 0x9f,
    0x4f, 0xf1, 0x0b, 0x0a, 0x61, 0xa2, 0x3f, 0xde, 0xda, 0xfe, 0x93, 0xe1,
    0x51, 0x19, 0x93, 0x8c, 0xc0, 0x07, 0x76, 0xdb, 0x4a, 0x9c, 0xef, 0x42,
    0x5f, 0x7b, 0x5b, 0x2c, 0xff, 0xe7, 0x5f, 0x6d, 0xcb, 0x7e, 0x9b, 0xc9,
    0xf3, 0x94, 0xe9, 0x73, 0xd6, 0xa3, 0x00, 0x7d, 0x11, 0xf8, 0xd6, 0x58,
    0x97, 0x3e, 0xdf, 0xb9, 0xd5, 0x57, 0xde, 0x51, 0x9e, 0x39, 0x5d, 0x59,
    0xe5, 0x50, 0x29, 0x3e, 0x58, 0x90, 0x37, 0xa6, 0xcc, 0x06, 0x89, 0xb1,
    0xa6, 0xa7, 0xef, 0x96, 0x30, 0xf5, 0xc1, 0xea, 0x03, 0xde, 0x46, 0x9a,
    0x5f, 0xa2, 0x5d, 0xb6, 0x74, 0xfd, 0xbf, 0xde, 0x77, 0x8e, 0x3e, 0x90,
    0x55, 0x3b, 0x4d, 0x59, 0xde, 0x19, 0x43, 0xcd, 0x5d, 0x1b, 0xba, 0x29,
    0xa9, 0x59, 0xad, 0xef, 0x2d, 0x9c, 0x3b, 0xc0
};
static const struct drbg_extra_kat drbg_extra_test128 = {
    NID_aes_128_ctr,
    expected128,
    61335566ULL
};

static const unsigned char expected192[BUFFSIZE] = {
    0x19, 0x1b, 0xe3, 0xa9, 0x58, 0xb4, 0xcc, 0x3a, 0x3a, 0x06, 0x30, 0x6a,
    0xdc, 0x8c, 0x78, 0xb2, 0x1e, 0xb5, 0x83, 0xa0, 0x04, 0x72, 0xae, 0xed,
    0xae, 0x88, 0x7c, 0x63, 0x8b, 0xe6, 0x20, 0x9b, 0xb4, 0x04, 0x14, 0xa9,
    0x83, 0xce, 0xb4, 0xd1, 0x69, 0xc2, 0x00, 0xa5, 0xf4, 0x45, 0xdb, 0xa1,
    0xcb, 0xd7, 0xfe, 0xf7, 0x19, 0x5d, 0x4d, 0x26, 0x7d, 0x7a, 0x2f, 0xde,
    0xc1, 0xb3, 0x8e, 0x3e, 0x19, 0xd1, 0xff, 0x25, 0x9b, 0x81, 0x90, 0x11,
    0x71, 0xb6, 0x08, 0x46, 0x76, 0x58, 0xca, 0x78, 0xae, 0xc3, 0x90, 0x29,
    0x07, 0x07, 0x85, 0x9d, 0x90, 0xd9, 0xf5, 0x5a, 0x8d, 0x28, 0x87, 0xd3,
    0x88, 0xd1, 0x14, 0xc1, 0xa8, 0x96, 0xa3, 0x7e, 0x6b, 0xe4, 0x1d, 0xf6,
    0x0d, 0xfb, 0x3d, 0xee, 0xa9, 0x8c, 0x70, 0xc9, 0x3f, 0x87, 0xad, 0x57,
    0xd9, 0xe3, 0x4e, 0xfc, 0x03, 0xeb, 0x79, 0xb8, 0xb9, 0xe9, 0xe4, 0x55,
    0x78, 0x4b, 0xdb, 0xfa, 0xb6, 0x5e, 0x1e, 0x5b, 0xab, 0x11, 0x35, 0x3c,
    0x97, 0x10, 0x0d, 0x7e, 0x68, 0x47, 0x1a, 0xd0, 0x59, 0xac, 0x41, 0x61,
    0x8e, 0xfc, 0xd1, 0x12, 0xf6, 0x81, 0x44, 0x5c, 0x78, 0xe4, 0x32, 0x2f,
    0xa5, 0x4c, 0x6e, 0xf8, 0xbf, 0x35, 0xc2, 0x66, 0xc0, 0x04, 0x77, 0x2e,
    0x1d, 0xe9, 0xc9, 0x8f, 0xca, 0xd7, 0x2a, 0xf0, 0x35, 0x34, 0x3a, 0xf6,
    0x7e, 0x0d, 0xc6, 0xc2, 0x4a, 0xd0, 0x91, 0x5d, 0xf5, 0x47, 0xdb, 0x32,
    0x88, 0x52, 0x98, 0xf0, 0xca, 0xc9, 0x57, 0x34, 0x10, 0xca, 0xfa, 0x9d,
    0x23, 0x61, 0x41, 0x55, 0xc8, 0x65, 0xa4, 0x4e, 0xa5, 0x8b, 0x1a, 0xdc,
    0x14, 0x80, 0x26, 0xe7, 0x72, 0x98, 0x90, 0x14, 0x01, 0x52, 0xa4, 0x9f,
    0x55, 0xe4, 0xaa, 0x15, 0x20, 0xd4, 0x34, 0x3f, 0x36, 0xce, 0x93, 0x5d,
    0x7b, 0x49, 0x29, 0x3f, 0xef, 0x78, 0xe9, 0x2e, 0x4c, 0x87, 0xa1, 0xb6,
    0x28, 0xb7, 0xa3, 0xb4, 0x49, 0x84, 0xf5, 0x5d, 0x24, 0x7d, 0x57, 0x57,
    0x79, 0xf5, 0xa1, 0x67, 0x5b, 0x8c, 0x59, 0x43, 0x3a, 0x1f, 0x10, 0x51,
    0x57, 0xa0, 0xe5, 0x18, 0xba, 0xdd, 0xca, 0x2d, 0xa9, 0xc5, 0xef, 0xad,
    0x7e, 0xd5, 0x17, 0xa3, 0x66, 0xe0, 0x93, 0x00, 0xda, 0xfc, 0x90, 0x89,
    0x64, 0x01, 0x10, 0x6a, 0xe7, 0x51, 0x7e, 0x5c, 0x16, 0x87, 0x4a, 0xc2,
    0x04, 0x27, 0x5c, 0x71, 0xdb, 0xcb, 0xb4, 0x70, 0xfc, 0x5e, 0xfb, 0xce,
    0xba, 0xc8, 0x20, 0x80, 0x5d, 0x73, 0xdb, 0xb4, 0x30, 0x8b, 0xfc, 0xcf,
    0xd1, 0x09, 0xa1, 0x93, 0xa3, 0x3e, 0x86, 0x76, 0x86, 0x5a, 0xa4, 0xba,
    0xda, 0xb8, 0x03, 0x1d, 0x69, 0x99, 0x29, 0xef, 0xe1, 0x7b, 0x6a, 0xd3,
    0x95, 0x9e, 0x98, 0x5a, 0x39, 0xb0, 0xde, 0x53, 0x49, 0x09, 0x7d, 0xdd,
    0x89, 0xed, 0x90, 0xc6, 0xa3, 0x8c, 0xe6, 0x15, 0x2f, 0x5a, 0x47, 0x8a,
    0x66, 0xf7, 0x1f, 0x38, 0x9e, 0xae, 0x9b, 0x46, 0x71, 0x96, 0x21, 0xbf,
    0x9f, 0x8e, 0x60, 0x2c, 0xda, 0xd6, 0x38, 0xf0, 0xc5, 0xe6, 0x27, 0x4b,
    0xfa, 0xc2, 0x13, 0x1a, 0x6b, 0xf5, 0x47, 0x97, 0x2f, 0xd0, 0x34, 0xe7,
    0x71, 0x9f, 0x8b, 0xc7, 0x22, 0xec, 0x97, 0x38, 0xe2, 0x07, 0x2b, 0x02,
    0x76, 0xf7, 0xb5, 0xdc, 0x55, 0xaf, 0x8e, 0xe6, 0x92, 0x57, 0x82, 0x82,
    0xf2, 0x1e, 0x10, 0x37, 0x40, 0xc4, 0x91, 0x25, 0xa7, 0x5c, 0xed, 0x1b,
    0x61, 0xc7, 0xc4, 0x47, 0xac, 0xe5, 0xb8, 0x7b, 0xdc, 0xfc, 0xaa, 0xd4,
    0x0c, 0xcc, 0x93, 0xee, 0x69, 0xd7, 0x35, 0xcf, 0xd7, 0x47, 0x05, 0xfc,
    0x88, 0xeb, 0x29, 0x0b, 0x62, 0x8e, 0x51, 0xb0, 0xd6, 0x71, 0xa0, 0xf2,
    0x17, 0x74, 0x1a, 0x2d, 0x4f, 0xb0, 0xd9, 0x8b
};
static const struct drbg_extra_kat drbg_extra_test192 = {
    NID_aes_192_ctr,
    expected192,
    10132467ULL
};

static const unsigned char expected256[BUFFSIZE] = {
    0x3f, 0x6d, 0x24, 0x4d, 0xe8, 0xdf, 0x93, 0xe5, 0xde, 0x4f, 0xba, 0x25,
    0x9c, 0x97, 0xfa, 0xbb, 0x78, 0x6f, 0xe7, 0xce, 0x74, 0x66, 0xa3, 0x81,
    0xf1, 0x0e, 0xf9, 0xea, 0xb6, 0xfc, 0xc2, 0xc5, 0xf0, 0x25, 0x0d, 0x45,
    0x45, 0x8a, 0x3e, 0xf2, 0x93, 0xf3, 0x2c, 0x41, 0x46, 0x2d, 0x58, 0x56,
    0x7e, 0x6a, 0x5c, 0x8a, 0x51, 0x3f, 0xd1, 0x76, 0x09, 0x6d, 0xf4, 0xf0,
    0x01, 0x8f, 0x60, 0x68, 0x9c, 0x52, 0xb2, 0xb1, 0xd2, 0x3e, 0x07, 0x10,
    0xab, 0x6c, 0xc9, 0x78, 0x22, 0xf4, 0xde, 0xbc, 0xee, 0x23, 0xa9, 0xb5,
    0x81, 0x2c, 0x4c, 0xf7, 0xd6, 0x35, 0x53, 0x1f, 0x66, 0x42, 0x1d, 0x7a,
    0x6d, 0x6e, 0x56, 0x8d, 0xbc, 0x03, 0xac, 0x76, 0x6c, 0x46, 0x4a, 0x80,
    0x87, 0x97, 0x62, 0xc6, 0xe8, 0x21, 0x35, 0xbd, 0x1d, 0x17, 0x28, 0x80,
    0x79, 0x6a, 0xe0, 0xc5, 0x33, 0x51, 0x38, 0xa6, 0xdf, 0x8d, 0xf6, 0xb3,
    0x69, 0x4f, 0x3b, 0xb2, 0xb1, 0x8e, 0x28, 0x8a, 0x3b, 0xba, 0x80, 0x43,
    0x7a, 0x92, 0x5e, 0x11, 0xd7, 0x4a, 0x8d, 0xa5, 0xee, 0x7c, 0x30, 0x69,
    0x7c, 0x27, 0x0f, 0xb8, 0x10, 0xd4, 0x32, 0x5f, 0xad, 0x27, 0xf4, 0xf1,
    0x31, 0xfe, 0x41, 0x08, 0x8c, 0x09, 0xe5, 0x9c, 0x55, 0x97, 0xa5, 0x38,
    0x7f, 0x72, 0x5b, 0x0a, 0xb3, 0x44, 0x2d, 0x4f, 0x65, 0xba, 0x74, 0x0c,
    0x35, 0x2f, 0x57, 0xfb, 0x21, 0x9c, 0x80, 0x2a, 0xd8, 0x0d, 0x56, 0xa7,
    0x99, 0x8b, 0xd9, 0xaf, 0x6f, 0x45, 0x06, 0x94, 0xf5, 0x6f, 0x56, 0x32,
    0x3d, 0x6c, 0xd6, 0x91, 0x30, 0x88, 0xdd, 0x61, 0x79, 0xa8, 0xac, 0x03,
    0xcf, 0x1c, 0x53, 0xd4, 0xdf, 0x6f, 0x39, 0x9c, 0x9f, 0xa9, 0xb5, 0x8c,
    0x8b, 0xc3, 0x20, 0x89, 0x5e, 0xf9, 0x74, 0x0c, 0xda, 0x7b, 0x2a, 0x22,
    0xa4, 0xcc, 0x00, 0x78, 0x66, 0x5e, 0xe5, 0x49, 0x5b, 0x3e, 0xfe, 0x85,
    0xcd, 0x9a, 0xa7, 0xcd, 0xb0, 0xf1, 0x2b, 0xe7, 0x3b, 0x23, 0x09, 0xb2,
    0x26, 0x66, 0x6c, 0x64, 0xd9, 0x35, 0x69, 0xd4, 0xfe, 0xc1, 0x3f, 0xbd,
    0xf3, 0x15, 0x15, 0x83, 0x1f, 0x92, 0x5f, 0x25, 0x0b, 0x1f, 0x54, 0xeb,
    0xfc, 0x40, 0x96, 0xb4, 0x71, 0x2a, 0x3b, 0xa3, 0x94, 0xfa, 0x41, 0xfa,
    0x43, 0x4e, 0xf1, 0x85, 0xc4, 0x2d, 0x40, 0xf0, 0x1a, 0x0d, 0x88, 0xf2,
    0xd4, 0x1f, 0x9d, 0x80, 0x69, 0x56, 0xb1, 0xa4, 0xcb, 0xa1, 0x35, 0x98,
    0xda, 0xa4, 0xa3, 0x6d, 0xd8, 0x01, 0x98, 0xe0, 0x2e, 0x13, 0xc2, 0x7d,
    0x74, 0x6a, 0x62, 0x7a, 0xef, 0x93, 0x65, 0x94, 0x37, 0xa6, 0x5b, 0xcb,
    0x23, 0xe6, 0x23, 0x3d, 0xa5, 0x7e, 0x30, 0x59, 0x36, 0x85, 0xee, 0xfb,
    0xe8, 0x33, 0x3c, 0xf5, 0xc3, 0x4e, 0xf2, 0xc4, 0x70, 0xc8, 0x4f, 0x00,
    0x39, 0x7d, 0x20, 0x7f, 0x72, 0x57, 0xd9, 0xb9, 0xf6, 0xfe, 0xf2, 0x50,
    0x44, 0xed, 0x53, 0xdc, 0xd6, 0xd5, 0x1f, 0x94, 0xea, 0x62, 0x9e, 0x64,
    0x23, 0xd6, 0x6d, 0x8a, 0x6e, 0xfb, 0xc2, 0xb3, 0x7e, 0x72, 0x26, 0xad,
    0x31, 0x30, 0x81, 0xb7, 0x62, 0x10, 0xaf, 0x8e, 0x45, 0x74, 0x6e, 0x8f,
    0x07, 0x5a, 0xfe, 0xe3, 0xa7, 0xe8, 0x47, 0x80, 0x7e, 0x1d, 0xb5, 0x8c,
    0xab, 0xcb, 0x0b, 0x95, 0xb2, 0x95, 0x64, 0x5e, 0xea, 0x42, 0x14, 0x6f,
    0x6b, 0xe4, 0x81, 0x51, 0x78, 0xb4, 0x0e, 0x0c, 0x6e, 0x22, 0x31, 0xb2,
    0x50, 0xd9, 0x78, 0xc6, 0xfa, 0xa4, 0xb6, 0xf4, 0x17, 0x20, 0xbe, 0xe3,
    0x69, 0xfa, 0x6c, 0xb9, 0x98, 0xad, 0xca, 0xba, 0x72, 0xa2, 0x22, 0x4c,
    0x0a, 0xb3, 0x6d, 0x9b, 0x11, 0x8a, 0x59, 0x06, 0xf1, 0xda, 0x43, 0xc2,
    0xae, 0xb0, 0x80, 0x35, 0x86, 0xbf, 0xf0, 0x8c
};
static const struct drbg_extra_kat drbg_extra_test256 = {
    NID_aes_256_ctr,
    expected256,
    40870394ULL
};

static const struct drbg_extra_kat *drbg_extra_test[] = {
    &drbg_extra_test128,
    &drbg_extra_test192,
    &drbg_extra_test256,
};

#endif
