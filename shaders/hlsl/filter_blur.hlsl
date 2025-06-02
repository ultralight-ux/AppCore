#include "common.hlsli"

// Gaussian blur filter pixel shader
// Used for ShaderType::FilterBlur

// Helper functions to access scalar values
float Scalar(int i) { 
    if (i < 4) return Scalar4[0][i]; 
    else return Scalar4[1][i - 4]; 
}

// Blur radius (std deviation, scaled to texture coordinates)
// first pass = horizontal (y will be 0), second pass = vertical (x will be 0)
float2 BlurRadius() { return float2(Scalar(0), Scalar(1)); }

// Size of the texture in texels
float2 TextureSize() { return float2(Scalar(2), Scalar(3)); }

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

float4 sampleColorAtRadius(VS_OUTPUT input, float radius) {
    float2 offset = radius * BlurRadius();
    float2 sampleCoord = clamp(input.tex + offset, 0.0, 1.0);
    return Texture0.Sample(Sampler0, sampleCoord) * input.color;
}

float4 filterBlur(VS_OUTPUT input) {
    float4 total = sampleColorAtRadius(input, 0.0) * GaussianKernel[0];

    [unroll]
    for (int i = 1; i < GAUSSIAN_HALF_WIDTH; i++) {
        total += sampleColorAtRadius(input, float(i) * GAUSSIAN_KERNEL_STEP) * GaussianKernel[i];
        total += sampleColorAtRadius(input, -float(i) * GAUSSIAN_KERNEL_STEP) * GaussianKernel[i];
    }

    return total;
}

// Two-pass Gaussian blur, first pass horizontal, second pass vertical
float4 main(VS_OUTPUT input) : SV_Target {
    float4 color = filterBlur(input);
    
    return color;
}