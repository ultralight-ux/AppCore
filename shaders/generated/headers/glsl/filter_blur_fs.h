// Generated shader header for filter_blur_fs
// Generated from: filter_blur.glsl

#pragma once

static const char* filter_blur_fs_source = R"GLSL(#version 330
#if defined(GL_EXT_control_flow_attributes)
#extension GL_EXT_control_flow_attributes : require
#define SPIRV_CROSS_FLATTEN [[flatten]]
#define SPIRV_CROSS_BRANCH [[dont_flatten]]
#define SPIRV_CROSS_UNROLL [[unroll]]
#define SPIRV_CROSS_LOOP [[dont_unroll]]
#else
#define SPIRV_CROSS_FLATTEN
#define SPIRV_CROSS_BRANCH
#define SPIRV_CROSS_UNROLL
#define SPIRV_CROSS_LOOP
#endif

const float _62[11] = float[](0.0827184617519378662109375, 0.081080533564090728759765625, 0.0763587653636932373046875, 0.069092273712158203125, 0.060065932571887969970703125, 0.0501712858676910400390625, 0.04026339948177337646484375, 0.0310451574623584747314453125, 0.02299881912767887115478515625, 0.01636987738311290740966796875, 0.011194727383553981781005859375);

layout(std140) uniform type_Uniforms
{
    vec4 State;
    layout(row_major) mat4 Transform;
    ivec4 Integer4[2];
    vec4 Scalar4[2];
    vec4 Vector[8];
    ivec4 ClipData;
    layout(row_major) mat4 Clip[8];
} Uniforms;

uniform sampler2D SPIRV_Cross_CombinedTexture0Sampler0;

in vec4 in_var_COLOR0;
in vec2 in_var_TEXCOORD0;
layout(location = 0) out vec4 out_var_SV_Target;

void main()
{
    vec4 _78;
    _78 = (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp(in_var_TEXCOORD0, vec2(0.0), vec2(1.0))) * in_var_COLOR0) * _62[0];
    SPIRV_CROSS_UNROLL
    for (int _81 = 1; _81 < 11; )
    {
        float _85 = float(_81);
        vec2 _91 = vec2(Uniforms.Scalar4[0].x, Uniforms.Scalar4[0].y);
        _78 = (_78 + ((texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp(in_var_TEXCOORD0 + (_91 * (_85 * 0.20000000298023223876953125)), vec2(0.0), vec2(1.0))) * in_var_COLOR0) * _62[_81])) + ((texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp(in_var_TEXCOORD0 + (_91 * (_85 * (-0.20000000298023223876953125))), vec2(0.0), vec2(1.0))) * in_var_COLOR0) * _62[_81]);
        _81++;
        continue;
    }
    out_var_SV_Target = _78;
}

)GLSL";