#include <metal_stdlib>
using namespace metal;

fragment float4 main0(constant float4& clearColor [[buffer(0)]]) {
    // Simply output the clear color passed as a constant
    return clearColor;
}