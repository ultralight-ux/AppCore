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

float4 blur(VS_OUTPUT input, float2 blurRadius, float2 texOffset, float2 texExtents)
{
    if (blurRadius.x < 0.001 && blurRadius.y < 0.001)
    {
        return Texture0.Sample(Sampler0, input.tex + texOffset);
    }
    
    float4 total = float4(0, 0, 0, 0);
    float totalWeight = 0.0;
    
    // Center sample
    float2 centerCoord = input.tex + texOffset;
    if (centerCoord.x >= 0.0 && centerCoord.y >= 0.0 &&
        centerCoord.x <= texExtents.x && centerCoord.y <= texExtents.y)
    {
        total += Texture0.Sample(Sampler0, centerCoord) * GaussianKernel[0];
        totalWeight += GaussianKernel[0];
    }
    
    [unroll]
    for (int i = 1; i < GAUSSIAN_HALF_WIDTH; i++)
    {
        float2 offset1 = float(i) * GAUSSIAN_KERNEL_STEP * blurRadius;
        float2 offset2 = -offset1;
        
        float2 coord1 = input.tex + offset1 + texOffset;
        float2 coord2 = input.tex + offset2 + texOffset;
        
        // Only add samples that are within bounds
        if (coord1.x >= 0.0 && coord1.y >= 0.0 &&
            coord1.x <= texExtents.x && coord1.y <= texExtents.y)
        {
            total += Texture0.Sample(Sampler0, coord1) * GaussianKernel[i];
            totalWeight += GaussianKernel[i];
        }
        
        if (coord2.x >= 0.0 && coord2.y >= 0.0 &&
            coord2.x <= texExtents.x && coord2.y <= texExtents.y)
        {
            total += Texture0.Sample(Sampler0, coord2) * GaussianKernel[i];
            totalWeight += GaussianKernel[i];
        }
    }
    
    // Normalize by actual weight
    return totalWeight > 0.0 ? total / totalWeight : float4(0, 0, 0, 0);
}

float blurAlpha(VS_OUTPUT input, float2 blurRadius, float2 texOffset, float2 texExtents)
{
    if (blurRadius.x < 0.001 && blurRadius.y < 0.001)
    {
        // No blur, return single sample alpha
        float2 coord = input.tex + texOffset;
        if (coord.x >= 0.0 && coord.y >= 0.0 && coord.x <= texExtents.x && coord.y <= texExtents.y)
        {
            return Texture0.Sample(Sampler0, coord).a;
        }
        return 0.0;
    }
    
    float total = 0.0;
    float totalWeight = 0.0;
    
    // Center sample
    float2 centerCoord = input.tex + texOffset;
    if (centerCoord.x >= 0.0 && centerCoord.y >= 0.0 &&
        centerCoord.x <= texExtents.x && centerCoord.y <= texExtents.y)
    {
        total += Texture0.Sample(Sampler0, centerCoord).a * GaussianKernel[0];
        totalWeight += GaussianKernel[0];
    }
    
    [unroll]
    for (int i = 1; i < GAUSSIAN_HALF_WIDTH; i++)
    {
        float radius = float(i) * GAUSSIAN_KERNEL_STEP;
        float2 offset = radius * blurRadius;
        
        // Positive offset
        float2 coord1 = input.tex + offset + texOffset;
        if (coord1.x >= 0.0 && coord1.y >= 0.0 &&
            coord1.x <= texExtents.x && coord1.y <= texExtents.y)
        {
            total += Texture0.Sample(Sampler0, coord1).a * GaussianKernel[i];
            totalWeight += GaussianKernel[i];
        }
        
        // Negative offset
        float2 coord2 = input.tex - offset + texOffset;
        if (coord2.x >= 0.0 && coord2.y >= 0.0 &&
            coord2.x <= texExtents.x && coord2.y <= texExtents.y)
        {
            total += Texture0.Sample(Sampler0, coord2).a * GaussianKernel[i];
            totalWeight += GaussianKernel[i];
        }
    }
    
    // Normalize by actual weight
    return totalWeight > 0.0 ? total / totalWeight : 0.0;
}
