#include <metal_stdlib>
using namespace metal;

struct ClearVertexOut {
    float4 position [[position]];
};

vertex ClearVertexOut main0(uint vid [[vertex_id]]) {
    ClearVertexOut out;
    
    // Generate full-screen triangle using vertex ID
    // This creates a triangle that covers the entire screen
    // with just 3 vertices instead of 4 (more efficient than a quad)
    float2 positions[3] = {
        float2(-1.0, -1.0),  // Bottom-left
        float2( 3.0, -1.0),  // Bottom-right (extends past screen)
        float2(-1.0,  3.0)   // Top-left (extends past screen)
    };
    
    out.position = float4(positions[vid], 0.0, 1.0);
    return out;
}