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

constant float4 _98 = {};
constant float _110 = {};

struct main0_out
{
    float4 out_var_SV_Target [[color(0)]];
};

struct main0_in
{
    float4 in_var_COLOR0 [[user(locn0)]];
    float2 in_var_TEXCOORD0 [[user(locn1)]];
    float2 in_var_TEXCOORD1 [[user(locn2)]];
    float4 in_var_COLOR1 [[user(locn3)]];
    float4 in_var_COLOR2 [[user(locn4)]];
    float4 in_var_COLOR3 [[user(locn5)]];
    float4 in_var_COLOR4 [[user(locn6)]];
    float4 in_var_COLOR5 [[user(locn7)]];
    float4 in_var_COLOR6 [[user(locn8)]];
    float4 in_var_COLOR7 [[user(locn9)]];
};

fragment main0_out main0(main0_in in [[stage_in]], constant type_Uniforms& Uniforms [[buffer(0)]], texture2d<float> Texture0 [[texture(0)]], texture2d<float> Texture1 [[texture(1)]], sampler Sampler0 [[sampler(0)]])
{
    main0_out out = {};
    float4 _1722;
    switch (uint(in.in_var_COLOR1.x + 0.5))
    {
        case 0u:
        {
            _1722 = in.in_var_COLOR0;
            break;
        }
        case 1u:
        {
            _1722 = Texture0.sample(Sampler0, in.in_var_TEXCOORD0) * in.in_var_COLOR0;
            break;
        }
        case 2u:
        {
            float4 _1716;
            do
            {
                float2 _1676 = Uniforms.Vector[1].zw + Uniforms.Vector[1].xy;
                float2 _1692 = fract((((Uniforms.Vector[2].xy * in.in_var_TEXCOORD1.x) + (Uniforms.Vector[2].zw * in.in_var_TEXCOORD1.y)) + Uniforms.Vector[3].xy) / _1676) * _1676;
                bool _1702;
                if (!(_1692.x >= Uniforms.Vector[1].z))
                {
                    _1702 = _1692.y >= Uniforms.Vector[1].w;
                }
                else
                {
                    _1702 = true;
                }
                if (_1702)
                {
                    _1716 = float4(0.0);
                    break;
                }
                _1716 = Texture0.sample(Sampler0, (((_1692 / Uniforms.Vector[1].zw) * (Uniforms.Vector[0].zw - Uniforms.Vector[0].xy)) + Uniforms.Vector[0].xy)) * in.in_var_COLOR0;
                break;
            } while(false);
            _1722 = _1716;
            break;
        }
        case 3u:
        {
            float _1471 = float(uint(in.in_var_COLOR1.y + 0.5));
            float _1536;
            if (uint(in.in_var_COLOR1.z + 0.5) != 0u)
            {
                float2 _1489 = in.in_var_TEXCOORD0 - in.in_var_COLOR2.xy;
                float _1491 = length(_1489 / in.in_var_COLOR3.zw);
                bool _1498;
                if (in.in_var_COLOR3.x > 9.9999997473787516355514526367188e-05)
                {
                    _1498 = in.in_var_COLOR3.y > 9.9999997473787516355514526367188e-05;
                }
                else
                {
                    _1498 = false;
                }
                float _1519;
                if (_1498)
                {
                    float _1504 = length(_1489 / in.in_var_COLOR3.xy);
                    float _1518;
                    if (_1504 <= 1.0)
                    {
                        _1518 = 0.0;
                    }
                    else
                    {
                        float _1517;
                        if (_1491 >= 1.0)
                        {
                            _1517 = 1.0;
                        }
                        else
                        {
                            _1517 = fast::clamp((_1504 - 1.0) / (_1504 - _1491), 0.0, 1.0);
                        }
                        _1518 = _1517;
                    }
                    _1519 = _1518;
                }
                else
                {
                    _1519 = fast::clamp(_1491, 0.0, 1.0);
                }
                float _1535;
                if (length(in.in_var_COLOR2.zw - in.in_var_COLOR2.xy) > 9.9999997473787516355514526367188e-05)
                {
                    _1535 = fast::clamp(_1519 * (1.0 + ((1.0 - dot(fast::normalize(in.in_var_TEXCOORD0 - in.in_var_COLOR2.zw), fast::normalize(in.in_var_COLOR2.xy - in.in_var_COLOR2.zw))) * 0.5)), 0.0, 1.0);
                }
                else
                {
                    _1535 = _1519;
                }
                _1536 = _1535;
            }
            else
            {
                float2 _1483 = in.in_var_COLOR2.zw - in.in_var_COLOR2.xy;
                _1536 = fast::clamp(dot(in.in_var_TEXCOORD0 - in.in_var_COLOR2.xy, _1483) / dot(_1483, _1483), 0.0, 1.0);
            }
            float4 _1544 = mix(in.in_var_COLOR5, in.in_var_COLOR6, float4(fast::clamp((_1536 - in.in_var_COLOR4.x) / (in.in_var_COLOR4.y - in.in_var_COLOR4.x), 0.0, 1.0)));
            float4 _1667;
            if (_1471 > 2.0)
            {
                float4 _1554 = mix(_1544, in.in_var_COLOR7, float4(fast::clamp((_1536 - in.in_var_COLOR4.y) / (in.in_var_COLOR4.z - in.in_var_COLOR4.y), 0.0, 1.0)));
                float4 _1666;
                if (_1471 > 3.0)
                {
                    float4 _1567 = mix(_1554, Uniforms.Vector[0u], float4(fast::clamp((_1536 - in.in_var_COLOR4.z) / (Uniforms.Scalar4[0].x - in.in_var_COLOR4.z), 0.0, 1.0)));
                    float4 _1665;
                    if (_1471 > 4.0)
                    {
                        float4 _1580 = mix(_1567, Uniforms.Vector[1u], float4(fast::clamp((_1536 - Uniforms.Scalar4[0].x) / (Uniforms.Scalar4[0].y - Uniforms.Scalar4[0].x), 0.0, 1.0)));
                        float4 _1664;
                        if (_1471 > 5.0)
                        {
                            float4 _1593 = mix(_1580, Uniforms.Vector[2u], float4(fast::clamp((_1536 - Uniforms.Scalar4[0].y) / (Uniforms.Scalar4[0].z - Uniforms.Scalar4[0].y), 0.0, 1.0)));
                            float4 _1663;
                            if (_1471 > 6.0)
                            {
                                float4 _1606 = mix(_1593, Uniforms.Vector[3u], float4(fast::clamp((_1536 - Uniforms.Scalar4[0].z) / (Uniforms.Scalar4[0].w - Uniforms.Scalar4[0].z), 0.0, 1.0)));
                                float4 _1662;
                                if (_1471 > 7.0)
                                {
                                    float4 _1619 = mix(_1606, Uniforms.Vector[4u], float4(fast::clamp((_1536 - Uniforms.Scalar4[0].w) / (Uniforms.Scalar4[1].x - Uniforms.Scalar4[0].w), 0.0, 1.0)));
                                    float4 _1661;
                                    if (_1471 > 8.0)
                                    {
                                        float4 _1632 = mix(_1619, Uniforms.Vector[5u], float4(fast::clamp((_1536 - Uniforms.Scalar4[1].x) / (Uniforms.Scalar4[1].y - Uniforms.Scalar4[1].x), 0.0, 1.0)));
                                        float4 _1660;
                                        if (_1471 > 9.0)
                                        {
                                            float4 _1645 = mix(_1632, Uniforms.Vector[6u], float4(fast::clamp((_1536 - Uniforms.Scalar4[1].y) / (Uniforms.Scalar4[1].z - Uniforms.Scalar4[1].y), 0.0, 1.0)));
                                            float4 _1659;
                                            if (_1471 > 10.0)
                                            {
                                                _1659 = mix(_1645, Uniforms.Vector[7u], float4(fast::clamp((_1536 - Uniforms.Scalar4[1].z) / (Uniforms.Scalar4[1].w - Uniforms.Scalar4[1].z), 0.0, 1.0)));
                                            }
                                            else
                                            {
                                                _1659 = _1645;
                                            }
                                            _1660 = _1659;
                                        }
                                        else
                                        {
                                            _1660 = _1632;
                                        }
                                        _1661 = _1660;
                                    }
                                    else
                                    {
                                        _1661 = _1619;
                                    }
                                    _1662 = _1661;
                                }
                                else
                                {
                                    _1662 = _1606;
                                }
                                _1663 = _1662;
                            }
                            else
                            {
                                _1663 = _1593;
                            }
                            _1664 = _1663;
                        }
                        else
                        {
                            _1664 = _1580;
                        }
                        _1665 = _1664;
                    }
                    else
                    {
                        _1665 = _1567;
                    }
                    _1666 = _1665;
                }
                else
                {
                    _1666 = _1554;
                }
                _1667 = _1666;
            }
            else
            {
                _1667 = _1544;
            }
            _1722 = _1667;
            break;
        }
        case 7u:
        {
            float2 _817 = (in.in_var_TEXCOORD0 - float2(0.5)) * in.in_var_COLOR1.zw;
            float _1430;
            do
            {
                float2 _820 = in.in_var_COLOR1.zw * 0.5;
                float _821 = _820.x;
                float _822 = -_821;
                float _824 = _822 + in.in_var_COLOR2.x;
                float _825 = _820.y;
                float _826 = -_825;
                float _828 = _826 + in.in_var_COLOR3.x;
                float2 _830 = _817 - float2(_824, _828);
                bool _837;
                if ((in.in_var_COLOR2.x * in.in_var_COLOR3.x) > 0.0)
                {
                    _837 = _817.x < _824;
                }
                else
                {
                    _837 = false;
                }
                bool _842;
                if (_837)
                {
                    _842 = _817.y <= _828;
                }
                else
                {
                    _842 = false;
                }
                if (_842)
                {
                    float2 _845 = float2(in.in_var_COLOR2.x, in.in_var_COLOR3.x);
                    float _973;
                    do
                    {
                        if (abs(in.in_var_COLOR2.x - in.in_var_COLOR3.x) < 0.100000001490116119384765625)
                        {
                            _973 = length(_830) - in.in_var_COLOR2.x;
                            break;
                        }
                        float2 _855 = abs(_830);
                        float2 _863;
                        float2 _864;
                        if (_855.x > _855.y)
                        {
                            _863 = _855.yx;
                            _864 = _845.yx;
                        }
                        else
                        {
                            _863 = _855;
                            _864 = _845;
                        }
                        float _869 = (_864.y * _864.y) - (_864.x * _864.x);
                        float _872 = (_864.x * _863.x) / _869;
                        float _875 = (_864.y * _863.y) / _869;
                        float _876 = _872 * _872;
                        float _877 = _875 * _875;
                        float _879 = (_876 + _877) - 1.0;
                        float _880 = _879 * 0.3333333432674407958984375;
                        float _882 = (_880 * _880) * _880;
                        float _883 = _876 * _877;
                        float _885 = _882 + (_883 * 2.0);
                        float _886 = _882 + _883;
                        float _888 = _872 + (_872 * _877);
                        float _959;
                        if (_886 < 0.0)
                        {
                            float _933 = acos(_885 / _882) * 0.3333333432674407958984375;
                            float _934 = cos(_933);
                            float _936 = sin(_933) * 1.73205077648162841796875;
                            float _937 = _879 * (-0.3333333432674407958984375);
                            float _942 = sqrt((_937 * ((_934 + _936) + 2.0)) + _876);
                            float _947 = sqrt((_937 * ((_934 - _936) + 2.0)) + _876);
                            _959 = (((_947 + (float(int(sign(_869))) * _942)) + (abs(_888) / (_942 * _947))) - _872) * 0.5;
                        }
                        else
                        {
                            float _896 = ((2.0 * _872) * _875) * sqrt(_886);
                            float _897 = _885 + _896;
                            float _903 = float(int(sign(_897))) * powr(abs(_897), 0.3333333432674407958984375);
                            float _904 = _885 - _896;
                            float _910 = float(int(sign(_904))) * powr(abs(_904), 0.3333333432674407958984375);
                            float _916 = (((-_903) - _910) - (_879 * 1.33333337306976318359375)) + (2.0 * _876);
                            float _918 = (_903 - _910) * 1.73205077648162841796875;
                            float _922 = sqrt((_916 * _916) + (_918 * _918));
                            _959 = (((_918 / sqrt(_922 - _916)) + ((2.0 * _888) / _922)) - _872) * 0.5;
                        }
                        float _964 = _864.y * sqrt(1.0 - (_959 * _959));
                        _973 = length(float2(_864.x * _959, _964) - _863) * float(int(sign(_863.y - _964)));
                        break;
                    } while(false);
                    _1430 = _973;
                    break;
                }
                float _975 = _821 - in.in_var_COLOR2.y;
                float _977 = _826 + in.in_var_COLOR3.y;
                float2 _979 = _817 - float2(_975, _977);
                bool _986;
                if ((in.in_var_COLOR2.y * in.in_var_COLOR3.y) > 0.0)
                {
                    _986 = _817.x >= _975;
                }
                else
                {
                    _986 = false;
                }
                bool _991;
                if (_986)
                {
                    _991 = _817.y <= _977;
                }
                else
                {
                    _991 = false;
                }
                if (_991)
                {
                    float2 _994 = float2(in.in_var_COLOR2.y, in.in_var_COLOR3.y);
                    float _1122;
                    do
                    {
                        if (abs(in.in_var_COLOR2.y - in.in_var_COLOR3.y) < 0.100000001490116119384765625)
                        {
                            _1122 = length(_979) - in.in_var_COLOR2.y;
                            break;
                        }
                        float2 _1004 = abs(_979);
                        float2 _1012;
                        float2 _1013;
                        if (_1004.x > _1004.y)
                        {
                            _1012 = _1004.yx;
                            _1013 = _994.yx;
                        }
                        else
                        {
                            _1012 = _1004;
                            _1013 = _994;
                        }
                        float _1018 = (_1013.y * _1013.y) - (_1013.x * _1013.x);
                        float _1021 = (_1013.x * _1012.x) / _1018;
                        float _1024 = (_1013.y * _1012.y) / _1018;
                        float _1025 = _1021 * _1021;
                        float _1026 = _1024 * _1024;
                        float _1028 = (_1025 + _1026) - 1.0;
                        float _1029 = _1028 * 0.3333333432674407958984375;
                        float _1031 = (_1029 * _1029) * _1029;
                        float _1032 = _1025 * _1026;
                        float _1034 = _1031 + (_1032 * 2.0);
                        float _1035 = _1031 + _1032;
                        float _1037 = _1021 + (_1021 * _1026);
                        float _1108;
                        if (_1035 < 0.0)
                        {
                            float _1082 = acos(_1034 / _1031) * 0.3333333432674407958984375;
                            float _1083 = cos(_1082);
                            float _1085 = sin(_1082) * 1.73205077648162841796875;
                            float _1086 = _1028 * (-0.3333333432674407958984375);
                            float _1091 = sqrt((_1086 * ((_1083 + _1085) + 2.0)) + _1025);
                            float _1096 = sqrt((_1086 * ((_1083 - _1085) + 2.0)) + _1025);
                            _1108 = (((_1096 + (float(int(sign(_1018))) * _1091)) + (abs(_1037) / (_1091 * _1096))) - _1021) * 0.5;
                        }
                        else
                        {
                            float _1045 = ((2.0 * _1021) * _1024) * sqrt(_1035);
                            float _1046 = _1034 + _1045;
                            float _1052 = float(int(sign(_1046))) * powr(abs(_1046), 0.3333333432674407958984375);
                            float _1053 = _1034 - _1045;
                            float _1059 = float(int(sign(_1053))) * powr(abs(_1053), 0.3333333432674407958984375);
                            float _1065 = (((-_1052) - _1059) - (_1028 * 1.33333337306976318359375)) + (2.0 * _1025);
                            float _1067 = (_1052 - _1059) * 1.73205077648162841796875;
                            float _1071 = sqrt((_1065 * _1065) + (_1067 * _1067));
                            _1108 = (((_1067 / sqrt(_1071 - _1065)) + ((2.0 * _1037) / _1071)) - _1021) * 0.5;
                        }
                        float _1113 = _1013.y * sqrt(1.0 - (_1108 * _1108));
                        _1122 = length(float2(_1013.x * _1108, _1113) - _1012) * float(int(sign(_1012.y - _1113)));
                        break;
                    } while(false);
                    _1430 = _1122;
                    break;
                }
                float _1124 = _821 - in.in_var_COLOR2.z;
                float _1126 = _825 - in.in_var_COLOR3.z;
                float2 _1128 = _817 - float2(_1124, _1126);
                bool _1135;
                if ((in.in_var_COLOR2.z * in.in_var_COLOR3.z) > 0.0)
                {
                    _1135 = _817.x >= _1124;
                }
                else
                {
                    _1135 = false;
                }
                bool _1140;
                if (_1135)
                {
                    _1140 = _817.y >= _1126;
                }
                else
                {
                    _1140 = false;
                }
                if (_1140)
                {
                    float2 _1143 = float2(in.in_var_COLOR2.z, in.in_var_COLOR3.z);
                    float _1271;
                    do
                    {
                        if (abs(in.in_var_COLOR2.z - in.in_var_COLOR3.z) < 0.100000001490116119384765625)
                        {
                            _1271 = length(_1128) - in.in_var_COLOR2.z;
                            break;
                        }
                        float2 _1153 = abs(_1128);
                        float2 _1161;
                        float2 _1162;
                        if (_1153.x > _1153.y)
                        {
                            _1161 = _1153.yx;
                            _1162 = _1143.yx;
                        }
                        else
                        {
                            _1161 = _1153;
                            _1162 = _1143;
                        }
                        float _1167 = (_1162.y * _1162.y) - (_1162.x * _1162.x);
                        float _1170 = (_1162.x * _1161.x) / _1167;
                        float _1173 = (_1162.y * _1161.y) / _1167;
                        float _1174 = _1170 * _1170;
                        float _1175 = _1173 * _1173;
                        float _1177 = (_1174 + _1175) - 1.0;
                        float _1178 = _1177 * 0.3333333432674407958984375;
                        float _1180 = (_1178 * _1178) * _1178;
                        float _1181 = _1174 * _1175;
                        float _1183 = _1180 + (_1181 * 2.0);
                        float _1184 = _1180 + _1181;
                        float _1186 = _1170 + (_1170 * _1175);
                        float _1257;
                        if (_1184 < 0.0)
                        {
                            float _1231 = acos(_1183 / _1180) * 0.3333333432674407958984375;
                            float _1232 = cos(_1231);
                            float _1234 = sin(_1231) * 1.73205077648162841796875;
                            float _1235 = _1177 * (-0.3333333432674407958984375);
                            float _1240 = sqrt((_1235 * ((_1232 + _1234) + 2.0)) + _1174);
                            float _1245 = sqrt((_1235 * ((_1232 - _1234) + 2.0)) + _1174);
                            _1257 = (((_1245 + (float(int(sign(_1167))) * _1240)) + (abs(_1186) / (_1240 * _1245))) - _1170) * 0.5;
                        }
                        else
                        {
                            float _1194 = ((2.0 * _1170) * _1173) * sqrt(_1184);
                            float _1195 = _1183 + _1194;
                            float _1201 = float(int(sign(_1195))) * powr(abs(_1195), 0.3333333432674407958984375);
                            float _1202 = _1183 - _1194;
                            float _1208 = float(int(sign(_1202))) * powr(abs(_1202), 0.3333333432674407958984375);
                            float _1214 = (((-_1201) - _1208) - (_1177 * 1.33333337306976318359375)) + (2.0 * _1174);
                            float _1216 = (_1201 - _1208) * 1.73205077648162841796875;
                            float _1220 = sqrt((_1214 * _1214) + (_1216 * _1216));
                            _1257 = (((_1216 / sqrt(_1220 - _1214)) + ((2.0 * _1186) / _1220)) - _1170) * 0.5;
                        }
                        float _1262 = _1162.y * sqrt(1.0 - (_1257 * _1257));
                        _1271 = length(float2(_1162.x * _1257, _1262) - _1161) * float(int(sign(_1161.y - _1262)));
                        break;
                    } while(false);
                    _1430 = _1271;
                    break;
                }
                float _1273 = _822 + in.in_var_COLOR2.w;
                float _1275 = _825 - in.in_var_COLOR3.w;
                float2 _1277 = _817 - float2(_1273, _1275);
                bool _1284;
                if ((in.in_var_COLOR2.w * in.in_var_COLOR3.w) > 0.0)
                {
                    _1284 = _817.x < _1273;
                }
                else
                {
                    _1284 = false;
                }
                bool _1289;
                if (_1284)
                {
                    _1289 = _817.y > _1275;
                }
                else
                {
                    _1289 = false;
                }
                if (_1289)
                {
                    float2 _1292 = float2(in.in_var_COLOR2.w, in.in_var_COLOR3.w);
                    float _1420;
                    do
                    {
                        if (abs(in.in_var_COLOR2.w - in.in_var_COLOR3.w) < 0.100000001490116119384765625)
                        {
                            _1420 = length(_1277) - in.in_var_COLOR2.w;
                            break;
                        }
                        float2 _1302 = abs(_1277);
                        float2 _1310;
                        float2 _1311;
                        if (_1302.x > _1302.y)
                        {
                            _1310 = _1302.yx;
                            _1311 = _1292.yx;
                        }
                        else
                        {
                            _1310 = _1302;
                            _1311 = _1292;
                        }
                        float _1316 = (_1311.y * _1311.y) - (_1311.x * _1311.x);
                        float _1319 = (_1311.x * _1310.x) / _1316;
                        float _1322 = (_1311.y * _1310.y) / _1316;
                        float _1323 = _1319 * _1319;
                        float _1324 = _1322 * _1322;
                        float _1326 = (_1323 + _1324) - 1.0;
                        float _1327 = _1326 * 0.3333333432674407958984375;
                        float _1329 = (_1327 * _1327) * _1327;
                        float _1330 = _1323 * _1324;
                        float _1332 = _1329 + (_1330 * 2.0);
                        float _1333 = _1329 + _1330;
                        float _1335 = _1319 + (_1319 * _1324);
                        float _1406;
                        if (_1333 < 0.0)
                        {
                            float _1380 = acos(_1332 / _1329) * 0.3333333432674407958984375;
                            float _1381 = cos(_1380);
                            float _1383 = sin(_1380) * 1.73205077648162841796875;
                            float _1384 = _1326 * (-0.3333333432674407958984375);
                            float _1389 = sqrt((_1384 * ((_1381 + _1383) + 2.0)) + _1323);
                            float _1394 = sqrt((_1384 * ((_1381 - _1383) + 2.0)) + _1323);
                            _1406 = (((_1394 + (float(int(sign(_1316))) * _1389)) + (abs(_1335) / (_1389 * _1394))) - _1319) * 0.5;
                        }
                        else
                        {
                            float _1343 = ((2.0 * _1319) * _1322) * sqrt(_1333);
                            float _1344 = _1332 + _1343;
                            float _1350 = float(int(sign(_1344))) * powr(abs(_1344), 0.3333333432674407958984375);
                            float _1351 = _1332 - _1343;
                            float _1357 = float(int(sign(_1351))) * powr(abs(_1351), 0.3333333432674407958984375);
                            float _1363 = (((-_1350) - _1357) - (_1326 * 1.33333337306976318359375)) + (2.0 * _1323);
                            float _1365 = (_1350 - _1357) * 1.73205077648162841796875;
                            float _1369 = sqrt((_1363 * _1363) + (_1365 * _1365));
                            _1406 = (((_1365 / sqrt(_1369 - _1363)) + ((2.0 * _1335) / _1369)) - _1319) * 0.5;
                        }
                        float _1411 = _1311.y * sqrt(1.0 - (_1406 * _1406));
                        _1420 = length(float2(_1311.x * _1406, _1411) - _1310) * float(int(sign(_1310.y - _1411)));
                        break;
                    } while(false);
                    _1430 = _1420;
                    break;
                }
                float2 _1422 = abs(_817) - _820;
                _1430 = precise::min(precise::max(_1422.x, _1422.y), 0.0) + length(precise::max(_1422, float2(0.0)));
                break;
            } while(false);
            float _1431 = -_1430;
            float _1432 = smoothstep(-0.3540000021457672119140625, 0.3540000021457672119140625, _1431);
            float _1434 = _1432 * in.in_var_COLOR0.w;
            float4 _1440 = float4(in.in_var_COLOR0.xyz * _1434, _1434);
            float4 _1467;
            if (in.in_var_COLOR4.x > 0.0)
            {
                float _1451 = precise::min(_1432, 1.0 - smoothstep(in.in_var_COLOR4.x - 0.3540000021457672119140625, in.in_var_COLOR4.x + 0.3540000021457672119140625, _1431)) * in.in_var_COLOR5.w;
                float _1460 = 1.0 - _1451;
                float3 _1462 = float4(in.in_var_COLOR5.xyz * _1451, _1451).xyz + (_1440.xyz * _1460);
                float4 _1463 = float4(_1462.x, _1462.y, _1462.z, _98.w);
                _1463.w = _1451 + (_1434 * _1460);
                _1467 = _1463;
            }
            else
            {
                _1467 = _1440;
            }
            _1722 = _1467;
            break;
        }
        case 8u:
        {
            _1722 = in.in_var_COLOR0;
            break;
        }
        case 9u:
        {
            float4 _814;
            do
            {
                float4 _193 = Texture0.sample(Sampler0, in.in_var_TEXCOORD0);
                float4 _194 = _193 * in.in_var_COLOR0;
                float4 _198 = Texture1.sample(Sampler0, in.in_var_TEXCOORD1);
                float4 _811;
                bool _812;
                switch (uint(in.in_var_COLOR1.y + 0.5))
                {
                    case 0u:
                    {
                        _811 = float4(0.0);
                        _812 = true;
                        break;
                    }
                    case 1u:
                    {
                        _811 = _194;
                        _812 = true;
                        break;
                    }
                    case 2u:
                    {
                        _811 = _194 + (_198 * (1.0 - _194.w));
                        _812 = true;
                        break;
                    }
                    case 3u:
                    {
                        _811 = _194 * _198.w;
                        _812 = true;
                        break;
                    }
                    case 4u:
                    {
                        _811 = _194 * (1.0 - _198.w);
                        _812 = true;
                        break;
                    }
                    case 5u:
                    {
                        _811 = (_194 * _198.w) + (_198 * (1.0 - _194.w));
                        _812 = true;
                        break;
                    }
                    case 6u:
                    {
                        _811 = (_194 * (1.0 - _198.w)) + _198;
                        _812 = true;
                        break;
                    }
                    case 7u:
                    {
                        _811 = _198 * _194.w;
                        _812 = true;
                        break;
                    }
                    case 8u:
                    {
                        _811 = _198 * (1.0 - _194.w);
                        _812 = true;
                        break;
                    }
                    case 9u:
                    {
                        _811 = (_194 * (1.0 - _198.w)) + (_198 * _194.w);
                        _812 = true;
                        break;
                    }
                    case 10u:
                    {
                        _811 = fast::clamp((_194 * (1.0 - _198.w)) + (_198 * (1.0 - _194.w)), float4(0.0), float4(1.0));
                        _812 = true;
                        break;
                    }
                    case 11u:
                    {
                        float _765 = _194.w;
                        _811 = float4(precise::min(_194.xyz, _198.xyz) * _765, _198.w * _765);
                        _812 = true;
                        break;
                    }
                    case 12u:
                    {
                        _811 = fast::clamp(_194 + _198, float4(0.0), float4(1.0));
                        _812 = true;
                        break;
                    }
                    case 13u:
                    {
                        float _752 = _194.w;
                        _811 = float4(abs(_198.xyz - _194.xyz) * _752, _198.w * _752);
                        _812 = true;
                        break;
                    }
                    case 14u:
                    {
                        float _740 = _194.w;
                        _811 = float4((_194.xyz * _198.xyz) * _740, _198.w * _740);
                        _812 = true;
                        break;
                    }
                    case 15u:
                    {
                        float _729 = _194.w;
                        _811 = float4((float3(1.0) - ((float3(1.0) - _198.xyz) * (float3(1.0) - _194.xyz))) * _729, _198.w * _729);
                        _812 = true;
                        break;
                    }
                    case 16u:
                    {
                        float3 _117 = _194.xyz;
                        float3 _118 = _198.xyz;
                        float3 _112;
                        for (uint _687 = 0u; _687 < 3u; _687++)
                        {
                            float _712;
                            if (_118[_687] < 0.5)
                            {
                                _712 = (2.0 * _118[_687]) * _117[_687];
                            }
                            else
                            {
                                _712 = 1.0 - ((2.0 * (1.0 - _118[_687])) * (1.0 - _117[_687]));
                            }
                            _112[_687] = _712;
                        }
                        float _715 = _194.w;
                        _811 = float4(_112 * _715, _198.w * _715);
                        _812 = true;
                        break;
                    }
                    case 17u:
                    {
                        float _676 = _194.w;
                        _811 = float4(precise::max(_194.xyz, _198.xyz) * _676, _198.w * _676);
                        _812 = true;
                        break;
                    }
                    case 18u:
                    {
                        float3 _119 = _194.xyz;
                        float3 _120 = _198.xyz;
                        float3 _113;
                        for (uint _643 = 0u; _643 < 3u; _643++)
                        {
                            float _662;
                            if (_119[_643] == 1.0)
                            {
                                _662 = _119[_643];
                            }
                            else
                            {
                                _662 = precise::min(_120[_643] / (1.0 - _119[_643]), 1.0);
                            }
                            _113[_643] = _662;
                        }
                        float _665 = _194.w;
                        _811 = float4(_113 * _665, _198.w * _665);
                        _812 = true;
                        break;
                    }
                    case 19u:
                    {
                        float3 _121 = _194.xyz;
                        float3 _122 = _198.xyz;
                        float3 _114;
                        for (uint _609 = 0u; _609 < 3u; _609++)
                        {
                            float _629;
                            if (_121[_609] == 0.0)
                            {
                                _629 = _121[_609];
                            }
                            else
                            {
                                _629 = precise::max(1.0 - ((1.0 - _122[_609]) / _121[_609]), 0.0);
                            }
                            _114[_609] = _629;
                        }
                        float _632 = _194.w;
                        _811 = float4(_114 * _632, _198.w * _632);
                        _812 = true;
                        break;
                    }
                    case 20u:
                    {
                        float3 _123 = _198.xyz;
                        float3 _124 = _194.xyz;
                        float3 _115;
                        for (uint _570 = 0u; _570 < 3u; _570++)
                        {
                            float _595;
                            if (_124[_570] < 0.5)
                            {
                                _595 = (2.0 * _124[_570]) * _123[_570];
                            }
                            else
                            {
                                _595 = 1.0 - ((2.0 * (1.0 - _124[_570])) * (1.0 - _123[_570]));
                            }
                            _115[_570] = _595;
                        }
                        float _598 = _194.w;
                        _811 = float4(_115 * _598, _198.w * _598);
                        _812 = true;
                        break;
                    }
                    case 21u:
                    {
                        float3 _125 = _194.xyz;
                        float3 _126 = _198.xyz;
                        float3 _116;
                        for (uint _518 = 0u; _518 < 3u; _518++)
                        {
                            float _556;
                            if (_125[_518] < 0.5)
                            {
                                _556 = ((2.0 * _126[_518]) * _125[_518]) + ((_126[_518] * _126[_518]) * (1.0 - (2.0 * _125[_518])));
                            }
                            else
                            {
                                _556 = (sqrt(_126[_518]) * ((2.0 * _125[_518]) - 1.0)) + ((2.0 * _126[_518]) * (1.0 - _125[_518]));
                            }
                            _116[_518] = _556;
                        }
                        float _559 = _194.w;
                        _811 = float4(_116 * _559, _198.w * _559);
                        _812 = true;
                        break;
                    }
                    case 22u:
                    {
                        float3 _501 = _198.xyz;
                        float3 _502 = _194.xyz;
                        float _507 = _194.w;
                        _811 = float4(((_501 + _502) - ((_501 * 2.0) * _502)) * _507, _198.w * _507);
                        _812 = true;
                        break;
                    }
                    case 23u:
                    {
                        float3 _435 = _194.xyz;
                        float _436 = _198.x;
                        float _437 = _198.y;
                        float _438 = _198.z;
                        float _440 = precise::min(_436, precise::min(_437, _438));
                        float _442 = precise::max(_436, precise::max(_437, _438));
                        float _444 = _440 + _442;
                        float _446 = abs(_444 - 1.0);
                        float _450 = _194.x;
                        float _451 = _194.y;
                        float _452 = _194.z;
                        float3 _471 = ((step(_194.yxx, _435) * step(_194.zzy, _435)) * (float3(0.0, 2.0, 4.0) + ((_194.yzx - _194.zxy) / float3((precise::max(_450, precise::max(_451, _452)) - precise::min(_450, precise::min(_451, _452))) + 1.0000000116860974230803549289703e-07)))) * float3(0.16666667163372039794921875);
                        float _493 = _194.w;
                        float3 _494 = (float3(_444 * 0.5) + (((fast::clamp(abs(fmod(float3(fract(((1.0 + _471.x) + _471.y) + _471.z) * 6.0) + float3(0.0, 4.0, 2.0), float3(6.0)) - float3(3.0)) - float3(1.0), float3(0.0), float3(1.0)) - float3(0.5)) * ((_442 - _440) / (1.00000011920928955078125 - _446))) * (1.0 - _446))) * _493;
                        _811 = float4(_494, _198.w * _493);
                        _812 = true;
                        break;
                    }
                    case 24u:
                    {
                        float3 _366 = _198.xyz;
                        float _367 = _198.x;
                        float _368 = _198.y;
                        float _369 = _198.z;
                        float _371 = precise::min(_367, precise::min(_368, _369));
                        float _373 = precise::max(_367, precise::max(_368, _369));
                        float3 _388 = ((step(_198.yxx, _366) * step(_198.zzy, _366)) * (float3(0.0, 2.0, 4.0) + ((_198.yzx - _198.zxy) / float3((_373 - _371) + 1.0000000116860974230803549289703e-07)))) * float3(0.16666667163372039794921875);
                        float _396 = _371 + _373;
                        float _398 = _194.x;
                        float _399 = _194.y;
                        float _400 = _194.z;
                        float _402 = precise::min(_398, precise::min(_399, _400));
                        float _404 = precise::max(_398, precise::max(_399, _400));
                        float _427 = _194.w;
                        _811 = float4((float3(_396 * 0.5) + (((fast::clamp(abs(fmod(float3(fract(((1.0 + _388.x) + _388.y) + _388.z) * 6.0) + float3(0.0, 4.0, 2.0), float3(6.0)) - float3(3.0)) - float3(1.0), float3(0.0), float3(1.0)) - float3(0.5)) * ((_404 - _402) / (1.00000011920928955078125 - abs((_402 + _404) - 1.0)))) * (1.0 - abs(_396 - 1.0)))) * _427, _198.w * _427);
                        _812 = true;
                        break;
                    }
                    case 25u:
                    {
                        float3 _298 = _194.xyz;
                        float _299 = _194.x;
                        float _300 = _194.y;
                        float _301 = _194.z;
                        float _303 = precise::min(_299, precise::min(_300, _301));
                        float _305 = precise::max(_299, precise::max(_300, _301));
                        float _314 = _305 - _303;
                        float3 _320 = ((step(_194.yxx, _298) * step(_194.zzy, _298)) * (float3(0.0, 2.0, 4.0) + ((_194.yzx - _194.zxy) / float3(_314 + 1.0000000116860974230803549289703e-07)))) * float3(0.16666667163372039794921875);
                        float _333 = _198.x;
                        float _334 = _198.y;
                        float _335 = _198.z;
                        float _340 = precise::min(_333, precise::min(_334, _335)) + precise::max(_333, precise::max(_334, _335));
                        float _358 = _194.w;
                        _811 = float4((float3(_340 * 0.5) + (((fast::clamp(abs(fmod(float3(fract(((1.0 + _320.x) + _320.y) + _320.z) * 6.0) + float3(0.0, 4.0, 2.0), float3(6.0)) - float3(3.0)) - float3(1.0), float3(0.0), float3(1.0)) - float3(0.5)) * (_314 / (1.00000011920928955078125 - abs((_303 + _305) - 1.0)))) * (1.0 - abs(_340 - 1.0)))) * _358, _198.w * _358);
                        _812 = true;
                        break;
                    }
                    case 26u:
                    {
                        float3 _230 = _198.xyz;
                        float _231 = _198.x;
                        float _232 = _198.y;
                        float _233 = _198.z;
                        float _235 = precise::min(_231, precise::min(_232, _233));
                        float _237 = precise::max(_231, precise::max(_232, _233));
                        float _246 = _237 - _235;
                        float3 _252 = ((step(_198.yxx, _230) * step(_198.zzy, _230)) * (float3(0.0, 2.0, 4.0) + ((_198.yzx - _198.zxy) / float3(_246 + 1.0000000116860974230803549289703e-07)))) * float3(0.16666667163372039794921875);
                        float _265 = _194.x;
                        float _266 = _194.y;
                        float _267 = _194.z;
                        float _272 = precise::min(_265, precise::min(_266, _267)) + precise::max(_265, precise::max(_266, _267));
                        float _290 = _194.w;
                        _811 = float4((float3(_272 * 0.5) + (((fast::clamp(abs(fmod(float3(fract(((1.0 + _252.x) + _252.y) + _252.z) * 6.0) + float3(0.0, 4.0, 2.0), float3(6.0)) - float3(3.0)) - float3(1.0), float3(0.0), float3(1.0)) - float3(0.5)) * (_246 / (1.00000011920928955078125 - abs((_235 + _237) - 1.0)))) * (1.0 - abs(_272 - 1.0)))) * _290, _198.w * _290);
                        _812 = true;
                        break;
                    }
                    default:
                    {
                        _811 = _98;
                        _812 = false;
                        break;
                    }
                }
                if (_812)
                {
                    _814 = _811;
                    break;
                }
                _814 = _194;
                break;
            } while(false);
            _1722 = _814;
            break;
        }
        case 10u:
        {
            float4 _174 = Texture0.sample(Sampler0, in.in_var_TEXCOORD0) * in.in_var_COLOR0;
            float4 _178 = Texture1.sample(Sampler0, in.in_var_TEXCOORD1);
            float _179 = _178.w;
            _1722 = float4(_174.xyz * _179, _174.w * _179);
            break;
        }
        case 11u:
        {
            float4 _162 = Texture1.sample(Sampler0, float2(Texture0.sample(Sampler0, in.in_var_TEXCOORD0).w * in.in_var_COLOR0.w, in.in_var_COLOR1.y));
            float _163 = _162.w;
            _1722 = float4(in.in_var_COLOR0.xyz * _163, _163);
            break;
        }
        default:
        {
            _1722 = in.in_var_COLOR0;
            break;
        }
    }
    float4 _1724;
    _1724 = _1722;
    float4 _1725;
    for (uint _1727 = 0u; _1727 < uint(Uniforms.ClipData.x); _1724 = _1725, _1727++)
    {
        float4 _1741 = float4(Uniforms.Clip[_1727][0][0], Uniforms.Clip[_1727][0][1], Uniforms.Clip[_1727][0][2], Uniforms.Clip[_1727][0][3]);
        float4 _1748 = float4(Uniforms.Clip[_1727][1][0], Uniforms.Clip[_1727][1][1], Uniforms.Clip[_1727][1][2], Uniforms.Clip[_1727][1][3]);
        float4 _1750 = floor(_1748 * float4(1.52587890625e-05));
        float4 _1753 = floor(_1748 - (_1750 * 65536.0));
        float4 _1763 = float4(Uniforms.Clip[_1727][2][0], Uniforms.Clip[_1727][2][1], Uniforms.Clip[_1727][2][2], Uniforms.Clip[_1727][2][3]);
        float2 _1773 = (((_1763.xy * in.in_var_TEXCOORD1.x) + (_1763.zw * in.in_var_TEXCOORD1.y)) + float4(Uniforms.Clip[_1727][3][0], Uniforms.Clip[_1727][3][1], Uniforms.Clip[_1727][3][2], _110).xy) - _1741.xy;
        float _2386;
        do
        {
            float2 _1776 = _1741.zw * 0.5;
            float _1777 = _1776.x;
            float _1778 = -_1777;
            float _1779 = _1750.x;
            float _1780 = _1778 + _1779;
            float _1781 = _1776.y;
            float _1782 = -_1781;
            float _1783 = _1753.x;
            float _1784 = _1782 + _1783;
            float2 _1786 = _1773 - float2(_1780, _1784);
            bool _1793;
            if ((_1779 * _1783) > 0.0)
            {
                _1793 = _1773.x < _1780;
            }
            else
            {
                _1793 = false;
            }
            bool _1798;
            if (_1793)
            {
                _1798 = _1773.y <= _1784;
            }
            else
            {
                _1798 = false;
            }
            if (_1798)
            {
                float2 _1801 = float2(_1779, _1783);
                float _1929;
                do
                {
                    if (abs(_1779 - _1783) < 0.100000001490116119384765625)
                    {
                        _1929 = length(_1786) - _1779;
                        break;
                    }
                    float2 _1811 = abs(_1786);
                    float2 _1819;
                    float2 _1820;
                    if (_1811.x > _1811.y)
                    {
                        _1819 = _1811.yx;
                        _1820 = _1801.yx;
                    }
                    else
                    {
                        _1819 = _1811;
                        _1820 = _1801;
                    }
                    float _1825 = (_1820.y * _1820.y) - (_1820.x * _1820.x);
                    float _1828 = (_1820.x * _1819.x) / _1825;
                    float _1831 = (_1820.y * _1819.y) / _1825;
                    float _1832 = _1828 * _1828;
                    float _1833 = _1831 * _1831;
                    float _1835 = (_1832 + _1833) - 1.0;
                    float _1836 = _1835 * 0.3333333432674407958984375;
                    float _1838 = (_1836 * _1836) * _1836;
                    float _1839 = _1832 * _1833;
                    float _1841 = _1838 + (_1839 * 2.0);
                    float _1842 = _1838 + _1839;
                    float _1844 = _1828 + (_1828 * _1833);
                    float _1915;
                    if (_1842 < 0.0)
                    {
                        float _1889 = acos(_1841 / _1838) * 0.3333333432674407958984375;
                        float _1890 = cos(_1889);
                        float _1892 = sin(_1889) * 1.73205077648162841796875;
                        float _1893 = _1835 * (-0.3333333432674407958984375);
                        float _1898 = sqrt((_1893 * ((_1890 + _1892) + 2.0)) + _1832);
                        float _1903 = sqrt((_1893 * ((_1890 - _1892) + 2.0)) + _1832);
                        _1915 = (((_1903 + (float(int(sign(_1825))) * _1898)) + (abs(_1844) / (_1898 * _1903))) - _1828) * 0.5;
                    }
                    else
                    {
                        float _1852 = ((2.0 * _1828) * _1831) * sqrt(_1842);
                        float _1853 = _1841 + _1852;
                        float _1859 = float(int(sign(_1853))) * powr(abs(_1853), 0.3333333432674407958984375);
                        float _1860 = _1841 - _1852;
                        float _1866 = float(int(sign(_1860))) * powr(abs(_1860), 0.3333333432674407958984375);
                        float _1872 = (((-_1859) - _1866) - (_1835 * 1.33333337306976318359375)) + (2.0 * _1832);
                        float _1874 = (_1859 - _1866) * 1.73205077648162841796875;
                        float _1878 = sqrt((_1872 * _1872) + (_1874 * _1874));
                        _1915 = (((_1874 / sqrt(_1878 - _1872)) + ((2.0 * _1844) / _1878)) - _1828) * 0.5;
                    }
                    float _1920 = _1820.y * sqrt(1.0 - (_1915 * _1915));
                    _1929 = length(float2(_1820.x * _1915, _1920) - _1819) * float(int(sign(_1819.y - _1920)));
                    break;
                } while(false);
                _2386 = _1929;
                break;
            }
            float _1930 = _1750.y;
            float _1931 = _1777 - _1930;
            float _1932 = _1753.y;
            float _1933 = _1782 + _1932;
            float2 _1935 = _1773 - float2(_1931, _1933);
            bool _1942;
            if ((_1930 * _1932) > 0.0)
            {
                _1942 = _1773.x >= _1931;
            }
            else
            {
                _1942 = false;
            }
            bool _1947;
            if (_1942)
            {
                _1947 = _1773.y <= _1933;
            }
            else
            {
                _1947 = false;
            }
            if (_1947)
            {
                float2 _1950 = float2(_1930, _1932);
                float _2078;
                do
                {
                    if (abs(_1930 - _1932) < 0.100000001490116119384765625)
                    {
                        _2078 = length(_1935) - _1930;
                        break;
                    }
                    float2 _1960 = abs(_1935);
                    float2 _1968;
                    float2 _1969;
                    if (_1960.x > _1960.y)
                    {
                        _1968 = _1960.yx;
                        _1969 = _1950.yx;
                    }
                    else
                    {
                        _1968 = _1960;
                        _1969 = _1950;
                    }
                    float _1974 = (_1969.y * _1969.y) - (_1969.x * _1969.x);
                    float _1977 = (_1969.x * _1968.x) / _1974;
                    float _1980 = (_1969.y * _1968.y) / _1974;
                    float _1981 = _1977 * _1977;
                    float _1982 = _1980 * _1980;
                    float _1984 = (_1981 + _1982) - 1.0;
                    float _1985 = _1984 * 0.3333333432674407958984375;
                    float _1987 = (_1985 * _1985) * _1985;
                    float _1988 = _1981 * _1982;
                    float _1990 = _1987 + (_1988 * 2.0);
                    float _1991 = _1987 + _1988;
                    float _1993 = _1977 + (_1977 * _1982);
                    float _2064;
                    if (_1991 < 0.0)
                    {
                        float _2038 = acos(_1990 / _1987) * 0.3333333432674407958984375;
                        float _2039 = cos(_2038);
                        float _2041 = sin(_2038) * 1.73205077648162841796875;
                        float _2042 = _1984 * (-0.3333333432674407958984375);
                        float _2047 = sqrt((_2042 * ((_2039 + _2041) + 2.0)) + _1981);
                        float _2052 = sqrt((_2042 * ((_2039 - _2041) + 2.0)) + _1981);
                        _2064 = (((_2052 + (float(int(sign(_1974))) * _2047)) + (abs(_1993) / (_2047 * _2052))) - _1977) * 0.5;
                    }
                    else
                    {
                        float _2001 = ((2.0 * _1977) * _1980) * sqrt(_1991);
                        float _2002 = _1990 + _2001;
                        float _2008 = float(int(sign(_2002))) * powr(abs(_2002), 0.3333333432674407958984375);
                        float _2009 = _1990 - _2001;
                        float _2015 = float(int(sign(_2009))) * powr(abs(_2009), 0.3333333432674407958984375);
                        float _2021 = (((-_2008) - _2015) - (_1984 * 1.33333337306976318359375)) + (2.0 * _1981);
                        float _2023 = (_2008 - _2015) * 1.73205077648162841796875;
                        float _2027 = sqrt((_2021 * _2021) + (_2023 * _2023));
                        _2064 = (((_2023 / sqrt(_2027 - _2021)) + ((2.0 * _1993) / _2027)) - _1977) * 0.5;
                    }
                    float _2069 = _1969.y * sqrt(1.0 - (_2064 * _2064));
                    _2078 = length(float2(_1969.x * _2064, _2069) - _1968) * float(int(sign(_1968.y - _2069)));
                    break;
                } while(false);
                _2386 = _2078;
                break;
            }
            float _2079 = _1750.z;
            float _2080 = _1777 - _2079;
            float _2081 = _1753.z;
            float _2082 = _1781 - _2081;
            float2 _2084 = _1773 - float2(_2080, _2082);
            bool _2091;
            if ((_2079 * _2081) > 0.0)
            {
                _2091 = _1773.x >= _2080;
            }
            else
            {
                _2091 = false;
            }
            bool _2096;
            if (_2091)
            {
                _2096 = _1773.y >= _2082;
            }
            else
            {
                _2096 = false;
            }
            if (_2096)
            {
                float2 _2099 = float2(_2079, _2081);
                float _2227;
                do
                {
                    if (abs(_2079 - _2081) < 0.100000001490116119384765625)
                    {
                        _2227 = length(_2084) - _2079;
                        break;
                    }
                    float2 _2109 = abs(_2084);
                    float2 _2117;
                    float2 _2118;
                    if (_2109.x > _2109.y)
                    {
                        _2117 = _2109.yx;
                        _2118 = _2099.yx;
                    }
                    else
                    {
                        _2117 = _2109;
                        _2118 = _2099;
                    }
                    float _2123 = (_2118.y * _2118.y) - (_2118.x * _2118.x);
                    float _2126 = (_2118.x * _2117.x) / _2123;
                    float _2129 = (_2118.y * _2117.y) / _2123;
                    float _2130 = _2126 * _2126;
                    float _2131 = _2129 * _2129;
                    float _2133 = (_2130 + _2131) - 1.0;
                    float _2134 = _2133 * 0.3333333432674407958984375;
                    float _2136 = (_2134 * _2134) * _2134;
                    float _2137 = _2130 * _2131;
                    float _2139 = _2136 + (_2137 * 2.0);
                    float _2140 = _2136 + _2137;
                    float _2142 = _2126 + (_2126 * _2131);
                    float _2213;
                    if (_2140 < 0.0)
                    {
                        float _2187 = acos(_2139 / _2136) * 0.3333333432674407958984375;
                        float _2188 = cos(_2187);
                        float _2190 = sin(_2187) * 1.73205077648162841796875;
                        float _2191 = _2133 * (-0.3333333432674407958984375);
                        float _2196 = sqrt((_2191 * ((_2188 + _2190) + 2.0)) + _2130);
                        float _2201 = sqrt((_2191 * ((_2188 - _2190) + 2.0)) + _2130);
                        _2213 = (((_2201 + (float(int(sign(_2123))) * _2196)) + (abs(_2142) / (_2196 * _2201))) - _2126) * 0.5;
                    }
                    else
                    {
                        float _2150 = ((2.0 * _2126) * _2129) * sqrt(_2140);
                        float _2151 = _2139 + _2150;
                        float _2157 = float(int(sign(_2151))) * powr(abs(_2151), 0.3333333432674407958984375);
                        float _2158 = _2139 - _2150;
                        float _2164 = float(int(sign(_2158))) * powr(abs(_2158), 0.3333333432674407958984375);
                        float _2170 = (((-_2157) - _2164) - (_2133 * 1.33333337306976318359375)) + (2.0 * _2130);
                        float _2172 = (_2157 - _2164) * 1.73205077648162841796875;
                        float _2176 = sqrt((_2170 * _2170) + (_2172 * _2172));
                        _2213 = (((_2172 / sqrt(_2176 - _2170)) + ((2.0 * _2142) / _2176)) - _2126) * 0.5;
                    }
                    float _2218 = _2118.y * sqrt(1.0 - (_2213 * _2213));
                    _2227 = length(float2(_2118.x * _2213, _2218) - _2117) * float(int(sign(_2117.y - _2218)));
                    break;
                } while(false);
                _2386 = _2227;
                break;
            }
            float _2228 = _1750.w;
            float _2229 = _1778 + _2228;
            float _2230 = _1753.w;
            float _2231 = _1781 - _2230;
            float2 _2233 = _1773 - float2(_2229, _2231);
            bool _2240;
            if ((_2228 * _2230) > 0.0)
            {
                _2240 = _1773.x < _2229;
            }
            else
            {
                _2240 = false;
            }
            bool _2245;
            if (_2240)
            {
                _2245 = _1773.y > _2231;
            }
            else
            {
                _2245 = false;
            }
            if (_2245)
            {
                float2 _2248 = float2(_2228, _2230);
                float _2376;
                do
                {
                    if (abs(_2228 - _2230) < 0.100000001490116119384765625)
                    {
                        _2376 = length(_2233) - _2228;
                        break;
                    }
                    float2 _2258 = abs(_2233);
                    float2 _2266;
                    float2 _2267;
                    if (_2258.x > _2258.y)
                    {
                        _2266 = _2258.yx;
                        _2267 = _2248.yx;
                    }
                    else
                    {
                        _2266 = _2258;
                        _2267 = _2248;
                    }
                    float _2272 = (_2267.y * _2267.y) - (_2267.x * _2267.x);
                    float _2275 = (_2267.x * _2266.x) / _2272;
                    float _2278 = (_2267.y * _2266.y) / _2272;
                    float _2279 = _2275 * _2275;
                    float _2280 = _2278 * _2278;
                    float _2282 = (_2279 + _2280) - 1.0;
                    float _2283 = _2282 * 0.3333333432674407958984375;
                    float _2285 = (_2283 * _2283) * _2283;
                    float _2286 = _2279 * _2280;
                    float _2288 = _2285 + (_2286 * 2.0);
                    float _2289 = _2285 + _2286;
                    float _2291 = _2275 + (_2275 * _2280);
                    float _2362;
                    if (_2289 < 0.0)
                    {
                        float _2336 = acos(_2288 / _2285) * 0.3333333432674407958984375;
                        float _2337 = cos(_2336);
                        float _2339 = sin(_2336) * 1.73205077648162841796875;
                        float _2340 = _2282 * (-0.3333333432674407958984375);
                        float _2345 = sqrt((_2340 * ((_2337 + _2339) + 2.0)) + _2279);
                        float _2350 = sqrt((_2340 * ((_2337 - _2339) + 2.0)) + _2279);
                        _2362 = (((_2350 + (float(int(sign(_2272))) * _2345)) + (abs(_2291) / (_2345 * _2350))) - _2275) * 0.5;
                    }
                    else
                    {
                        float _2299 = ((2.0 * _2275) * _2278) * sqrt(_2289);
                        float _2300 = _2288 + _2299;
                        float _2306 = float(int(sign(_2300))) * powr(abs(_2300), 0.3333333432674407958984375);
                        float _2307 = _2288 - _2299;
                        float _2313 = float(int(sign(_2307))) * powr(abs(_2307), 0.3333333432674407958984375);
                        float _2319 = (((-_2306) - _2313) - (_2282 * 1.33333337306976318359375)) + (2.0 * _2279);
                        float _2321 = (_2306 - _2313) * 1.73205077648162841796875;
                        float _2325 = sqrt((_2319 * _2319) + (_2321 * _2321));
                        _2362 = (((_2321 / sqrt(_2325 - _2319)) + ((2.0 * _2291) / _2325)) - _2275) * 0.5;
                    }
                    float _2367 = _2267.y * sqrt(1.0 - (_2362 * _2362));
                    _2376 = length(float2(_2267.x * _2362, _2367) - _2266) * float(int(sign(_2266.y - _2367)));
                    break;
                } while(false);
                _2386 = _2376;
                break;
            }
            float2 _2378 = abs(_1773) - _1776;
            _2386 = precise::min(precise::max(_2378.x, _2378.y), 0.0) + length(precise::max(_2378, float2(0.0)));
            break;
        } while(false);
        float _2389 = -(_2386 * ((Uniforms.Clip[_1727][3][2] != 0.0) ? (-1.0) : 1.0));
        float _2392 = smoothstep(-0.61804687976837158203125, 0.61804687976837158203125, _2389 / fwidth(_2389));
        _1725 = float4(_1724.xyz * _2392, _1724.w * _2392);
    }
    out.out_var_SV_Target = _1724;
    return out;
}

