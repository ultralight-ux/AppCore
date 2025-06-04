#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wmissing-braces"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

template<typename T, size_t Num>
struct spvUnsafeArray
{
    T elements[Num ? Num : 1];
    
    thread T& operator [] (size_t pos) thread
    {
        return elements[pos];
    }
    constexpr const thread T& operator [] (size_t pos) const thread
    {
        return elements[pos];
    }
    
    device T& operator [] (size_t pos) device
    {
        return elements[pos];
    }
    constexpr const device T& operator [] (size_t pos) const device
    {
        return elements[pos];
    }
    
    constexpr const constant T& operator [] (size_t pos) const constant
    {
        return elements[pos];
    }
    
    threadgroup T& operator [] (size_t pos) threadgroup
    {
        return elements[pos];
    }
    constexpr const threadgroup T& operator [] (size_t pos) const threadgroup
    {
        return elements[pos];
    }
};

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

constant float4 _63 = {};

constant spvUnsafeArray<float2, 9> _64 = spvUnsafeArray<float2, 9>({ float2(0.0), float2(-1.0), float2(0.0, -1.0), float2(1.0, -1.0), float2(-1.0, 0.0), float2(1.0, 0.0), float2(-1.0, 1.0), float2(0.0, 1.0), float2(1.0) });

struct main0_out
{
    float4 out_var_SV_Target [[color(0)]];
};

struct main0_in
{
    float4 in_var_COLOR0 [[user(locn0)]];
    float2 in_var_TEXCOORD0 [[user(locn1)]];
};

fragment main0_out main0(main0_in in [[stage_in]], constant type_Uniforms& Uniforms [[buffer(0)]], texture2d<float> Texture0 [[texture(0)]], sampler Sampler0 [[sampler(0)]])
{
    main0_out out = {};
    float2 _78 = float2(Uniforms.Scalar4[0].x, Uniforms.Scalar4[0].y) / Uniforms.State.yz;
    float _86 = Uniforms.Scalar4[0].z / precise::max(Uniforms.State.y, Uniforms.State.z);
    float4 _92 = Texture0.sample(Sampler0, in.in_var_TEXCOORD0);
    float4 _93 = _92 * in.in_var_COLOR0;
    float _131;
    if (_86 > 0.001000000047497451305389404296875)
    {
        float2 _106 = in.in_var_TEXCOORD0 - _78;
        float _108;
        float _111;
        _108 = 0.0;
        _111 = 0.0;
        for (int _113 = 0; _113 < 9; )
        {
            float _121 = (_113 == 0) ? 2.0 : 1.0;
            _108 += _121;
            _111 += (Texture0.sample(Sampler0, fast::clamp(_106 + (_64[_113] * _86), float2(0.0), float2(1.0))).w * _121);
            _113++;
            continue;
        }
        _131 = _111 / _108;
    }
    else
    {
        _131 = Texture0.sample(Sampler0, fast::clamp(in.in_var_TEXCOORD0 + (-_78), float2(0.0), float2(1.0))).w;
    }
    float4 _132 = Uniforms.Vector[0] * _131;
    float _135 = _93.w;
    float _136 = 1.0 - _135;
    float3 _138 = _93.xyz + (_132.xyz * _136);
    float4 _139 = float4(_138.x, _138.y, _138.z, _63.w);
    _139.w = _135 + (_132.w * _136);
    out.out_var_SV_Target = _139;
    return out;
}

