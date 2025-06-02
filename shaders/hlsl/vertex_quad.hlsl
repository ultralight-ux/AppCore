#include "common.hlsli"

// Quad rendering vertex shader (complex vertex format)
// Used for ShaderType::Fill, FilterBasic, FilterBlur, FilterDropShadow

VS_OUTPUT main(VS_INPUT_QUAD input) {
    VS_OUTPUT output;
    
    // Transform position from 2D to clip space
    output.position = mul(Transform, float4(input.pos, 0.0, 1.0));
    
    // Normalize color from 0-255 to 0-1 range
    output.color = input.color / 255.0;
    
    // Pass through texture coordinates
    output.tex = input.tex;
    
    // Pass through object coordinates for clipping
    output.obj = input.obj;
    
    // Pass through all additional data
    output.data0 = input.data0;
    output.data1 = input.data1;
    output.data2 = input.data2;
    output.data3 = input.data3;
    output.data4 = input.data4;
    output.data5 = input.data5;
    output.data6 = input.data6;
    
    return output;
}