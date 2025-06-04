#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct type_Uniforms
{
    float4 State;
    float4x4 Transform;
    int4 Integer4[2];
    float4 Scalar4[2];
    float4 Vector[8];
    int4 ClipData;
    float4x4 Clip[8];
};

struct spvDescriptorSetBuffer0
{
    constant type_Uniforms* Uniforms [[id(0)]];
};

struct main0_out
{
    float4 out_var_COLOR0 [[user(locn0)]];
    float2 out_var_TEXCOORD0 [[user(locn1)]];
    float2 out_var_TEXCOORD1 [[user(locn2)]];
    float4 out_var_COLOR1 [[user(locn3)]];
    float4 out_var_COLOR2 [[user(locn4)]];
    float4 out_var_COLOR3 [[user(locn5)]];
    float4 out_var_COLOR4 [[user(locn6)]];
    float4 out_var_COLOR5 [[user(locn7)]];
    float4 out_var_COLOR6 [[user(locn8)]];
    float4 out_var_COLOR7 [[user(locn9)]];
    float4 gl_Position [[position]];
};

struct main0_in
{
    float2 in_var_POSITION [[attribute(0)]];
    uint4 in_var_COLOR0 [[attribute(1)]];
    float2 in_var_TEXCOORD0 [[attribute(2)]];
    float2 in_var_TEXCOORD1 [[attribute(3)]];
    float4 in_var_COLOR1 [[attribute(4)]];
    float4 in_var_COLOR2 [[attribute(5)]];
    float4 in_var_COLOR3 [[attribute(6)]];
    float4 in_var_COLOR4 [[attribute(7)]];
    float4 in_var_COLOR5 [[attribute(8)]];
    float4 in_var_COLOR6 [[attribute(9)]];
    float4 in_var_COLOR7 [[attribute(10)]];
};

vertex main0_out main0(main0_in in [[stage_in]], constant spvDescriptorSetBuffer0& spvDescriptorSet0 [[buffer(0)]])
{
    main0_out out = {};
    out.gl_Position = (*spvDescriptorSet0.Uniforms).Transform * float4(in.in_var_POSITION, 0.0, 1.0);
    out.out_var_COLOR0 = float4(in.in_var_COLOR0) * float4(0.0039215688593685626983642578125);
    out.out_var_TEXCOORD0 = in.in_var_TEXCOORD0;
    out.out_var_TEXCOORD1 = in.in_var_TEXCOORD1;
    out.out_var_COLOR1 = in.in_var_COLOR1;
    out.out_var_COLOR2 = in.in_var_COLOR2;
    out.out_var_COLOR3 = in.in_var_COLOR3;
    out.out_var_COLOR4 = in.in_var_COLOR4;
    out.out_var_COLOR5 = in.in_var_COLOR5;
    out.out_var_COLOR6 = in.in_var_COLOR6;
    out.out_var_COLOR7 = in.in_var_COLOR7;
    return out;
}

