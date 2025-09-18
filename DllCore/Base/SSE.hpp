#pragma once
// SSE4.2
#include <nmmintrin.h>

#define MY_SHUFFLE(fp0,fp1,fp2,fp3) (((fp3) << 6) | ((fp2) << 4) | ((fp1) << 2) | ((fp0)))
// unused xmm register location
#define my_unused_xmm 0
