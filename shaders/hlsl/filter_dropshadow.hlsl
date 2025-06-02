#include "common.hlsli"
#include "clip.hlsli"

// Drop shadow filter pixel shader
// Used for ShaderType::FilterDropShadow

// Helper functions to access shader parameters
float Scalar(int i) { 
    if (i < 4) return Scalar4[0][i]; 
    else return Scalar4[1][i - 4]; 
}

// Drop shadow parameters from uniforms
float2 ShadowOffset() { return float2(Scalar(0), Scalar(1)); }  // Shadow offset in pixels
float ShadowBlurRadius() { return Scalar(2); }                  // Blur radius for the shadow
float4 ShadowColor() { return Vector[0]; }                      // Shadow color and alpha

// Sample the alpha channel with offset for shadow
float sampleAlphaAtOffset(float2 texCoord, float2 offset) {
    float2 sampleCoord = texCoord + offset;
    
    // Clamp to texture bounds
    sampleCoord = clamp(sampleCoord, 0.0, 1.0);
    
    // Sample only the alpha channel for the shadow
    return Texture0.Sample(Sampler0, sampleCoord).a;
}

// Simple box blur for the shadow (faster than Gaussian for drop shadows)
float blurShadowAlpha(float2 texCoord, float blurRadius) {
    float totalAlpha = 0.0;
    float totalWeight = 0.0;
    
    // Define sampling pattern - 9 samples for reasonable quality
    const int sampleCount = 9;
    const float2 offsets[9] = {
        float2(0, 0),
        float2(-1, -1), float2(0, -1), float2(1, -1),
        float2(-1, 0),                  float2(1, 0),
        float2(-1, 1),  float2(0, 1),  float2(1, 1)
    };
    
    // Sample in a pattern around the center
    for (int i = 0; i < sampleCount; i++) {
        float2 sampleOffset = offsets[i] * blurRadius;
        float weight = (i == 0) ? 2.0 : 1.0; // Center sample has more weight
        
        totalAlpha += sampleAlphaAtOffset(texCoord, sampleOffset) * weight;
        totalWeight += weight;
    }
    
    return totalAlpha / totalWeight;
}

float4 main(VS_OUTPUT input) : SV_Target {
    // Get shadow parameters
    float2 shadowOffset = ShadowOffset() / State.yz; // Convert pixel offset to UV space
    float blurRadius = ShadowBlurRadius() / max(State.y, State.z); // Convert to UV space
    float4 shadowColor = ShadowColor();
    
    // Sample the original texture
    float4 originalColor = Texture0.Sample(Sampler0, input.tex) * input.color;
    
    // Calculate shadow alpha at offset position
    float shadowAlpha = 0.0;
    
    if (blurRadius > 0.001) {
        // Blur the shadow if radius is specified
        shadowAlpha = blurShadowAlpha(input.tex - shadowOffset, blurRadius);
    } else {
        // Hard shadow without blur
        shadowAlpha = sampleAlphaAtOffset(input.tex, -shadowOffset);
    }
    
    // Apply shadow color and alpha
    float4 shadow = shadowColor * shadowAlpha;
    
    // Composite: shadow underneath, original on top
    // This uses the standard Porter-Duff "over" operation
    float4 result;
    result.rgb = originalColor.rgb + shadow.rgb * (1.0 - originalColor.a);
    result.a = originalColor.a + shadow.a * (1.0 - originalColor.a);
    
    return result;
}