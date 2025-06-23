#include "blur.hlsli"

// Gaussian blur filter pixel shader
// Used for ShaderType::FilterBlur

// Blur radius (std deviation, scaled to texture coordinates)
// first pass = horizontal (y will be 0), second pass = vertical (x will be 0)
float2 BlurRadius() { return float2(Scalar(0), Scalar(1)); }

// Texture offset for sampling in the blur shader (in texture coordinates)
float2 TextureOffset() { return float2(Scalar(2), Scalar(3)); }

// Texture extents (in texture coordinates), may be < 1.0, used for clamping.
float2 TextureExtents() { return float2(Scalar(4), Scalar(5)); }

int ShaderPass() { return Integer(0); } // Current shader pass (0 = first, 1 = second)
int AlphaOnly() { return Integer(1); } // If true, only blur alpha channel

// Two-pass Gaussian blur, first pass horizontal, second pass vertical
float4 main(VS_OUTPUT input) : SV_Target {
    float4 color;

    if (AlphaOnly()) {
        float alpha = blurAlpha(input, BlurRadius(), TextureOffset(), TextureExtents());
        color = float4(alpha, alpha, alpha, alpha);
    } else {
        color = blur(input, BlurRadius(), TextureOffset(), TextureExtents());
    }

    if (ShaderPass() == 1) {
        // Second pass, apply vertex color to the blurred result
        color *= input.color;
    }

    return color;
}