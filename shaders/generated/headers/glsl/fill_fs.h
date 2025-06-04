// Generated shader header for fill_fs
// Generated from: fill.glsl

#pragma once

static const char* fill_fs_source = R"GLSL(#version 330

vec4 _103;
float _116;

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
uniform sampler2D SPIRV_Cross_CombinedTexture1Sampler0;

in vec4 in_var_COLOR0;
in vec2 in_var_TEXCOORD0;
in vec2 in_var_TEXCOORD1;
in vec4 in_var_COLOR1;
in vec4 in_var_COLOR2;
in vec4 in_var_COLOR3;
in vec4 in_var_COLOR4;
in vec4 in_var_COLOR5;
in vec4 in_var_COLOR6;
in vec4 in_var_COLOR7;
layout(location = 0) out vec4 out_var_SV_Target;

mat4 spvWorkaroundRowMajor(mat4 wrap) { return wrap; }

void main()
{
    vec4 _2988;
    switch (uint(in_var_COLOR1.x + 0.5))
    {
        case 0u:
        {
            _2988 = in_var_COLOR0;
            break;
        }
        case 1u:
        {
            _2988 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0) * in_var_COLOR0;
            break;
        }
        case 2u:
        {
            _2988 = texture(SPIRV_Cross_CombinedTexture0Sampler0, (fract((((Uniforms.Vector[2].xy * in_var_TEXCOORD1.x) + (Uniforms.Vector[2].zw * in_var_TEXCOORD1.y)) + Uniforms.Vector[3].xy) / Uniforms.Vector[1].zw) * (Uniforms.Vector[0].zw - Uniforms.Vector[0].xy)) + Uniforms.Vector[0].xy) * in_var_COLOR0;
            break;
        }
        case 3u:
        {
            float _2756 = float(uint(in_var_COLOR1.y + 0.5));
            float _2821;
            if (uint(in_var_COLOR1.z + 0.5) != 0u)
            {
                vec2 _2774 = in_var_TEXCOORD0 - in_var_COLOR2.xy;
                float _2776 = length(_2774 / in_var_COLOR3.zw);
                bool _2783;
                if (in_var_COLOR3.x > 9.9999997473787516355514526367188e-05)
                {
                    _2783 = in_var_COLOR3.y > 9.9999997473787516355514526367188e-05;
                }
                else
                {
                    _2783 = false;
                }
                float _2804;
                if (_2783)
                {
                    float _2789 = length(_2774 / in_var_COLOR3.xy);
                    float _2803;
                    if (_2789 <= 1.0)
                    {
                        _2803 = 0.0;
                    }
                    else
                    {
                        float _2802;
                        if (_2776 >= 1.0)
                        {
                            _2802 = 1.0;
                        }
                        else
                        {
                            _2802 = clamp((_2789 - 1.0) / (_2789 - _2776), 0.0, 1.0);
                        }
                        _2803 = _2802;
                    }
                    _2804 = _2803;
                }
                else
                {
                    _2804 = clamp(_2776, 0.0, 1.0);
                }
                float _2820;
                if (length(in_var_COLOR2.zw - in_var_COLOR2.xy) > 9.9999997473787516355514526367188e-05)
                {
                    _2820 = clamp(_2804 * (1.0 + ((1.0 - dot(normalize(in_var_TEXCOORD0 - in_var_COLOR2.zw), normalize(in_var_COLOR2.xy - in_var_COLOR2.zw))) * 0.5)), 0.0, 1.0);
                }
                else
                {
                    _2820 = _2804;
                }
                _2821 = _2820;
            }
            else
            {
                vec2 _2768 = in_var_COLOR2.zw - in_var_COLOR2.xy;
                _2821 = clamp(dot(in_var_TEXCOORD0 - in_var_COLOR2.xy, _2768) / dot(_2768, _2768), 0.0, 1.0);
            }
            vec4 _2829 = mix(in_var_COLOR5, in_var_COLOR6, vec4(clamp((_2821 - in_var_COLOR4.x) / (in_var_COLOR4.y - in_var_COLOR4.x), 0.0, 1.0)));
            vec4 _2952;
            if (_2756 > 2.0)
            {
                vec4 _2839 = mix(_2829, in_var_COLOR7, vec4(clamp((_2821 - in_var_COLOR4.y) / (in_var_COLOR4.z - in_var_COLOR4.y), 0.0, 1.0)));
                vec4 _2951;
                if (_2756 > 3.0)
                {
                    vec4 _2852 = mix(_2839, Uniforms.Vector[0u], vec4(clamp((_2821 - in_var_COLOR4.z) / (Uniforms.Scalar4[0].x - in_var_COLOR4.z), 0.0, 1.0)));
                    vec4 _2950;
                    if (_2756 > 4.0)
                    {
                        vec4 _2865 = mix(_2852, Uniforms.Vector[1u], vec4(clamp((_2821 - Uniforms.Scalar4[0].x) / (Uniforms.Scalar4[0].y - Uniforms.Scalar4[0].x), 0.0, 1.0)));
                        vec4 _2949;
                        if (_2756 > 5.0)
                        {
                            vec4 _2878 = mix(_2865, Uniforms.Vector[2u], vec4(clamp((_2821 - Uniforms.Scalar4[0].y) / (Uniforms.Scalar4[0].z - Uniforms.Scalar4[0].y), 0.0, 1.0)));
                            vec4 _2948;
                            if (_2756 > 6.0)
                            {
                                vec4 _2891 = mix(_2878, Uniforms.Vector[3u], vec4(clamp((_2821 - Uniforms.Scalar4[0].z) / (Uniforms.Scalar4[0].w - Uniforms.Scalar4[0].z), 0.0, 1.0)));
                                vec4 _2947;
                                if (_2756 > 7.0)
                                {
                                    vec4 _2904 = mix(_2891, Uniforms.Vector[4u], vec4(clamp((_2821 - Uniforms.Scalar4[0].w) / (Uniforms.Scalar4[1].x - Uniforms.Scalar4[0].w), 0.0, 1.0)));
                                    vec4 _2946;
                                    if (_2756 > 8.0)
                                    {
                                        vec4 _2917 = mix(_2904, Uniforms.Vector[5u], vec4(clamp((_2821 - Uniforms.Scalar4[1].x) / (Uniforms.Scalar4[1].y - Uniforms.Scalar4[1].x), 0.0, 1.0)));
                                        vec4 _2945;
                                        if (_2756 > 9.0)
                                        {
                                            vec4 _2930 = mix(_2917, Uniforms.Vector[6u], vec4(clamp((_2821 - Uniforms.Scalar4[1].y) / (Uniforms.Scalar4[1].z - Uniforms.Scalar4[1].y), 0.0, 1.0)));
                                            vec4 _2944;
                                            if (_2756 > 10.0)
                                            {
                                                _2944 = mix(_2930, Uniforms.Vector[7u], vec4(clamp((_2821 - Uniforms.Scalar4[1].z) / (Uniforms.Scalar4[1].w - Uniforms.Scalar4[1].z), 0.0, 1.0)));
                                            }
                                            else
                                            {
                                                _2944 = _2930;
                                            }
                                            _2945 = _2944;
                                        }
                                        else
                                        {
                                            _2945 = _2917;
                                        }
                                        _2946 = _2945;
                                    }
                                    else
                                    {
                                        _2946 = _2904;
                                    }
                                    _2947 = _2946;
                                }
                                else
                                {
                                    _2947 = _2891;
                                }
                                _2948 = _2947;
                            }
                            else
                            {
                                _2948 = _2878;
                            }
                            _2949 = _2948;
                        }
                        else
                        {
                            _2949 = _2865;
                        }
                        _2950 = _2949;
                    }
                    else
                    {
                        _2950 = _2852;
                    }
                    _2951 = _2950;
                }
                else
                {
                    _2951 = _2839;
                }
                _2952 = _2951;
            }
            else
            {
                _2952 = _2829;
            }
            _2988 = _2952;
            break;
        }
        case 7u:
        {
            vec2 _2102 = (in_var_TEXCOORD0 - vec2(0.5)) * in_var_COLOR1.zw;
            float _2715;
            do
            {
                vec2 _2105 = in_var_COLOR1.zw * 0.5;
                float _2106 = _2105.x;
                float _2107 = -_2106;
                float _2109 = _2107 + in_var_COLOR2.x;
                float _2110 = _2105.y;
                float _2111 = -_2110;
                float _2113 = _2111 + in_var_COLOR3.x;
                vec2 _2115 = _2102 - vec2(_2109, _2113);
                bool _2122;
                if ((in_var_COLOR2.x * in_var_COLOR3.x) > 0.0)
                {
                    _2122 = _2102.x < _2109;
                }
                else
                {
                    _2122 = false;
                }
                bool _2127;
                if (_2122)
                {
                    _2127 = _2102.y <= _2113;
                }
                else
                {
                    _2127 = false;
                }
                if (_2127)
                {
                    vec2 _2130 = vec2(in_var_COLOR2.x, in_var_COLOR3.x);
                    float _2258;
                    do
                    {
                        if (abs(in_var_COLOR2.x - in_var_COLOR3.x) < 0.100000001490116119384765625)
                        {
                            _2258 = length(_2115) - in_var_COLOR2.x;
                            break;
                        }
                        vec2 _2140 = abs(_2115);
                        vec2 _2148;
                        vec2 _2149;
                        if (_2140.x > _2140.y)
                        {
                            _2148 = _2140.yx;
                            _2149 = _2130.yx;
                        }
                        else
                        {
                            _2148 = _2140;
                            _2149 = _2130;
                        }
                        float _2154 = (_2149.y * _2149.y) - (_2149.x * _2149.x);
                        float _2157 = (_2149.x * _2148.x) / _2154;
                        float _2160 = (_2149.y * _2148.y) / _2154;
                        float _2161 = _2157 * _2157;
                        float _2162 = _2160 * _2160;
                        float _2164 = (_2161 + _2162) - 1.0;
                        float _2165 = _2164 * 0.3333333432674407958984375;
                        float _2167 = (_2165 * _2165) * _2165;
                        float _2168 = _2161 * _2162;
                        float _2170 = _2167 + (_2168 * 2.0);
                        float _2171 = _2167 + _2168;
                        float _2173 = _2157 + (_2157 * _2162);
                        float _2244;
                        if (_2171 < 0.0)
                        {
                            float _2218 = acos(_2170 / _2167) * 0.3333333432674407958984375;
                            float _2219 = cos(_2218);
                            float _2221 = sin(_2218) * 1.73205077648162841796875;
                            float _2222 = _2164 * (-0.3333333432674407958984375);
                            float _2227 = sqrt((_2222 * ((_2219 + _2221) + 2.0)) + _2161);
                            float _2232 = sqrt((_2222 * ((_2219 - _2221) + 2.0)) + _2161);
                            _2244 = (((_2232 + (float(int(sign(_2154))) * _2227)) + (abs(_2173) / (_2227 * _2232))) - _2157) * 0.5;
                        }
                        else
                        {
                            float _2181 = ((2.0 * _2157) * _2160) * sqrt(_2171);
                            float _2182 = _2170 + _2181;
                            float _2188 = float(int(sign(_2182))) * pow(abs(_2182), 0.3333333432674407958984375);
                            float _2189 = _2170 - _2181;
                            float _2195 = float(int(sign(_2189))) * pow(abs(_2189), 0.3333333432674407958984375);
                            float _2201 = (((-_2188) - _2195) - (_2164 * 1.33333337306976318359375)) + (2.0 * _2161);
                            float _2203 = (_2188 - _2195) * 1.73205077648162841796875;
                            float _2207 = sqrt((_2201 * _2201) + (_2203 * _2203));
                            _2244 = (((_2203 / sqrt(_2207 - _2201)) + ((2.0 * _2173) / _2207)) - _2157) * 0.5;
                        }
                        float _2249 = _2149.y * sqrt(1.0 - (_2244 * _2244));
                        _2258 = length(vec2(_2149.x * _2244, _2249) - _2148) * float(int(sign(_2148.y - _2249)));
                        break;
                    } while(false);
                    _2715 = _2258;
                    break;
                }
                float _2260 = _2106 - in_var_COLOR2.y;
                float _2262 = _2111 + in_var_COLOR3.y;
                vec2 _2264 = _2102 - vec2(_2260, _2262);
                bool _2271;
                if ((in_var_COLOR2.y * in_var_COLOR3.y) > 0.0)
                {
                    _2271 = _2102.x >= _2260;
                }
                else
                {
                    _2271 = false;
                }
                bool _2276;
                if (_2271)
                {
                    _2276 = _2102.y <= _2262;
                }
                else
                {
                    _2276 = false;
                }
                if (_2276)
                {
                    vec2 _2279 = vec2(in_var_COLOR2.y, in_var_COLOR3.y);
                    float _2407;
                    do
                    {
                        if (abs(in_var_COLOR2.y - in_var_COLOR3.y) < 0.100000001490116119384765625)
                        {
                            _2407 = length(_2264) - in_var_COLOR2.y;
                            break;
                        }
                        vec2 _2289 = abs(_2264);
                        vec2 _2297;
                        vec2 _2298;
                        if (_2289.x > _2289.y)
                        {
                            _2297 = _2289.yx;
                            _2298 = _2279.yx;
                        }
                        else
                        {
                            _2297 = _2289;
                            _2298 = _2279;
                        }
                        float _2303 = (_2298.y * _2298.y) - (_2298.x * _2298.x);
                        float _2306 = (_2298.x * _2297.x) / _2303;
                        float _2309 = (_2298.y * _2297.y) / _2303;
                        float _2310 = _2306 * _2306;
                        float _2311 = _2309 * _2309;
                        float _2313 = (_2310 + _2311) - 1.0;
                        float _2314 = _2313 * 0.3333333432674407958984375;
                        float _2316 = (_2314 * _2314) * _2314;
                        float _2317 = _2310 * _2311;
                        float _2319 = _2316 + (_2317 * 2.0);
                        float _2320 = _2316 + _2317;
                        float _2322 = _2306 + (_2306 * _2311);
                        float _2393;
                        if (_2320 < 0.0)
                        {
                            float _2367 = acos(_2319 / _2316) * 0.3333333432674407958984375;
                            float _2368 = cos(_2367);
                            float _2370 = sin(_2367) * 1.73205077648162841796875;
                            float _2371 = _2313 * (-0.3333333432674407958984375);
                            float _2376 = sqrt((_2371 * ((_2368 + _2370) + 2.0)) + _2310);
                            float _2381 = sqrt((_2371 * ((_2368 - _2370) + 2.0)) + _2310);
                            _2393 = (((_2381 + (float(int(sign(_2303))) * _2376)) + (abs(_2322) / (_2376 * _2381))) - _2306) * 0.5;
                        }
                        else
                        {
                            float _2330 = ((2.0 * _2306) * _2309) * sqrt(_2320);
                            float _2331 = _2319 + _2330;
                            float _2337 = float(int(sign(_2331))) * pow(abs(_2331), 0.3333333432674407958984375);
                            float _2338 = _2319 - _2330;
                            float _2344 = float(int(sign(_2338))) * pow(abs(_2338), 0.3333333432674407958984375);
                            float _2350 = (((-_2337) - _2344) - (_2313 * 1.33333337306976318359375)) + (2.0 * _2310);
                            float _2352 = (_2337 - _2344) * 1.73205077648162841796875;
                            float _2356 = sqrt((_2350 * _2350) + (_2352 * _2352));
                            _2393 = (((_2352 / sqrt(_2356 - _2350)) + ((2.0 * _2322) / _2356)) - _2306) * 0.5;
                        }
                        float _2398 = _2298.y * sqrt(1.0 - (_2393 * _2393));
                        _2407 = length(vec2(_2298.x * _2393, _2398) - _2297) * float(int(sign(_2297.y - _2398)));
                        break;
                    } while(false);
                    _2715 = _2407;
                    break;
                }
                float _2409 = _2106 - in_var_COLOR2.z;
                float _2411 = _2110 - in_var_COLOR3.z;
                vec2 _2413 = _2102 - vec2(_2409, _2411);
                bool _2420;
                if ((in_var_COLOR2.z * in_var_COLOR3.z) > 0.0)
                {
                    _2420 = _2102.x >= _2409;
                }
                else
                {
                    _2420 = false;
                }
                bool _2425;
                if (_2420)
                {
                    _2425 = _2102.y >= _2411;
                }
                else
                {
                    _2425 = false;
                }
                if (_2425)
                {
                    vec2 _2428 = vec2(in_var_COLOR2.z, in_var_COLOR3.z);
                    float _2556;
                    do
                    {
                        if (abs(in_var_COLOR2.z - in_var_COLOR3.z) < 0.100000001490116119384765625)
                        {
                            _2556 = length(_2413) - in_var_COLOR2.z;
                            break;
                        }
                        vec2 _2438 = abs(_2413);
                        vec2 _2446;
                        vec2 _2447;
                        if (_2438.x > _2438.y)
                        {
                            _2446 = _2438.yx;
                            _2447 = _2428.yx;
                        }
                        else
                        {
                            _2446 = _2438;
                            _2447 = _2428;
                        }
                        float _2452 = (_2447.y * _2447.y) - (_2447.x * _2447.x);
                        float _2455 = (_2447.x * _2446.x) / _2452;
                        float _2458 = (_2447.y * _2446.y) / _2452;
                        float _2459 = _2455 * _2455;
                        float _2460 = _2458 * _2458;
                        float _2462 = (_2459 + _2460) - 1.0;
                        float _2463 = _2462 * 0.3333333432674407958984375;
                        float _2465 = (_2463 * _2463) * _2463;
                        float _2466 = _2459 * _2460;
                        float _2468 = _2465 + (_2466 * 2.0);
                        float _2469 = _2465 + _2466;
                        float _2471 = _2455 + (_2455 * _2460);
                        float _2542;
                        if (_2469 < 0.0)
                        {
                            float _2516 = acos(_2468 / _2465) * 0.3333333432674407958984375;
                            float _2517 = cos(_2516);
                            float _2519 = sin(_2516) * 1.73205077648162841796875;
                            float _2520 = _2462 * (-0.3333333432674407958984375);
                            float _2525 = sqrt((_2520 * ((_2517 + _2519) + 2.0)) + _2459);
                            float _2530 = sqrt((_2520 * ((_2517 - _2519) + 2.0)) + _2459);
                            _2542 = (((_2530 + (float(int(sign(_2452))) * _2525)) + (abs(_2471) / (_2525 * _2530))) - _2455) * 0.5;
                        }
                        else
                        {
                            float _2479 = ((2.0 * _2455) * _2458) * sqrt(_2469);
                            float _2480 = _2468 + _2479;
                            float _2486 = float(int(sign(_2480))) * pow(abs(_2480), 0.3333333432674407958984375);
                            float _2487 = _2468 - _2479;
                            float _2493 = float(int(sign(_2487))) * pow(abs(_2487), 0.3333333432674407958984375);
                            float _2499 = (((-_2486) - _2493) - (_2462 * 1.33333337306976318359375)) + (2.0 * _2459);
                            float _2501 = (_2486 - _2493) * 1.73205077648162841796875;
                            float _2505 = sqrt((_2499 * _2499) + (_2501 * _2501));
                            _2542 = (((_2501 / sqrt(_2505 - _2499)) + ((2.0 * _2471) / _2505)) - _2455) * 0.5;
                        }
                        float _2547 = _2447.y * sqrt(1.0 - (_2542 * _2542));
                        _2556 = length(vec2(_2447.x * _2542, _2547) - _2446) * float(int(sign(_2446.y - _2547)));
                        break;
                    } while(false);
                    _2715 = _2556;
                    break;
                }
                float _2558 = _2107 + in_var_COLOR2.w;
                float _2560 = _2110 - in_var_COLOR3.w;
                vec2 _2562 = _2102 - vec2(_2558, _2560);
                bool _2569;
                if ((in_var_COLOR2.w * in_var_COLOR3.w) > 0.0)
                {
                    _2569 = _2102.x < _2558;
                }
                else
                {
                    _2569 = false;
                }
                bool _2574;
                if (_2569)
                {
                    _2574 = _2102.y > _2560;
                }
                else
                {
                    _2574 = false;
                }
                if (_2574)
                {
                    vec2 _2577 = vec2(in_var_COLOR2.w, in_var_COLOR3.w);
                    float _2705;
                    do
                    {
                        if (abs(in_var_COLOR2.w - in_var_COLOR3.w) < 0.100000001490116119384765625)
                        {
                            _2705 = length(_2562) - in_var_COLOR2.w;
                            break;
                        }
                        vec2 _2587 = abs(_2562);
                        vec2 _2595;
                        vec2 _2596;
                        if (_2587.x > _2587.y)
                        {
                            _2595 = _2587.yx;
                            _2596 = _2577.yx;
                        }
                        else
                        {
                            _2595 = _2587;
                            _2596 = _2577;
                        }
                        float _2601 = (_2596.y * _2596.y) - (_2596.x * _2596.x);
                        float _2604 = (_2596.x * _2595.x) / _2601;
                        float _2607 = (_2596.y * _2595.y) / _2601;
                        float _2608 = _2604 * _2604;
                        float _2609 = _2607 * _2607;
                        float _2611 = (_2608 + _2609) - 1.0;
                        float _2612 = _2611 * 0.3333333432674407958984375;
                        float _2614 = (_2612 * _2612) * _2612;
                        float _2615 = _2608 * _2609;
                        float _2617 = _2614 + (_2615 * 2.0);
                        float _2618 = _2614 + _2615;
                        float _2620 = _2604 + (_2604 * _2609);
                        float _2691;
                        if (_2618 < 0.0)
                        {
                            float _2665 = acos(_2617 / _2614) * 0.3333333432674407958984375;
                            float _2666 = cos(_2665);
                            float _2668 = sin(_2665) * 1.73205077648162841796875;
                            float _2669 = _2611 * (-0.3333333432674407958984375);
                            float _2674 = sqrt((_2669 * ((_2666 + _2668) + 2.0)) + _2608);
                            float _2679 = sqrt((_2669 * ((_2666 - _2668) + 2.0)) + _2608);
                            _2691 = (((_2679 + (float(int(sign(_2601))) * _2674)) + (abs(_2620) / (_2674 * _2679))) - _2604) * 0.5;
                        }
                        else
                        {
                            float _2628 = ((2.0 * _2604) * _2607) * sqrt(_2618);
                            float _2629 = _2617 + _2628;
                            float _2635 = float(int(sign(_2629))) * pow(abs(_2629), 0.3333333432674407958984375);
                            float _2636 = _2617 - _2628;
                            float _2642 = float(int(sign(_2636))) * pow(abs(_2636), 0.3333333432674407958984375);
                            float _2648 = (((-_2635) - _2642) - (_2611 * 1.33333337306976318359375)) + (2.0 * _2608);
                            float _2650 = (_2635 - _2642) * 1.73205077648162841796875;
                            float _2654 = sqrt((_2648 * _2648) + (_2650 * _2650));
                            _2691 = (((_2650 / sqrt(_2654 - _2648)) + ((2.0 * _2620) / _2654)) - _2604) * 0.5;
                        }
                        float _2696 = _2596.y * sqrt(1.0 - (_2691 * _2691));
                        _2705 = length(vec2(_2596.x * _2691, _2696) - _2595) * float(int(sign(_2595.y - _2696)));
                        break;
                    } while(false);
                    _2715 = _2705;
                    break;
                }
                vec2 _2707 = abs(_2102) - _2105;
                float _2708 = _2707.x;
                float _2709 = _2707.y;
                float _2710 = isnan(_2709) ? _2708 : (isnan(_2708) ? _2709 : max(_2708, _2709));
                bvec2 _3681 = isnan(_2707);
                bvec2 _3682 = isnan(vec2(0.0));
                vec2 _3683 = max(_2707, vec2(0.0));
                vec2 _3684 = vec2(_3681.x ? vec2(0.0).x : _3683.x, _3681.y ? vec2(0.0).y : _3683.y);
                _2715 = (isnan(0.0) ? _2710 : (isnan(_2710) ? 0.0 : min(_2710, 0.0))) + length(vec2(_3682.x ? _2707.x : _3684.x, _3682.y ? _2707.y : _3684.y));
                break;
            } while(false);
            float _2716 = -_2715;
            float _2717 = smoothstep(-0.3540000021457672119140625, 0.3540000021457672119140625, _2716);
            float _2719 = _2717 * in_var_COLOR0.w;
            vec4 _2725 = vec4(in_var_COLOR0.xyz * _2719, _2719);
            vec4 _2752;
            if (in_var_COLOR4.x > 0.0)
            {
                float _2733 = 1.0 - smoothstep(in_var_COLOR4.x - 0.3540000021457672119140625, in_var_COLOR4.x + 0.3540000021457672119140625, _2716);
                float _2736 = (isnan(_2733) ? _2717 : (isnan(_2717) ? _2733 : min(_2717, _2733))) * in_var_COLOR5.w;
                float _2745 = 1.0 - _2736;
                vec3 _2747 = vec4(in_var_COLOR5.xyz * _2736, _2736).xyz + (_2725.xyz * _2745);
                vec4 _2748 = vec4(_2747.x, _2747.y, _2747.z, _103.w);
                _2748.w = _2736 + (_2719 * _2745);
                _2752 = _2748;
            }
            else
            {
                _2752 = _2725;
            }
            _2988 = _2752;
            break;
        }
        case 8u:
        {
            bool _824 = uint(in_var_COLOR1.y + 0.5) != 0u;
            vec2 _830 = in_var_TEXCOORD1 - in_var_COLOR5.xy;
            float _1443;
            do
            {
                vec2 _833 = in_var_COLOR5.zw * 0.5;
                float _834 = _833.x;
                float _835 = -_834;
                float _837 = _835 + in_var_COLOR6.x;
                float _838 = _833.y;
                float _839 = -_838;
                float _841 = _839 + in_var_COLOR7.x;
                vec2 _843 = _830 - vec2(_837, _841);
                bool _850;
                if ((in_var_COLOR6.x * in_var_COLOR7.x) > 0.0)
                {
                    _850 = _830.x < _837;
                }
                else
                {
                    _850 = false;
                }
                bool _855;
                if (_850)
                {
                    _855 = _830.y <= _841;
                }
                else
                {
                    _855 = false;
                }
                if (_855)
                {
                    vec2 _858 = vec2(in_var_COLOR6.x, in_var_COLOR7.x);
                    float _986;
                    do
                    {
                        if (abs(in_var_COLOR6.x - in_var_COLOR7.x) < 0.100000001490116119384765625)
                        {
                            _986 = length(_843) - in_var_COLOR6.x;
                            break;
                        }
                        vec2 _868 = abs(_843);
                        vec2 _876;
                        vec2 _877;
                        if (_868.x > _868.y)
                        {
                            _876 = _868.yx;
                            _877 = _858.yx;
                        }
                        else
                        {
                            _876 = _868;
                            _877 = _858;
                        }
                        float _882 = (_877.y * _877.y) - (_877.x * _877.x);
                        float _885 = (_877.x * _876.x) / _882;
                        float _888 = (_877.y * _876.y) / _882;
                        float _889 = _885 * _885;
                        float _890 = _888 * _888;
                        float _892 = (_889 + _890) - 1.0;
                        float _893 = _892 * 0.3333333432674407958984375;
                        float _895 = (_893 * _893) * _893;
                        float _896 = _889 * _890;
                        float _898 = _895 + (_896 * 2.0);
                        float _899 = _895 + _896;
                        float _901 = _885 + (_885 * _890);
                        float _972;
                        if (_899 < 0.0)
                        {
                            float _946 = acos(_898 / _895) * 0.3333333432674407958984375;
                            float _947 = cos(_946);
                            float _949 = sin(_946) * 1.73205077648162841796875;
                            float _950 = _892 * (-0.3333333432674407958984375);
                            float _955 = sqrt((_950 * ((_947 + _949) + 2.0)) + _889);
                            float _960 = sqrt((_950 * ((_947 - _949) + 2.0)) + _889);
                            _972 = (((_960 + (float(int(sign(_882))) * _955)) + (abs(_901) / (_955 * _960))) - _885) * 0.5;
                        }
                        else
                        {
                            float _909 = ((2.0 * _885) * _888) * sqrt(_899);
                            float _910 = _898 + _909;
                            float _916 = float(int(sign(_910))) * pow(abs(_910), 0.3333333432674407958984375);
                            float _917 = _898 - _909;
                            float _923 = float(int(sign(_917))) * pow(abs(_917), 0.3333333432674407958984375);
                            float _929 = (((-_916) - _923) - (_892 * 1.33333337306976318359375)) + (2.0 * _889);
                            float _931 = (_916 - _923) * 1.73205077648162841796875;
                            float _935 = sqrt((_929 * _929) + (_931 * _931));
                            _972 = (((_931 / sqrt(_935 - _929)) + ((2.0 * _901) / _935)) - _885) * 0.5;
                        }
                        float _977 = _877.y * sqrt(1.0 - (_972 * _972));
                        _986 = length(vec2(_877.x * _972, _977) - _876) * float(int(sign(_876.y - _977)));
                        break;
                    } while(false);
                    _1443 = _986;
                    break;
                }
                float _988 = _834 - in_var_COLOR6.y;
                float _990 = _839 + in_var_COLOR7.y;
                vec2 _992 = _830 - vec2(_988, _990);
                bool _999;
                if ((in_var_COLOR6.y * in_var_COLOR7.y) > 0.0)
                {
                    _999 = _830.x >= _988;
                }
                else
                {
                    _999 = false;
                }
                bool _1004;
                if (_999)
                {
                    _1004 = _830.y <= _990;
                }
                else
                {
                    _1004 = false;
                }
                if (_1004)
                {
                    vec2 _1007 = vec2(in_var_COLOR6.y, in_var_COLOR7.y);
                    float _1135;
                    do
                    {
                        if (abs(in_var_COLOR6.y - in_var_COLOR7.y) < 0.100000001490116119384765625)
                        {
                            _1135 = length(_992) - in_var_COLOR6.y;
                            break;
                        }
                        vec2 _1017 = abs(_992);
                        vec2 _1025;
                        vec2 _1026;
                        if (_1017.x > _1017.y)
                        {
                            _1025 = _1017.yx;
                            _1026 = _1007.yx;
                        }
                        else
                        {
                            _1025 = _1017;
                            _1026 = _1007;
                        }
                        float _1031 = (_1026.y * _1026.y) - (_1026.x * _1026.x);
                        float _1034 = (_1026.x * _1025.x) / _1031;
                        float _1037 = (_1026.y * _1025.y) / _1031;
                        float _1038 = _1034 * _1034;
                        float _1039 = _1037 * _1037;
                        float _1041 = (_1038 + _1039) - 1.0;
                        float _1042 = _1041 * 0.3333333432674407958984375;
                        float _1044 = (_1042 * _1042) * _1042;
                        float _1045 = _1038 * _1039;
                        float _1047 = _1044 + (_1045 * 2.0);
                        float _1048 = _1044 + _1045;
                        float _1050 = _1034 + (_1034 * _1039);
                        float _1121;
                        if (_1048 < 0.0)
                        {
                            float _1095 = acos(_1047 / _1044) * 0.3333333432674407958984375;
                            float _1096 = cos(_1095);
                            float _1098 = sin(_1095) * 1.73205077648162841796875;
                            float _1099 = _1041 * (-0.3333333432674407958984375);
                            float _1104 = sqrt((_1099 * ((_1096 + _1098) + 2.0)) + _1038);
                            float _1109 = sqrt((_1099 * ((_1096 - _1098) + 2.0)) + _1038);
                            _1121 = (((_1109 + (float(int(sign(_1031))) * _1104)) + (abs(_1050) / (_1104 * _1109))) - _1034) * 0.5;
                        }
                        else
                        {
                            float _1058 = ((2.0 * _1034) * _1037) * sqrt(_1048);
                            float _1059 = _1047 + _1058;
                            float _1065 = float(int(sign(_1059))) * pow(abs(_1059), 0.3333333432674407958984375);
                            float _1066 = _1047 - _1058;
                            float _1072 = float(int(sign(_1066))) * pow(abs(_1066), 0.3333333432674407958984375);
                            float _1078 = (((-_1065) - _1072) - (_1041 * 1.33333337306976318359375)) + (2.0 * _1038);
                            float _1080 = (_1065 - _1072) * 1.73205077648162841796875;
                            float _1084 = sqrt((_1078 * _1078) + (_1080 * _1080));
                            _1121 = (((_1080 / sqrt(_1084 - _1078)) + ((2.0 * _1050) / _1084)) - _1034) * 0.5;
                        }
                        float _1126 = _1026.y * sqrt(1.0 - (_1121 * _1121));
                        _1135 = length(vec2(_1026.x * _1121, _1126) - _1025) * float(int(sign(_1025.y - _1126)));
                        break;
                    } while(false);
                    _1443 = _1135;
                    break;
                }
                float _1137 = _834 - in_var_COLOR6.z;
                float _1139 = _838 - in_var_COLOR7.z;
                vec2 _1141 = _830 - vec2(_1137, _1139);
                bool _1148;
                if ((in_var_COLOR6.z * in_var_COLOR7.z) > 0.0)
                {
                    _1148 = _830.x >= _1137;
                }
                else
                {
                    _1148 = false;
                }
                bool _1153;
                if (_1148)
                {
                    _1153 = _830.y >= _1139;
                }
                else
                {
                    _1153 = false;
                }
                if (_1153)
                {
                    vec2 _1156 = vec2(in_var_COLOR6.z, in_var_COLOR7.z);
                    float _1284;
                    do
                    {
                        if (abs(in_var_COLOR6.z - in_var_COLOR7.z) < 0.100000001490116119384765625)
                        {
                            _1284 = length(_1141) - in_var_COLOR6.z;
                            break;
                        }
                        vec2 _1166 = abs(_1141);
                        vec2 _1174;
                        vec2 _1175;
                        if (_1166.x > _1166.y)
                        {
                            _1174 = _1166.yx;
                            _1175 = _1156.yx;
                        }
                        else
                        {
                            _1174 = _1166;
                            _1175 = _1156;
                        }
                        float _1180 = (_1175.y * _1175.y) - (_1175.x * _1175.x);
                        float _1183 = (_1175.x * _1174.x) / _1180;
                        float _1186 = (_1175.y * _1174.y) / _1180;
                        float _1187 = _1183 * _1183;
                        float _1188 = _1186 * _1186;
                        float _1190 = (_1187 + _1188) - 1.0;
                        float _1191 = _1190 * 0.3333333432674407958984375;
                        float _1193 = (_1191 * _1191) * _1191;
                        float _1194 = _1187 * _1188;
                        float _1196 = _1193 + (_1194 * 2.0);
                        float _1197 = _1193 + _1194;
                        float _1199 = _1183 + (_1183 * _1188);
                        float _1270;
                        if (_1197 < 0.0)
                        {
                            float _1244 = acos(_1196 / _1193) * 0.3333333432674407958984375;
                            float _1245 = cos(_1244);
                            float _1247 = sin(_1244) * 1.73205077648162841796875;
                            float _1248 = _1190 * (-0.3333333432674407958984375);
                            float _1253 = sqrt((_1248 * ((_1245 + _1247) + 2.0)) + _1187);
                            float _1258 = sqrt((_1248 * ((_1245 - _1247) + 2.0)) + _1187);
                            _1270 = (((_1258 + (float(int(sign(_1180))) * _1253)) + (abs(_1199) / (_1253 * _1258))) - _1183) * 0.5;
                        }
                        else
                        {
                            float _1207 = ((2.0 * _1183) * _1186) * sqrt(_1197);
                            float _1208 = _1196 + _1207;
                            float _1214 = float(int(sign(_1208))) * pow(abs(_1208), 0.3333333432674407958984375);
                            float _1215 = _1196 - _1207;
                            float _1221 = float(int(sign(_1215))) * pow(abs(_1215), 0.3333333432674407958984375);
                            float _1227 = (((-_1214) - _1221) - (_1190 * 1.33333337306976318359375)) + (2.0 * _1187);
                            float _1229 = (_1214 - _1221) * 1.73205077648162841796875;
                            float _1233 = sqrt((_1227 * _1227) + (_1229 * _1229));
                            _1270 = (((_1229 / sqrt(_1233 - _1227)) + ((2.0 * _1199) / _1233)) - _1183) * 0.5;
                        }
                        float _1275 = _1175.y * sqrt(1.0 - (_1270 * _1270));
                        _1284 = length(vec2(_1175.x * _1270, _1275) - _1174) * float(int(sign(_1174.y - _1275)));
                        break;
                    } while(false);
                    _1443 = _1284;
                    break;
                }
                float _1286 = _835 + in_var_COLOR6.w;
                float _1288 = _838 - in_var_COLOR7.w;
                vec2 _1290 = _830 - vec2(_1286, _1288);
                bool _1297;
                if ((in_var_COLOR6.w * in_var_COLOR7.w) > 0.0)
                {
                    _1297 = _830.x < _1286;
                }
                else
                {
                    _1297 = false;
                }
                bool _1302;
                if (_1297)
                {
                    _1302 = _830.y > _1288;
                }
                else
                {
                    _1302 = false;
                }
                if (_1302)
                {
                    vec2 _1305 = vec2(in_var_COLOR6.w, in_var_COLOR7.w);
                    float _1433;
                    do
                    {
                        if (abs(in_var_COLOR6.w - in_var_COLOR7.w) < 0.100000001490116119384765625)
                        {
                            _1433 = length(_1290) - in_var_COLOR6.w;
                            break;
                        }
                        vec2 _1315 = abs(_1290);
                        vec2 _1323;
                        vec2 _1324;
                        if (_1315.x > _1315.y)
                        {
                            _1323 = _1315.yx;
                            _1324 = _1305.yx;
                        }
                        else
                        {
                            _1323 = _1315;
                            _1324 = _1305;
                        }
                        float _1329 = (_1324.y * _1324.y) - (_1324.x * _1324.x);
                        float _1332 = (_1324.x * _1323.x) / _1329;
                        float _1335 = (_1324.y * _1323.y) / _1329;
                        float _1336 = _1332 * _1332;
                        float _1337 = _1335 * _1335;
                        float _1339 = (_1336 + _1337) - 1.0;
                        float _1340 = _1339 * 0.3333333432674407958984375;
                        float _1342 = (_1340 * _1340) * _1340;
                        float _1343 = _1336 * _1337;
                        float _1345 = _1342 + (_1343 * 2.0);
                        float _1346 = _1342 + _1343;
                        float _1348 = _1332 + (_1332 * _1337);
                        float _1419;
                        if (_1346 < 0.0)
                        {
                            float _1393 = acos(_1345 / _1342) * 0.3333333432674407958984375;
                            float _1394 = cos(_1393);
                            float _1396 = sin(_1393) * 1.73205077648162841796875;
                            float _1397 = _1339 * (-0.3333333432674407958984375);
                            float _1402 = sqrt((_1397 * ((_1394 + _1396) + 2.0)) + _1336);
                            float _1407 = sqrt((_1397 * ((_1394 - _1396) + 2.0)) + _1336);
                            _1419 = (((_1407 + (float(int(sign(_1329))) * _1402)) + (abs(_1348) / (_1402 * _1407))) - _1332) * 0.5;
                        }
                        else
                        {
                            float _1356 = ((2.0 * _1332) * _1335) * sqrt(_1346);
                            float _1357 = _1345 + _1356;
                            float _1363 = float(int(sign(_1357))) * pow(abs(_1357), 0.3333333432674407958984375);
                            float _1364 = _1345 - _1356;
                            float _1370 = float(int(sign(_1364))) * pow(abs(_1364), 0.3333333432674407958984375);
                            float _1376 = (((-_1363) - _1370) - (_1339 * 1.33333337306976318359375)) + (2.0 * _1336);
                            float _1378 = (_1363 - _1370) * 1.73205077648162841796875;
                            float _1382 = sqrt((_1376 * _1376) + (_1378 * _1378));
                            _1419 = (((_1378 / sqrt(_1382 - _1376)) + ((2.0 * _1348) / _1382)) - _1332) * 0.5;
                        }
                        float _1424 = _1324.y * sqrt(1.0 - (_1419 * _1419));
                        _1433 = length(vec2(_1324.x * _1419, _1424) - _1323) * float(int(sign(_1323.y - _1424)));
                        break;
                    } while(false);
                    _1443 = _1433;
                    break;
                }
                vec2 _1435 = abs(_830) - _833;
                float _1436 = _1435.x;
                float _1437 = _1435.y;
                float _1438 = isnan(_1437) ? _1436 : (isnan(_1436) ? _1437 : max(_1436, _1437));
                bvec2 _3701 = isnan(_1435);
                bvec2 _3702 = isnan(vec2(0.0));
                vec2 _3703 = max(_1435, vec2(0.0));
                vec2 _3704 = vec2(_3701.x ? vec2(0.0).x : _3703.x, _3701.y ? vec2(0.0).y : _3703.y);
                _1443 = (isnan(0.0) ? _1438 : (isnan(_1438) ? 0.0 : min(_1438, 0.0))) + length(vec2(_3702.x ? _1435.x : _3704.x, _3702.y ? _1435.y : _3704.y));
                break;
            } while(false);
            vec2 _1444 = in_var_TEXCOORD1 - in_var_COLOR2.xy;
            float _2057;
            do
            {
                vec2 _1447 = in_var_COLOR2.zw * 0.5;
                float _1448 = _1447.x;
                float _1449 = -_1448;
                float _1451 = _1449 + in_var_COLOR3.x;
                float _1452 = _1447.y;
                float _1453 = -_1452;
                float _1455 = _1453 + in_var_COLOR4.x;
                vec2 _1457 = _1444 - vec2(_1451, _1455);
                bool _1464;
                if ((in_var_COLOR3.x * in_var_COLOR4.x) > 0.0)
                {
                    _1464 = _1444.x < _1451;
                }
                else
                {
                    _1464 = false;
                }
                bool _1469;
                if (_1464)
                {
                    _1469 = _1444.y <= _1455;
                }
                else
                {
                    _1469 = false;
                }
                if (_1469)
                {
                    vec2 _1472 = vec2(in_var_COLOR3.x, in_var_COLOR4.x);
                    float _1600;
                    do
                    {
                        if (abs(in_var_COLOR3.x - in_var_COLOR4.x) < 0.100000001490116119384765625)
                        {
                            _1600 = length(_1457) - in_var_COLOR3.x;
                            break;
                        }
                        vec2 _1482 = abs(_1457);
                        vec2 _1490;
                        vec2 _1491;
                        if (_1482.x > _1482.y)
                        {
                            _1490 = _1482.yx;
                            _1491 = _1472.yx;
                        }
                        else
                        {
                            _1490 = _1482;
                            _1491 = _1472;
                        }
                        float _1496 = (_1491.y * _1491.y) - (_1491.x * _1491.x);
                        float _1499 = (_1491.x * _1490.x) / _1496;
                        float _1502 = (_1491.y * _1490.y) / _1496;
                        float _1503 = _1499 * _1499;
                        float _1504 = _1502 * _1502;
                        float _1506 = (_1503 + _1504) - 1.0;
                        float _1507 = _1506 * 0.3333333432674407958984375;
                        float _1509 = (_1507 * _1507) * _1507;
                        float _1510 = _1503 * _1504;
                        float _1512 = _1509 + (_1510 * 2.0);
                        float _1513 = _1509 + _1510;
                        float _1515 = _1499 + (_1499 * _1504);
                        float _1586;
                        if (_1513 < 0.0)
                        {
                            float _1560 = acos(_1512 / _1509) * 0.3333333432674407958984375;
                            float _1561 = cos(_1560);
                            float _1563 = sin(_1560) * 1.73205077648162841796875;
                            float _1564 = _1506 * (-0.3333333432674407958984375);
                            float _1569 = sqrt((_1564 * ((_1561 + _1563) + 2.0)) + _1503);
                            float _1574 = sqrt((_1564 * ((_1561 - _1563) + 2.0)) + _1503);
                            _1586 = (((_1574 + (float(int(sign(_1496))) * _1569)) + (abs(_1515) / (_1569 * _1574))) - _1499) * 0.5;
                        }
                        else
                        {
                            float _1523 = ((2.0 * _1499) * _1502) * sqrt(_1513);
                            float _1524 = _1512 + _1523;
                            float _1530 = float(int(sign(_1524))) * pow(abs(_1524), 0.3333333432674407958984375);
                            float _1531 = _1512 - _1523;
                            float _1537 = float(int(sign(_1531))) * pow(abs(_1531), 0.3333333432674407958984375);
                            float _1543 = (((-_1530) - _1537) - (_1506 * 1.33333337306976318359375)) + (2.0 * _1503);
                            float _1545 = (_1530 - _1537) * 1.73205077648162841796875;
                            float _1549 = sqrt((_1543 * _1543) + (_1545 * _1545));
                            _1586 = (((_1545 / sqrt(_1549 - _1543)) + ((2.0 * _1515) / _1549)) - _1499) * 0.5;
                        }
                        float _1591 = _1491.y * sqrt(1.0 - (_1586 * _1586));
                        _1600 = length(vec2(_1491.x * _1586, _1591) - _1490) * float(int(sign(_1490.y - _1591)));
                        break;
                    } while(false);
                    _2057 = _1600;
                    break;
                }
                float _1602 = _1448 - in_var_COLOR3.y;
                float _1604 = _1453 + in_var_COLOR4.y;
                vec2 _1606 = _1444 - vec2(_1602, _1604);
                bool _1613;
                if ((in_var_COLOR3.y * in_var_COLOR4.y) > 0.0)
                {
                    _1613 = _1444.x >= _1602;
                }
                else
                {
                    _1613 = false;
                }
                bool _1618;
                if (_1613)
                {
                    _1618 = _1444.y <= _1604;
                }
                else
                {
                    _1618 = false;
                }
                if (_1618)
                {
                    vec2 _1621 = vec2(in_var_COLOR3.y, in_var_COLOR4.y);
                    float _1749;
                    do
                    {
                        if (abs(in_var_COLOR3.y - in_var_COLOR4.y) < 0.100000001490116119384765625)
                        {
                            _1749 = length(_1606) - in_var_COLOR3.y;
                            break;
                        }
                        vec2 _1631 = abs(_1606);
                        vec2 _1639;
                        vec2 _1640;
                        if (_1631.x > _1631.y)
                        {
                            _1639 = _1631.yx;
                            _1640 = _1621.yx;
                        }
                        else
                        {
                            _1639 = _1631;
                            _1640 = _1621;
                        }
                        float _1645 = (_1640.y * _1640.y) - (_1640.x * _1640.x);
                        float _1648 = (_1640.x * _1639.x) / _1645;
                        float _1651 = (_1640.y * _1639.y) / _1645;
                        float _1652 = _1648 * _1648;
                        float _1653 = _1651 * _1651;
                        float _1655 = (_1652 + _1653) - 1.0;
                        float _1656 = _1655 * 0.3333333432674407958984375;
                        float _1658 = (_1656 * _1656) * _1656;
                        float _1659 = _1652 * _1653;
                        float _1661 = _1658 + (_1659 * 2.0);
                        float _1662 = _1658 + _1659;
                        float _1664 = _1648 + (_1648 * _1653);
                        float _1735;
                        if (_1662 < 0.0)
                        {
                            float _1709 = acos(_1661 / _1658) * 0.3333333432674407958984375;
                            float _1710 = cos(_1709);
                            float _1712 = sin(_1709) * 1.73205077648162841796875;
                            float _1713 = _1655 * (-0.3333333432674407958984375);
                            float _1718 = sqrt((_1713 * ((_1710 + _1712) + 2.0)) + _1652);
                            float _1723 = sqrt((_1713 * ((_1710 - _1712) + 2.0)) + _1652);
                            _1735 = (((_1723 + (float(int(sign(_1645))) * _1718)) + (abs(_1664) / (_1718 * _1723))) - _1648) * 0.5;
                        }
                        else
                        {
                            float _1672 = ((2.0 * _1648) * _1651) * sqrt(_1662);
                            float _1673 = _1661 + _1672;
                            float _1679 = float(int(sign(_1673))) * pow(abs(_1673), 0.3333333432674407958984375);
                            float _1680 = _1661 - _1672;
                            float _1686 = float(int(sign(_1680))) * pow(abs(_1680), 0.3333333432674407958984375);
                            float _1692 = (((-_1679) - _1686) - (_1655 * 1.33333337306976318359375)) + (2.0 * _1652);
                            float _1694 = (_1679 - _1686) * 1.73205077648162841796875;
                            float _1698 = sqrt((_1692 * _1692) + (_1694 * _1694));
                            _1735 = (((_1694 / sqrt(_1698 - _1692)) + ((2.0 * _1664) / _1698)) - _1648) * 0.5;
                        }
                        float _1740 = _1640.y * sqrt(1.0 - (_1735 * _1735));
                        _1749 = length(vec2(_1640.x * _1735, _1740) - _1639) * float(int(sign(_1639.y - _1740)));
                        break;
                    } while(false);
                    _2057 = _1749;
                    break;
                }
                float _1751 = _1448 - in_var_COLOR3.z;
                float _1753 = _1452 - in_var_COLOR4.z;
                vec2 _1755 = _1444 - vec2(_1751, _1753);
                bool _1762;
                if ((in_var_COLOR3.z * in_var_COLOR4.z) > 0.0)
                {
                    _1762 = _1444.x >= _1751;
                }
                else
                {
                    _1762 = false;
                }
                bool _1767;
                if (_1762)
                {
                    _1767 = _1444.y >= _1753;
                }
                else
                {
                    _1767 = false;
                }
                if (_1767)
                {
                    vec2 _1770 = vec2(in_var_COLOR3.z, in_var_COLOR4.z);
                    float _1898;
                    do
                    {
                        if (abs(in_var_COLOR3.z - in_var_COLOR4.z) < 0.100000001490116119384765625)
                        {
                            _1898 = length(_1755) - in_var_COLOR3.z;
                            break;
                        }
                        vec2 _1780 = abs(_1755);
                        vec2 _1788;
                        vec2 _1789;
                        if (_1780.x > _1780.y)
                        {
                            _1788 = _1780.yx;
                            _1789 = _1770.yx;
                        }
                        else
                        {
                            _1788 = _1780;
                            _1789 = _1770;
                        }
                        float _1794 = (_1789.y * _1789.y) - (_1789.x * _1789.x);
                        float _1797 = (_1789.x * _1788.x) / _1794;
                        float _1800 = (_1789.y * _1788.y) / _1794;
                        float _1801 = _1797 * _1797;
                        float _1802 = _1800 * _1800;
                        float _1804 = (_1801 + _1802) - 1.0;
                        float _1805 = _1804 * 0.3333333432674407958984375;
                        float _1807 = (_1805 * _1805) * _1805;
                        float _1808 = _1801 * _1802;
                        float _1810 = _1807 + (_1808 * 2.0);
                        float _1811 = _1807 + _1808;
                        float _1813 = _1797 + (_1797 * _1802);
                        float _1884;
                        if (_1811 < 0.0)
                        {
                            float _1858 = acos(_1810 / _1807) * 0.3333333432674407958984375;
                            float _1859 = cos(_1858);
                            float _1861 = sin(_1858) * 1.73205077648162841796875;
                            float _1862 = _1804 * (-0.3333333432674407958984375);
                            float _1867 = sqrt((_1862 * ((_1859 + _1861) + 2.0)) + _1801);
                            float _1872 = sqrt((_1862 * ((_1859 - _1861) + 2.0)) + _1801);
                            _1884 = (((_1872 + (float(int(sign(_1794))) * _1867)) + (abs(_1813) / (_1867 * _1872))) - _1797) * 0.5;
                        }
                        else
                        {
                            float _1821 = ((2.0 * _1797) * _1800) * sqrt(_1811);
                            float _1822 = _1810 + _1821;
                            float _1828 = float(int(sign(_1822))) * pow(abs(_1822), 0.3333333432674407958984375);
                            float _1829 = _1810 - _1821;
                            float _1835 = float(int(sign(_1829))) * pow(abs(_1829), 0.3333333432674407958984375);
                            float _1841 = (((-_1828) - _1835) - (_1804 * 1.33333337306976318359375)) + (2.0 * _1801);
                            float _1843 = (_1828 - _1835) * 1.73205077648162841796875;
                            float _1847 = sqrt((_1841 * _1841) + (_1843 * _1843));
                            _1884 = (((_1843 / sqrt(_1847 - _1841)) + ((2.0 * _1813) / _1847)) - _1797) * 0.5;
                        }
                        float _1889 = _1789.y * sqrt(1.0 - (_1884 * _1884));
                        _1898 = length(vec2(_1789.x * _1884, _1889) - _1788) * float(int(sign(_1788.y - _1889)));
                        break;
                    } while(false);
                    _2057 = _1898;
                    break;
                }
                float _1900 = _1449 + in_var_COLOR3.w;
                float _1902 = _1452 - in_var_COLOR4.w;
                vec2 _1904 = _1444 - vec2(_1900, _1902);
                bool _1911;
                if ((in_var_COLOR3.w * in_var_COLOR4.w) > 0.0)
                {
                    _1911 = _1444.x < _1900;
                }
                else
                {
                    _1911 = false;
                }
                bool _1916;
                if (_1911)
                {
                    _1916 = _1444.y > _1902;
                }
                else
                {
                    _1916 = false;
                }
                if (_1916)
                {
                    vec2 _1919 = vec2(in_var_COLOR3.w, in_var_COLOR4.w);
                    float _2047;
                    do
                    {
                        if (abs(in_var_COLOR3.w - in_var_COLOR4.w) < 0.100000001490116119384765625)
                        {
                            _2047 = length(_1904) - in_var_COLOR3.w;
                            break;
                        }
                        vec2 _1929 = abs(_1904);
                        vec2 _1937;
                        vec2 _1938;
                        if (_1929.x > _1929.y)
                        {
                            _1937 = _1929.yx;
                            _1938 = _1919.yx;
                        }
                        else
                        {
                            _1937 = _1929;
                            _1938 = _1919;
                        }
                        float _1943 = (_1938.y * _1938.y) - (_1938.x * _1938.x);
                        float _1946 = (_1938.x * _1937.x) / _1943;
                        float _1949 = (_1938.y * _1937.y) / _1943;
                        float _1950 = _1946 * _1946;
                        float _1951 = _1949 * _1949;
                        float _1953 = (_1950 + _1951) - 1.0;
                        float _1954 = _1953 * 0.3333333432674407958984375;
                        float _1956 = (_1954 * _1954) * _1954;
                        float _1957 = _1950 * _1951;
                        float _1959 = _1956 + (_1957 * 2.0);
                        float _1960 = _1956 + _1957;
                        float _1962 = _1946 + (_1946 * _1951);
                        float _2033;
                        if (_1960 < 0.0)
                        {
                            float _2007 = acos(_1959 / _1956) * 0.3333333432674407958984375;
                            float _2008 = cos(_2007);
                            float _2010 = sin(_2007) * 1.73205077648162841796875;
                            float _2011 = _1953 * (-0.3333333432674407958984375);
                            float _2016 = sqrt((_2011 * ((_2008 + _2010) + 2.0)) + _1950);
                            float _2021 = sqrt((_2011 * ((_2008 - _2010) + 2.0)) + _1950);
                            _2033 = (((_2021 + (float(int(sign(_1943))) * _2016)) + (abs(_1962) / (_2016 * _2021))) - _1946) * 0.5;
                        }
                        else
                        {
                            float _1970 = ((2.0 * _1946) * _1949) * sqrt(_1960);
                            float _1971 = _1959 + _1970;
                            float _1977 = float(int(sign(_1971))) * pow(abs(_1971), 0.3333333432674407958984375);
                            float _1978 = _1959 - _1970;
                            float _1984 = float(int(sign(_1978))) * pow(abs(_1978), 0.3333333432674407958984375);
                            float _1990 = (((-_1977) - _1984) - (_1953 * 1.33333337306976318359375)) + (2.0 * _1950);
                            float _1992 = (_1977 - _1984) * 1.73205077648162841796875;
                            float _1996 = sqrt((_1990 * _1990) + (_1992 * _1992));
                            _2033 = (((_1992 / sqrt(_1996 - _1990)) + ((2.0 * _1962) / _1996)) - _1946) * 0.5;
                        }
                        float _2038 = _1938.y * sqrt(1.0 - (_2033 * _2033));
                        _2047 = length(vec2(_1938.x * _2033, _2038) - _1937) * float(int(sign(_1937.y - _2038)));
                        break;
                    } while(false);
                    _2057 = _2047;
                    break;
                }
                vec2 _2049 = abs(_1444) - _1447;
                float _2050 = _2049.x;
                float _2051 = _2049.y;
                float _2052 = isnan(_2051) ? _2050 : (isnan(_2050) ? _2051 : max(_2050, _2051));
                bvec2 _3716 = isnan(_2049);
                bvec2 _3717 = isnan(vec2(0.0));
                vec2 _3718 = max(_2049, vec2(0.0));
                vec2 _3719 = vec2(_3716.x ? vec2(0.0).x : _3718.x, _3716.y ? vec2(0.0).y : _3718.y);
                _2057 = (isnan(0.0) ? _2052 : (isnan(_2052) ? 0.0 : min(_2052, 0.0))) + length(vec2(_3717.x ? _2049.x : _3719.x, _3717.y ? _2049.y : _3719.y));
                break;
            } while(false);
            float _2062;
            if (_824)
            {
                _2062 = -_2057;
            }
            else
            {
                _2062 = _1443;
            }
            float _2067;
            if (_824)
            {
                _2067 = -_1443;
            }
            else
            {
                _2067 = _2057;
            }
            if (_2062 < 0.0)
            {
                discard;
            }
            float _2090;
            if (in_var_COLOR1.z >= 1.0)
            {
                float _2081 = in_var_COLOR1.z * 2.0;
                _2090 = (pow(smoothstep((-0.20000000298023223876953125) - _2081, _2081 + 0.20000000298023223876953125, -_2067), 1.89999997615814208984375) * 3.2999999523162841796875) / pow(in_var_COLOR1.z * 1.2000000476837158203125, 0.1500000059604644775390625);
            }
            else
            {
                float _2076 = _824 ? (-1.0) : 1.0;
                _2090 = smoothstep(_2076 - 0.3540000021457672119140625, _2076 + 0.3540000021457672119140625, -_2067);
            }
            float _2093 = clamp(_2090, 0.0, 1.0) * in_var_COLOR0.w;
            _2988 = vec4(in_var_COLOR0.xyz * _2093, _2093);
            break;
        }
        case 9u:
        {
            vec4 _820;
            do
            {
                vec4 _199 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0);
                vec4 _200 = _199 * in_var_COLOR0;
                vec4 _204 = texture(SPIRV_Cross_CombinedTexture1Sampler0, in_var_TEXCOORD1);
                vec4 _817;
                bool _818;
                switch (uint(in_var_COLOR1.y + 0.5))
                {
                    case 0u:
                    {
                        _817 = vec4(0.0);
                        _818 = true;
                        break;
                    }
                    case 1u:
                    {
                        _817 = _200;
                        _818 = true;
                        break;
                    }
                    case 2u:
                    {
                        _817 = _200 + (_204 * (1.0 - _200.w));
                        _818 = true;
                        break;
                    }
                    case 3u:
                    {
                        _817 = _200 * _204.w;
                        _818 = true;
                        break;
                    }
                    case 4u:
                    {
                        _817 = _200 * (1.0 - _204.w);
                        _818 = true;
                        break;
                    }
                    case 5u:
                    {
                        _817 = (_200 * _204.w) + (_204 * (1.0 - _200.w));
                        _818 = true;
                        break;
                    }
                    case 6u:
                    {
                        _817 = (_200 * (1.0 - _204.w)) + _204;
                        _818 = true;
                        break;
                    }
                    case 7u:
                    {
                        _817 = _204 * _200.w;
                        _818 = true;
                        break;
                    }
                    case 8u:
                    {
                        _817 = _204 * (1.0 - _200.w);
                        _818 = true;
                        break;
                    }
                    case 9u:
                    {
                        _817 = (_200 * (1.0 - _204.w)) + (_204 * _200.w);
                        _818 = true;
                        break;
                    }
                    case 10u:
                    {
                        _817 = clamp((_200 * (1.0 - _204.w)) + (_204 * (1.0 - _200.w)), vec4(0.0), vec4(1.0));
                        _818 = true;
                        break;
                    }
                    case 11u:
                    {
                        vec3 _768 = _200.xyz;
                        vec3 _769 = _204.xyz;
                        bvec3 _3721 = isnan(_768);
                        bvec3 _3722 = isnan(_769);
                        vec3 _3723 = min(_768, _769);
                        vec3 _3724 = vec3(_3721.x ? _769.x : _3723.x, _3721.y ? _769.y : _3723.y, _3721.z ? _769.z : _3723.z);
                        float _771 = _200.w;
                        _817 = vec4(vec3(_3722.x ? _768.x : _3724.x, _3722.y ? _768.y : _3724.y, _3722.z ? _768.z : _3724.z) * _771, _204.w * _771);
                        _818 = true;
                        break;
                    }
                    case 12u:
                    {
                        _817 = clamp(_200 + _204, vec4(0.0), vec4(1.0));
                        _818 = true;
                        break;
                    }
                    case 13u:
                    {
                        float _758 = _200.w;
                        _817 = vec4(abs(_204.xyz - _200.xyz) * _758, _204.w * _758);
                        _818 = true;
                        break;
                    }
                    case 14u:
                    {
                        float _746 = _200.w;
                        _817 = vec4((_200.xyz * _204.xyz) * _746, _204.w * _746);
                        _818 = true;
                        break;
                    }
                    case 15u:
                    {
                        float _735 = _200.w;
                        _817 = vec4((vec3(1.0) - ((vec3(1.0) - _204.xyz) * (vec3(1.0) - _200.xyz))) * _735, _204.w * _735);
                        _818 = true;
                        break;
                    }
                    case 16u:
                    {
                        vec3 _123 = _200.xyz;
                        vec3 _124 = _204.xyz;
                        vec3 _118;
                        for (uint _693 = 0u; _693 < 3u; _693++)
                        {
                            float _718;
                            if (_124[_693] < 0.5)
                            {
                                _718 = (2.0 * _124[_693]) * _123[_693];
                            }
                            else
                            {
                                _718 = 1.0 - ((2.0 * (1.0 - _124[_693])) * (1.0 - _123[_693]));
                            }
                            _118[_693] = _718;
                        }
                        float _721 = _200.w;
                        _817 = vec4(_118 * _721, _204.w * _721);
                        _818 = true;
                        break;
                    }
                    case 17u:
                    {
                        vec3 _679 = _200.xyz;
                        vec3 _680 = _204.xyz;
                        bvec3 _3726 = isnan(_679);
                        bvec3 _3727 = isnan(_680);
                        vec3 _3728 = max(_679, _680);
                        vec3 _3729 = vec3(_3726.x ? _680.x : _3728.x, _3726.y ? _680.y : _3728.y, _3726.z ? _680.z : _3728.z);
                        float _682 = _200.w;
                        _817 = vec4(vec3(_3727.x ? _679.x : _3729.x, _3727.y ? _679.y : _3729.y, _3727.z ? _679.z : _3729.z) * _682, _204.w * _682);
                        _818 = true;
                        break;
                    }
                    case 18u:
                    {
                        vec3 _125 = _200.xyz;
                        vec3 _126 = _204.xyz;
                        vec3 _119;
                        for (uint _649 = 0u; _649 < 3u; _649++)
                        {
                            float _668;
                            if (_125[_649] == 1.0)
                            {
                                _668 = _125[_649];
                            }
                            else
                            {
                                float _665 = _126[_649] / (1.0 - _125[_649]);
                                _668 = isnan(1.0) ? _665 : (isnan(_665) ? 1.0 : min(_665, 1.0));
                            }
                            _119[_649] = _668;
                        }
                        float _671 = _200.w;
                        _817 = vec4(_119 * _671, _204.w * _671);
                        _818 = true;
                        break;
                    }
                    case 19u:
                    {
                        vec3 _127 = _200.xyz;
                        vec3 _128 = _204.xyz;
                        vec3 _120;
                        for (uint _615 = 0u; _615 < 3u; _615++)
                        {
                            float _635;
                            if (_127[_615] == 0.0)
                            {
                                _635 = _127[_615];
                            }
                            else
                            {
                                float _632 = 1.0 - ((1.0 - _128[_615]) / _127[_615]);
                                _635 = isnan(0.0) ? _632 : (isnan(_632) ? 0.0 : max(_632, 0.0));
                            }
                            _120[_615] = _635;
                        }
                        float _638 = _200.w;
                        _817 = vec4(_120 * _638, _204.w * _638);
                        _818 = true;
                        break;
                    }
                    case 20u:
                    {
                        vec3 _129 = _204.xyz;
                        vec3 _130 = _200.xyz;
                        vec3 _121;
                        for (uint _576 = 0u; _576 < 3u; _576++)
                        {
                            float _601;
                            if (_130[_576] < 0.5)
                            {
                                _601 = (2.0 * _130[_576]) * _129[_576];
                            }
                            else
                            {
                                _601 = 1.0 - ((2.0 * (1.0 - _130[_576])) * (1.0 - _129[_576]));
                            }
                            _121[_576] = _601;
                        }
                        float _604 = _200.w;
                        _817 = vec4(_121 * _604, _204.w * _604);
                        _818 = true;
                        break;
                    }
                    case 21u:
                    {
                        vec3 _131 = _200.xyz;
                        vec3 _132 = _204.xyz;
                        vec3 _122;
                        for (uint _524 = 0u; _524 < 3u; _524++)
                        {
                            float _562;
                            if (_131[_524] < 0.5)
                            {
                                _562 = ((2.0 * _132[_524]) * _131[_524]) + ((_132[_524] * _132[_524]) * (1.0 - (2.0 * _131[_524])));
                            }
                            else
                            {
                                _562 = (sqrt(_132[_524]) * ((2.0 * _131[_524]) - 1.0)) + ((2.0 * _132[_524]) * (1.0 - _131[_524]));
                            }
                            _122[_524] = _562;
                        }
                        float _565 = _200.w;
                        _817 = vec4(_122 * _565, _204.w * _565);
                        _818 = true;
                        break;
                    }
                    case 22u:
                    {
                        vec3 _507 = _204.xyz;
                        vec3 _508 = _200.xyz;
                        float _513 = _200.w;
                        _817 = vec4(((_507 + _508) - ((_507 * 2.0) * _508)) * _513, _204.w * _513);
                        _818 = true;
                        break;
                    }
                    case 23u:
                    {
                        vec3 _441 = _200.xyz;
                        float _442 = _204.x;
                        float _443 = _204.y;
                        float _444 = _204.z;
                        float _445 = isnan(_444) ? _443 : (isnan(_443) ? _444 : min(_443, _444));
                        float _446 = isnan(_445) ? _442 : (isnan(_442) ? _445 : min(_442, _445));
                        float _447 = isnan(_444) ? _443 : (isnan(_443) ? _444 : max(_443, _444));
                        float _448 = isnan(_447) ? _442 : (isnan(_442) ? _447 : max(_442, _447));
                        float _450 = _446 + _448;
                        float _452 = abs(_450 - 1.0);
                        float _456 = _200.x;
                        float _457 = _200.y;
                        float _458 = _200.z;
                        float _459 = isnan(_458) ? _457 : (isnan(_457) ? _458 : min(_457, _458));
                        float _461 = isnan(_458) ? _457 : (isnan(_457) ? _458 : max(_457, _458));
                        vec3 _477 = ((step(_200.yxx, _441) * step(_200.zzy, _441)) * (vec3(0.0, 2.0, 4.0) + ((_200.yzx - _200.zxy) / vec3(((isnan(_461) ? _456 : (isnan(_456) ? _461 : max(_456, _461))) - (isnan(_459) ? _456 : (isnan(_456) ? _459 : min(_456, _459)))) + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        vec3 _487 = vec3(fract(((1.0 + _477.x) + _477.y) + _477.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _499 = _200.w;
                        _817 = vec4((vec3(_450 * 0.5) + (((clamp(abs((_487 - vec3(6.0) * trunc(_487 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * ((_448 - _446) / (1.00000011920928955078125 - _452))) * (1.0 - _452))) * _499, _204.w * _499);
                        _818 = true;
                        break;
                    }
                    case 24u:
                    {
                        vec3 _372 = _204.xyz;
                        float _373 = _204.x;
                        float _374 = _204.y;
                        float _375 = _204.z;
                        float _376 = isnan(_375) ? _374 : (isnan(_374) ? _375 : min(_374, _375));
                        float _377 = isnan(_376) ? _373 : (isnan(_373) ? _376 : min(_373, _376));
                        float _378 = isnan(_375) ? _374 : (isnan(_374) ? _375 : max(_374, _375));
                        float _379 = isnan(_378) ? _373 : (isnan(_373) ? _378 : max(_373, _378));
                        vec3 _394 = ((step(_204.yxx, _372) * step(_204.zzy, _372)) * (vec3(0.0, 2.0, 4.0) + ((_204.yzx - _204.zxy) / vec3((_379 - _377) + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        float _402 = _377 + _379;
                        float _404 = _200.x;
                        float _405 = _200.y;
                        float _406 = _200.z;
                        float _407 = isnan(_406) ? _405 : (isnan(_405) ? _406 : min(_405, _406));
                        float _408 = isnan(_407) ? _404 : (isnan(_404) ? _407 : min(_404, _407));
                        float _409 = isnan(_406) ? _405 : (isnan(_405) ? _406 : max(_405, _406));
                        float _410 = isnan(_409) ? _404 : (isnan(_404) ? _409 : max(_404, _409));
                        vec3 _419 = vec3(fract(((1.0 + _394.x) + _394.y) + _394.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _433 = _200.w;
                        _817 = vec4((vec3(_402 * 0.5) + (((clamp(abs((_419 - vec3(6.0) * trunc(_419 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * ((_410 - _408) / (1.00000011920928955078125 - abs((_408 + _410) - 1.0)))) * (1.0 - abs(_402 - 1.0)))) * _433, _204.w * _433);
                        _818 = true;
                        break;
                    }
                    case 25u:
                    {
                        vec3 _304 = _200.xyz;
                        float _305 = _200.x;
                        float _306 = _200.y;
                        float _307 = _200.z;
                        float _308 = isnan(_307) ? _306 : (isnan(_306) ? _307 : min(_306, _307));
                        float _309 = isnan(_308) ? _305 : (isnan(_305) ? _308 : min(_305, _308));
                        float _310 = isnan(_307) ? _306 : (isnan(_306) ? _307 : max(_306, _307));
                        float _311 = isnan(_310) ? _305 : (isnan(_305) ? _310 : max(_305, _310));
                        float _320 = _311 - _309;
                        vec3 _326 = ((step(_200.yxx, _304) * step(_200.zzy, _304)) * (vec3(0.0, 2.0, 4.0) + ((_200.yzx - _200.zxy) / vec3(_320 + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        float _339 = _204.x;
                        float _340 = _204.y;
                        float _341 = _204.z;
                        float _342 = isnan(_341) ? _340 : (isnan(_340) ? _341 : min(_340, _341));
                        float _344 = isnan(_341) ? _340 : (isnan(_340) ? _341 : max(_340, _341));
                        float _346 = (isnan(_342) ? _339 : (isnan(_339) ? _342 : min(_339, _342))) + (isnan(_344) ? _339 : (isnan(_339) ? _344 : max(_339, _344)));
                        vec3 _350 = vec3(fract(((1.0 + _326.x) + _326.y) + _326.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _364 = _200.w;
                        _817 = vec4((vec3(_346 * 0.5) + (((clamp(abs((_350 - vec3(6.0) * trunc(_350 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * (_320 / (1.00000011920928955078125 - abs((_309 + _311) - 1.0)))) * (1.0 - abs(_346 - 1.0)))) * _364, _204.w * _364);
                        _818 = true;
                        break;
                    }
                    case 26u:
                    {
                        vec3 _236 = _204.xyz;
                        float _237 = _204.x;
                        float _238 = _204.y;
                        float _239 = _204.z;
                        float _240 = isnan(_239) ? _238 : (isnan(_238) ? _239 : min(_238, _239));
                        float _241 = isnan(_240) ? _237 : (isnan(_237) ? _240 : min(_237, _240));
                        float _242 = isnan(_239) ? _238 : (isnan(_238) ? _239 : max(_238, _239));
                        float _243 = isnan(_242) ? _237 : (isnan(_237) ? _242 : max(_237, _242));
                        float _252 = _243 - _241;
                        vec3 _258 = ((step(_204.yxx, _236) * step(_204.zzy, _236)) * (vec3(0.0, 2.0, 4.0) + ((_204.yzx - _204.zxy) / vec3(_252 + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        float _271 = _200.x;
                        float _272 = _200.y;
                        float _273 = _200.z;
                        float _274 = isnan(_273) ? _272 : (isnan(_272) ? _273 : min(_272, _273));
                        float _276 = isnan(_273) ? _272 : (isnan(_272) ? _273 : max(_272, _273));
                        float _278 = (isnan(_274) ? _271 : (isnan(_271) ? _274 : min(_271, _274))) + (isnan(_276) ? _271 : (isnan(_271) ? _276 : max(_271, _276)));
                        vec3 _282 = vec3(fract(((1.0 + _258.x) + _258.y) + _258.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _296 = _200.w;
                        _817 = vec4((vec3(_278 * 0.5) + (((clamp(abs((_282 - vec3(6.0) * trunc(_282 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * (_252 / (1.00000011920928955078125 - abs((_241 + _243) - 1.0)))) * (1.0 - abs(_278 - 1.0)))) * _296, _204.w * _296);
                        _818 = true;
                        break;
                    }
                    default:
                    {
                        _817 = _103;
                        _818 = false;
                        break;
                    }
                }
                if (_818)
                {
                    _820 = _817;
                    break;
                }
                _820 = _200;
                break;
            } while(false);
            _2988 = _820;
            break;
        }
        case 10u:
        {
            vec4 _180 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0) * in_var_COLOR0;
            vec4 _184 = texture(SPIRV_Cross_CombinedTexture1Sampler0, in_var_TEXCOORD1);
            float _185 = _184.w;
            _2988 = vec4(_180.xyz * _185, _180.w * _185);
            break;
        }
        case 11u:
        {
            vec4 _168 = texture(SPIRV_Cross_CombinedTexture1Sampler0, vec2(texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0).w * in_var_COLOR0.w, in_var_COLOR1.y));
            float _169 = _168.w;
            _2988 = vec4(in_var_COLOR0.xyz * _169, _169);
            break;
        }
        default:
        {
            _2988 = in_var_COLOR0;
            break;
        }
    }
    vec4 _2990;
    _2990 = _2988;
    vec4 _2991;
    for (uint _2993 = 0u; _2993 < uint(Uniforms.ClipData.x); _2990 = _2991, _2993++)
    {
        vec4 _3007 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_2993])[0].x, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[1].x, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[2].x, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[3].x);
        vec4 _3014 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_2993])[0].y, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[1].y, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[2].y, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[3].y);
        vec4 _3016 = floor(_3014 * vec4(1.52587890625e-05));
        vec4 _3019 = floor(_3014 - (_3016 * 65536.0));
        vec4 _3029 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_2993])[0].z, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[1].z, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[2].z, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[3].z);
        vec2 _3039 = (((_3029.xy * in_var_TEXCOORD1.x) + (_3029.zw * in_var_TEXCOORD1.y)) + vec4(spvWorkaroundRowMajor(Uniforms.Clip[_2993])[0].w, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[1].w, spvWorkaroundRowMajor(Uniforms.Clip[_2993])[2].w, _116).xy) - _3007.xy;
        float _3652;
        do
        {
            vec2 _3042 = _3007.zw * 0.5;
            float _3043 = _3042.x;
            float _3044 = -_3043;
            float _3045 = _3016.x;
            float _3046 = _3044 + _3045;
            float _3047 = _3042.y;
            float _3048 = -_3047;
            float _3049 = _3019.x;
            float _3050 = _3048 + _3049;
            vec2 _3052 = _3039 - vec2(_3046, _3050);
            bool _3059;
            if ((_3045 * _3049) > 0.0)
            {
                _3059 = _3039.x < _3046;
            }
            else
            {
                _3059 = false;
            }
            bool _3064;
            if (_3059)
            {
                _3064 = _3039.y <= _3050;
            }
            else
            {
                _3064 = false;
            }
            if (_3064)
            {
                vec2 _3067 = vec2(_3045, _3049);
                float _3195;
                do
                {
                    if (abs(_3045 - _3049) < 0.100000001490116119384765625)
                    {
                        _3195 = length(_3052) - _3045;
                        break;
                    }
                    vec2 _3077 = abs(_3052);
                    vec2 _3085;
                    vec2 _3086;
                    if (_3077.x > _3077.y)
                    {
                        _3085 = _3077.yx;
                        _3086 = _3067.yx;
                    }
                    else
                    {
                        _3085 = _3077;
                        _3086 = _3067;
                    }
                    float _3091 = (_3086.y * _3086.y) - (_3086.x * _3086.x);
                    float _3094 = (_3086.x * _3085.x) / _3091;
                    float _3097 = (_3086.y * _3085.y) / _3091;
                    float _3098 = _3094 * _3094;
                    float _3099 = _3097 * _3097;
                    float _3101 = (_3098 + _3099) - 1.0;
                    float _3102 = _3101 * 0.3333333432674407958984375;
                    float _3104 = (_3102 * _3102) * _3102;
                    float _3105 = _3098 * _3099;
                    float _3107 = _3104 + (_3105 * 2.0);
                    float _3108 = _3104 + _3105;
                    float _3110 = _3094 + (_3094 * _3099);
                    float _3181;
                    if (_3108 < 0.0)
                    {
                        float _3155 = acos(_3107 / _3104) * 0.3333333432674407958984375;
                        float _3156 = cos(_3155);
                        float _3158 = sin(_3155) * 1.73205077648162841796875;
                        float _3159 = _3101 * (-0.3333333432674407958984375);
                        float _3164 = sqrt((_3159 * ((_3156 + _3158) + 2.0)) + _3098);
                        float _3169 = sqrt((_3159 * ((_3156 - _3158) + 2.0)) + _3098);
                        _3181 = (((_3169 + (float(int(sign(_3091))) * _3164)) + (abs(_3110) / (_3164 * _3169))) - _3094) * 0.5;
                    }
                    else
                    {
                        float _3118 = ((2.0 * _3094) * _3097) * sqrt(_3108);
                        float _3119 = _3107 + _3118;
                        float _3125 = float(int(sign(_3119))) * pow(abs(_3119), 0.3333333432674407958984375);
                        float _3126 = _3107 - _3118;
                        float _3132 = float(int(sign(_3126))) * pow(abs(_3126), 0.3333333432674407958984375);
                        float _3138 = (((-_3125) - _3132) - (_3101 * 1.33333337306976318359375)) + (2.0 * _3098);
                        float _3140 = (_3125 - _3132) * 1.73205077648162841796875;
                        float _3144 = sqrt((_3138 * _3138) + (_3140 * _3140));
                        _3181 = (((_3140 / sqrt(_3144 - _3138)) + ((2.0 * _3110) / _3144)) - _3094) * 0.5;
                    }
                    float _3186 = _3086.y * sqrt(1.0 - (_3181 * _3181));
                    _3195 = length(vec2(_3086.x * _3181, _3186) - _3085) * float(int(sign(_3085.y - _3186)));
                    break;
                } while(false);
                _3652 = _3195;
                break;
            }
            float _3196 = _3016.y;
            float _3197 = _3043 - _3196;
            float _3198 = _3019.y;
            float _3199 = _3048 + _3198;
            vec2 _3201 = _3039 - vec2(_3197, _3199);
            bool _3208;
            if ((_3196 * _3198) > 0.0)
            {
                _3208 = _3039.x >= _3197;
            }
            else
            {
                _3208 = false;
            }
            bool _3213;
            if (_3208)
            {
                _3213 = _3039.y <= _3199;
            }
            else
            {
                _3213 = false;
            }
            if (_3213)
            {
                vec2 _3216 = vec2(_3196, _3198);
                float _3344;
                do
                {
                    if (abs(_3196 - _3198) < 0.100000001490116119384765625)
                    {
                        _3344 = length(_3201) - _3196;
                        break;
                    }
                    vec2 _3226 = abs(_3201);
                    vec2 _3234;
                    vec2 _3235;
                    if (_3226.x > _3226.y)
                    {
                        _3234 = _3226.yx;
                        _3235 = _3216.yx;
                    }
                    else
                    {
                        _3234 = _3226;
                        _3235 = _3216;
                    }
                    float _3240 = (_3235.y * _3235.y) - (_3235.x * _3235.x);
                    float _3243 = (_3235.x * _3234.x) / _3240;
                    float _3246 = (_3235.y * _3234.y) / _3240;
                    float _3247 = _3243 * _3243;
                    float _3248 = _3246 * _3246;
                    float _3250 = (_3247 + _3248) - 1.0;
                    float _3251 = _3250 * 0.3333333432674407958984375;
                    float _3253 = (_3251 * _3251) * _3251;
                    float _3254 = _3247 * _3248;
                    float _3256 = _3253 + (_3254 * 2.0);
                    float _3257 = _3253 + _3254;
                    float _3259 = _3243 + (_3243 * _3248);
                    float _3330;
                    if (_3257 < 0.0)
                    {
                        float _3304 = acos(_3256 / _3253) * 0.3333333432674407958984375;
                        float _3305 = cos(_3304);
                        float _3307 = sin(_3304) * 1.73205077648162841796875;
                        float _3308 = _3250 * (-0.3333333432674407958984375);
                        float _3313 = sqrt((_3308 * ((_3305 + _3307) + 2.0)) + _3247);
                        float _3318 = sqrt((_3308 * ((_3305 - _3307) + 2.0)) + _3247);
                        _3330 = (((_3318 + (float(int(sign(_3240))) * _3313)) + (abs(_3259) / (_3313 * _3318))) - _3243) * 0.5;
                    }
                    else
                    {
                        float _3267 = ((2.0 * _3243) * _3246) * sqrt(_3257);
                        float _3268 = _3256 + _3267;
                        float _3274 = float(int(sign(_3268))) * pow(abs(_3268), 0.3333333432674407958984375);
                        float _3275 = _3256 - _3267;
                        float _3281 = float(int(sign(_3275))) * pow(abs(_3275), 0.3333333432674407958984375);
                        float _3287 = (((-_3274) - _3281) - (_3250 * 1.33333337306976318359375)) + (2.0 * _3247);
                        float _3289 = (_3274 - _3281) * 1.73205077648162841796875;
                        float _3293 = sqrt((_3287 * _3287) + (_3289 * _3289));
                        _3330 = (((_3289 / sqrt(_3293 - _3287)) + ((2.0 * _3259) / _3293)) - _3243) * 0.5;
                    }
                    float _3335 = _3235.y * sqrt(1.0 - (_3330 * _3330));
                    _3344 = length(vec2(_3235.x * _3330, _3335) - _3234) * float(int(sign(_3234.y - _3335)));
                    break;
                } while(false);
                _3652 = _3344;
                break;
            }
            float _3345 = _3016.z;
            float _3346 = _3043 - _3345;
            float _3347 = _3019.z;
            float _3348 = _3047 - _3347;
            vec2 _3350 = _3039 - vec2(_3346, _3348);
            bool _3357;
            if ((_3345 * _3347) > 0.0)
            {
                _3357 = _3039.x >= _3346;
            }
            else
            {
                _3357 = false;
            }
            bool _3362;
            if (_3357)
            {
                _3362 = _3039.y >= _3348;
            }
            else
            {
                _3362 = false;
            }
            if (_3362)
            {
                vec2 _3365 = vec2(_3345, _3347);
                float _3493;
                do
                {
                    if (abs(_3345 - _3347) < 0.100000001490116119384765625)
                    {
                        _3493 = length(_3350) - _3345;
                        break;
                    }
                    vec2 _3375 = abs(_3350);
                    vec2 _3383;
                    vec2 _3384;
                    if (_3375.x > _3375.y)
                    {
                        _3383 = _3375.yx;
                        _3384 = _3365.yx;
                    }
                    else
                    {
                        _3383 = _3375;
                        _3384 = _3365;
                    }
                    float _3389 = (_3384.y * _3384.y) - (_3384.x * _3384.x);
                    float _3392 = (_3384.x * _3383.x) / _3389;
                    float _3395 = (_3384.y * _3383.y) / _3389;
                    float _3396 = _3392 * _3392;
                    float _3397 = _3395 * _3395;
                    float _3399 = (_3396 + _3397) - 1.0;
                    float _3400 = _3399 * 0.3333333432674407958984375;
                    float _3402 = (_3400 * _3400) * _3400;
                    float _3403 = _3396 * _3397;
                    float _3405 = _3402 + (_3403 * 2.0);
                    float _3406 = _3402 + _3403;
                    float _3408 = _3392 + (_3392 * _3397);
                    float _3479;
                    if (_3406 < 0.0)
                    {
                        float _3453 = acos(_3405 / _3402) * 0.3333333432674407958984375;
                        float _3454 = cos(_3453);
                        float _3456 = sin(_3453) * 1.73205077648162841796875;
                        float _3457 = _3399 * (-0.3333333432674407958984375);
                        float _3462 = sqrt((_3457 * ((_3454 + _3456) + 2.0)) + _3396);
                        float _3467 = sqrt((_3457 * ((_3454 - _3456) + 2.0)) + _3396);
                        _3479 = (((_3467 + (float(int(sign(_3389))) * _3462)) + (abs(_3408) / (_3462 * _3467))) - _3392) * 0.5;
                    }
                    else
                    {
                        float _3416 = ((2.0 * _3392) * _3395) * sqrt(_3406);
                        float _3417 = _3405 + _3416;
                        float _3423 = float(int(sign(_3417))) * pow(abs(_3417), 0.3333333432674407958984375);
                        float _3424 = _3405 - _3416;
                        float _3430 = float(int(sign(_3424))) * pow(abs(_3424), 0.3333333432674407958984375);
                        float _3436 = (((-_3423) - _3430) - (_3399 * 1.33333337306976318359375)) + (2.0 * _3396);
                        float _3438 = (_3423 - _3430) * 1.73205077648162841796875;
                        float _3442 = sqrt((_3436 * _3436) + (_3438 * _3438));
                        _3479 = (((_3438 / sqrt(_3442 - _3436)) + ((2.0 * _3408) / _3442)) - _3392) * 0.5;
                    }
                    float _3484 = _3384.y * sqrt(1.0 - (_3479 * _3479));
                    _3493 = length(vec2(_3384.x * _3479, _3484) - _3383) * float(int(sign(_3383.y - _3484)));
                    break;
                } while(false);
                _3652 = _3493;
                break;
            }
            float _3494 = _3016.w;
            float _3495 = _3044 + _3494;
            float _3496 = _3019.w;
            float _3497 = _3047 - _3496;
            vec2 _3499 = _3039 - vec2(_3495, _3497);
            bool _3506;
            if ((_3494 * _3496) > 0.0)
            {
                _3506 = _3039.x < _3495;
            }
            else
            {
                _3506 = false;
            }
            bool _3511;
            if (_3506)
            {
                _3511 = _3039.y > _3497;
            }
            else
            {
                _3511 = false;
            }
            if (_3511)
            {
                vec2 _3514 = vec2(_3494, _3496);
                float _3642;
                do
                {
                    if (abs(_3494 - _3496) < 0.100000001490116119384765625)
                    {
                        _3642 = length(_3499) - _3494;
                        break;
                    }
                    vec2 _3524 = abs(_3499);
                    vec2 _3532;
                    vec2 _3533;
                    if (_3524.x > _3524.y)
                    {
                        _3532 = _3524.yx;
                        _3533 = _3514.yx;
                    }
                    else
                    {
                        _3532 = _3524;
                        _3533 = _3514;
                    }
                    float _3538 = (_3533.y * _3533.y) - (_3533.x * _3533.x);
                    float _3541 = (_3533.x * _3532.x) / _3538;
                    float _3544 = (_3533.y * _3532.y) / _3538;
                    float _3545 = _3541 * _3541;
                    float _3546 = _3544 * _3544;
                    float _3548 = (_3545 + _3546) - 1.0;
                    float _3549 = _3548 * 0.3333333432674407958984375;
                    float _3551 = (_3549 * _3549) * _3549;
                    float _3552 = _3545 * _3546;
                    float _3554 = _3551 + (_3552 * 2.0);
                    float _3555 = _3551 + _3552;
                    float _3557 = _3541 + (_3541 * _3546);
                    float _3628;
                    if (_3555 < 0.0)
                    {
                        float _3602 = acos(_3554 / _3551) * 0.3333333432674407958984375;
                        float _3603 = cos(_3602);
                        float _3605 = sin(_3602) * 1.73205077648162841796875;
                        float _3606 = _3548 * (-0.3333333432674407958984375);
                        float _3611 = sqrt((_3606 * ((_3603 + _3605) + 2.0)) + _3545);
                        float _3616 = sqrt((_3606 * ((_3603 - _3605) + 2.0)) + _3545);
                        _3628 = (((_3616 + (float(int(sign(_3538))) * _3611)) + (abs(_3557) / (_3611 * _3616))) - _3541) * 0.5;
                    }
                    else
                    {
                        float _3565 = ((2.0 * _3541) * _3544) * sqrt(_3555);
                        float _3566 = _3554 + _3565;
                        float _3572 = float(int(sign(_3566))) * pow(abs(_3566), 0.3333333432674407958984375);
                        float _3573 = _3554 - _3565;
                        float _3579 = float(int(sign(_3573))) * pow(abs(_3573), 0.3333333432674407958984375);
                        float _3585 = (((-_3572) - _3579) - (_3548 * 1.33333337306976318359375)) + (2.0 * _3545);
                        float _3587 = (_3572 - _3579) * 1.73205077648162841796875;
                        float _3591 = sqrt((_3585 * _3585) + (_3587 * _3587));
                        _3628 = (((_3587 / sqrt(_3591 - _3585)) + ((2.0 * _3557) / _3591)) - _3541) * 0.5;
                    }
                    float _3633 = _3533.y * sqrt(1.0 - (_3628 * _3628));
                    _3642 = length(vec2(_3533.x * _3628, _3633) - _3532) * float(int(sign(_3532.y - _3633)));
                    break;
                } while(false);
                _3652 = _3642;
                break;
            }
            vec2 _3644 = abs(_3039) - _3042;
            float _3645 = _3644.x;
            float _3646 = _3644.y;
            float _3647 = isnan(_3646) ? _3645 : (isnan(_3645) ? _3646 : max(_3645, _3646));
            bvec2 _3911 = isnan(_3644);
            bvec2 _3912 = isnan(vec2(0.0));
            vec2 _3913 = max(_3644, vec2(0.0));
            vec2 _3914 = vec2(_3911.x ? vec2(0.0).x : _3913.x, _3911.y ? vec2(0.0).y : _3913.y);
            _3652 = (isnan(0.0) ? _3647 : (isnan(_3647) ? 0.0 : min(_3647, 0.0))) + length(vec2(_3912.x ? _3644.x : _3914.x, _3912.y ? _3644.y : _3914.y));
            break;
        } while(false);
        float _3655 = -(_3652 * ((spvWorkaroundRowMajor(Uniforms.Clip[_2993])[2].w != 0.0) ? (-1.0) : 1.0));
        float _3658 = smoothstep(-0.61804687976837158203125, 0.61804687976837158203125, _3655 / fwidth(_3655));
        _2991 = vec4(_2990.xyz * _3658, _2990.w * _3658);
    }
    out_var_SV_Target = _2990;
}

)GLSL";