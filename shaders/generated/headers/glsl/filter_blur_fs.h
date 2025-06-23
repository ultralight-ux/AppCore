// Generated shader header for filter_blur_fs
// Generated from: filter_blur.glsl

#pragma once

static const char* filter_blur_fs_source = R"GLSL(#version 330

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
    vec4 _959;
    if (Uniforms.Integer4[0].y != 0)
    {
        vec2 _329 = vec2(Uniforms.Scalar4[0].x, Uniforms.Scalar4[0].y);
        vec2 _334 = vec2(Uniforms.Scalar4[0].z, Uniforms.Scalar4[0].w);
        float _957;
        do
        {
            bool _345;
            if (Uniforms.Scalar4[0].x < 0.001000000047497451305389404296875)
            {
                _345 = Uniforms.Scalar4[0].y < 0.001000000047497451305389404296875;
            }
            else
            {
                _345 = false;
            }
            if (_345)
            {
                vec2 _348 = in_var_TEXCOORD0 + _334;
                vec4 _352 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _348);
                float _354 = _348.x;
                bool _360;
                if (_354 > 0.0)
                {
                    _360 = _348.y > 0.0;
                }
                else
                {
                    _360 = false;
                }
                bool _364;
                if (_360)
                {
                    _364 = _354 < Uniforms.Scalar4[1].x;
                }
                else
                {
                    _364 = false;
                }
                bool _369;
                if (_364)
                {
                    _369 = _348.y < Uniforms.Scalar4[1].y;
                }
                else
                {
                    _369 = false;
                }
                _957 = _352.w * float(_369);
                break;
            }
            vec2 _372 = in_var_TEXCOORD0 + _334;
            vec4 _376 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _372);
            float _378 = _372.x;
            bool _384;
            if (_378 > 0.0)
            {
                _384 = _372.y > 0.0;
            }
            else
            {
                _384 = false;
            }
            bool _388;
            if (_384)
            {
                _388 = _378 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _388 = false;
            }
            bool _393;
            if (_388)
            {
                _393 = _372.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _393 = false;
            }
            vec2 _399 = (in_var_TEXCOORD0 + (_329 * 0.20000000298023223876953125)) + _334;
            vec4 _403 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _399);
            float _405 = _399.x;
            bool _411;
            if (_405 > 0.0)
            {
                _411 = _399.y > 0.0;
            }
            else
            {
                _411 = false;
            }
            bool _415;
            if (_411)
            {
                _415 = _405 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _415 = false;
            }
            bool _420;
            if (_415)
            {
                _420 = _399.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _420 = false;
            }
            vec2 _427 = (in_var_TEXCOORD0 + (_329 * (-0.20000000298023223876953125))) + _334;
            vec4 _431 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _427);
            float _433 = _427.x;
            bool _439;
            if (_433 > 0.0)
            {
                _439 = _427.y > 0.0;
            }
            else
            {
                _439 = false;
            }
            bool _443;
            if (_439)
            {
                _443 = _433 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _443 = false;
            }
            bool _448;
            if (_443)
            {
                _448 = _427.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _448 = false;
            }
            vec2 _455 = (in_var_TEXCOORD0 + (_329 * 0.4000000059604644775390625)) + _334;
            vec4 _459 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _455);
            float _461 = _455.x;
            bool _467;
            if (_461 > 0.0)
            {
                _467 = _455.y > 0.0;
            }
            else
            {
                _467 = false;
            }
            bool _471;
            if (_467)
            {
                _471 = _461 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _471 = false;
            }
            bool _476;
            if (_471)
            {
                _476 = _455.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _476 = false;
            }
            vec2 _483 = (in_var_TEXCOORD0 + (_329 * (-0.4000000059604644775390625))) + _334;
            vec4 _487 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _483);
            float _489 = _483.x;
            bool _495;
            if (_489 > 0.0)
            {
                _495 = _483.y > 0.0;
            }
            else
            {
                _495 = false;
            }
            bool _499;
            if (_495)
            {
                _499 = _489 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _499 = false;
            }
            bool _504;
            if (_499)
            {
                _504 = _483.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _504 = false;
            }
            vec2 _511 = (in_var_TEXCOORD0 + (_329 * 0.60000002384185791015625)) + _334;
            vec4 _515 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _511);
            float _517 = _511.x;
            bool _523;
            if (_517 > 0.0)
            {
                _523 = _511.y > 0.0;
            }
            else
            {
                _523 = false;
            }
            bool _527;
            if (_523)
            {
                _527 = _517 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _527 = false;
            }
            bool _532;
            if (_527)
            {
                _532 = _511.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _532 = false;
            }
            vec2 _539 = (in_var_TEXCOORD0 + (_329 * (-0.60000002384185791015625))) + _334;
            vec4 _543 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _539);
            float _545 = _539.x;
            bool _551;
            if (_545 > 0.0)
            {
                _551 = _539.y > 0.0;
            }
            else
            {
                _551 = false;
            }
            bool _555;
            if (_551)
            {
                _555 = _545 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _555 = false;
            }
            bool _560;
            if (_555)
            {
                _560 = _539.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _560 = false;
            }
            vec2 _567 = (in_var_TEXCOORD0 + (_329 * 0.800000011920928955078125)) + _334;
            vec4 _571 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _567);
            float _573 = _567.x;
            bool _579;
            if (_573 > 0.0)
            {
                _579 = _567.y > 0.0;
            }
            else
            {
                _579 = false;
            }
            bool _583;
            if (_579)
            {
                _583 = _573 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _583 = false;
            }
            bool _588;
            if (_583)
            {
                _588 = _567.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _588 = false;
            }
            vec2 _595 = (in_var_TEXCOORD0 + (_329 * (-0.800000011920928955078125))) + _334;
            vec4 _599 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _595);
            float _601 = _595.x;
            bool _607;
            if (_601 > 0.0)
            {
                _607 = _595.y > 0.0;
            }
            else
            {
                _607 = false;
            }
            bool _611;
            if (_607)
            {
                _611 = _601 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _611 = false;
            }
            bool _616;
            if (_611)
            {
                _616 = _595.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _616 = false;
            }
            vec2 _623 = (in_var_TEXCOORD0 + (_329 * 1.0)) + _334;
            vec4 _627 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _623);
            float _629 = _623.x;
            bool _635;
            if (_629 > 0.0)
            {
                _635 = _623.y > 0.0;
            }
            else
            {
                _635 = false;
            }
            bool _639;
            if (_635)
            {
                _639 = _629 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _639 = false;
            }
            bool _644;
            if (_639)
            {
                _644 = _623.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _644 = false;
            }
            vec2 _651 = (in_var_TEXCOORD0 + (_329 * (-1.0))) + _334;
            vec4 _655 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _651);
            float _657 = _651.x;
            bool _663;
            if (_657 > 0.0)
            {
                _663 = _651.y > 0.0;
            }
            else
            {
                _663 = false;
            }
            bool _667;
            if (_663)
            {
                _667 = _657 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _667 = false;
            }
            bool _672;
            if (_667)
            {
                _672 = _651.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _672 = false;
            }
            vec2 _679 = (in_var_TEXCOORD0 + (_329 * 1.2000000476837158203125)) + _334;
            vec4 _683 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _679);
            float _685 = _679.x;
            bool _691;
            if (_685 > 0.0)
            {
                _691 = _679.y > 0.0;
            }
            else
            {
                _691 = false;
            }
            bool _695;
            if (_691)
            {
                _695 = _685 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _695 = false;
            }
            bool _700;
            if (_695)
            {
                _700 = _679.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _700 = false;
            }
            float _704 = ((((((((((((_376.w * float(_393)) * 0.0827184617519378662109375) + ((_403.w * float(_420)) * 0.081080533564090728759765625)) + ((_431.w * float(_448)) * 0.081080533564090728759765625)) + ((_459.w * float(_476)) * 0.0763587653636932373046875)) + ((_487.w * float(_504)) * 0.0763587653636932373046875)) + ((_515.w * float(_532)) * 0.069092273712158203125)) + ((_543.w * float(_560)) * 0.069092273712158203125)) + ((_571.w * float(_588)) * 0.060065932571887969970703125)) + ((_599.w * float(_616)) * 0.060065932571887969970703125)) + ((_627.w * float(_644)) * 0.0501712858676910400390625)) + ((_655.w * float(_672)) * 0.0501712858676910400390625)) + ((_683.w * float(_700)) * 0.04026339948177337646484375);
            vec2 _707 = (in_var_TEXCOORD0 + (_329 * (-1.2000000476837158203125))) + _334;
            vec4 _711 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _707);
            float _713 = _707.x;
            bool _719;
            if (_713 > 0.0)
            {
                _719 = _707.y > 0.0;
            }
            else
            {
                _719 = false;
            }
            bool _723;
            if (_719)
            {
                _723 = _713 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _723 = false;
            }
            bool _728;
            if (_723)
            {
                _728 = _707.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _728 = false;
            }
            vec2 _735 = (in_var_TEXCOORD0 + (_329 * 1.39999997615814208984375)) + _334;
            vec4 _739 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _735);
            float _741 = _735.x;
            bool _747;
            if (_741 > 0.0)
            {
                _747 = _735.y > 0.0;
            }
            else
            {
                _747 = false;
            }
            bool _751;
            if (_747)
            {
                _751 = _741 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _751 = false;
            }
            bool _756;
            if (_751)
            {
                _756 = _735.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _756 = false;
            }
            vec2 _763 = (in_var_TEXCOORD0 + (_329 * (-1.39999997615814208984375))) + _334;
            vec4 _767 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _763);
            float _769 = _763.x;
            bool _775;
            if (_769 > 0.0)
            {
                _775 = _763.y > 0.0;
            }
            else
            {
                _775 = false;
            }
            bool _779;
            if (_775)
            {
                _779 = _769 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _779 = false;
            }
            bool _784;
            if (_779)
            {
                _784 = _763.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _784 = false;
            }
            vec2 _791 = (in_var_TEXCOORD0 + (_329 * 1.60000002384185791015625)) + _334;
            vec4 _795 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _791);
            float _797 = _791.x;
            bool _803;
            if (_797 > 0.0)
            {
                _803 = _791.y > 0.0;
            }
            else
            {
                _803 = false;
            }
            bool _807;
            if (_803)
            {
                _807 = _797 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _807 = false;
            }
            bool _812;
            if (_807)
            {
                _812 = _791.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _812 = false;
            }
            vec2 _819 = (in_var_TEXCOORD0 + (_329 * (-1.60000002384185791015625))) + _334;
            vec4 _823 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _819);
            float _825 = _819.x;
            bool _831;
            if (_825 > 0.0)
            {
                _831 = _819.y > 0.0;
            }
            else
            {
                _831 = false;
            }
            bool _835;
            if (_831)
            {
                _835 = _825 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _835 = false;
            }
            bool _840;
            if (_835)
            {
                _840 = _819.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _840 = false;
            }
            vec2 _847 = (in_var_TEXCOORD0 + (_329 * 1.80000007152557373046875)) + _334;
            vec4 _851 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _847);
            float _853 = _847.x;
            bool _859;
            if (_853 > 0.0)
            {
                _859 = _847.y > 0.0;
            }
            else
            {
                _859 = false;
            }
            bool _863;
            if (_859)
            {
                _863 = _853 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _863 = false;
            }
            bool _868;
            if (_863)
            {
                _868 = _847.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _868 = false;
            }
            vec2 _875 = (in_var_TEXCOORD0 + (_329 * (-1.80000007152557373046875))) + _334;
            vec4 _879 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _875);
            float _881 = _875.x;
            bool _887;
            if (_881 > 0.0)
            {
                _887 = _875.y > 0.0;
            }
            else
            {
                _887 = false;
            }
            bool _891;
            if (_887)
            {
                _891 = _881 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _891 = false;
            }
            bool _896;
            if (_891)
            {
                _896 = _875.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _896 = false;
            }
            vec2 _903 = (in_var_TEXCOORD0 + (_329 * 2.0)) + _334;
            vec4 _907 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _903);
            float _909 = _903.x;
            bool _915;
            if (_909 > 0.0)
            {
                _915 = _903.y > 0.0;
            }
            else
            {
                _915 = false;
            }
            bool _919;
            if (_915)
            {
                _919 = _909 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _919 = false;
            }
            bool _924;
            if (_919)
            {
                _924 = _903.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _924 = false;
            }
            vec2 _931 = (in_var_TEXCOORD0 + (_329 * (-2.0))) + _334;
            vec4 _935 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _931);
            float _937 = _931.x;
            bool _943;
            if (_937 > 0.0)
            {
                _943 = _931.y > 0.0;
            }
            else
            {
                _943 = false;
            }
            bool _947;
            if (_943)
            {
                _947 = _937 < Uniforms.Scalar4[1].x;
            }
            else
            {
                _947 = false;
            }
            bool _952;
            if (_947)
            {
                _952 = _931.y < Uniforms.Scalar4[1].y;
            }
            else
            {
                _952 = false;
            }
            _957 = ((((((((_704 + ((_711.w * float(_728)) * 0.04026339948177337646484375)) + ((_739.w * float(_756)) * 0.0310451574623584747314453125)) + ((_767.w * float(_784)) * 0.0310451574623584747314453125)) + ((_795.w * float(_812)) * 0.02299881912767887115478515625)) + ((_823.w * float(_840)) * 0.02299881912767887115478515625)) + ((_851.w * float(_868)) * 0.01636987738311290740966796875)) + ((_879.w * float(_896)) * 0.01636987738311290740966796875)) + ((_907.w * float(_924)) * 0.011194727383553981781005859375)) + ((_935.w * float(_952)) * 0.011194727383553981781005859375);
            break;
        } while(false);
        _959 = vec4(_957);
    }
    else
    {
        vec2 _92 = vec2(Uniforms.Scalar4[0].x, Uniforms.Scalar4[0].y);
        vec2 _97 = vec2(Uniforms.Scalar4[0].z, Uniforms.Scalar4[0].w);
        vec2 _102 = vec2(Uniforms.Scalar4[1].x, Uniforms.Scalar4[1].y);
        vec4 _324;
        do
        {
            bool _109;
            if (Uniforms.Scalar4[0].x < 0.001000000047497451305389404296875)
            {
                _109 = Uniforms.Scalar4[0].y < 0.001000000047497451305389404296875;
            }
            else
            {
                _109 = false;
            }
            if (_109)
            {
                _324 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0 + _97);
                break;
            }
            vec4 _223 = ((((((((((texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp(in_var_TEXCOORD0 + _97, vec2(0.0), _102)) * 0.0827184617519378662109375) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 0.20000000298023223876953125)) + _97, vec2(0.0), _102)) * 0.081080533564090728759765625)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-0.20000000298023223876953125))) + _97, vec2(0.0), _102)) * 0.081080533564090728759765625)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 0.4000000059604644775390625)) + _97, vec2(0.0), _102)) * 0.0763587653636932373046875)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-0.4000000059604644775390625))) + _97, vec2(0.0), _102)) * 0.0763587653636932373046875)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 0.60000002384185791015625)) + _97, vec2(0.0), _102)) * 0.069092273712158203125)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-0.60000002384185791015625))) + _97, vec2(0.0), _102)) * 0.069092273712158203125)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 0.800000011920928955078125)) + _97, vec2(0.0), _102)) * 0.060065932571887969970703125)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-0.800000011920928955078125))) + _97, vec2(0.0), _102)) * 0.060065932571887969970703125)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 1.0)) + _97, vec2(0.0), _102)) * 0.0501712858676910400390625)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-1.0))) + _97, vec2(0.0), _102)) * 0.0501712858676910400390625);
            vec4 _323 = (((((((((_223 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 1.2000000476837158203125)) + _97, vec2(0.0), _102)) * 0.04026339948177337646484375)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-1.2000000476837158203125))) + _97, vec2(0.0), _102)) * 0.04026339948177337646484375)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 1.39999997615814208984375)) + _97, vec2(0.0), _102)) * 0.0310451574623584747314453125)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-1.39999997615814208984375))) + _97, vec2(0.0), _102)) * 0.0310451574623584747314453125)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 1.60000002384185791015625)) + _97, vec2(0.0), _102)) * 0.02299881912767887115478515625)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-1.60000002384185791015625))) + _97, vec2(0.0), _102)) * 0.02299881912767887115478515625)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 1.80000007152557373046875)) + _97, vec2(0.0), _102)) * 0.01636987738311290740966796875)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-1.80000007152557373046875))) + _97, vec2(0.0), _102)) * 0.01636987738311290740966796875)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * 2.0)) + _97, vec2(0.0), _102)) * 0.011194727383553981781005859375)) + (texture(SPIRV_Cross_CombinedTexture0Sampler0, clamp((in_var_TEXCOORD0 + (_92 * (-2.0))) + _97, vec2(0.0), _102)) * 0.011194727383553981781005859375);
            _324 = _323;
            break;
        } while(false);
        _959 = _324;
    }
    vec4 _966;
    if (Uniforms.Integer4[0].x == 1)
    {
        _966 = _959 * in_var_COLOR0;
    }
    else
    {
        _966 = _959;
    }
    out_var_SV_Target = _966;
}

)GLSL";