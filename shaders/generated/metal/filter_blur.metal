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
    float4 _951;
    if (Uniforms.Integer4[0].y != 0)
    {
        float2 _325 = float2(Uniforms.Scalar4[0].x, Uniforms.Scalar4[0].y);
        float2 _330 = float2(Uniforms.Scalar4[0].z, Uniforms.Scalar4[0].w);
        float _949;
        do
        {
            bool _337;
            if (Uniforms.Scalar4[0].x < 0.001000000047497451305389404296875)
            {
                _337 = Uniforms.Scalar4[0].y < 0.001000000047497451305389404296875;
            }
            else
            {
                _337 = false;
            }
            if (_337)
            {
                float2 _340 = in.in_var_TEXCOORD0 + _330;
                float4 _344 = Texture0.sample(Sampler0, _340);
                float _346 = _340.x;
                bool _352;
                if (_346 > 0.0)
                {
                    _352 = _340.y > 0.0;
                }
                else
                {
                    _352 = false;
                }
                bool _356;
                if (_352)
                {
                    _356 = _346 < 1.0;
                }
                else
                {
                    _356 = false;
                }
                bool _361;
                if (_356)
                {
                    _361 = _340.y < 1.0;
                }
                else
                {
                    _361 = false;
                }
                _949 = _344.w * float(_361);
                break;
            }
            float2 _364 = in.in_var_TEXCOORD0 + _330;
            float4 _368 = Texture0.sample(Sampler0, _364);
            float _370 = _364.x;
            bool _376;
            if (_370 > 0.0)
            {
                _376 = _364.y > 0.0;
            }
            else
            {
                _376 = false;
            }
            bool _380;
            if (_376)
            {
                _380 = _370 < 1.0;
            }
            else
            {
                _380 = false;
            }
            bool _385;
            if (_380)
            {
                _385 = _364.y < 1.0;
            }
            else
            {
                _385 = false;
            }
            float2 _391 = (in.in_var_TEXCOORD0 + (_325 * 0.20000000298023223876953125)) + _330;
            float4 _395 = Texture0.sample(Sampler0, _391);
            float _397 = _391.x;
            bool _403;
            if (_397 > 0.0)
            {
                _403 = _391.y > 0.0;
            }
            else
            {
                _403 = false;
            }
            bool _407;
            if (_403)
            {
                _407 = _397 < 1.0;
            }
            else
            {
                _407 = false;
            }
            bool _412;
            if (_407)
            {
                _412 = _391.y < 1.0;
            }
            else
            {
                _412 = false;
            }
            float2 _419 = (in.in_var_TEXCOORD0 + (_325 * (-0.20000000298023223876953125))) + _330;
            float4 _423 = Texture0.sample(Sampler0, _419);
            float _425 = _419.x;
            bool _431;
            if (_425 > 0.0)
            {
                _431 = _419.y > 0.0;
            }
            else
            {
                _431 = false;
            }
            bool _435;
            if (_431)
            {
                _435 = _425 < 1.0;
            }
            else
            {
                _435 = false;
            }
            bool _440;
            if (_435)
            {
                _440 = _419.y < 1.0;
            }
            else
            {
                _440 = false;
            }
            float2 _447 = (in.in_var_TEXCOORD0 + (_325 * 0.4000000059604644775390625)) + _330;
            float4 _451 = Texture0.sample(Sampler0, _447);
            float _453 = _447.x;
            bool _459;
            if (_453 > 0.0)
            {
                _459 = _447.y > 0.0;
            }
            else
            {
                _459 = false;
            }
            bool _463;
            if (_459)
            {
                _463 = _453 < 1.0;
            }
            else
            {
                _463 = false;
            }
            bool _468;
            if (_463)
            {
                _468 = _447.y < 1.0;
            }
            else
            {
                _468 = false;
            }
            float2 _475 = (in.in_var_TEXCOORD0 + (_325 * (-0.4000000059604644775390625))) + _330;
            float4 _479 = Texture0.sample(Sampler0, _475);
            float _481 = _475.x;
            bool _487;
            if (_481 > 0.0)
            {
                _487 = _475.y > 0.0;
            }
            else
            {
                _487 = false;
            }
            bool _491;
            if (_487)
            {
                _491 = _481 < 1.0;
            }
            else
            {
                _491 = false;
            }
            bool _496;
            if (_491)
            {
                _496 = _475.y < 1.0;
            }
            else
            {
                _496 = false;
            }
            float2 _503 = (in.in_var_TEXCOORD0 + (_325 * 0.60000002384185791015625)) + _330;
            float4 _507 = Texture0.sample(Sampler0, _503);
            float _509 = _503.x;
            bool _515;
            if (_509 > 0.0)
            {
                _515 = _503.y > 0.0;
            }
            else
            {
                _515 = false;
            }
            bool _519;
            if (_515)
            {
                _519 = _509 < 1.0;
            }
            else
            {
                _519 = false;
            }
            bool _524;
            if (_519)
            {
                _524 = _503.y < 1.0;
            }
            else
            {
                _524 = false;
            }
            float2 _531 = (in.in_var_TEXCOORD0 + (_325 * (-0.60000002384185791015625))) + _330;
            float4 _535 = Texture0.sample(Sampler0, _531);
            float _537 = _531.x;
            bool _543;
            if (_537 > 0.0)
            {
                _543 = _531.y > 0.0;
            }
            else
            {
                _543 = false;
            }
            bool _547;
            if (_543)
            {
                _547 = _537 < 1.0;
            }
            else
            {
                _547 = false;
            }
            bool _552;
            if (_547)
            {
                _552 = _531.y < 1.0;
            }
            else
            {
                _552 = false;
            }
            float2 _559 = (in.in_var_TEXCOORD0 + (_325 * 0.800000011920928955078125)) + _330;
            float4 _563 = Texture0.sample(Sampler0, _559);
            float _565 = _559.x;
            bool _571;
            if (_565 > 0.0)
            {
                _571 = _559.y > 0.0;
            }
            else
            {
                _571 = false;
            }
            bool _575;
            if (_571)
            {
                _575 = _565 < 1.0;
            }
            else
            {
                _575 = false;
            }
            bool _580;
            if (_575)
            {
                _580 = _559.y < 1.0;
            }
            else
            {
                _580 = false;
            }
            float2 _587 = (in.in_var_TEXCOORD0 + (_325 * (-0.800000011920928955078125))) + _330;
            float4 _591 = Texture0.sample(Sampler0, _587);
            float _593 = _587.x;
            bool _599;
            if (_593 > 0.0)
            {
                _599 = _587.y > 0.0;
            }
            else
            {
                _599 = false;
            }
            bool _603;
            if (_599)
            {
                _603 = _593 < 1.0;
            }
            else
            {
                _603 = false;
            }
            bool _608;
            if (_603)
            {
                _608 = _587.y < 1.0;
            }
            else
            {
                _608 = false;
            }
            float2 _615 = (in.in_var_TEXCOORD0 + (_325 * 1.0)) + _330;
            float4 _619 = Texture0.sample(Sampler0, _615);
            float _621 = _615.x;
            bool _627;
            if (_621 > 0.0)
            {
                _627 = _615.y > 0.0;
            }
            else
            {
                _627 = false;
            }
            bool _631;
            if (_627)
            {
                _631 = _621 < 1.0;
            }
            else
            {
                _631 = false;
            }
            bool _636;
            if (_631)
            {
                _636 = _615.y < 1.0;
            }
            else
            {
                _636 = false;
            }
            float2 _643 = (in.in_var_TEXCOORD0 + (_325 * (-1.0))) + _330;
            float4 _647 = Texture0.sample(Sampler0, _643);
            float _649 = _643.x;
            bool _655;
            if (_649 > 0.0)
            {
                _655 = _643.y > 0.0;
            }
            else
            {
                _655 = false;
            }
            bool _659;
            if (_655)
            {
                _659 = _649 < 1.0;
            }
            else
            {
                _659 = false;
            }
            bool _664;
            if (_659)
            {
                _664 = _643.y < 1.0;
            }
            else
            {
                _664 = false;
            }
            float2 _671 = (in.in_var_TEXCOORD0 + (_325 * 1.2000000476837158203125)) + _330;
            float4 _675 = Texture0.sample(Sampler0, _671);
            float _677 = _671.x;
            bool _683;
            if (_677 > 0.0)
            {
                _683 = _671.y > 0.0;
            }
            else
            {
                _683 = false;
            }
            bool _687;
            if (_683)
            {
                _687 = _677 < 1.0;
            }
            else
            {
                _687 = false;
            }
            bool _692;
            if (_687)
            {
                _692 = _671.y < 1.0;
            }
            else
            {
                _692 = false;
            }
            float _696 = ((((((((((((_368.w * float(_385)) * 0.0827184617519378662109375) + ((_395.w * float(_412)) * 0.081080533564090728759765625)) + ((_423.w * float(_440)) * 0.081080533564090728759765625)) + ((_451.w * float(_468)) * 0.0763587653636932373046875)) + ((_479.w * float(_496)) * 0.0763587653636932373046875)) + ((_507.w * float(_524)) * 0.069092273712158203125)) + ((_535.w * float(_552)) * 0.069092273712158203125)) + ((_563.w * float(_580)) * 0.060065932571887969970703125)) + ((_591.w * float(_608)) * 0.060065932571887969970703125)) + ((_619.w * float(_636)) * 0.0501712858676910400390625)) + ((_647.w * float(_664)) * 0.0501712858676910400390625)) + ((_675.w * float(_692)) * 0.04026339948177337646484375);
            float2 _699 = (in.in_var_TEXCOORD0 + (_325 * (-1.2000000476837158203125))) + _330;
            float4 _703 = Texture0.sample(Sampler0, _699);
            float _705 = _699.x;
            bool _711;
            if (_705 > 0.0)
            {
                _711 = _699.y > 0.0;
            }
            else
            {
                _711 = false;
            }
            bool _715;
            if (_711)
            {
                _715 = _705 < 1.0;
            }
            else
            {
                _715 = false;
            }
            bool _720;
            if (_715)
            {
                _720 = _699.y < 1.0;
            }
            else
            {
                _720 = false;
            }
            float2 _727 = (in.in_var_TEXCOORD0 + (_325 * 1.39999997615814208984375)) + _330;
            float4 _731 = Texture0.sample(Sampler0, _727);
            float _733 = _727.x;
            bool _739;
            if (_733 > 0.0)
            {
                _739 = _727.y > 0.0;
            }
            else
            {
                _739 = false;
            }
            bool _743;
            if (_739)
            {
                _743 = _733 < 1.0;
            }
            else
            {
                _743 = false;
            }
            bool _748;
            if (_743)
            {
                _748 = _727.y < 1.0;
            }
            else
            {
                _748 = false;
            }
            float2 _755 = (in.in_var_TEXCOORD0 + (_325 * (-1.39999997615814208984375))) + _330;
            float4 _759 = Texture0.sample(Sampler0, _755);
            float _761 = _755.x;
            bool _767;
            if (_761 > 0.0)
            {
                _767 = _755.y > 0.0;
            }
            else
            {
                _767 = false;
            }
            bool _771;
            if (_767)
            {
                _771 = _761 < 1.0;
            }
            else
            {
                _771 = false;
            }
            bool _776;
            if (_771)
            {
                _776 = _755.y < 1.0;
            }
            else
            {
                _776 = false;
            }
            float2 _783 = (in.in_var_TEXCOORD0 + (_325 * 1.60000002384185791015625)) + _330;
            float4 _787 = Texture0.sample(Sampler0, _783);
            float _789 = _783.x;
            bool _795;
            if (_789 > 0.0)
            {
                _795 = _783.y > 0.0;
            }
            else
            {
                _795 = false;
            }
            bool _799;
            if (_795)
            {
                _799 = _789 < 1.0;
            }
            else
            {
                _799 = false;
            }
            bool _804;
            if (_799)
            {
                _804 = _783.y < 1.0;
            }
            else
            {
                _804 = false;
            }
            float2 _811 = (in.in_var_TEXCOORD0 + (_325 * (-1.60000002384185791015625))) + _330;
            float4 _815 = Texture0.sample(Sampler0, _811);
            float _817 = _811.x;
            bool _823;
            if (_817 > 0.0)
            {
                _823 = _811.y > 0.0;
            }
            else
            {
                _823 = false;
            }
            bool _827;
            if (_823)
            {
                _827 = _817 < 1.0;
            }
            else
            {
                _827 = false;
            }
            bool _832;
            if (_827)
            {
                _832 = _811.y < 1.0;
            }
            else
            {
                _832 = false;
            }
            float2 _839 = (in.in_var_TEXCOORD0 + (_325 * 1.80000007152557373046875)) + _330;
            float4 _843 = Texture0.sample(Sampler0, _839);
            float _845 = _839.x;
            bool _851;
            if (_845 > 0.0)
            {
                _851 = _839.y > 0.0;
            }
            else
            {
                _851 = false;
            }
            bool _855;
            if (_851)
            {
                _855 = _845 < 1.0;
            }
            else
            {
                _855 = false;
            }
            bool _860;
            if (_855)
            {
                _860 = _839.y < 1.0;
            }
            else
            {
                _860 = false;
            }
            float2 _867 = (in.in_var_TEXCOORD0 + (_325 * (-1.80000007152557373046875))) + _330;
            float4 _871 = Texture0.sample(Sampler0, _867);
            float _873 = _867.x;
            bool _879;
            if (_873 > 0.0)
            {
                _879 = _867.y > 0.0;
            }
            else
            {
                _879 = false;
            }
            bool _883;
            if (_879)
            {
                _883 = _873 < 1.0;
            }
            else
            {
                _883 = false;
            }
            bool _888;
            if (_883)
            {
                _888 = _867.y < 1.0;
            }
            else
            {
                _888 = false;
            }
            float2 _895 = (in.in_var_TEXCOORD0 + (_325 * 2.0)) + _330;
            float4 _899 = Texture0.sample(Sampler0, _895);
            float _901 = _895.x;
            bool _907;
            if (_901 > 0.0)
            {
                _907 = _895.y > 0.0;
            }
            else
            {
                _907 = false;
            }
            bool _911;
            if (_907)
            {
                _911 = _901 < 1.0;
            }
            else
            {
                _911 = false;
            }
            bool _916;
            if (_911)
            {
                _916 = _895.y < 1.0;
            }
            else
            {
                _916 = false;
            }
            float2 _923 = (in.in_var_TEXCOORD0 + (_325 * (-2.0))) + _330;
            float4 _927 = Texture0.sample(Sampler0, _923);
            float _929 = _923.x;
            bool _935;
            if (_929 > 0.0)
            {
                _935 = _923.y > 0.0;
            }
            else
            {
                _935 = false;
            }
            bool _939;
            if (_935)
            {
                _939 = _929 < 1.0;
            }
            else
            {
                _939 = false;
            }
            bool _944;
            if (_939)
            {
                _944 = _923.y < 1.0;
            }
            else
            {
                _944 = false;
            }
            _949 = ((((((((_696 + ((_703.w * float(_720)) * 0.04026339948177337646484375)) + ((_731.w * float(_748)) * 0.0310451574623584747314453125)) + ((_759.w * float(_776)) * 0.0310451574623584747314453125)) + ((_787.w * float(_804)) * 0.02299881912767887115478515625)) + ((_815.w * float(_832)) * 0.02299881912767887115478515625)) + ((_843.w * float(_860)) * 0.01636987738311290740966796875)) + ((_871.w * float(_888)) * 0.01636987738311290740966796875)) + ((_899.w * float(_916)) * 0.011194727383553981781005859375)) + ((_927.w * float(_944)) * 0.011194727383553981781005859375);
            break;
        } while(false);
        _951 = float4(_949);
    }
    else
    {
        float2 _93 = float2(Uniforms.Scalar4[0].x, Uniforms.Scalar4[0].y);
        float2 _98 = float2(Uniforms.Scalar4[0].z, Uniforms.Scalar4[0].w);
        float4 _320;
        do
        {
            bool _105;
            if (Uniforms.Scalar4[0].x < 0.001000000047497451305389404296875)
            {
                _105 = Uniforms.Scalar4[0].y < 0.001000000047497451305389404296875;
            }
            else
            {
                _105 = false;
            }
            if (_105)
            {
                _320 = Texture0.sample(Sampler0, (in.in_var_TEXCOORD0 + _98));
                break;
            }
            float4 _219 = ((((((((((Texture0.sample(Sampler0, fast::clamp(in.in_var_TEXCOORD0 + _98, float2(0.0), float2(1.0))) * 0.0827184617519378662109375) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 0.20000000298023223876953125)) + _98, float2(0.0), float2(1.0))) * 0.081080533564090728759765625)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-0.20000000298023223876953125))) + _98, float2(0.0), float2(1.0))) * 0.081080533564090728759765625)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 0.4000000059604644775390625)) + _98, float2(0.0), float2(1.0))) * 0.0763587653636932373046875)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-0.4000000059604644775390625))) + _98, float2(0.0), float2(1.0))) * 0.0763587653636932373046875)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 0.60000002384185791015625)) + _98, float2(0.0), float2(1.0))) * 0.069092273712158203125)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-0.60000002384185791015625))) + _98, float2(0.0), float2(1.0))) * 0.069092273712158203125)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 0.800000011920928955078125)) + _98, float2(0.0), float2(1.0))) * 0.060065932571887969970703125)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-0.800000011920928955078125))) + _98, float2(0.0), float2(1.0))) * 0.060065932571887969970703125)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 1.0)) + _98, float2(0.0), float2(1.0))) * 0.0501712858676910400390625)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-1.0))) + _98, float2(0.0), float2(1.0))) * 0.0501712858676910400390625);
            float4 _319 = (((((((((_219 + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 1.2000000476837158203125)) + _98, float2(0.0), float2(1.0))) * 0.04026339948177337646484375)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-1.2000000476837158203125))) + _98, float2(0.0), float2(1.0))) * 0.04026339948177337646484375)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 1.39999997615814208984375)) + _98, float2(0.0), float2(1.0))) * 0.0310451574623584747314453125)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-1.39999997615814208984375))) + _98, float2(0.0), float2(1.0))) * 0.0310451574623584747314453125)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 1.60000002384185791015625)) + _98, float2(0.0), float2(1.0))) * 0.02299881912767887115478515625)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-1.60000002384185791015625))) + _98, float2(0.0), float2(1.0))) * 0.02299881912767887115478515625)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 1.80000007152557373046875)) + _98, float2(0.0), float2(1.0))) * 0.01636987738311290740966796875)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-1.80000007152557373046875))) + _98, float2(0.0), float2(1.0))) * 0.01636987738311290740966796875)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * 2.0)) + _98, float2(0.0), float2(1.0))) * 0.011194727383553981781005859375)) + (Texture0.sample(Sampler0, fast::clamp((in.in_var_TEXCOORD0 + (_93 * (-2.0))) + _98, float2(0.0), float2(1.0))) * 0.011194727383553981781005859375);
            _320 = _319;
            break;
        } while(false);
        _951 = _320;
    }
    float4 _958;
    if (Uniforms.Integer4[0].x == 1)
    {
        _958 = _951 * in.in_var_COLOR0;
    }
    else
    {
        _958 = _951;
    }
    out.out_var_SV_Target = _958;
    return out;
}

