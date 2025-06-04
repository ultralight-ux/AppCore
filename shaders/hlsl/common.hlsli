#ifndef COMMON_HLSLI
#define COMMON_HLSLI

// Uniform buffer - must match C++ structure exactly
// Designed for cross-platform 16-byte alignment compatibility
cbuffer Uniforms : register(b0) {
    float4 State;           // time, screenWidth, screenHeight, screenScale
    float4x4 Transform;     // Model-view-projection matrix
    int4 Integer4[2];       // 8 integer values for shader parameters
    float4 Scalar4[2];      // 8 scalar values for shader parameters
    float4 Vector[8];       // 8 vector values for colors, gradients, etc.
    int4 ClipData;          // x = ClipSize, yzw = reserved for future use
    float4x4 Clip[8];       // Clip region matrices
};

// Texture and sampler bindings
Texture2D Texture0 : register(t0);
Texture2D Texture1 : register(t1);
SamplerState Sampler0 : register(s0);

// Vertex input/output structures
struct VS_INPUT_PATH {
    float2 pos : POSITION;
    uint4 color : COLOR0;
    float2 obj : TEXCOORD0;
};

struct VS_INPUT_QUAD {
    float2 pos : POSITION;
    uint4 color : COLOR0;
    float2 tex : TEXCOORD0;
    float2 obj : TEXCOORD1;
    float4 data0 : COLOR1;
    float4 data1 : COLOR2;
    float4 data2 : COLOR3;
    float4 data3 : COLOR4;
    float4 data4 : COLOR5;
    float4 data5 : COLOR6;
    float4 data6 : COLOR7;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 tex : TEXCOORD0;
    float2 obj : TEXCOORD1;
    float4 data0 : COLOR1;
    float4 data1 : COLOR2;
    float4 data2 : COLOR3;
    float4 data3 : COLOR4;
    float4 data4 : COLOR5;
    float4 data5 : COLOR6;
    float4 data6 : COLOR7;
};

// Common constants
#define PI 3.14159265359

// Color conversion functions
float3 sRGBToLinear(float3 srgb) {
    return srgb * (srgb * (srgb * 0.305306011 + 0.682171111) + 0.012522878);
}

float3 LinearTosRGB(float3 linearColor) {
    float3 S1 = sqrt(linearColor);
    float3 S2 = sqrt(S1);
    float3 S3 = sqrt(S2);
    return 0.585122381 * S1 + 0.783140355 * S2 - 0.368262736 * S3;
}

#endif // COMMON_HLSLI