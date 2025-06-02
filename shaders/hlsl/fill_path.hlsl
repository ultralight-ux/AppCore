#include "common.hlsli"
#include "clip.hlsli"

// Path fill pixel shader - simple solid color fill with clipping
// Used for ShaderType::FillPath

float4 main(VS_OUTPUT input) : SV_Target {
    float4 outColor = input.color;
    
    // Apply clipping regions
    applyClip(input.obj, outColor);
    
    return outColor;
}