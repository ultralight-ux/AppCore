// Generated shader header for filter_dropshadow_fs
// Generated from: filter_dropshadow.glsl

#pragma once

static const char* filter_dropshadow_fs_source = R"GLSL(#version 330

vec4 _63;
const vec2 _64[9] = vec2[](vec2(0.0), vec2(-1.0), vec2(0.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 0.0), vec2(1.0, 0.0), vec2(-1.0, 1.0), vec2(0.0, 1.0), vec2(1.0));

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
    vec2 _78 = vec2(Uniforms.Scalar4[0].x, Uniforms.Scalar4[0].y) / Uniforms.State.yz;
    float _86 = Uniforms.Scalar4[0].z / (isnan(Uniforms.State.z) ? Uniforms.State.y : (isnan(Uniforms.State.y) ? Uniforms.State.z : max(Uniforms.State.y, Uniforms.State.z)));
    vec4 _92 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0);
    vec4 _93 = _92 * in_var_COLOR0;
    float _131;
    if (_86 > 0.001000000047497451305389404296875)
    {
        vec2 _106 = in_var_TEXCOORD0 - _78;
        float _108;
        float _111;
        _108 = 0.0;
        _111 = 0.0;
        for (int _113 = 0; _113 < 9; )
        {
            float _121 = (_113 == 0) ? 2.0 : 1.0;
            _108 += _121;
            _111 += (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp(_106 + (_64[_113] * _86), vec2(0.0), vec2(1.0))).w * _121);
            _113++;
            continue;
        }
        _131 = _111 / _108;
    }
    else
    {
        _131 = texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp(in_var_TEXCOORD0 + (-_78), vec2(0.0), vec2(1.0))).w;
    }
    vec4 _132 = Uniforms.Vector[0] * _131;
    float _135 = _93.w;
    float _136 = 1.0 - _135;
    vec3 _138 = _93.xyz + (_132.xyz * _136);
    vec4 _139 = vec4(_138.x, _138.y, _138.z, _63.w);
    _139.w = _135 + (_132.w * _136);
    out_var_SV_Target = _139;
}

)GLSL";