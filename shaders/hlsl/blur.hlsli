#include "common.hlsli"

// Gaussian blur helper functions

// Gaussian kernel
#define GAUSSIAN_HALF_WIDTH 11
#define GAUSSIAN_KERNEL_STEP 0.2

// Hardcoded normalized Gaussian kernel (half-width 11)
// Offsets: 0, 0.2, 0.4, ..., 2.0. Note that for a symmetric blur you'd use
// kernel[0] for the center and mirror kernel[1..10] for positive/negative offsets.
static const float GaussianKernel[11] = {
    0.08271846539774,  // offset 0.0
    0.08108053004084,  // offset 0.2
    0.07635876755667,  // offset 0.4
    0.06909227008039,  // offset 0.6
    0.06006593399678,  // offset 0.8
    0.05017128538811,  // offset 1.0
    0.04026339964190,  // offset 1.2
    0.03104515814373,  // offset 1.4
    0.02299881881682,  // offset 1.6
    0.01636987669241,  // offset 1.8
    0.01119472694350   // offset 2.0
};

float4 sampleColorAtRadius(VS_OUTPUT input, float radius, float2 blurRadius) {
    float2 offset = radius * blurRadius;
    float2 sampleCoord = clamp(input.tex + offset, 0.0, 1.0);
    return Texture0.Sample(Sampler0, sampleCoord);
}

float4 blur(VS_OUTPUT input, float2 blurRadius) {
    if (blurRadius.x < 0.001 && blurRadius.y < 0.001) {
        // No blur, return single sample
        return Texture0.Sample(Sampler0, input.tex);
    }

    float4 total = sampleColorAtRadius(input, 0.0, blurRadius) * GaussianKernel[0];

    [unroll]
    for (int i = 1; i < GAUSSIAN_HALF_WIDTH; i++) {
        total += sampleColorAtRadius(input, float(i) * GAUSSIAN_KERNEL_STEP, blurRadius) * GaussianKernel[i];
        total += sampleColorAtRadius(input, -float(i) * GAUSSIAN_KERNEL_STEP, blurRadius) * GaussianKernel[i];
    }

    return total;
}

float sampleAlphaAtRadius(VS_OUTPUT input, float radius, float2 blurRadius) {
    float2 offset = radius * blurRadius;
    float2 sampleCoord = clamp(input.tex + offset, 0.0, 1.0);
    return Texture0.Sample(Sampler0, sampleCoord).a;
}

float blurAlpha(VS_OUTPUT input, float2 blurRadius) {
    if (blurRadius.x < 0.001 && blurRadius.y < 0.001) {
        // No blur, return single sample alpha
        return Texture0.Sample(Sampler0, input.tex).a;
    }

    float total = sampleAlphaAtRadius(input, 0.0, blurRadius) * GaussianKernel[0];

    [unroll]
    for (int i = 1; i < GAUSSIAN_HALF_WIDTH; i++) {
        total += sampleAlphaAtRadius(input, float(i) * GAUSSIAN_KERNEL_STEP, blurRadius) * GaussianKernel[i];
        total += sampleAlphaAtRadius(input, -float(i) * GAUSSIAN_KERNEL_STEP, blurRadius) * GaussianKernel[i];
    }

    return total;
}