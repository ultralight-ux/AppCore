#include "blur.hlsli"

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
int ShaderPass() { return Integer4[0][0]; } // Current shader pass (0 = first, 1 = second)
int AlphaOnly() { return Integer4[0][1]; } // If true, only blur alpha channel

// Two-pass Gaussian blur, first pass horizontal, second pass vertical
float4 main(VS_OUTPUT input) : SV_Target {
    float4 color;

    if (AlphaOnly()) {
        color = float4(blurAlpha(input, BlurRadius()));
    } else {
        color = blur(input, BlurRadius());
    }

    if (ShaderPass() == 1) {
        // Second pass, apply vertex color to the blurred result
        color *= input.color;
    }

    return color;
}