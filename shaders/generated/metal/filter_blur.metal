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

struct spvDescriptorSetBuffer0
{
    constant type_Uniforms* Uniforms [[id(0)]];
    texture2d<float> Texture0 [[id(1)]];
    sampler Sampler0 [[id(2)]];
};

constant spvUnsafeArray<float, 11> _62 = spvUnsafeArray<float, 11>({ 0.0827184617519378662109375, 0.081080533564090728759765625, 0.0763587653636932373046875, 0.069092273712158203125, 0.060065932571887969970703125, 0.0501712858676910400390625, 0.04026339948177337646484375, 0.0310451574623584747314453125, 0.02299881912767887115478515625, 0.01636987738311290740966796875, 0.011194727383553981781005859375 });

struct main0_out
{
    float4 out_var_SV_Target [[color(0)]];
};

struct main0_in
{
    float4 in_var_COLOR0 [[user(locn0)]];
    float2 in_var_TEXCOORD0 [[user(locn1)]];
};

fragment main0_out main0(main0_in in [[stage_in]], constant spvDescriptorSetBuffer0& spvDescriptorSet0 [[buffer(0)]])
{
    main0_out out = {};
    float4 _78;
    _78 = (spvDescriptorSet0.Texture0.sample(spvDescriptorSet0.Sampler0, fast::clamp(in.in_var_TEXCOORD0, float2(0.0), float2(1.0))) * in.in_var_COLOR0) * _62[0];
    for (int _81 = 1; _81 < 11; )
    {
        float _85 = float(_81);
        float2 _91 = float2((*spvDescriptorSet0.Uniforms).Scalar4[0].x, (*spvDescriptorSet0.Uniforms).Scalar4[0].y);
        _78 = (_78 + ((spvDescriptorSet0.Texture0.sample(spvDescriptorSet0.Sampler0, fast::clamp(in.in_var_TEXCOORD0 + (_91 * (_85 * 0.20000000298023223876953125)), float2(0.0), float2(1.0))) * in.in_var_COLOR0) * _62[_81])) + ((spvDescriptorSet0.Texture0.sample(spvDescriptorSet0.Sampler0, fast::clamp(in.in_var_TEXCOORD0 + (_91 * (_85 * (-0.20000000298023223876953125))), float2(0.0), float2(1.0))) * in.in_var_COLOR0) * _62[_81]);
        _81++;
        continue;
    }
    out.out_var_SV_Target = _78;
    return out;
}

