// Generated shader header for fill_path_fs
// Generated from: fill_path.glsl

#pragma once

static const char* fill_path_fs_source = R"GLSL(#version 420

float _53;

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

layout(location = 0) in vec4 in_var_COLOR0;
layout(location = 2) in vec2 in_var_TEXCOORD1;
layout(location = 0) out vec4 out_var_SV_Target;

mat4 spvWorkaroundRowMajor(mat4 wrap) { return wrap; }

void main()
{
    vec4 _58;
    _58 = in_var_COLOR0;
    vec4 _59;
    for (uint _61 = 0u; _61 < uint(Uniforms.ClipData.x); _58 = _59, _61++)
    {
        vec4 _75 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_61])[0].x, spvWorkaroundRowMajor(Uniforms.Clip[_61])[1].x, spvWorkaroundRowMajor(Uniforms.Clip[_61])[2].x, spvWorkaroundRowMajor(Uniforms.Clip[_61])[3].x);
        vec4 _82 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_61])[0].y, spvWorkaroundRowMajor(Uniforms.Clip[_61])[1].y, spvWorkaroundRowMajor(Uniforms.Clip[_61])[2].y, spvWorkaroundRowMajor(Uniforms.Clip[_61])[3].y);
        vec4 _84 = floor(_82 * vec4(1.52587890625e-05));
        vec4 _87 = floor(_82 - (_84 * 65536.0));
        vec4 _97 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_61])[0].z, spvWorkaroundRowMajor(Uniforms.Clip[_61])[1].z, spvWorkaroundRowMajor(Uniforms.Clip[_61])[2].z, spvWorkaroundRowMajor(Uniforms.Clip[_61])[3].z);
        vec2 _107 = (((_97.xy * in_var_TEXCOORD1.x) + (_97.zw * in_var_TEXCOORD1.y)) + vec4(spvWorkaroundRowMajor(Uniforms.Clip[_61])[0].w, spvWorkaroundRowMajor(Uniforms.Clip[_61])[1].w, spvWorkaroundRowMajor(Uniforms.Clip[_61])[2].w, _53).xy) - _75.xy;
        float _720;
        do
        {
            vec2 _110 = _75.zw * 0.5;
            float _111 = _110.x;
            float _112 = -_111;
            float _113 = _84.x;
            float _114 = _112 + _113;
            float _115 = _110.y;
            float _116 = -_115;
            float _117 = _87.x;
            float _118 = _116 + _117;
            vec2 _120 = _107 - vec2(_114, _118);
            bool _127;
            if ((_113 * _117) > 0.0)
            {
                _127 = _107.x < _114;
            }
            else
            {
                _127 = false;
            }
            bool _132;
            if (_127)
            {
                _132 = _107.y <= _118;
            }
            else
            {
                _132 = false;
            }
            if (_132)
            {
                vec2 _135 = vec2(_113, _117);
                float _263;
                do
                {
                    if (abs(_113 - _117) < 0.100000001490116119384765625)
                    {
                        _263 = length(_120) - _113;
                        break;
                    }
                    vec2 _145 = abs(_120);
                    vec2 _153;
                    vec2 _154;
                    if (_145.x > _145.y)
                    {
                        _153 = _145.yx;
                        _154 = _135.yx;
                    }
                    else
                    {
                        _153 = _145;
                        _154 = _135;
                    }
                    float _159 = (_154.y * _154.y) - (_154.x * _154.x);
                    float _162 = (_154.x * _153.x) / _159;
                    float _165 = (_154.y * _153.y) / _159;
                    float _166 = _162 * _162;
                    float _167 = _165 * _165;
                    float _169 = (_166 + _167) - 1.0;
                    float _170 = _169 * 0.3333333432674407958984375;
                    float _172 = (_170 * _170) * _170;
                    float _173 = _166 * _167;
                    float _175 = _172 + (_173 * 2.0);
                    float _176 = _172 + _173;
                    float _178 = _162 + (_162 * _167);
                    float _249;
                    if (_176 < 0.0)
                    {
                        float _185 = acos(_175 / _172) * 0.3333333432674407958984375;
                        float _186 = cos(_185);
                        float _188 = sin(_185) * 1.73205077648162841796875;
                        float _189 = _169 * (-0.3333333432674407958984375);
                        float _194 = sqrt((_189 * ((_186 + _188) + 2.0)) + _166);
                        float _199 = sqrt((_189 * ((_186 - _188) + 2.0)) + _166);
                        _249 = (((_199 + (float(int(sign(_159))) * _194)) + (abs(_178) / (_194 * _199))) - _162) * 0.5;
                    }
                    else
                    {
                        float _214 = ((2.0 * _162) * _165) * sqrt(_176);
                        float _215 = _175 + _214;
                        float _221 = float(int(sign(_215))) * pow(abs(_215), 0.3333333432674407958984375);
                        float _222 = _175 - _214;
                        float _228 = float(int(sign(_222))) * pow(abs(_222), 0.3333333432674407958984375);
                        float _234 = (((-_221) - _228) - (_169 * 1.33333337306976318359375)) + (2.0 * _166);
                        float _236 = (_221 - _228) * 1.73205077648162841796875;
                        float _240 = sqrt((_234 * _234) + (_236 * _236));
                        _249 = (((_236 / sqrt(_240 - _234)) + ((2.0 * _178) / _240)) - _162) * 0.5;
                    }
                    float _254 = _154.y * sqrt(1.0 - (_249 * _249));
                    _263 = length(vec2(_154.x * _249, _254) - _153) * float(int(sign(_153.y - _254)));
                    break;
                } while(false);
                _720 = _263;
                break;
            }
            float _264 = _84.y;
            float _265 = _111 - _264;
            float _266 = _87.y;
            float _267 = _116 + _266;
            vec2 _269 = _107 - vec2(_265, _267);
            bool _276;
            if ((_264 * _266) > 0.0)
            {
                _276 = _107.x >= _265;
            }
            else
            {
                _276 = false;
            }
            bool _281;
            if (_276)
            {
                _281 = _107.y <= _267;
            }
            else
            {
                _281 = false;
            }
            if (_281)
            {
                vec2 _284 = vec2(_264, _266);
                float _412;
                do
                {
                    if (abs(_264 - _266) < 0.100000001490116119384765625)
                    {
                        _412 = length(_269) - _264;
                        break;
                    }
                    vec2 _294 = abs(_269);
                    vec2 _302;
                    vec2 _303;
                    if (_294.x > _294.y)
                    {
                        _302 = _294.yx;
                        _303 = _284.yx;
                    }
                    else
                    {
                        _302 = _294;
                        _303 = _284;
                    }
                    float _308 = (_303.y * _303.y) - (_303.x * _303.x);
                    float _311 = (_303.x * _302.x) / _308;
                    float _314 = (_303.y * _302.y) / _308;
                    float _315 = _311 * _311;
                    float _316 = _314 * _314;
                    float _318 = (_315 + _316) - 1.0;
                    float _319 = _318 * 0.3333333432674407958984375;
                    float _321 = (_319 * _319) * _319;
                    float _322 = _315 * _316;
                    float _324 = _321 + (_322 * 2.0);
                    float _325 = _321 + _322;
                    float _327 = _311 + (_311 * _316);
                    float _398;
                    if (_325 < 0.0)
                    {
                        float _334 = acos(_324 / _321) * 0.3333333432674407958984375;
                        float _335 = cos(_334);
                        float _337 = sin(_334) * 1.73205077648162841796875;
                        float _338 = _318 * (-0.3333333432674407958984375);
                        float _343 = sqrt((_338 * ((_335 + _337) + 2.0)) + _315);
                        float _348 = sqrt((_338 * ((_335 - _337) + 2.0)) + _315);
                        _398 = (((_348 + (float(int(sign(_308))) * _343)) + (abs(_327) / (_343 * _348))) - _311) * 0.5;
                    }
                    else
                    {
                        float _363 = ((2.0 * _311) * _314) * sqrt(_325);
                        float _364 = _324 + _363;
                        float _370 = float(int(sign(_364))) * pow(abs(_364), 0.3333333432674407958984375);
                        float _371 = _324 - _363;
                        float _377 = float(int(sign(_371))) * pow(abs(_371), 0.3333333432674407958984375);
                        float _383 = (((-_370) - _377) - (_318 * 1.33333337306976318359375)) + (2.0 * _315);
                        float _385 = (_370 - _377) * 1.73205077648162841796875;
                        float _389 = sqrt((_383 * _383) + (_385 * _385));
                        _398 = (((_385 / sqrt(_389 - _383)) + ((2.0 * _327) / _389)) - _311) * 0.5;
                    }
                    float _403 = _303.y * sqrt(1.0 - (_398 * _398));
                    _412 = length(vec2(_303.x * _398, _403) - _302) * float(int(sign(_302.y - _403)));
                    break;
                } while(false);
                _720 = _412;
                break;
            }
            float _413 = _84.z;
            float _414 = _111 - _413;
            float _415 = _87.z;
            float _416 = _115 - _415;
            vec2 _418 = _107 - vec2(_414, _416);
            bool _425;
            if ((_413 * _415) > 0.0)
            {
                _425 = _107.x >= _414;
            }
            else
            {
                _425 = false;
            }
            bool _430;
            if (_425)
            {
                _430 = _107.y >= _416;
            }
            else
            {
                _430 = false;
            }
            if (_430)
            {
                vec2 _433 = vec2(_413, _415);
                float _561;
                do
                {
                    if (abs(_413 - _415) < 0.100000001490116119384765625)
                    {
                        _561 = length(_418) - _413;
                        break;
                    }
                    vec2 _443 = abs(_418);
                    vec2 _451;
                    vec2 _452;
                    if (_443.x > _443.y)
                    {
                        _451 = _443.yx;
                        _452 = _433.yx;
                    }
                    else
                    {
                        _451 = _443;
                        _452 = _433;
                    }
                    float _457 = (_452.y * _452.y) - (_452.x * _452.x);
                    float _460 = (_452.x * _451.x) / _457;
                    float _463 = (_452.y * _451.y) / _457;
                    float _464 = _460 * _460;
                    float _465 = _463 * _463;
                    float _467 = (_464 + _465) - 1.0;
                    float _468 = _467 * 0.3333333432674407958984375;
                    float _470 = (_468 * _468) * _468;
                    float _471 = _464 * _465;
                    float _473 = _470 + (_471 * 2.0);
                    float _474 = _470 + _471;
                    float _476 = _460 + (_460 * _465);
                    float _547;
                    if (_474 < 0.0)
                    {
                        float _483 = acos(_473 / _470) * 0.3333333432674407958984375;
                        float _484 = cos(_483);
                        float _486 = sin(_483) * 1.73205077648162841796875;
                        float _487 = _467 * (-0.3333333432674407958984375);
                        float _492 = sqrt((_487 * ((_484 + _486) + 2.0)) + _464);
                        float _497 = sqrt((_487 * ((_484 - _486) + 2.0)) + _464);
                        _547 = (((_497 + (float(int(sign(_457))) * _492)) + (abs(_476) / (_492 * _497))) - _460) * 0.5;
                    }
                    else
                    {
                        float _512 = ((2.0 * _460) * _463) * sqrt(_474);
                        float _513 = _473 + _512;
                        float _519 = float(int(sign(_513))) * pow(abs(_513), 0.3333333432674407958984375);
                        float _520 = _473 - _512;
                        float _526 = float(int(sign(_520))) * pow(abs(_520), 0.3333333432674407958984375);
                        float _532 = (((-_519) - _526) - (_467 * 1.33333337306976318359375)) + (2.0 * _464);
                        float _534 = (_519 - _526) * 1.73205077648162841796875;
                        float _538 = sqrt((_532 * _532) + (_534 * _534));
                        _547 = (((_534 / sqrt(_538 - _532)) + ((2.0 * _476) / _538)) - _460) * 0.5;
                    }
                    float _552 = _452.y * sqrt(1.0 - (_547 * _547));
                    _561 = length(vec2(_452.x * _547, _552) - _451) * float(int(sign(_451.y - _552)));
                    break;
                } while(false);
                _720 = _561;
                break;
            }
            float _562 = _84.w;
            float _563 = _112 + _562;
            float _564 = _87.w;
            float _565 = _115 - _564;
            vec2 _567 = _107 - vec2(_563, _565);
            bool _574;
            if ((_562 * _564) > 0.0)
            {
                _574 = _107.x < _563;
            }
            else
            {
                _574 = false;
            }
            bool _579;
            if (_574)
            {
                _579 = _107.y > _565;
            }
            else
            {
                _579 = false;
            }
            if (_579)
            {
                vec2 _582 = vec2(_562, _564);
                float _710;
                do
                {
                    if (abs(_562 - _564) < 0.100000001490116119384765625)
                    {
                        _710 = length(_567) - _562;
                        break;
                    }
                    vec2 _592 = abs(_567);
                    vec2 _600;
                    vec2 _601;
                    if (_592.x > _592.y)
                    {
                        _600 = _592.yx;
                        _601 = _582.yx;
                    }
                    else
                    {
                        _600 = _592;
                        _601 = _582;
                    }
                    float _606 = (_601.y * _601.y) - (_601.x * _601.x);
                    float _609 = (_601.x * _600.x) / _606;
                    float _612 = (_601.y * _600.y) / _606;
                    float _613 = _609 * _609;
                    float _614 = _612 * _612;
                    float _616 = (_613 + _614) - 1.0;
                    float _617 = _616 * 0.3333333432674407958984375;
                    float _619 = (_617 * _617) * _617;
                    float _620 = _613 * _614;
                    float _622 = _619 + (_620 * 2.0);
                    float _623 = _619 + _620;
                    float _625 = _609 + (_609 * _614);
                    float _696;
                    if (_623 < 0.0)
                    {
                        float _632 = acos(_622 / _619) * 0.3333333432674407958984375;
                        float _633 = cos(_632);
                        float _635 = sin(_632) * 1.73205077648162841796875;
                        float _636 = _616 * (-0.3333333432674407958984375);
                        float _641 = sqrt((_636 * ((_633 + _635) + 2.0)) + _613);
                        float _646 = sqrt((_636 * ((_633 - _635) + 2.0)) + _613);
                        _696 = (((_646 + (float(int(sign(_606))) * _641)) + (abs(_625) / (_641 * _646))) - _609) * 0.5;
                    }
                    else
                    {
                        float _661 = ((2.0 * _609) * _612) * sqrt(_623);
                        float _662 = _622 + _661;
                        float _668 = float(int(sign(_662))) * pow(abs(_662), 0.3333333432674407958984375);
                        float _669 = _622 - _661;
                        float _675 = float(int(sign(_669))) * pow(abs(_669), 0.3333333432674407958984375);
                        float _681 = (((-_668) - _675) - (_616 * 1.33333337306976318359375)) + (2.0 * _613);
                        float _683 = (_668 - _675) * 1.73205077648162841796875;
                        float _687 = sqrt((_681 * _681) + (_683 * _683));
                        _696 = (((_683 / sqrt(_687 - _681)) + ((2.0 * _625) / _687)) - _609) * 0.5;
                    }
                    float _701 = _601.y * sqrt(1.0 - (_696 * _696));
                    _710 = length(vec2(_601.x * _696, _701) - _600) * float(int(sign(_600.y - _701)));
                    break;
                } while(false);
                _720 = _710;
                break;
            }
            vec2 _712 = abs(_107) - _110;
            float _713 = _712.x;
            float _714 = _712.y;
            float _715 = isnan(_714) ? _713 : (isnan(_713) ? _714 : max(_713, _714));
            bvec2 _745 = isnan(_712);
            bvec2 _746 = isnan(vec2(0.0));
            vec2 _747 = max(_712, vec2(0.0));
            vec2 _748 = vec2(_745.x ? vec2(0.0).x : _747.x, _745.y ? vec2(0.0).y : _747.y);
            _720 = (isnan(0.0) ? _715 : (isnan(_715) ? 0.0 : min(_715, 0.0))) + length(vec2(_746.x ? _712.x : _748.x, _746.y ? _712.y : _748.y));
            break;
        } while(false);
        float _723 = -(_720 * ((spvWorkaroundRowMajor(Uniforms.Clip[_61])[2].w != 0.0) ? (-1.0) : 1.0));
        float _726 = smoothstep(-0.61804687976837158203125, 0.61804687976837158203125, _723 / fwidth(_723));
        _59 = vec4(_58.xyz * _726, _58.w * _726);
    }
    out_var_SV_Target = _58;
}

)GLSL";