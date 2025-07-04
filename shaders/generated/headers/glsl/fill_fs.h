// Generated shader header for fill_fs
// Generated from: fill.glsl

#pragma once

static const char* fill_fs_source = R"GLSL(#version 330

vec4 _98;
float _110;

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
    vec4 _1703;
    switch (uint(in_var_COLOR1.x + 0.5))
    {
        case 0u:
        {
            _1703 = in_var_COLOR0;
            break;
        }
        case 1u:
        {
            _1703 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0) * in_var_COLOR0;
            break;
        }
        case 2u:
        {
            _1703 = texture(SPIRV_Cross_CombinedTexture0Sampler0, (fract((((Uniforms.Vector[2].xy * in_var_TEXCOORD1.x) + (Uniforms.Vector[2].zw * in_var_TEXCOORD1.y)) + Uniforms.Vector[3].xy) / Uniforms.Vector[1].zw) * (Uniforms.Vector[0].zw - Uniforms.Vector[0].xy)) + Uniforms.Vector[0].xy) * in_var_COLOR0;
            break;
        }
        case 3u:
        {
            float _1471 = float(uint(in_var_COLOR1.y + 0.5));
            float _1536;
            if (uint(in_var_COLOR1.z + 0.5) != 0u)
            {
                vec2 _1489 = in_var_TEXCOORD0 - in_var_COLOR2.xy;
                float _1491 = length(_1489 / in_var_COLOR3.zw);
                bool _1498;
                if (in_var_COLOR3.x > 9.9999997473787516355514526367188e-05)
                {
                    _1498 = in_var_COLOR3.y > 9.9999997473787516355514526367188e-05;
                }
                else
                {
                    _1498 = false;
                }
                float _1519;
                if (_1498)
                {
                    float _1504 = length(_1489 / in_var_COLOR3.xy);
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
                            _1517 = clamp((_1504 - 1.0) / (_1504 - _1491), 0.0, 1.0);
                        }
                        _1518 = _1517;
                    }
                    _1519 = _1518;
                }
                else
                {
                    _1519 = clamp(_1491, 0.0, 1.0);
                }
                float _1535;
                if (length(in_var_COLOR2.zw - in_var_COLOR2.xy) > 9.9999997473787516355514526367188e-05)
                {
                    _1535 = clamp(_1519 * (1.0 + ((1.0 - dot(normalize(in_var_TEXCOORD0 - in_var_COLOR2.zw), normalize(in_var_COLOR2.xy - in_var_COLOR2.zw))) * 0.5)), 0.0, 1.0);
                }
                else
                {
                    _1535 = _1519;
                }
                _1536 = _1535;
            }
            else
            {
                vec2 _1483 = in_var_COLOR2.zw - in_var_COLOR2.xy;
                _1536 = clamp(dot(in_var_TEXCOORD0 - in_var_COLOR2.xy, _1483) / dot(_1483, _1483), 0.0, 1.0);
            }
            vec4 _1544 = mix(in_var_COLOR5, in_var_COLOR6, vec4(clamp((_1536 - in_var_COLOR4.x) / (in_var_COLOR4.y - in_var_COLOR4.x), 0.0, 1.0)));
            vec4 _1667;
            if (_1471 > 2.0)
            {
                vec4 _1554 = mix(_1544, in_var_COLOR7, vec4(clamp((_1536 - in_var_COLOR4.y) / (in_var_COLOR4.z - in_var_COLOR4.y), 0.0, 1.0)));
                vec4 _1666;
                if (_1471 > 3.0)
                {
                    vec4 _1567 = mix(_1554, Uniforms.Vector[0u], vec4(clamp((_1536 - in_var_COLOR4.z) / (Uniforms.Scalar4[0].x - in_var_COLOR4.z), 0.0, 1.0)));
                    vec4 _1665;
                    if (_1471 > 4.0)
                    {
                        vec4 _1580 = mix(_1567, Uniforms.Vector[1u], vec4(clamp((_1536 - Uniforms.Scalar4[0].x) / (Uniforms.Scalar4[0].y - Uniforms.Scalar4[0].x), 0.0, 1.0)));
                        vec4 _1664;
                        if (_1471 > 5.0)
                        {
                            vec4 _1593 = mix(_1580, Uniforms.Vector[2u], vec4(clamp((_1536 - Uniforms.Scalar4[0].y) / (Uniforms.Scalar4[0].z - Uniforms.Scalar4[0].y), 0.0, 1.0)));
                            vec4 _1663;
                            if (_1471 > 6.0)
                            {
                                vec4 _1606 = mix(_1593, Uniforms.Vector[3u], vec4(clamp((_1536 - Uniforms.Scalar4[0].z) / (Uniforms.Scalar4[0].w - Uniforms.Scalar4[0].z), 0.0, 1.0)));
                                vec4 _1662;
                                if (_1471 > 7.0)
                                {
                                    vec4 _1619 = mix(_1606, Uniforms.Vector[4u], vec4(clamp((_1536 - Uniforms.Scalar4[0].w) / (Uniforms.Scalar4[1].x - Uniforms.Scalar4[0].w), 0.0, 1.0)));
                                    vec4 _1661;
                                    if (_1471 > 8.0)
                                    {
                                        vec4 _1632 = mix(_1619, Uniforms.Vector[5u], vec4(clamp((_1536 - Uniforms.Scalar4[1].x) / (Uniforms.Scalar4[1].y - Uniforms.Scalar4[1].x), 0.0, 1.0)));
                                        vec4 _1660;
                                        if (_1471 > 9.0)
                                        {
                                            vec4 _1645 = mix(_1632, Uniforms.Vector[6u], vec4(clamp((_1536 - Uniforms.Scalar4[1].y) / (Uniforms.Scalar4[1].z - Uniforms.Scalar4[1].y), 0.0, 1.0)));
                                            vec4 _1659;
                                            if (_1471 > 10.0)
                                            {
                                                _1659 = mix(_1645, Uniforms.Vector[7u], vec4(clamp((_1536 - Uniforms.Scalar4[1].z) / (Uniforms.Scalar4[1].w - Uniforms.Scalar4[1].z), 0.0, 1.0)));
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
            _1703 = _1667;
            break;
        }
        case 7u:
        {
            vec2 _817 = (in_var_TEXCOORD0 - vec2(0.5)) * in_var_COLOR1.zw;
            float _1430;
            do
            {
                vec2 _820 = in_var_COLOR1.zw * 0.5;
                float _821 = _820.x;
                float _822 = -_821;
                float _824 = _822 + in_var_COLOR2.x;
                float _825 = _820.y;
                float _826 = -_825;
                float _828 = _826 + in_var_COLOR3.x;
                vec2 _830 = _817 - vec2(_824, _828);
                bool _837;
                if ((in_var_COLOR2.x * in_var_COLOR3.x) > 0.0)
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
                    vec2 _845 = vec2(in_var_COLOR2.x, in_var_COLOR3.x);
                    float _973;
                    do
                    {
                        if (abs(in_var_COLOR2.x - in_var_COLOR3.x) < 0.100000001490116119384765625)
                        {
                            _973 = length(_830) - in_var_COLOR2.x;
                            break;
                        }
                        vec2 _855 = abs(_830);
                        vec2 _863;
                        vec2 _864;
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
                            float _903 = float(int(sign(_897))) * pow(abs(_897), 0.3333333432674407958984375);
                            float _904 = _885 - _896;
                            float _910 = float(int(sign(_904))) * pow(abs(_904), 0.3333333432674407958984375);
                            float _916 = (((-_903) - _910) - (_879 * 1.33333337306976318359375)) + (2.0 * _876);
                            float _918 = (_903 - _910) * 1.73205077648162841796875;
                            float _922 = sqrt((_916 * _916) + (_918 * _918));
                            _959 = (((_918 / sqrt(_922 - _916)) + ((2.0 * _888) / _922)) - _872) * 0.5;
                        }
                        float _964 = _864.y * sqrt(1.0 - (_959 * _959));
                        _973 = length(vec2(_864.x * _959, _964) - _863) * float(int(sign(_863.y - _964)));
                        break;
                    } while(false);
                    _1430 = _973;
                    break;
                }
                float _975 = _821 - in_var_COLOR2.y;
                float _977 = _826 + in_var_COLOR3.y;
                vec2 _979 = _817 - vec2(_975, _977);
                bool _986;
                if ((in_var_COLOR2.y * in_var_COLOR3.y) > 0.0)
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
                    vec2 _994 = vec2(in_var_COLOR2.y, in_var_COLOR3.y);
                    float _1122;
                    do
                    {
                        if (abs(in_var_COLOR2.y - in_var_COLOR3.y) < 0.100000001490116119384765625)
                        {
                            _1122 = length(_979) - in_var_COLOR2.y;
                            break;
                        }
                        vec2 _1004 = abs(_979);
                        vec2 _1012;
                        vec2 _1013;
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
                            float _1052 = float(int(sign(_1046))) * pow(abs(_1046), 0.3333333432674407958984375);
                            float _1053 = _1034 - _1045;
                            float _1059 = float(int(sign(_1053))) * pow(abs(_1053), 0.3333333432674407958984375);
                            float _1065 = (((-_1052) - _1059) - (_1028 * 1.33333337306976318359375)) + (2.0 * _1025);
                            float _1067 = (_1052 - _1059) * 1.73205077648162841796875;
                            float _1071 = sqrt((_1065 * _1065) + (_1067 * _1067));
                            _1108 = (((_1067 / sqrt(_1071 - _1065)) + ((2.0 * _1037) / _1071)) - _1021) * 0.5;
                        }
                        float _1113 = _1013.y * sqrt(1.0 - (_1108 * _1108));
                        _1122 = length(vec2(_1013.x * _1108, _1113) - _1012) * float(int(sign(_1012.y - _1113)));
                        break;
                    } while(false);
                    _1430 = _1122;
                    break;
                }
                float _1124 = _821 - in_var_COLOR2.z;
                float _1126 = _825 - in_var_COLOR3.z;
                vec2 _1128 = _817 - vec2(_1124, _1126);
                bool _1135;
                if ((in_var_COLOR2.z * in_var_COLOR3.z) > 0.0)
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
                    vec2 _1143 = vec2(in_var_COLOR2.z, in_var_COLOR3.z);
                    float _1271;
                    do
                    {
                        if (abs(in_var_COLOR2.z - in_var_COLOR3.z) < 0.100000001490116119384765625)
                        {
                            _1271 = length(_1128) - in_var_COLOR2.z;
                            break;
                        }
                        vec2 _1153 = abs(_1128);
                        vec2 _1161;
                        vec2 _1162;
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
                            float _1201 = float(int(sign(_1195))) * pow(abs(_1195), 0.3333333432674407958984375);
                            float _1202 = _1183 - _1194;
                            float _1208 = float(int(sign(_1202))) * pow(abs(_1202), 0.3333333432674407958984375);
                            float _1214 = (((-_1201) - _1208) - (_1177 * 1.33333337306976318359375)) + (2.0 * _1174);
                            float _1216 = (_1201 - _1208) * 1.73205077648162841796875;
                            float _1220 = sqrt((_1214 * _1214) + (_1216 * _1216));
                            _1257 = (((_1216 / sqrt(_1220 - _1214)) + ((2.0 * _1186) / _1220)) - _1170) * 0.5;
                        }
                        float _1262 = _1162.y * sqrt(1.0 - (_1257 * _1257));
                        _1271 = length(vec2(_1162.x * _1257, _1262) - _1161) * float(int(sign(_1161.y - _1262)));
                        break;
                    } while(false);
                    _1430 = _1271;
                    break;
                }
                float _1273 = _822 + in_var_COLOR2.w;
                float _1275 = _825 - in_var_COLOR3.w;
                vec2 _1277 = _817 - vec2(_1273, _1275);
                bool _1284;
                if ((in_var_COLOR2.w * in_var_COLOR3.w) > 0.0)
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
                    vec2 _1292 = vec2(in_var_COLOR2.w, in_var_COLOR3.w);
                    float _1420;
                    do
                    {
                        if (abs(in_var_COLOR2.w - in_var_COLOR3.w) < 0.100000001490116119384765625)
                        {
                            _1420 = length(_1277) - in_var_COLOR2.w;
                            break;
                        }
                        vec2 _1302 = abs(_1277);
                        vec2 _1310;
                        vec2 _1311;
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
                            float _1350 = float(int(sign(_1344))) * pow(abs(_1344), 0.3333333432674407958984375);
                            float _1351 = _1332 - _1343;
                            float _1357 = float(int(sign(_1351))) * pow(abs(_1351), 0.3333333432674407958984375);
                            float _1363 = (((-_1350) - _1357) - (_1326 * 1.33333337306976318359375)) + (2.0 * _1323);
                            float _1365 = (_1350 - _1357) * 1.73205077648162841796875;
                            float _1369 = sqrt((_1363 * _1363) + (_1365 * _1365));
                            _1406 = (((_1365 / sqrt(_1369 - _1363)) + ((2.0 * _1335) / _1369)) - _1319) * 0.5;
                        }
                        float _1411 = _1311.y * sqrt(1.0 - (_1406 * _1406));
                        _1420 = length(vec2(_1311.x * _1406, _1411) - _1310) * float(int(sign(_1310.y - _1411)));
                        break;
                    } while(false);
                    _1430 = _1420;
                    break;
                }
                vec2 _1422 = abs(_817) - _820;
                float _1423 = _1422.x;
                float _1424 = _1422.y;
                float _1425 = isnan(_1424) ? _1423 : (isnan(_1423) ? _1424 : max(_1423, _1424));
                bvec2 _2396 = isnan(_1422);
                bvec2 _2397 = isnan(vec2(0.0));
                vec2 _2398 = max(_1422, vec2(0.0));
                vec2 _2399 = vec2(_2396.x ? vec2(0.0).x : _2398.x, _2396.y ? vec2(0.0).y : _2398.y);
                _1430 = (isnan(0.0) ? _1425 : (isnan(_1425) ? 0.0 : min(_1425, 0.0))) + length(vec2(_2397.x ? _1422.x : _2399.x, _2397.y ? _1422.y : _2399.y));
                break;
            } while(false);
            float _1431 = -_1430;
            float _1432 = smoothstep(-0.3540000021457672119140625, 0.3540000021457672119140625, _1431);
            float _1434 = _1432 * in_var_COLOR0.w;
            vec4 _1440 = vec4(in_var_COLOR0.xyz * _1434, _1434);
            vec4 _1467;
            if (in_var_COLOR4.x > 0.0)
            {
                float _1448 = 1.0 - smoothstep(in_var_COLOR4.x - 0.3540000021457672119140625, in_var_COLOR4.x + 0.3540000021457672119140625, _1431);
                float _1451 = (isnan(_1448) ? _1432 : (isnan(_1432) ? _1448 : min(_1432, _1448))) * in_var_COLOR5.w;
                float _1460 = 1.0 - _1451;
                vec3 _1462 = vec4(in_var_COLOR5.xyz * _1451, _1451).xyz + (_1440.xyz * _1460);
                vec4 _1463 = vec4(_1462.x, _1462.y, _1462.z, _98.w);
                _1463.w = _1451 + (_1434 * _1460);
                _1467 = _1463;
            }
            else
            {
                _1467 = _1440;
            }
            _1703 = _1467;
            break;
        }
        case 8u:
        {
            _1703 = in_var_COLOR0;
            break;
        }
        case 9u:
        {
            vec4 _814;
            do
            {
                vec4 _193 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0);
                vec4 _194 = _193 * in_var_COLOR0;
                vec4 _198 = texture(SPIRV_Cross_CombinedTexture1Sampler0, in_var_TEXCOORD1);
                vec4 _811;
                bool _812;
                switch (uint(in_var_COLOR1.y + 0.5))
                {
                    case 0u:
                    {
                        _811 = vec4(0.0);
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
                        _811 = clamp((_194 * (1.0 - _198.w)) + (_198 * (1.0 - _194.w)), vec4(0.0), vec4(1.0));
                        _812 = true;
                        break;
                    }
                    case 11u:
                    {
                        vec3 _762 = _194.xyz;
                        vec3 _763 = _198.xyz;
                        bvec3 _2406 = isnan(_762);
                        bvec3 _2407 = isnan(_763);
                        vec3 _2408 = min(_762, _763);
                        vec3 _2409 = vec3(_2406.x ? _763.x : _2408.x, _2406.y ? _763.y : _2408.y, _2406.z ? _763.z : _2408.z);
                        float _765 = _194.w;
                        _811 = vec4(vec3(_2407.x ? _762.x : _2409.x, _2407.y ? _762.y : _2409.y, _2407.z ? _762.z : _2409.z) * _765, _198.w * _765);
                        _812 = true;
                        break;
                    }
                    case 12u:
                    {
                        _811 = clamp(_194 + _198, vec4(0.0), vec4(1.0));
                        _812 = true;
                        break;
                    }
                    case 13u:
                    {
                        float _752 = _194.w;
                        _811 = vec4(abs(_198.xyz - _194.xyz) * _752, _198.w * _752);
                        _812 = true;
                        break;
                    }
                    case 14u:
                    {
                        float _740 = _194.w;
                        _811 = vec4((_194.xyz * _198.xyz) * _740, _198.w * _740);
                        _812 = true;
                        break;
                    }
                    case 15u:
                    {
                        float _729 = _194.w;
                        _811 = vec4((vec3(1.0) - ((vec3(1.0) - _198.xyz) * (vec3(1.0) - _194.xyz))) * _729, _198.w * _729);
                        _812 = true;
                        break;
                    }
                    case 16u:
                    {
                        vec3 _117 = _194.xyz;
                        vec3 _118 = _198.xyz;
                        vec3 _112;
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
                        _811 = vec4(_112 * _715, _198.w * _715);
                        _812 = true;
                        break;
                    }
                    case 17u:
                    {
                        vec3 _673 = _194.xyz;
                        vec3 _674 = _198.xyz;
                        bvec3 _2411 = isnan(_673);
                        bvec3 _2412 = isnan(_674);
                        vec3 _2413 = max(_673, _674);
                        vec3 _2414 = vec3(_2411.x ? _674.x : _2413.x, _2411.y ? _674.y : _2413.y, _2411.z ? _674.z : _2413.z);
                        float _676 = _194.w;
                        _811 = vec4(vec3(_2412.x ? _673.x : _2414.x, _2412.y ? _673.y : _2414.y, _2412.z ? _673.z : _2414.z) * _676, _198.w * _676);
                        _812 = true;
                        break;
                    }
                    case 18u:
                    {
                        vec3 _119 = _194.xyz;
                        vec3 _120 = _198.xyz;
                        vec3 _113;
                        for (uint _643 = 0u; _643 < 3u; _643++)
                        {
                            float _662;
                            if (_119[_643] == 1.0)
                            {
                                _662 = _119[_643];
                            }
                            else
                            {
                                float _659 = _120[_643] / (1.0 - _119[_643]);
                                _662 = isnan(1.0) ? _659 : (isnan(_659) ? 1.0 : min(_659, 1.0));
                            }
                            _113[_643] = _662;
                        }
                        float _665 = _194.w;
                        _811 = vec4(_113 * _665, _198.w * _665);
                        _812 = true;
                        break;
                    }
                    case 19u:
                    {
                        vec3 _121 = _194.xyz;
                        vec3 _122 = _198.xyz;
                        vec3 _114;
                        for (uint _609 = 0u; _609 < 3u; _609++)
                        {
                            float _629;
                            if (_121[_609] == 0.0)
                            {
                                _629 = _121[_609];
                            }
                            else
                            {
                                float _626 = 1.0 - ((1.0 - _122[_609]) / _121[_609]);
                                _629 = isnan(0.0) ? _626 : (isnan(_626) ? 0.0 : max(_626, 0.0));
                            }
                            _114[_609] = _629;
                        }
                        float _632 = _194.w;
                        _811 = vec4(_114 * _632, _198.w * _632);
                        _812 = true;
                        break;
                    }
                    case 20u:
                    {
                        vec3 _123 = _198.xyz;
                        vec3 _124 = _194.xyz;
                        vec3 _115;
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
                        _811 = vec4(_115 * _598, _198.w * _598);
                        _812 = true;
                        break;
                    }
                    case 21u:
                    {
                        vec3 _125 = _194.xyz;
                        vec3 _126 = _198.xyz;
                        vec3 _116;
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
                        _811 = vec4(_116 * _559, _198.w * _559);
                        _812 = true;
                        break;
                    }
                    case 22u:
                    {
                        vec3 _501 = _198.xyz;
                        vec3 _502 = _194.xyz;
                        float _507 = _194.w;
                        _811 = vec4(((_501 + _502) - ((_501 * 2.0) * _502)) * _507, _198.w * _507);
                        _812 = true;
                        break;
                    }
                    case 23u:
                    {
                        vec3 _435 = _194.xyz;
                        float _436 = _198.x;
                        float _437 = _198.y;
                        float _438 = _198.z;
                        float _439 = isnan(_438) ? _437 : (isnan(_437) ? _438 : min(_437, _438));
                        float _440 = isnan(_439) ? _436 : (isnan(_436) ? _439 : min(_436, _439));
                        float _441 = isnan(_438) ? _437 : (isnan(_437) ? _438 : max(_437, _438));
                        float _442 = isnan(_441) ? _436 : (isnan(_436) ? _441 : max(_436, _441));
                        float _444 = _440 + _442;
                        float _446 = abs(_444 - 1.0);
                        float _450 = _194.x;
                        float _451 = _194.y;
                        float _452 = _194.z;
                        float _453 = isnan(_452) ? _451 : (isnan(_451) ? _452 : min(_451, _452));
                        float _455 = isnan(_452) ? _451 : (isnan(_451) ? _452 : max(_451, _452));
                        vec3 _471 = ((step(_194.yxx, _435) * step(_194.zzy, _435)) * (vec3(0.0, 2.0, 4.0) + ((_194.yzx - _194.zxy) / vec3(((isnan(_455) ? _450 : (isnan(_450) ? _455 : max(_450, _455))) - (isnan(_453) ? _450 : (isnan(_450) ? _453 : min(_450, _453)))) + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        vec3 _481 = vec3(fract(((1.0 + _471.x) + _471.y) + _471.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _493 = _194.w;
                        _811 = vec4((vec3(_444 * 0.5) + (((clamp(abs((_481 - vec3(6.0) * trunc(_481 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * ((_442 - _440) / (1.00000011920928955078125 - _446))) * (1.0 - _446))) * _493, _198.w * _493);
                        _812 = true;
                        break;
                    }
                    case 24u:
                    {
                        vec3 _366 = _198.xyz;
                        float _367 = _198.x;
                        float _368 = _198.y;
                        float _369 = _198.z;
                        float _370 = isnan(_369) ? _368 : (isnan(_368) ? _369 : min(_368, _369));
                        float _371 = isnan(_370) ? _367 : (isnan(_367) ? _370 : min(_367, _370));
                        float _372 = isnan(_369) ? _368 : (isnan(_368) ? _369 : max(_368, _369));
                        float _373 = isnan(_372) ? _367 : (isnan(_367) ? _372 : max(_367, _372));
                        vec3 _388 = ((step(_198.yxx, _366) * step(_198.zzy, _366)) * (vec3(0.0, 2.0, 4.0) + ((_198.yzx - _198.zxy) / vec3((_373 - _371) + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        float _396 = _371 + _373;
                        float _398 = _194.x;
                        float _399 = _194.y;
                        float _400 = _194.z;
                        float _401 = isnan(_400) ? _399 : (isnan(_399) ? _400 : min(_399, _400));
                        float _402 = isnan(_401) ? _398 : (isnan(_398) ? _401 : min(_398, _401));
                        float _403 = isnan(_400) ? _399 : (isnan(_399) ? _400 : max(_399, _400));
                        float _404 = isnan(_403) ? _398 : (isnan(_398) ? _403 : max(_398, _403));
                        vec3 _413 = vec3(fract(((1.0 + _388.x) + _388.y) + _388.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _427 = _194.w;
                        _811 = vec4((vec3(_396 * 0.5) + (((clamp(abs((_413 - vec3(6.0) * trunc(_413 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * ((_404 - _402) / (1.00000011920928955078125 - abs((_402 + _404) - 1.0)))) * (1.0 - abs(_396 - 1.0)))) * _427, _198.w * _427);
                        _812 = true;
                        break;
                    }
                    case 25u:
                    {
                        vec3 _298 = _194.xyz;
                        float _299 = _194.x;
                        float _300 = _194.y;
                        float _301 = _194.z;
                        float _302 = isnan(_301) ? _300 : (isnan(_300) ? _301 : min(_300, _301));
                        float _303 = isnan(_302) ? _299 : (isnan(_299) ? _302 : min(_299, _302));
                        float _304 = isnan(_301) ? _300 : (isnan(_300) ? _301 : max(_300, _301));
                        float _305 = isnan(_304) ? _299 : (isnan(_299) ? _304 : max(_299, _304));
                        float _314 = _305 - _303;
                        vec3 _320 = ((step(_194.yxx, _298) * step(_194.zzy, _298)) * (vec3(0.0, 2.0, 4.0) + ((_194.yzx - _194.zxy) / vec3(_314 + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        float _333 = _198.x;
                        float _334 = _198.y;
                        float _335 = _198.z;
                        float _336 = isnan(_335) ? _334 : (isnan(_334) ? _335 : min(_334, _335));
                        float _338 = isnan(_335) ? _334 : (isnan(_334) ? _335 : max(_334, _335));
                        float _340 = (isnan(_336) ? _333 : (isnan(_333) ? _336 : min(_333, _336))) + (isnan(_338) ? _333 : (isnan(_333) ? _338 : max(_333, _338)));
                        vec3 _344 = vec3(fract(((1.0 + _320.x) + _320.y) + _320.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _358 = _194.w;
                        _811 = vec4((vec3(_340 * 0.5) + (((clamp(abs((_344 - vec3(6.0) * trunc(_344 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * (_314 / (1.00000011920928955078125 - abs((_303 + _305) - 1.0)))) * (1.0 - abs(_340 - 1.0)))) * _358, _198.w * _358);
                        _812 = true;
                        break;
                    }
                    case 26u:
                    {
                        vec3 _230 = _198.xyz;
                        float _231 = _198.x;
                        float _232 = _198.y;
                        float _233 = _198.z;
                        float _234 = isnan(_233) ? _232 : (isnan(_232) ? _233 : min(_232, _233));
                        float _235 = isnan(_234) ? _231 : (isnan(_231) ? _234 : min(_231, _234));
                        float _236 = isnan(_233) ? _232 : (isnan(_232) ? _233 : max(_232, _233));
                        float _237 = isnan(_236) ? _231 : (isnan(_231) ? _236 : max(_231, _236));
                        float _246 = _237 - _235;
                        vec3 _252 = ((step(_198.yxx, _230) * step(_198.zzy, _230)) * (vec3(0.0, 2.0, 4.0) + ((_198.yzx - _198.zxy) / vec3(_246 + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        float _265 = _194.x;
                        float _266 = _194.y;
                        float _267 = _194.z;
                        float _268 = isnan(_267) ? _266 : (isnan(_266) ? _267 : min(_266, _267));
                        float _270 = isnan(_267) ? _266 : (isnan(_266) ? _267 : max(_266, _267));
                        float _272 = (isnan(_268) ? _265 : (isnan(_265) ? _268 : min(_265, _268))) + (isnan(_270) ? _265 : (isnan(_265) ? _270 : max(_265, _270)));
                        vec3 _276 = vec3(fract(((1.0 + _252.x) + _252.y) + _252.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _290 = _194.w;
                        _811 = vec4((vec3(_272 * 0.5) + (((clamp(abs((_276 - vec3(6.0) * trunc(_276 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * (_246 / (1.00000011920928955078125 - abs((_235 + _237) - 1.0)))) * (1.0 - abs(_272 - 1.0)))) * _290, _198.w * _290);
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
            _1703 = _814;
            break;
        }
        case 10u:
        {
            vec4 _174 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0) * in_var_COLOR0;
            vec4 _178 = texture(SPIRV_Cross_CombinedTexture1Sampler0, in_var_TEXCOORD1);
            float _179 = _178.w;
            _1703 = vec4(_174.xyz * _179, _174.w * _179);
            break;
        }
        case 11u:
        {
            vec4 _162 = texture(SPIRV_Cross_CombinedTexture1Sampler0, vec2(texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0).w * in_var_COLOR0.w, in_var_COLOR1.y));
            float _163 = _162.w;
            _1703 = vec4(in_var_COLOR0.xyz * _163, _163);
            break;
        }
        default:
        {
            _1703 = in_var_COLOR0;
            break;
        }
    }
    vec4 _1705;
    _1705 = _1703;
    vec4 _1706;
    for (uint _1708 = 0u; _1708 < uint(Uniforms.ClipData.x); _1705 = _1706, _1708++)
    {
        vec4 _1722 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_1708])[0].x, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[1].x, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[2].x, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[3].x);
        vec4 _1729 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_1708])[0].y, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[1].y, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[2].y, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[3].y);
        vec4 _1731 = floor(_1729 * vec4(1.52587890625e-05));
        vec4 _1734 = floor(_1729 - (_1731 * 65536.0));
        vec4 _1744 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_1708])[0].z, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[1].z, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[2].z, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[3].z);
        vec2 _1754 = (((_1744.xy * in_var_TEXCOORD1.x) + (_1744.zw * in_var_TEXCOORD1.y)) + vec4(spvWorkaroundRowMajor(Uniforms.Clip[_1708])[0].w, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[1].w, spvWorkaroundRowMajor(Uniforms.Clip[_1708])[2].w, _110).xy) - _1722.xy;
        float _2367;
        do
        {
            vec2 _1757 = _1722.zw * 0.5;
            float _1758 = _1757.x;
            float _1759 = -_1758;
            float _1760 = _1731.x;
            float _1761 = _1759 + _1760;
            float _1762 = _1757.y;
            float _1763 = -_1762;
            float _1764 = _1734.x;
            float _1765 = _1763 + _1764;
            vec2 _1767 = _1754 - vec2(_1761, _1765);
            bool _1774;
            if ((_1760 * _1764) > 0.0)
            {
                _1774 = _1754.x < _1761;
            }
            else
            {
                _1774 = false;
            }
            bool _1779;
            if (_1774)
            {
                _1779 = _1754.y <= _1765;
            }
            else
            {
                _1779 = false;
            }
            if (_1779)
            {
                vec2 _1782 = vec2(_1760, _1764);
                float _1910;
                do
                {
                    if (abs(_1760 - _1764) < 0.100000001490116119384765625)
                    {
                        _1910 = length(_1767) - _1760;
                        break;
                    }
                    vec2 _1792 = abs(_1767);
                    vec2 _1800;
                    vec2 _1801;
                    if (_1792.x > _1792.y)
                    {
                        _1800 = _1792.yx;
                        _1801 = _1782.yx;
                    }
                    else
                    {
                        _1800 = _1792;
                        _1801 = _1782;
                    }
                    float _1806 = (_1801.y * _1801.y) - (_1801.x * _1801.x);
                    float _1809 = (_1801.x * _1800.x) / _1806;
                    float _1812 = (_1801.y * _1800.y) / _1806;
                    float _1813 = _1809 * _1809;
                    float _1814 = _1812 * _1812;
                    float _1816 = (_1813 + _1814) - 1.0;
                    float _1817 = _1816 * 0.3333333432674407958984375;
                    float _1819 = (_1817 * _1817) * _1817;
                    float _1820 = _1813 * _1814;
                    float _1822 = _1819 + (_1820 * 2.0);
                    float _1823 = _1819 + _1820;
                    float _1825 = _1809 + (_1809 * _1814);
                    float _1896;
                    if (_1823 < 0.0)
                    {
                        float _1870 = acos(_1822 / _1819) * 0.3333333432674407958984375;
                        float _1871 = cos(_1870);
                        float _1873 = sin(_1870) * 1.73205077648162841796875;
                        float _1874 = _1816 * (-0.3333333432674407958984375);
                        float _1879 = sqrt((_1874 * ((_1871 + _1873) + 2.0)) + _1813);
                        float _1884 = sqrt((_1874 * ((_1871 - _1873) + 2.0)) + _1813);
                        _1896 = (((_1884 + (float(int(sign(_1806))) * _1879)) + (abs(_1825) / (_1879 * _1884))) - _1809) * 0.5;
                    }
                    else
                    {
                        float _1833 = ((2.0 * _1809) * _1812) * sqrt(_1823);
                        float _1834 = _1822 + _1833;
                        float _1840 = float(int(sign(_1834))) * pow(abs(_1834), 0.3333333432674407958984375);
                        float _1841 = _1822 - _1833;
                        float _1847 = float(int(sign(_1841))) * pow(abs(_1841), 0.3333333432674407958984375);
                        float _1853 = (((-_1840) - _1847) - (_1816 * 1.33333337306976318359375)) + (2.0 * _1813);
                        float _1855 = (_1840 - _1847) * 1.73205077648162841796875;
                        float _1859 = sqrt((_1853 * _1853) + (_1855 * _1855));
                        _1896 = (((_1855 / sqrt(_1859 - _1853)) + ((2.0 * _1825) / _1859)) - _1809) * 0.5;
                    }
                    float _1901 = _1801.y * sqrt(1.0 - (_1896 * _1896));
                    _1910 = length(vec2(_1801.x * _1896, _1901) - _1800) * float(int(sign(_1800.y - _1901)));
                    break;
                } while(false);
                _2367 = _1910;
                break;
            }
            float _1911 = _1731.y;
            float _1912 = _1758 - _1911;
            float _1913 = _1734.y;
            float _1914 = _1763 + _1913;
            vec2 _1916 = _1754 - vec2(_1912, _1914);
            bool _1923;
            if ((_1911 * _1913) > 0.0)
            {
                _1923 = _1754.x >= _1912;
            }
            else
            {
                _1923 = false;
            }
            bool _1928;
            if (_1923)
            {
                _1928 = _1754.y <= _1914;
            }
            else
            {
                _1928 = false;
            }
            if (_1928)
            {
                vec2 _1931 = vec2(_1911, _1913);
                float _2059;
                do
                {
                    if (abs(_1911 - _1913) < 0.100000001490116119384765625)
                    {
                        _2059 = length(_1916) - _1911;
                        break;
                    }
                    vec2 _1941 = abs(_1916);
                    vec2 _1949;
                    vec2 _1950;
                    if (_1941.x > _1941.y)
                    {
                        _1949 = _1941.yx;
                        _1950 = _1931.yx;
                    }
                    else
                    {
                        _1949 = _1941;
                        _1950 = _1931;
                    }
                    float _1955 = (_1950.y * _1950.y) - (_1950.x * _1950.x);
                    float _1958 = (_1950.x * _1949.x) / _1955;
                    float _1961 = (_1950.y * _1949.y) / _1955;
                    float _1962 = _1958 * _1958;
                    float _1963 = _1961 * _1961;
                    float _1965 = (_1962 + _1963) - 1.0;
                    float _1966 = _1965 * 0.3333333432674407958984375;
                    float _1968 = (_1966 * _1966) * _1966;
                    float _1969 = _1962 * _1963;
                    float _1971 = _1968 + (_1969 * 2.0);
                    float _1972 = _1968 + _1969;
                    float _1974 = _1958 + (_1958 * _1963);
                    float _2045;
                    if (_1972 < 0.0)
                    {
                        float _2019 = acos(_1971 / _1968) * 0.3333333432674407958984375;
                        float _2020 = cos(_2019);
                        float _2022 = sin(_2019) * 1.73205077648162841796875;
                        float _2023 = _1965 * (-0.3333333432674407958984375);
                        float _2028 = sqrt((_2023 * ((_2020 + _2022) + 2.0)) + _1962);
                        float _2033 = sqrt((_2023 * ((_2020 - _2022) + 2.0)) + _1962);
                        _2045 = (((_2033 + (float(int(sign(_1955))) * _2028)) + (abs(_1974) / (_2028 * _2033))) - _1958) * 0.5;
                    }
                    else
                    {
                        float _1982 = ((2.0 * _1958) * _1961) * sqrt(_1972);
                        float _1983 = _1971 + _1982;
                        float _1989 = float(int(sign(_1983))) * pow(abs(_1983), 0.3333333432674407958984375);
                        float _1990 = _1971 - _1982;
                        float _1996 = float(int(sign(_1990))) * pow(abs(_1990), 0.3333333432674407958984375);
                        float _2002 = (((-_1989) - _1996) - (_1965 * 1.33333337306976318359375)) + (2.0 * _1962);
                        float _2004 = (_1989 - _1996) * 1.73205077648162841796875;
                        float _2008 = sqrt((_2002 * _2002) + (_2004 * _2004));
                        _2045 = (((_2004 / sqrt(_2008 - _2002)) + ((2.0 * _1974) / _2008)) - _1958) * 0.5;
                    }
                    float _2050 = _1950.y * sqrt(1.0 - (_2045 * _2045));
                    _2059 = length(vec2(_1950.x * _2045, _2050) - _1949) * float(int(sign(_1949.y - _2050)));
                    break;
                } while(false);
                _2367 = _2059;
                break;
            }
            float _2060 = _1731.z;
            float _2061 = _1758 - _2060;
            float _2062 = _1734.z;
            float _2063 = _1762 - _2062;
            vec2 _2065 = _1754 - vec2(_2061, _2063);
            bool _2072;
            if ((_2060 * _2062) > 0.0)
            {
                _2072 = _1754.x >= _2061;
            }
            else
            {
                _2072 = false;
            }
            bool _2077;
            if (_2072)
            {
                _2077 = _1754.y >= _2063;
            }
            else
            {
                _2077 = false;
            }
            if (_2077)
            {
                vec2 _2080 = vec2(_2060, _2062);
                float _2208;
                do
                {
                    if (abs(_2060 - _2062) < 0.100000001490116119384765625)
                    {
                        _2208 = length(_2065) - _2060;
                        break;
                    }
                    vec2 _2090 = abs(_2065);
                    vec2 _2098;
                    vec2 _2099;
                    if (_2090.x > _2090.y)
                    {
                        _2098 = _2090.yx;
                        _2099 = _2080.yx;
                    }
                    else
                    {
                        _2098 = _2090;
                        _2099 = _2080;
                    }
                    float _2104 = (_2099.y * _2099.y) - (_2099.x * _2099.x);
                    float _2107 = (_2099.x * _2098.x) / _2104;
                    float _2110 = (_2099.y * _2098.y) / _2104;
                    float _2111 = _2107 * _2107;
                    float _2112 = _2110 * _2110;
                    float _2114 = (_2111 + _2112) - 1.0;
                    float _2115 = _2114 * 0.3333333432674407958984375;
                    float _2117 = (_2115 * _2115) * _2115;
                    float _2118 = _2111 * _2112;
                    float _2120 = _2117 + (_2118 * 2.0);
                    float _2121 = _2117 + _2118;
                    float _2123 = _2107 + (_2107 * _2112);
                    float _2194;
                    if (_2121 < 0.0)
                    {
                        float _2168 = acos(_2120 / _2117) * 0.3333333432674407958984375;
                        float _2169 = cos(_2168);
                        float _2171 = sin(_2168) * 1.73205077648162841796875;
                        float _2172 = _2114 * (-0.3333333432674407958984375);
                        float _2177 = sqrt((_2172 * ((_2169 + _2171) + 2.0)) + _2111);
                        float _2182 = sqrt((_2172 * ((_2169 - _2171) + 2.0)) + _2111);
                        _2194 = (((_2182 + (float(int(sign(_2104))) * _2177)) + (abs(_2123) / (_2177 * _2182))) - _2107) * 0.5;
                    }
                    else
                    {
                        float _2131 = ((2.0 * _2107) * _2110) * sqrt(_2121);
                        float _2132 = _2120 + _2131;
                        float _2138 = float(int(sign(_2132))) * pow(abs(_2132), 0.3333333432674407958984375);
                        float _2139 = _2120 - _2131;
                        float _2145 = float(int(sign(_2139))) * pow(abs(_2139), 0.3333333432674407958984375);
                        float _2151 = (((-_2138) - _2145) - (_2114 * 1.33333337306976318359375)) + (2.0 * _2111);
                        float _2153 = (_2138 - _2145) * 1.73205077648162841796875;
                        float _2157 = sqrt((_2151 * _2151) + (_2153 * _2153));
                        _2194 = (((_2153 / sqrt(_2157 - _2151)) + ((2.0 * _2123) / _2157)) - _2107) * 0.5;
                    }
                    float _2199 = _2099.y * sqrt(1.0 - (_2194 * _2194));
                    _2208 = length(vec2(_2099.x * _2194, _2199) - _2098) * float(int(sign(_2098.y - _2199)));
                    break;
                } while(false);
                _2367 = _2208;
                break;
            }
            float _2209 = _1731.w;
            float _2210 = _1759 + _2209;
            float _2211 = _1734.w;
            float _2212 = _1762 - _2211;
            vec2 _2214 = _1754 - vec2(_2210, _2212);
            bool _2221;
            if ((_2209 * _2211) > 0.0)
            {
                _2221 = _1754.x < _2210;
            }
            else
            {
                _2221 = false;
            }
            bool _2226;
            if (_2221)
            {
                _2226 = _1754.y > _2212;
            }
            else
            {
                _2226 = false;
            }
            if (_2226)
            {
                vec2 _2229 = vec2(_2209, _2211);
                float _2357;
                do
                {
                    if (abs(_2209 - _2211) < 0.100000001490116119384765625)
                    {
                        _2357 = length(_2214) - _2209;
                        break;
                    }
                    vec2 _2239 = abs(_2214);
                    vec2 _2247;
                    vec2 _2248;
                    if (_2239.x > _2239.y)
                    {
                        _2247 = _2239.yx;
                        _2248 = _2229.yx;
                    }
                    else
                    {
                        _2247 = _2239;
                        _2248 = _2229;
                    }
                    float _2253 = (_2248.y * _2248.y) - (_2248.x * _2248.x);
                    float _2256 = (_2248.x * _2247.x) / _2253;
                    float _2259 = (_2248.y * _2247.y) / _2253;
                    float _2260 = _2256 * _2256;
                    float _2261 = _2259 * _2259;
                    float _2263 = (_2260 + _2261) - 1.0;
                    float _2264 = _2263 * 0.3333333432674407958984375;
                    float _2266 = (_2264 * _2264) * _2264;
                    float _2267 = _2260 * _2261;
                    float _2269 = _2266 + (_2267 * 2.0);
                    float _2270 = _2266 + _2267;
                    float _2272 = _2256 + (_2256 * _2261);
                    float _2343;
                    if (_2270 < 0.0)
                    {
                        float _2317 = acos(_2269 / _2266) * 0.3333333432674407958984375;
                        float _2318 = cos(_2317);
                        float _2320 = sin(_2317) * 1.73205077648162841796875;
                        float _2321 = _2263 * (-0.3333333432674407958984375);
                        float _2326 = sqrt((_2321 * ((_2318 + _2320) + 2.0)) + _2260);
                        float _2331 = sqrt((_2321 * ((_2318 - _2320) + 2.0)) + _2260);
                        _2343 = (((_2331 + (float(int(sign(_2253))) * _2326)) + (abs(_2272) / (_2326 * _2331))) - _2256) * 0.5;
                    }
                    else
                    {
                        float _2280 = ((2.0 * _2256) * _2259) * sqrt(_2270);
                        float _2281 = _2269 + _2280;
                        float _2287 = float(int(sign(_2281))) * pow(abs(_2281), 0.3333333432674407958984375);
                        float _2288 = _2269 - _2280;
                        float _2294 = float(int(sign(_2288))) * pow(abs(_2288), 0.3333333432674407958984375);
                        float _2300 = (((-_2287) - _2294) - (_2263 * 1.33333337306976318359375)) + (2.0 * _2260);
                        float _2302 = (_2287 - _2294) * 1.73205077648162841796875;
                        float _2306 = sqrt((_2300 * _2300) + (_2302 * _2302));
                        _2343 = (((_2302 / sqrt(_2306 - _2300)) + ((2.0 * _2272) / _2306)) - _2256) * 0.5;
                    }
                    float _2348 = _2248.y * sqrt(1.0 - (_2343 * _2343));
                    _2357 = length(vec2(_2248.x * _2343, _2348) - _2247) * float(int(sign(_2247.y - _2348)));
                    break;
                } while(false);
                _2367 = _2357;
                break;
            }
            vec2 _2359 = abs(_1754) - _1757;
            float _2360 = _2359.x;
            float _2361 = _2359.y;
            float _2362 = isnan(_2361) ? _2360 : (isnan(_2360) ? _2361 : max(_2360, _2361));
            bvec2 _2596 = isnan(_2359);
            bvec2 _2597 = isnan(vec2(0.0));
            vec2 _2598 = max(_2359, vec2(0.0));
            vec2 _2599 = vec2(_2596.x ? vec2(0.0).x : _2598.x, _2596.y ? vec2(0.0).y : _2598.y);
            _2367 = (isnan(0.0) ? _2362 : (isnan(_2362) ? 0.0 : min(_2362, 0.0))) + length(vec2(_2597.x ? _2359.x : _2599.x, _2597.y ? _2359.y : _2599.y));
            break;
        } while(false);
        float _2370 = -(_2367 * ((spvWorkaroundRowMajor(Uniforms.Clip[_1708])[2].w != 0.0) ? (-1.0) : 1.0));
        float _2373 = smoothstep(-0.61804687976837158203125, 0.61804687976837158203125, _2370 / fwidth(_2370));
        _1706 = vec4(_1705.xyz * _2373, _1705.w * _2373);
    }
    out_var_SV_Target = _1705;
}

)GLSL";