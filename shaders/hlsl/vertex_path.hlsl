#include "common.hlsli"

// Path rendering vertex shader (simple vertex format)
// Used for ShaderType::FillPath

VS_OUTPUT main(VS_INPUT_PATH input) {
    VS_OUTPUT output;
    
    // Transform position from 2D to clip space
    output.position = mul(Transform, float4(input.pos, 0.0, 1.0));
    
    // Normalize color from UINT format (0-255 range to 0.0-1.0 range)
    output.color = float4(input.color) / 255.0;
    
    // Pass through object coordinates for clipping
    output.obj = input.obj;
    
    // Path shader doesn't use texture coordinates or extra data
    output.tex = float2(0.0, 0.0);
    output.data0 = float4(0.0, 0.0, 0.0, 0.0);
    output.data1 = float4(0.0, 0.0, 0.0, 0.0);
    output.data2 = float4(0.0, 0.0, 0.0, 0.0);
    output.data3 = float4(0.0, 0.0, 0.0, 0.0);
    output.data4 = float4(0.0, 0.0, 0.0, 0.0);
    output.data5 = float4(0.0, 0.0, 0.0, 0.0);
    output.data6 = float4(0.0, 0.0, 0.0, 0.0);
    
    return output;
}