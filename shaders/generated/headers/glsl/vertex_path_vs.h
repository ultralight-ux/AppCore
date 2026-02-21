// Generated shader header for vertex_path_vs
// Generated from: vertex_path.glsl

#pragma once

static const char* vertex_path_vs_source = R"GLSL(#version 420

layout(binding = 0, std140) uniform type_Uniforms
{
    vec4 State;
    layout(row_major) mat4 Transform;
    ivec4 Integer4[2];
    vec4 Scalar4[2];
    vec4 Vector[8];
    ivec4 ClipData;
    layout(row_major) mat4 Clip[8];
} Uniforms;

layout(location = 0) in vec2 in_var_POSITION;
layout(location = 1) in vec4 in_var_COLOR0;
layout(location = 2) in vec2 in_var_TEXCOORD0;
layout(location = 0) out vec4 out_var_COLOR0;
layout(location = 1) out vec2 out_var_TEXCOORD0;
layout(location = 2) out vec2 out_var_TEXCOORD1;
layout(location = 3) out vec4 out_var_COLOR1;
layout(location = 4) out vec4 out_var_COLOR2;
layout(location = 5) out vec4 out_var_COLOR3;
layout(location = 6) out vec4 out_var_COLOR4;
layout(location = 7) out vec4 out_var_COLOR5;
layout(location = 8) out vec4 out_var_COLOR6;
layout(location = 9) out vec4 out_var_COLOR7;

mat4 spvWorkaroundRowMajor(mat4 wrap) { return wrap; }

void main()
{
    gl_Position = vec4(in_var_POSITION, 0.0, 1.0) * spvWorkaroundRowMajor(Uniforms.Transform);
    out_var_COLOR0 = in_var_COLOR0;
    out_var_TEXCOORD0 = vec2(0.0);
    out_var_TEXCOORD1 = in_var_TEXCOORD0;
    out_var_COLOR1 = vec4(0.0);
    out_var_COLOR2 = vec4(0.0);
    out_var_COLOR3 = vec4(0.0);
    out_var_COLOR4 = vec4(0.0);
    out_var_COLOR5 = vec4(0.0);
    out_var_COLOR6 = vec4(0.0);
    out_var_COLOR7 = vec4(0.0);
}

)GLSL";