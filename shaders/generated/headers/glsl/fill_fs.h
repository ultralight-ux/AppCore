// Generated shader header for fill_fs
// Generated from: fill.glsl

#pragma once

static const char* fill_fs_source = R"GLSL(#version 330

vec4 _135;
float _147;

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
    vec4 _1868;
    switch (uint(in_var_COLOR1.x + 0.5))
    {
        case 0u:
        {
            _1868 = in_var_COLOR0;
            break;
        }
        case 1u:
        {
            _1868 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0) * in_var_COLOR0;
            break;
        }
        case 2u:
        {
            vec4 _1862;
            do
            {
                vec2 _1822 = Uniforms.Vector[1].zw + Uniforms.Vector[1].xy;
                vec2 _1838 = fract((((Uniforms.Vector[2].xy * in_var_TEXCOORD1.x) + (Uniforms.Vector[2].zw * in_var_TEXCOORD1.y)) + Uniforms.Vector[3].xy) / _1822) * _1822;
                bool _1848;
                if (!(_1838.x >= Uniforms.Vector[1].z))
                {
                    _1848 = _1838.y >= Uniforms.Vector[1].w;
                }
                else
                {
                    _1848 = true;
                }
                if (_1848)
                {
                    _1862 = vec4(0.0);
                    break;
                }
                _1862 = texture(SPIRV_Cross_CombinedTexture0Sampler0, ((_1838 / Uniforms.Vector[1].zw) * (Uniforms.Vector[0].zw - Uniforms.Vector[0].xy)) + Uniforms.Vector[0].xy) * in_var_COLOR0;
                break;
            } while(false);
            _1868 = _1862;
            break;
        }
        case 3u:
        {
            float _1617 = float(uint(in_var_COLOR1.y + 0.5));
            float _1682;
            if (uint(in_var_COLOR1.z + 0.5) != 0u)
            {
                vec2 _1635 = in_var_TEXCOORD0 - in_var_COLOR2.xy;
                float _1637 = length(_1635 / in_var_COLOR3.zw);
                bool _1644;
                if (in_var_COLOR3.x > 9.9999997473787516355514526367188e-05)
                {
                    _1644 = in_var_COLOR3.y > 9.9999997473787516355514526367188e-05;
                }
                else
                {
                    _1644 = false;
                }
                float _1665;
                if (_1644)
                {
                    float _1650 = length(_1635 / in_var_COLOR3.xy);
                    float _1664;
                    if (_1650 <= 1.0)
                    {
                        _1664 = 0.0;
                    }
                    else
                    {
                        float _1663;
                        if (_1637 >= 1.0)
                        {
                            _1663 = 1.0;
                        }
                        else
                        {
                            _1663 = clamp((_1650 - 1.0) / (_1650 - _1637), 0.0, 1.0);
                        }
                        _1664 = _1663;
                    }
                    _1665 = _1664;
                }
                else
                {
                    _1665 = clamp(_1637, 0.0, 1.0);
                }
                float _1681;
                if (length(in_var_COLOR2.zw - in_var_COLOR2.xy) > 9.9999997473787516355514526367188e-05)
                {
                    _1681 = clamp(_1665 * (1.0 + ((1.0 - dot(normalize(in_var_TEXCOORD0 - in_var_COLOR2.zw), normalize(in_var_COLOR2.xy - in_var_COLOR2.zw))) * 0.5)), 0.0, 1.0);
                }
                else
                {
                    _1681 = _1665;
                }
                _1682 = _1681;
            }
            else
            {
                vec2 _1629 = in_var_COLOR2.zw - in_var_COLOR2.xy;
                _1682 = clamp(dot(in_var_TEXCOORD0 - in_var_COLOR2.xy, _1629) / dot(_1629, _1629), 0.0, 1.0);
            }
            vec4 _1690 = mix(in_var_COLOR5, in_var_COLOR6, vec4(clamp((_1682 - in_var_COLOR4.x) / (in_var_COLOR4.y - in_var_COLOR4.x), 0.0, 1.0)));
            vec4 _1813;
            if (_1617 > 2.0)
            {
                vec4 _1700 = mix(_1690, in_var_COLOR7, vec4(clamp((_1682 - in_var_COLOR4.y) / (in_var_COLOR4.z - in_var_COLOR4.y), 0.0, 1.0)));
                vec4 _1812;
                if (_1617 > 3.0)
                {
                    vec4 _1713 = mix(_1700, Uniforms.Vector[0u], vec4(clamp((_1682 - in_var_COLOR4.z) / (Uniforms.Scalar4[0].x - in_var_COLOR4.z), 0.0, 1.0)));
                    vec4 _1811;
                    if (_1617 > 4.0)
                    {
                        vec4 _1726 = mix(_1713, Uniforms.Vector[1u], vec4(clamp((_1682 - Uniforms.Scalar4[0].x) / (Uniforms.Scalar4[0].y - Uniforms.Scalar4[0].x), 0.0, 1.0)));
                        vec4 _1810;
                        if (_1617 > 5.0)
                        {
                            vec4 _1739 = mix(_1726, Uniforms.Vector[2u], vec4(clamp((_1682 - Uniforms.Scalar4[0].y) / (Uniforms.Scalar4[0].z - Uniforms.Scalar4[0].y), 0.0, 1.0)));
                            vec4 _1809;
                            if (_1617 > 6.0)
                            {
                                vec4 _1752 = mix(_1739, Uniforms.Vector[3u], vec4(clamp((_1682 - Uniforms.Scalar4[0].z) / (Uniforms.Scalar4[0].w - Uniforms.Scalar4[0].z), 0.0, 1.0)));
                                vec4 _1808;
                                if (_1617 > 7.0)
                                {
                                    vec4 _1765 = mix(_1752, Uniforms.Vector[4u], vec4(clamp((_1682 - Uniforms.Scalar4[0].w) / (Uniforms.Scalar4[1].x - Uniforms.Scalar4[0].w), 0.0, 1.0)));
                                    vec4 _1807;
                                    if (_1617 > 8.0)
                                    {
                                        vec4 _1778 = mix(_1765, Uniforms.Vector[5u], vec4(clamp((_1682 - Uniforms.Scalar4[1].x) / (Uniforms.Scalar4[1].y - Uniforms.Scalar4[1].x), 0.0, 1.0)));
                                        vec4 _1806;
                                        if (_1617 > 9.0)
                                        {
                                            vec4 _1791 = mix(_1778, Uniforms.Vector[6u], vec4(clamp((_1682 - Uniforms.Scalar4[1].y) / (Uniforms.Scalar4[1].z - Uniforms.Scalar4[1].y), 0.0, 1.0)));
                                            vec4 _1805;
                                            if (_1617 > 10.0)
                                            {
                                                _1805 = mix(_1791, Uniforms.Vector[7u], vec4(clamp((_1682 - Uniforms.Scalar4[1].z) / (Uniforms.Scalar4[1].w - Uniforms.Scalar4[1].z), 0.0, 1.0)));
                                            }
                                            else
                                            {
                                                _1805 = _1791;
                                            }
                                            _1806 = _1805;
                                        }
                                        else
                                        {
                                            _1806 = _1778;
                                        }
                                        _1807 = _1806;
                                    }
                                    else
                                    {
                                        _1807 = _1765;
                                    }
                                    _1808 = _1807;
                                }
                                else
                                {
                                    _1808 = _1752;
                                }
                                _1809 = _1808;
                            }
                            else
                            {
                                _1809 = _1739;
                            }
                            _1810 = _1809;
                        }
                        else
                        {
                            _1810 = _1726;
                        }
                        _1811 = _1810;
                    }
                    else
                    {
                        _1811 = _1713;
                    }
                    _1812 = _1811;
                }
                else
                {
                    _1812 = _1700;
                }
                _1813 = _1812;
            }
            else
            {
                _1813 = _1690;
            }
            _1868 = _1813;
            break;
        }
        case 7u:
        {
            vec2 _963 = (in_var_TEXCOORD0 - vec2(0.5)) * in_var_COLOR1.zw;
            float _1576;
            do
            {
                vec2 _966 = in_var_COLOR1.zw * 0.5;
                float _967 = _966.x;
                float _968 = -_967;
                float _970 = _968 + in_var_COLOR2.x;
                float _971 = _966.y;
                float _972 = -_971;
                float _974 = _972 + in_var_COLOR3.x;
                vec2 _976 = _963 - vec2(_970, _974);
                bool _983;
                if ((in_var_COLOR2.x * in_var_COLOR3.x) > 0.0)
                {
                    _983 = _963.x < _970;
                }
                else
                {
                    _983 = false;
                }
                bool _988;
                if (_983)
                {
                    _988 = _963.y <= _974;
                }
                else
                {
                    _988 = false;
                }
                if (_988)
                {
                    vec2 _991 = vec2(in_var_COLOR2.x, in_var_COLOR3.x);
                    float _1119;
                    do
                    {
                        if (abs(in_var_COLOR2.x - in_var_COLOR3.x) < 0.100000001490116119384765625)
                        {
                            _1119 = length(_976) - in_var_COLOR2.x;
                            break;
                        }
                        vec2 _1001 = abs(_976);
                        vec2 _1009;
                        vec2 _1010;
                        if (_1001.x > _1001.y)
                        {
                            _1009 = _1001.yx;
                            _1010 = _991.yx;
                        }
                        else
                        {
                            _1009 = _1001;
                            _1010 = _991;
                        }
                        float _1015 = (_1010.y * _1010.y) - (_1010.x * _1010.x);
                        float _1018 = (_1010.x * _1009.x) / _1015;
                        float _1021 = (_1010.y * _1009.y) / _1015;
                        float _1022 = _1018 * _1018;
                        float _1023 = _1021 * _1021;
                        float _1025 = (_1022 + _1023) - 1.0;
                        float _1026 = _1025 * 0.3333333432674407958984375;
                        float _1028 = (_1026 * _1026) * _1026;
                        float _1029 = _1022 * _1023;
                        float _1031 = _1028 + (_1029 * 2.0);
                        float _1032 = _1028 + _1029;
                        float _1034 = _1018 + (_1018 * _1023);
                        float _1105;
                        if (_1032 < 0.0)
                        {
                            float _1079 = acos(_1031 / _1028) * 0.3333333432674407958984375;
                            float _1080 = cos(_1079);
                            float _1082 = sin(_1079) * 1.73205077648162841796875;
                            float _1083 = _1025 * (-0.3333333432674407958984375);
                            float _1088 = sqrt((_1083 * ((_1080 + _1082) + 2.0)) + _1022);
                            float _1093 = sqrt((_1083 * ((_1080 - _1082) + 2.0)) + _1022);
                            _1105 = (((_1093 + (float(int(sign(_1015))) * _1088)) + (abs(_1034) / (_1088 * _1093))) - _1018) * 0.5;
                        }
                        else
                        {
                            float _1042 = ((2.0 * _1018) * _1021) * sqrt(_1032);
                            float _1043 = _1031 + _1042;
                            float _1049 = float(int(sign(_1043))) * pow(abs(_1043), 0.3333333432674407958984375);
                            float _1050 = _1031 - _1042;
                            float _1056 = float(int(sign(_1050))) * pow(abs(_1050), 0.3333333432674407958984375);
                            float _1062 = (((-_1049) - _1056) - (_1025 * 1.33333337306976318359375)) + (2.0 * _1022);
                            float _1064 = (_1049 - _1056) * 1.73205077648162841796875;
                            float _1068 = sqrt((_1062 * _1062) + (_1064 * _1064));
                            _1105 = (((_1064 / sqrt(_1068 - _1062)) + ((2.0 * _1034) / _1068)) - _1018) * 0.5;
                        }
                        float _1110 = _1010.y * sqrt(1.0 - (_1105 * _1105));
                        _1119 = length(vec2(_1010.x * _1105, _1110) - _1009) * float(int(sign(_1009.y - _1110)));
                        break;
                    } while(false);
                    _1576 = _1119;
                    break;
                }
                float _1121 = _967 - in_var_COLOR2.y;
                float _1123 = _972 + in_var_COLOR3.y;
                vec2 _1125 = _963 - vec2(_1121, _1123);
                bool _1132;
                if ((in_var_COLOR2.y * in_var_COLOR3.y) > 0.0)
                {
                    _1132 = _963.x >= _1121;
                }
                else
                {
                    _1132 = false;
                }
                bool _1137;
                if (_1132)
                {
                    _1137 = _963.y <= _1123;
                }
                else
                {
                    _1137 = false;
                }
                if (_1137)
                {
                    vec2 _1140 = vec2(in_var_COLOR2.y, in_var_COLOR3.y);
                    float _1268;
                    do
                    {
                        if (abs(in_var_COLOR2.y - in_var_COLOR3.y) < 0.100000001490116119384765625)
                        {
                            _1268 = length(_1125) - in_var_COLOR2.y;
                            break;
                        }
                        vec2 _1150 = abs(_1125);
                        vec2 _1158;
                        vec2 _1159;
                        if (_1150.x > _1150.y)
                        {
                            _1158 = _1150.yx;
                            _1159 = _1140.yx;
                        }
                        else
                        {
                            _1158 = _1150;
                            _1159 = _1140;
                        }
                        float _1164 = (_1159.y * _1159.y) - (_1159.x * _1159.x);
                        float _1167 = (_1159.x * _1158.x) / _1164;
                        float _1170 = (_1159.y * _1158.y) / _1164;
                        float _1171 = _1167 * _1167;
                        float _1172 = _1170 * _1170;
                        float _1174 = (_1171 + _1172) - 1.0;
                        float _1175 = _1174 * 0.3333333432674407958984375;
                        float _1177 = (_1175 * _1175) * _1175;
                        float _1178 = _1171 * _1172;
                        float _1180 = _1177 + (_1178 * 2.0);
                        float _1181 = _1177 + _1178;
                        float _1183 = _1167 + (_1167 * _1172);
                        float _1254;
                        if (_1181 < 0.0)
                        {
                            float _1228 = acos(_1180 / _1177) * 0.3333333432674407958984375;
                            float _1229 = cos(_1228);
                            float _1231 = sin(_1228) * 1.73205077648162841796875;
                            float _1232 = _1174 * (-0.3333333432674407958984375);
                            float _1237 = sqrt((_1232 * ((_1229 + _1231) + 2.0)) + _1171);
                            float _1242 = sqrt((_1232 * ((_1229 - _1231) + 2.0)) + _1171);
                            _1254 = (((_1242 + (float(int(sign(_1164))) * _1237)) + (abs(_1183) / (_1237 * _1242))) - _1167) * 0.5;
                        }
                        else
                        {
                            float _1191 = ((2.0 * _1167) * _1170) * sqrt(_1181);
                            float _1192 = _1180 + _1191;
                            float _1198 = float(int(sign(_1192))) * pow(abs(_1192), 0.3333333432674407958984375);
                            float _1199 = _1180 - _1191;
                            float _1205 = float(int(sign(_1199))) * pow(abs(_1199), 0.3333333432674407958984375);
                            float _1211 = (((-_1198) - _1205) - (_1174 * 1.33333337306976318359375)) + (2.0 * _1171);
                            float _1213 = (_1198 - _1205) * 1.73205077648162841796875;
                            float _1217 = sqrt((_1211 * _1211) + (_1213 * _1213));
                            _1254 = (((_1213 / sqrt(_1217 - _1211)) + ((2.0 * _1183) / _1217)) - _1167) * 0.5;
                        }
                        float _1259 = _1159.y * sqrt(1.0 - (_1254 * _1254));
                        _1268 = length(vec2(_1159.x * _1254, _1259) - _1158) * float(int(sign(_1158.y - _1259)));
                        break;
                    } while(false);
                    _1576 = _1268;
                    break;
                }
                float _1270 = _967 - in_var_COLOR2.z;
                float _1272 = _971 - in_var_COLOR3.z;
                vec2 _1274 = _963 - vec2(_1270, _1272);
                bool _1281;
                if ((in_var_COLOR2.z * in_var_COLOR3.z) > 0.0)
                {
                    _1281 = _963.x >= _1270;
                }
                else
                {
                    _1281 = false;
                }
                bool _1286;
                if (_1281)
                {
                    _1286 = _963.y >= _1272;
                }
                else
                {
                    _1286 = false;
                }
                if (_1286)
                {
                    vec2 _1289 = vec2(in_var_COLOR2.z, in_var_COLOR3.z);
                    float _1417;
                    do
                    {
                        if (abs(in_var_COLOR2.z - in_var_COLOR3.z) < 0.100000001490116119384765625)
                        {
                            _1417 = length(_1274) - in_var_COLOR2.z;
                            break;
                        }
                        vec2 _1299 = abs(_1274);
                        vec2 _1307;
                        vec2 _1308;
                        if (_1299.x > _1299.y)
                        {
                            _1307 = _1299.yx;
                            _1308 = _1289.yx;
                        }
                        else
                        {
                            _1307 = _1299;
                            _1308 = _1289;
                        }
                        float _1313 = (_1308.y * _1308.y) - (_1308.x * _1308.x);
                        float _1316 = (_1308.x * _1307.x) / _1313;
                        float _1319 = (_1308.y * _1307.y) / _1313;
                        float _1320 = _1316 * _1316;
                        float _1321 = _1319 * _1319;
                        float _1323 = (_1320 + _1321) - 1.0;
                        float _1324 = _1323 * 0.3333333432674407958984375;
                        float _1326 = (_1324 * _1324) * _1324;
                        float _1327 = _1320 * _1321;
                        float _1329 = _1326 + (_1327 * 2.0);
                        float _1330 = _1326 + _1327;
                        float _1332 = _1316 + (_1316 * _1321);
                        float _1403;
                        if (_1330 < 0.0)
                        {
                            float _1377 = acos(_1329 / _1326) * 0.3333333432674407958984375;
                            float _1378 = cos(_1377);
                            float _1380 = sin(_1377) * 1.73205077648162841796875;
                            float _1381 = _1323 * (-0.3333333432674407958984375);
                            float _1386 = sqrt((_1381 * ((_1378 + _1380) + 2.0)) + _1320);
                            float _1391 = sqrt((_1381 * ((_1378 - _1380) + 2.0)) + _1320);
                            _1403 = (((_1391 + (float(int(sign(_1313))) * _1386)) + (abs(_1332) / (_1386 * _1391))) - _1316) * 0.5;
                        }
                        else
                        {
                            float _1340 = ((2.0 * _1316) * _1319) * sqrt(_1330);
                            float _1341 = _1329 + _1340;
                            float _1347 = float(int(sign(_1341))) * pow(abs(_1341), 0.3333333432674407958984375);
                            float _1348 = _1329 - _1340;
                            float _1354 = float(int(sign(_1348))) * pow(abs(_1348), 0.3333333432674407958984375);
                            float _1360 = (((-_1347) - _1354) - (_1323 * 1.33333337306976318359375)) + (2.0 * _1320);
                            float _1362 = (_1347 - _1354) * 1.73205077648162841796875;
                            float _1366 = sqrt((_1360 * _1360) + (_1362 * _1362));
                            _1403 = (((_1362 / sqrt(_1366 - _1360)) + ((2.0 * _1332) / _1366)) - _1316) * 0.5;
                        }
                        float _1408 = _1308.y * sqrt(1.0 - (_1403 * _1403));
                        _1417 = length(vec2(_1308.x * _1403, _1408) - _1307) * float(int(sign(_1307.y - _1408)));
                        break;
                    } while(false);
                    _1576 = _1417;
                    break;
                }
                float _1419 = _968 + in_var_COLOR2.w;
                float _1421 = _971 - in_var_COLOR3.w;
                vec2 _1423 = _963 - vec2(_1419, _1421);
                bool _1430;
                if ((in_var_COLOR2.w * in_var_COLOR3.w) > 0.0)
                {
                    _1430 = _963.x < _1419;
                }
                else
                {
                    _1430 = false;
                }
                bool _1435;
                if (_1430)
                {
                    _1435 = _963.y > _1421;
                }
                else
                {
                    _1435 = false;
                }
                if (_1435)
                {
                    vec2 _1438 = vec2(in_var_COLOR2.w, in_var_COLOR3.w);
                    float _1566;
                    do
                    {
                        if (abs(in_var_COLOR2.w - in_var_COLOR3.w) < 0.100000001490116119384765625)
                        {
                            _1566 = length(_1423) - in_var_COLOR2.w;
                            break;
                        }
                        vec2 _1448 = abs(_1423);
                        vec2 _1456;
                        vec2 _1457;
                        if (_1448.x > _1448.y)
                        {
                            _1456 = _1448.yx;
                            _1457 = _1438.yx;
                        }
                        else
                        {
                            _1456 = _1448;
                            _1457 = _1438;
                        }
                        float _1462 = (_1457.y * _1457.y) - (_1457.x * _1457.x);
                        float _1465 = (_1457.x * _1456.x) / _1462;
                        float _1468 = (_1457.y * _1456.y) / _1462;
                        float _1469 = _1465 * _1465;
                        float _1470 = _1468 * _1468;
                        float _1472 = (_1469 + _1470) - 1.0;
                        float _1473 = _1472 * 0.3333333432674407958984375;
                        float _1475 = (_1473 * _1473) * _1473;
                        float _1476 = _1469 * _1470;
                        float _1478 = _1475 + (_1476 * 2.0);
                        float _1479 = _1475 + _1476;
                        float _1481 = _1465 + (_1465 * _1470);
                        float _1552;
                        if (_1479 < 0.0)
                        {
                            float _1526 = acos(_1478 / _1475) * 0.3333333432674407958984375;
                            float _1527 = cos(_1526);
                            float _1529 = sin(_1526) * 1.73205077648162841796875;
                            float _1530 = _1472 * (-0.3333333432674407958984375);
                            float _1535 = sqrt((_1530 * ((_1527 + _1529) + 2.0)) + _1469);
                            float _1540 = sqrt((_1530 * ((_1527 - _1529) + 2.0)) + _1469);
                            _1552 = (((_1540 + (float(int(sign(_1462))) * _1535)) + (abs(_1481) / (_1535 * _1540))) - _1465) * 0.5;
                        }
                        else
                        {
                            float _1489 = ((2.0 * _1465) * _1468) * sqrt(_1479);
                            float _1490 = _1478 + _1489;
                            float _1496 = float(int(sign(_1490))) * pow(abs(_1490), 0.3333333432674407958984375);
                            float _1497 = _1478 - _1489;
                            float _1503 = float(int(sign(_1497))) * pow(abs(_1497), 0.3333333432674407958984375);
                            float _1509 = (((-_1496) - _1503) - (_1472 * 1.33333337306976318359375)) + (2.0 * _1469);
                            float _1511 = (_1496 - _1503) * 1.73205077648162841796875;
                            float _1515 = sqrt((_1509 * _1509) + (_1511 * _1511));
                            _1552 = (((_1511 / sqrt(_1515 - _1509)) + ((2.0 * _1481) / _1515)) - _1465) * 0.5;
                        }
                        float _1557 = _1457.y * sqrt(1.0 - (_1552 * _1552));
                        _1566 = length(vec2(_1457.x * _1552, _1557) - _1456) * float(int(sign(_1456.y - _1557)));
                        break;
                    } while(false);
                    _1576 = _1566;
                    break;
                }
                vec2 _1568 = abs(_963) - _966;
                float _1569 = _1568.x;
                float _1570 = _1568.y;
                float _1571 = isnan(_1570) ? _1569 : (isnan(_1569) ? _1570 : max(_1569, _1570));
                bvec2 _2561 = isnan(_1568);
                bvec2 _2562 = isnan(vec2(0.0));
                vec2 _2563 = max(_1568, vec2(0.0));
                vec2 _2564 = vec2(_2561.x ? vec2(0.0).x : _2563.x, _2561.y ? vec2(0.0).y : _2563.y);
                _1576 = (isnan(0.0) ? _1571 : (isnan(_1571) ? 0.0 : min(_1571, 0.0))) + length(vec2(_2562.x ? _1568.x : _2564.x, _2562.y ? _1568.y : _2564.y));
                break;
            } while(false);
            float _1577 = -_1576;
            float _1578 = smoothstep(-0.3540000021457672119140625, 0.3540000021457672119140625, _1577);
            float _1580 = _1578 * in_var_COLOR0.w;
            vec4 _1586 = vec4(in_var_COLOR0.xyz * _1580, _1580);
            vec4 _1613;
            if (in_var_COLOR4.x > 0.0)
            {
                float _1594 = 1.0 - smoothstep(in_var_COLOR4.x - 0.3540000021457672119140625, in_var_COLOR4.x + 0.3540000021457672119140625, _1577);
                float _1597 = (isnan(_1594) ? _1578 : (isnan(_1578) ? _1594 : min(_1578, _1594))) * in_var_COLOR5.w;
                float _1606 = 1.0 - _1597;
                vec3 _1608 = vec4(in_var_COLOR5.xyz * _1597, _1597).xyz + (_1586.xyz * _1606);
                vec4 _1609 = vec4(_1608.x, _1608.y, _1608.z, _135.w);
                _1609.w = _1597 + (_1580 * _1606);
                _1613 = _1609;
            }
            else
            {
                _1613 = _1586;
            }
            _1868 = _1613;
            break;
        }
        case 8u:
        {
            _1868 = in_var_COLOR0;
            break;
        }
        case 9u:
        {
            vec4 _960;
            do
            {
                vec4 _339 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0);
                vec4 _340 = _339 * in_var_COLOR0;
                vec4 _344 = texture(SPIRV_Cross_CombinedTexture1Sampler0, in_var_TEXCOORD1);
                vec4 _957;
                bool _958;
                switch (uint(in_var_COLOR1.y + 0.5))
                {
                    case 0u:
                    {
                        _957 = vec4(0.0);
                        _958 = true;
                        break;
                    }
                    case 1u:
                    {
                        _957 = _340;
                        _958 = true;
                        break;
                    }
                    case 2u:
                    {
                        _957 = _340 + (_344 * (1.0 - _340.w));
                        _958 = true;
                        break;
                    }
                    case 3u:
                    {
                        _957 = _340 * _344.w;
                        _958 = true;
                        break;
                    }
                    case 4u:
                    {
                        _957 = _340 * (1.0 - _344.w);
                        _958 = true;
                        break;
                    }
                    case 5u:
                    {
                        _957 = (_340 * _344.w) + (_344 * (1.0 - _340.w));
                        _958 = true;
                        break;
                    }
                    case 6u:
                    {
                        _957 = (_340 * (1.0 - _344.w)) + _344;
                        _958 = true;
                        break;
                    }
                    case 7u:
                    {
                        _957 = _344 * _340.w;
                        _958 = true;
                        break;
                    }
                    case 8u:
                    {
                        _957 = _344 * (1.0 - _340.w);
                        _958 = true;
                        break;
                    }
                    case 9u:
                    {
                        _957 = (_340 * (1.0 - _344.w)) + (_344 * _340.w);
                        _958 = true;
                        break;
                    }
                    case 10u:
                    {
                        _957 = clamp((_340 * (1.0 - _344.w)) + (_344 * (1.0 - _340.w)), vec4(0.0), vec4(1.0));
                        _958 = true;
                        break;
                    }
                    case 11u:
                    {
                        vec3 _908 = _340.xyz;
                        vec3 _909 = _344.xyz;
                        bvec3 _2571 = isnan(_908);
                        bvec3 _2572 = isnan(_909);
                        vec3 _2573 = min(_908, _909);
                        vec3 _2574 = vec3(_2571.x ? _909.x : _2573.x, _2571.y ? _909.y : _2573.y, _2571.z ? _909.z : _2573.z);
                        float _911 = _340.w;
                        _957 = vec4(vec3(_2572.x ? _908.x : _2574.x, _2572.y ? _908.y : _2574.y, _2572.z ? _908.z : _2574.z) * _911, _344.w * _911);
                        _958 = true;
                        break;
                    }
                    case 12u:
                    {
                        _957 = clamp(_340 + _344, vec4(0.0), vec4(1.0));
                        _958 = true;
                        break;
                    }
                    case 13u:
                    {
                        float _898 = _340.w;
                        _957 = vec4(abs(_344.xyz - _340.xyz) * _898, _344.w * _898);
                        _958 = true;
                        break;
                    }
                    case 14u:
                    {
                        float _886 = _340.w;
                        _957 = vec4((_340.xyz * _344.xyz) * _886, _344.w * _886);
                        _958 = true;
                        break;
                    }
                    case 15u:
                    {
                        float _875 = _340.w;
                        _957 = vec4((vec3(1.0) - ((vec3(1.0) - _344.xyz) * (vec3(1.0) - _340.xyz))) * _875, _344.w * _875);
                        _958 = true;
                        break;
                    }
                    case 16u:
                    {
                        vec3 _154 = _340.xyz;
                        vec3 _155 = _344.xyz;
                        vec3 _149;
                        for (uint _833 = 0u; _833 < 3u; _833++)
                        {
                            float _858;
                            if (_155[_833] < 0.5)
                            {
                                _858 = (2.0 * _155[_833]) * _154[_833];
                            }
                            else
                            {
                                _858 = 1.0 - ((2.0 * (1.0 - _155[_833])) * (1.0 - _154[_833]));
                            }
                            _149[_833] = _858;
                        }
                        float _861 = _340.w;
                        _957 = vec4(_149 * _861, _344.w * _861);
                        _958 = true;
                        break;
                    }
                    case 17u:
                    {
                        vec3 _819 = _340.xyz;
                        vec3 _820 = _344.xyz;
                        bvec3 _2576 = isnan(_819);
                        bvec3 _2577 = isnan(_820);
                        vec3 _2578 = max(_819, _820);
                        vec3 _2579 = vec3(_2576.x ? _820.x : _2578.x, _2576.y ? _820.y : _2578.y, _2576.z ? _820.z : _2578.z);
                        float _822 = _340.w;
                        _957 = vec4(vec3(_2577.x ? _819.x : _2579.x, _2577.y ? _819.y : _2579.y, _2577.z ? _819.z : _2579.z) * _822, _344.w * _822);
                        _958 = true;
                        break;
                    }
                    case 18u:
                    {
                        vec3 _156 = _340.xyz;
                        vec3 _157 = _344.xyz;
                        vec3 _150;
                        for (uint _789 = 0u; _789 < 3u; _789++)
                        {
                            float _808;
                            if (_156[_789] == 1.0)
                            {
                                _808 = _156[_789];
                            }
                            else
                            {
                                float _805 = _157[_789] / (1.0 - _156[_789]);
                                _808 = isnan(1.0) ? _805 : (isnan(_805) ? 1.0 : min(_805, 1.0));
                            }
                            _150[_789] = _808;
                        }
                        float _811 = _340.w;
                        _957 = vec4(_150 * _811, _344.w * _811);
                        _958 = true;
                        break;
                    }
                    case 19u:
                    {
                        vec3 _158 = _340.xyz;
                        vec3 _159 = _344.xyz;
                        vec3 _151;
                        for (uint _755 = 0u; _755 < 3u; _755++)
                        {
                            float _775;
                            if (_158[_755] == 0.0)
                            {
                                _775 = _158[_755];
                            }
                            else
                            {
                                float _772 = 1.0 - ((1.0 - _159[_755]) / _158[_755]);
                                _775 = isnan(0.0) ? _772 : (isnan(_772) ? 0.0 : max(_772, 0.0));
                            }
                            _151[_755] = _775;
                        }
                        float _778 = _340.w;
                        _957 = vec4(_151 * _778, _344.w * _778);
                        _958 = true;
                        break;
                    }
                    case 20u:
                    {
                        vec3 _160 = _344.xyz;
                        vec3 _161 = _340.xyz;
                        vec3 _152;
                        for (uint _716 = 0u; _716 < 3u; _716++)
                        {
                            float _741;
                            if (_161[_716] < 0.5)
                            {
                                _741 = (2.0 * _161[_716]) * _160[_716];
                            }
                            else
                            {
                                _741 = 1.0 - ((2.0 * (1.0 - _161[_716])) * (1.0 - _160[_716]));
                            }
                            _152[_716] = _741;
                        }
                        float _744 = _340.w;
                        _957 = vec4(_152 * _744, _344.w * _744);
                        _958 = true;
                        break;
                    }
                    case 21u:
                    {
                        vec3 _162 = _340.xyz;
                        vec3 _163 = _344.xyz;
                        vec3 _153;
                        for (uint _664 = 0u; _664 < 3u; _664++)
                        {
                            float _702;
                            if (_162[_664] < 0.5)
                            {
                                _702 = ((2.0 * _163[_664]) * _162[_664]) + ((_163[_664] * _163[_664]) * (1.0 - (2.0 * _162[_664])));
                            }
                            else
                            {
                                _702 = (sqrt(_163[_664]) * ((2.0 * _162[_664]) - 1.0)) + ((2.0 * _163[_664]) * (1.0 - _162[_664]));
                            }
                            _153[_664] = _702;
                        }
                        float _705 = _340.w;
                        _957 = vec4(_153 * _705, _344.w * _705);
                        _958 = true;
                        break;
                    }
                    case 22u:
                    {
                        vec3 _647 = _344.xyz;
                        vec3 _648 = _340.xyz;
                        float _653 = _340.w;
                        _957 = vec4(((_647 + _648) - ((_647 * 2.0) * _648)) * _653, _344.w * _653);
                        _958 = true;
                        break;
                    }
                    case 23u:
                    {
                        vec3 _581 = _340.xyz;
                        float _582 = _344.x;
                        float _583 = _344.y;
                        float _584 = _344.z;
                        float _585 = isnan(_584) ? _583 : (isnan(_583) ? _584 : min(_583, _584));
                        float _586 = isnan(_585) ? _582 : (isnan(_582) ? _585 : min(_582, _585));
                        float _587 = isnan(_584) ? _583 : (isnan(_583) ? _584 : max(_583, _584));
                        float _588 = isnan(_587) ? _582 : (isnan(_582) ? _587 : max(_582, _587));
                        float _590 = _586 + _588;
                        float _592 = abs(_590 - 1.0);
                        float _596 = _340.x;
                        float _597 = _340.y;
                        float _598 = _340.z;
                        float _599 = isnan(_598) ? _597 : (isnan(_597) ? _598 : min(_597, _598));
                        float _601 = isnan(_598) ? _597 : (isnan(_597) ? _598 : max(_597, _598));
                        vec3 _617 = ((step(_340.yxx, _581) * step(_340.zzy, _581)) * (vec3(0.0, 2.0, 4.0) + ((_340.yzx - _340.zxy) / vec3(((isnan(_601) ? _596 : (isnan(_596) ? _601 : max(_596, _601))) - (isnan(_599) ? _596 : (isnan(_596) ? _599 : min(_596, _599)))) + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        vec3 _627 = vec3(fract(((1.0 + _617.x) + _617.y) + _617.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _639 = _340.w;
                        _957 = vec4((vec3(_590 * 0.5) + (((clamp(abs((_627 - vec3(6.0) * trunc(_627 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * ((_588 - _586) / (1.00000011920928955078125 - _592))) * (1.0 - _592))) * _639, _344.w * _639);
                        _958 = true;
                        break;
                    }
                    case 24u:
                    {
                        vec3 _512 = _344.xyz;
                        float _513 = _344.x;
                        float _514 = _344.y;
                        float _515 = _344.z;
                        float _516 = isnan(_515) ? _514 : (isnan(_514) ? _515 : min(_514, _515));
                        float _517 = isnan(_516) ? _513 : (isnan(_513) ? _516 : min(_513, _516));
                        float _518 = isnan(_515) ? _514 : (isnan(_514) ? _515 : max(_514, _515));
                        float _519 = isnan(_518) ? _513 : (isnan(_513) ? _518 : max(_513, _518));
                        vec3 _534 = ((step(_344.yxx, _512) * step(_344.zzy, _512)) * (vec3(0.0, 2.0, 4.0) + ((_344.yzx - _344.zxy) / vec3((_519 - _517) + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        float _542 = _517 + _519;
                        float _544 = _340.x;
                        float _545 = _340.y;
                        float _546 = _340.z;
                        float _547 = isnan(_546) ? _545 : (isnan(_545) ? _546 : min(_545, _546));
                        float _548 = isnan(_547) ? _544 : (isnan(_544) ? _547 : min(_544, _547));
                        float _549 = isnan(_546) ? _545 : (isnan(_545) ? _546 : max(_545, _546));
                        float _550 = isnan(_549) ? _544 : (isnan(_544) ? _549 : max(_544, _549));
                        vec3 _559 = vec3(fract(((1.0 + _534.x) + _534.y) + _534.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _573 = _340.w;
                        _957 = vec4((vec3(_542 * 0.5) + (((clamp(abs((_559 - vec3(6.0) * trunc(_559 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * ((_550 - _548) / (1.00000011920928955078125 - abs((_548 + _550) - 1.0)))) * (1.0 - abs(_542 - 1.0)))) * _573, _344.w * _573);
                        _958 = true;
                        break;
                    }
                    case 25u:
                    {
                        vec3 _444 = _340.xyz;
                        float _445 = _340.x;
                        float _446 = _340.y;
                        float _447 = _340.z;
                        float _448 = isnan(_447) ? _446 : (isnan(_446) ? _447 : min(_446, _447));
                        float _449 = isnan(_448) ? _445 : (isnan(_445) ? _448 : min(_445, _448));
                        float _450 = isnan(_447) ? _446 : (isnan(_446) ? _447 : max(_446, _447));
                        float _451 = isnan(_450) ? _445 : (isnan(_445) ? _450 : max(_445, _450));
                        float _460 = _451 - _449;
                        vec3 _466 = ((step(_340.yxx, _444) * step(_340.zzy, _444)) * (vec3(0.0, 2.0, 4.0) + ((_340.yzx - _340.zxy) / vec3(_460 + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        float _479 = _344.x;
                        float _480 = _344.y;
                        float _481 = _344.z;
                        float _482 = isnan(_481) ? _480 : (isnan(_480) ? _481 : min(_480, _481));
                        float _484 = isnan(_481) ? _480 : (isnan(_480) ? _481 : max(_480, _481));
                        float _486 = (isnan(_482) ? _479 : (isnan(_479) ? _482 : min(_479, _482))) + (isnan(_484) ? _479 : (isnan(_479) ? _484 : max(_479, _484)));
                        vec3 _490 = vec3(fract(((1.0 + _466.x) + _466.y) + _466.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _504 = _340.w;
                        _957 = vec4((vec3(_486 * 0.5) + (((clamp(abs((_490 - vec3(6.0) * trunc(_490 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * (_460 / (1.00000011920928955078125 - abs((_449 + _451) - 1.0)))) * (1.0 - abs(_486 - 1.0)))) * _504, _344.w * _504);
                        _958 = true;
                        break;
                    }
                    case 26u:
                    {
                        vec3 _376 = _344.xyz;
                        float _377 = _344.x;
                        float _378 = _344.y;
                        float _379 = _344.z;
                        float _380 = isnan(_379) ? _378 : (isnan(_378) ? _379 : min(_378, _379));
                        float _381 = isnan(_380) ? _377 : (isnan(_377) ? _380 : min(_377, _380));
                        float _382 = isnan(_379) ? _378 : (isnan(_378) ? _379 : max(_378, _379));
                        float _383 = isnan(_382) ? _377 : (isnan(_377) ? _382 : max(_377, _382));
                        float _392 = _383 - _381;
                        vec3 _398 = ((step(_344.yxx, _376) * step(_344.zzy, _376)) * (vec3(0.0, 2.0, 4.0) + ((_344.yzx - _344.zxy) / vec3(_392 + 1.0000000116860974230803549289703e-07)))) * vec3(0.16666667163372039794921875);
                        float _411 = _340.x;
                        float _412 = _340.y;
                        float _413 = _340.z;
                        float _414 = isnan(_413) ? _412 : (isnan(_412) ? _413 : min(_412, _413));
                        float _416 = isnan(_413) ? _412 : (isnan(_412) ? _413 : max(_412, _413));
                        float _418 = (isnan(_414) ? _411 : (isnan(_411) ? _414 : min(_411, _414))) + (isnan(_416) ? _411 : (isnan(_411) ? _416 : max(_411, _416)));
                        vec3 _422 = vec3(fract(((1.0 + _398.x) + _398.y) + _398.z) * 6.0) + vec3(0.0, 4.0, 2.0);
                        float _436 = _340.w;
                        _957 = vec4((vec3(_418 * 0.5) + (((clamp(abs((_422 - vec3(6.0) * trunc(_422 / vec3(6.0))) - vec3(3.0)) - vec3(1.0), vec3(0.0), vec3(1.0)) - vec3(0.5)) * (_392 / (1.00000011920928955078125 - abs((_381 + _383) - 1.0)))) * (1.0 - abs(_418 - 1.0)))) * _436, _344.w * _436);
                        _958 = true;
                        break;
                    }
                    default:
                    {
                        _957 = _135;
                        _958 = false;
                        break;
                    }
                }
                if (_958)
                {
                    _960 = _957;
                    break;
                }
                _960 = _340;
                break;
            } while(false);
            _1868 = _960;
            break;
        }
        case 10u:
        {
            vec4 _320 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0) * in_var_COLOR0;
            vec4 _324 = texture(SPIRV_Cross_CombinedTexture1Sampler0, in_var_TEXCOORD1);
            float _325 = _324.w;
            _1868 = vec4(_320.xyz * _325, _320.w * _325);
            break;
        }
        case 11u:
        {
            vec4 _190 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0);
            float _191 = _190.w;
            float _307;
            do
            {
                if (_191 < 0.866666972637176513671875)
                {
                    float _244 = _191 * _191;
                    float _245 = _244 * _191;
                    float _246 = _245 * _191;
                    float _247 = in_var_COLOR1.y * in_var_COLOR1.y;
                    float _248 = _247 * in_var_COLOR1.y;
                    float _249 = _248 * in_var_COLOR1.y;
                    float _306 = clamp(_191 + ((_191 * (1.0 - _191)) * (((((((((((((((((((((-0.529514312744140625) + (0.672933638095855712890625 * in_var_COLOR1.y)) - (0.997008144855499267578125 * _191)) - (6.97618961334228515625 * _247)) - ((1.241634368896484375 * _191) * in_var_COLOR1.y)) + (8.015369415283203125 * _244)) + (52.680553436279296875 * _248)) - ((23.9237995147705078125 * _191) * _247)) + ((26.8822956085205078125 * _244) * in_var_COLOR1.y)) - (31.4318714141845703125 * _245)) - (72.309783935546875 * _249)) + ((7.179834842681884765625 * _191) * _248)) + ((22.1490955352783203125 * _244) * _247)) - ((44.219791412353515625 * _245) * in_var_COLOR1.y)) + (45.147449493408203125 * _246)) + (29.235294342041015625 * pow(in_var_COLOR1.y, 5.0))) + ((3.7994887828826904296875 * _191) * _249)) - ((5.774257659912109375 * _244) * _248)) - ((12.09290027618408203125 * _245) * _247)) + ((27.816440582275390625 * _246) * in_var_COLOR1.y)) - (23.56078338623046875 * pow(_191, 5.0)))), 0.0, 1.0);
                    _307 = _306;
                    break;
                }
                else
                {
                    float _199 = _191 * _191;
                    float _200 = _199 * _191;
                    float _202 = in_var_COLOR1.y * in_var_COLOR1.y;
                    float _203 = _202 * in_var_COLOR1.y;
                    _307 = clamp(_191 + ((_191 * (1.0 - _191)) * ((((((((((((((359.744049072265625 - (192.8896636962890625 * in_var_COLOR1.y)) - (856.576904296875 * _191)) - (309.42547607421875 * _202)) + ((1099.493896484375 * _191) * in_var_COLOR1.y)) + (75.8802642822265625 * _199)) - (82.2700958251953125 * _203)) + ((837.44329833984375 * _191) * _202)) - ((1796.80322265625 * _199) * in_var_COLOR1.y)) + (1033.9127197265625 * _200)) - (2.5168631076812744140625 * (_203 * in_var_COLOR1.y))) + ((98.20291900634765625 * _191) * _203)) - ((559.15911865234375 * _199) * _202)) + ((914.3739013671875 * _200) * in_var_COLOR1.y)) - (619.1995849609375 * (_200 * _191)))), 0.0, 1.0);
                    break;
                }
                break; // unreachable workaround
            } while(false);
            float _309 = _307 * in_var_COLOR0.w;
            _1868 = vec4(in_var_COLOR0.xyz * _309, _309);
            break;
        }
        default:
        {
            _1868 = in_var_COLOR0;
            break;
        }
    }
    vec4 _1870;
    _1870 = _1868;
    vec4 _1871;
    for (uint _1873 = 0u; _1873 < uint(Uniforms.ClipData.x); _1870 = _1871, _1873++)
    {
        vec4 _1887 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_1873])[0].x, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[1].x, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[2].x, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[3].x);
        vec4 _1894 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_1873])[0].y, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[1].y, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[2].y, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[3].y);
        vec4 _1896 = floor(_1894 * vec4(1.52587890625e-05));
        vec4 _1899 = floor(_1894 - (_1896 * 65536.0));
        vec4 _1909 = vec4(spvWorkaroundRowMajor(Uniforms.Clip[_1873])[0].z, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[1].z, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[2].z, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[3].z);
        vec2 _1919 = (((_1909.xy * in_var_TEXCOORD1.x) + (_1909.zw * in_var_TEXCOORD1.y)) + vec4(spvWorkaroundRowMajor(Uniforms.Clip[_1873])[0].w, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[1].w, spvWorkaroundRowMajor(Uniforms.Clip[_1873])[2].w, _147).xy) - _1887.xy;
        float _2532;
        do
        {
            vec2 _1922 = _1887.zw * 0.5;
            float _1923 = _1922.x;
            float _1924 = -_1923;
            float _1925 = _1896.x;
            float _1926 = _1924 + _1925;
            float _1927 = _1922.y;
            float _1928 = -_1927;
            float _1929 = _1899.x;
            float _1930 = _1928 + _1929;
            vec2 _1932 = _1919 - vec2(_1926, _1930);
            bool _1939;
            if ((_1925 * _1929) > 0.0)
            {
                _1939 = _1919.x < _1926;
            }
            else
            {
                _1939 = false;
            }
            bool _1944;
            if (_1939)
            {
                _1944 = _1919.y <= _1930;
            }
            else
            {
                _1944 = false;
            }
            if (_1944)
            {
                vec2 _1947 = vec2(_1925, _1929);
                float _2075;
                do
                {
                    if (abs(_1925 - _1929) < 0.100000001490116119384765625)
                    {
                        _2075 = length(_1932) - _1925;
                        break;
                    }
                    vec2 _1957 = abs(_1932);
                    vec2 _1965;
                    vec2 _1966;
                    if (_1957.x > _1957.y)
                    {
                        _1965 = _1957.yx;
                        _1966 = _1947.yx;
                    }
                    else
                    {
                        _1965 = _1957;
                        _1966 = _1947;
                    }
                    float _1971 = (_1966.y * _1966.y) - (_1966.x * _1966.x);
                    float _1974 = (_1966.x * _1965.x) / _1971;
                    float _1977 = (_1966.y * _1965.y) / _1971;
                    float _1978 = _1974 * _1974;
                    float _1979 = _1977 * _1977;
                    float _1981 = (_1978 + _1979) - 1.0;
                    float _1982 = _1981 * 0.3333333432674407958984375;
                    float _1984 = (_1982 * _1982) * _1982;
                    float _1985 = _1978 * _1979;
                    float _1987 = _1984 + (_1985 * 2.0);
                    float _1988 = _1984 + _1985;
                    float _1990 = _1974 + (_1974 * _1979);
                    float _2061;
                    if (_1988 < 0.0)
                    {
                        float _2035 = acos(_1987 / _1984) * 0.3333333432674407958984375;
                        float _2036 = cos(_2035);
                        float _2038 = sin(_2035) * 1.73205077648162841796875;
                        float _2039 = _1981 * (-0.3333333432674407958984375);
                        float _2044 = sqrt((_2039 * ((_2036 + _2038) + 2.0)) + _1978);
                        float _2049 = sqrt((_2039 * ((_2036 - _2038) + 2.0)) + _1978);
                        _2061 = (((_2049 + (float(int(sign(_1971))) * _2044)) + (abs(_1990) / (_2044 * _2049))) - _1974) * 0.5;
                    }
                    else
                    {
                        float _1998 = ((2.0 * _1974) * _1977) * sqrt(_1988);
                        float _1999 = _1987 + _1998;
                        float _2005 = float(int(sign(_1999))) * pow(abs(_1999), 0.3333333432674407958984375);
                        float _2006 = _1987 - _1998;
                        float _2012 = float(int(sign(_2006))) * pow(abs(_2006), 0.3333333432674407958984375);
                        float _2018 = (((-_2005) - _2012) - (_1981 * 1.33333337306976318359375)) + (2.0 * _1978);
                        float _2020 = (_2005 - _2012) * 1.73205077648162841796875;
                        float _2024 = sqrt((_2018 * _2018) + (_2020 * _2020));
                        _2061 = (((_2020 / sqrt(_2024 - _2018)) + ((2.0 * _1990) / _2024)) - _1974) * 0.5;
                    }
                    float _2066 = _1966.y * sqrt(1.0 - (_2061 * _2061));
                    _2075 = length(vec2(_1966.x * _2061, _2066) - _1965) * float(int(sign(_1965.y - _2066)));
                    break;
                } while(false);
                _2532 = _2075;
                break;
            }
            float _2076 = _1896.y;
            float _2077 = _1923 - _2076;
            float _2078 = _1899.y;
            float _2079 = _1928 + _2078;
            vec2 _2081 = _1919 - vec2(_2077, _2079);
            bool _2088;
            if ((_2076 * _2078) > 0.0)
            {
                _2088 = _1919.x >= _2077;
            }
            else
            {
                _2088 = false;
            }
            bool _2093;
            if (_2088)
            {
                _2093 = _1919.y <= _2079;
            }
            else
            {
                _2093 = false;
            }
            if (_2093)
            {
                vec2 _2096 = vec2(_2076, _2078);
                float _2224;
                do
                {
                    if (abs(_2076 - _2078) < 0.100000001490116119384765625)
                    {
                        _2224 = length(_2081) - _2076;
                        break;
                    }
                    vec2 _2106 = abs(_2081);
                    vec2 _2114;
                    vec2 _2115;
                    if (_2106.x > _2106.y)
                    {
                        _2114 = _2106.yx;
                        _2115 = _2096.yx;
                    }
                    else
                    {
                        _2114 = _2106;
                        _2115 = _2096;
                    }
                    float _2120 = (_2115.y * _2115.y) - (_2115.x * _2115.x);
                    float _2123 = (_2115.x * _2114.x) / _2120;
                    float _2126 = (_2115.y * _2114.y) / _2120;
                    float _2127 = _2123 * _2123;
                    float _2128 = _2126 * _2126;
                    float _2130 = (_2127 + _2128) - 1.0;
                    float _2131 = _2130 * 0.3333333432674407958984375;
                    float _2133 = (_2131 * _2131) * _2131;
                    float _2134 = _2127 * _2128;
                    float _2136 = _2133 + (_2134 * 2.0);
                    float _2137 = _2133 + _2134;
                    float _2139 = _2123 + (_2123 * _2128);
                    float _2210;
                    if (_2137 < 0.0)
                    {
                        float _2184 = acos(_2136 / _2133) * 0.3333333432674407958984375;
                        float _2185 = cos(_2184);
                        float _2187 = sin(_2184) * 1.73205077648162841796875;
                        float _2188 = _2130 * (-0.3333333432674407958984375);
                        float _2193 = sqrt((_2188 * ((_2185 + _2187) + 2.0)) + _2127);
                        float _2198 = sqrt((_2188 * ((_2185 - _2187) + 2.0)) + _2127);
                        _2210 = (((_2198 + (float(int(sign(_2120))) * _2193)) + (abs(_2139) / (_2193 * _2198))) - _2123) * 0.5;
                    }
                    else
                    {
                        float _2147 = ((2.0 * _2123) * _2126) * sqrt(_2137);
                        float _2148 = _2136 + _2147;
                        float _2154 = float(int(sign(_2148))) * pow(abs(_2148), 0.3333333432674407958984375);
                        float _2155 = _2136 - _2147;
                        float _2161 = float(int(sign(_2155))) * pow(abs(_2155), 0.3333333432674407958984375);
                        float _2167 = (((-_2154) - _2161) - (_2130 * 1.33333337306976318359375)) + (2.0 * _2127);
                        float _2169 = (_2154 - _2161) * 1.73205077648162841796875;
                        float _2173 = sqrt((_2167 * _2167) + (_2169 * _2169));
                        _2210 = (((_2169 / sqrt(_2173 - _2167)) + ((2.0 * _2139) / _2173)) - _2123) * 0.5;
                    }
                    float _2215 = _2115.y * sqrt(1.0 - (_2210 * _2210));
                    _2224 = length(vec2(_2115.x * _2210, _2215) - _2114) * float(int(sign(_2114.y - _2215)));
                    break;
                } while(false);
                _2532 = _2224;
                break;
            }
            float _2225 = _1896.z;
            float _2226 = _1923 - _2225;
            float _2227 = _1899.z;
            float _2228 = _1927 - _2227;
            vec2 _2230 = _1919 - vec2(_2226, _2228);
            bool _2237;
            if ((_2225 * _2227) > 0.0)
            {
                _2237 = _1919.x >= _2226;
            }
            else
            {
                _2237 = false;
            }
            bool _2242;
            if (_2237)
            {
                _2242 = _1919.y >= _2228;
            }
            else
            {
                _2242 = false;
            }
            if (_2242)
            {
                vec2 _2245 = vec2(_2225, _2227);
                float _2373;
                do
                {
                    if (abs(_2225 - _2227) < 0.100000001490116119384765625)
                    {
                        _2373 = length(_2230) - _2225;
                        break;
                    }
                    vec2 _2255 = abs(_2230);
                    vec2 _2263;
                    vec2 _2264;
                    if (_2255.x > _2255.y)
                    {
                        _2263 = _2255.yx;
                        _2264 = _2245.yx;
                    }
                    else
                    {
                        _2263 = _2255;
                        _2264 = _2245;
                    }
                    float _2269 = (_2264.y * _2264.y) - (_2264.x * _2264.x);
                    float _2272 = (_2264.x * _2263.x) / _2269;
                    float _2275 = (_2264.y * _2263.y) / _2269;
                    float _2276 = _2272 * _2272;
                    float _2277 = _2275 * _2275;
                    float _2279 = (_2276 + _2277) - 1.0;
                    float _2280 = _2279 * 0.3333333432674407958984375;
                    float _2282 = (_2280 * _2280) * _2280;
                    float _2283 = _2276 * _2277;
                    float _2285 = _2282 + (_2283 * 2.0);
                    float _2286 = _2282 + _2283;
                    float _2288 = _2272 + (_2272 * _2277);
                    float _2359;
                    if (_2286 < 0.0)
                    {
                        float _2333 = acos(_2285 / _2282) * 0.3333333432674407958984375;
                        float _2334 = cos(_2333);
                        float _2336 = sin(_2333) * 1.73205077648162841796875;
                        float _2337 = _2279 * (-0.3333333432674407958984375);
                        float _2342 = sqrt((_2337 * ((_2334 + _2336) + 2.0)) + _2276);
                        float _2347 = sqrt((_2337 * ((_2334 - _2336) + 2.0)) + _2276);
                        _2359 = (((_2347 + (float(int(sign(_2269))) * _2342)) + (abs(_2288) / (_2342 * _2347))) - _2272) * 0.5;
                    }
                    else
                    {
                        float _2296 = ((2.0 * _2272) * _2275) * sqrt(_2286);
                        float _2297 = _2285 + _2296;
                        float _2303 = float(int(sign(_2297))) * pow(abs(_2297), 0.3333333432674407958984375);
                        float _2304 = _2285 - _2296;
                        float _2310 = float(int(sign(_2304))) * pow(abs(_2304), 0.3333333432674407958984375);
                        float _2316 = (((-_2303) - _2310) - (_2279 * 1.33333337306976318359375)) + (2.0 * _2276);
                        float _2318 = (_2303 - _2310) * 1.73205077648162841796875;
                        float _2322 = sqrt((_2316 * _2316) + (_2318 * _2318));
                        _2359 = (((_2318 / sqrt(_2322 - _2316)) + ((2.0 * _2288) / _2322)) - _2272) * 0.5;
                    }
                    float _2364 = _2264.y * sqrt(1.0 - (_2359 * _2359));
                    _2373 = length(vec2(_2264.x * _2359, _2364) - _2263) * float(int(sign(_2263.y - _2364)));
                    break;
                } while(false);
                _2532 = _2373;
                break;
            }
            float _2374 = _1896.w;
            float _2375 = _1924 + _2374;
            float _2376 = _1899.w;
            float _2377 = _1927 - _2376;
            vec2 _2379 = _1919 - vec2(_2375, _2377);
            bool _2386;
            if ((_2374 * _2376) > 0.0)
            {
                _2386 = _1919.x < _2375;
            }
            else
            {
                _2386 = false;
            }
            bool _2391;
            if (_2386)
            {
                _2391 = _1919.y > _2377;
            }
            else
            {
                _2391 = false;
            }
            if (_2391)
            {
                vec2 _2394 = vec2(_2374, _2376);
                float _2522;
                do
                {
                    if (abs(_2374 - _2376) < 0.100000001490116119384765625)
                    {
                        _2522 = length(_2379) - _2374;
                        break;
                    }
                    vec2 _2404 = abs(_2379);
                    vec2 _2412;
                    vec2 _2413;
                    if (_2404.x > _2404.y)
                    {
                        _2412 = _2404.yx;
                        _2413 = _2394.yx;
                    }
                    else
                    {
                        _2412 = _2404;
                        _2413 = _2394;
                    }
                    float _2418 = (_2413.y * _2413.y) - (_2413.x * _2413.x);
                    float _2421 = (_2413.x * _2412.x) / _2418;
                    float _2424 = (_2413.y * _2412.y) / _2418;
                    float _2425 = _2421 * _2421;
                    float _2426 = _2424 * _2424;
                    float _2428 = (_2425 + _2426) - 1.0;
                    float _2429 = _2428 * 0.3333333432674407958984375;
                    float _2431 = (_2429 * _2429) * _2429;
                    float _2432 = _2425 * _2426;
                    float _2434 = _2431 + (_2432 * 2.0);
                    float _2435 = _2431 + _2432;
                    float _2437 = _2421 + (_2421 * _2426);
                    float _2508;
                    if (_2435 < 0.0)
                    {
                        float _2482 = acos(_2434 / _2431) * 0.3333333432674407958984375;
                        float _2483 = cos(_2482);
                        float _2485 = sin(_2482) * 1.73205077648162841796875;
                        float _2486 = _2428 * (-0.3333333432674407958984375);
                        float _2491 = sqrt((_2486 * ((_2483 + _2485) + 2.0)) + _2425);
                        float _2496 = sqrt((_2486 * ((_2483 - _2485) + 2.0)) + _2425);
                        _2508 = (((_2496 + (float(int(sign(_2418))) * _2491)) + (abs(_2437) / (_2491 * _2496))) - _2421) * 0.5;
                    }
                    else
                    {
                        float _2445 = ((2.0 * _2421) * _2424) * sqrt(_2435);
                        float _2446 = _2434 + _2445;
                        float _2452 = float(int(sign(_2446))) * pow(abs(_2446), 0.3333333432674407958984375);
                        float _2453 = _2434 - _2445;
                        float _2459 = float(int(sign(_2453))) * pow(abs(_2453), 0.3333333432674407958984375);
                        float _2465 = (((-_2452) - _2459) - (_2428 * 1.33333337306976318359375)) + (2.0 * _2425);
                        float _2467 = (_2452 - _2459) * 1.73205077648162841796875;
                        float _2471 = sqrt((_2465 * _2465) + (_2467 * _2467));
                        _2508 = (((_2467 / sqrt(_2471 - _2465)) + ((2.0 * _2437) / _2471)) - _2421) * 0.5;
                    }
                    float _2513 = _2413.y * sqrt(1.0 - (_2508 * _2508));
                    _2522 = length(vec2(_2413.x * _2508, _2513) - _2412) * float(int(sign(_2412.y - _2513)));
                    break;
                } while(false);
                _2532 = _2522;
                break;
            }
            vec2 _2524 = abs(_1919) - _1922;
            float _2525 = _2524.x;
            float _2526 = _2524.y;
            float _2527 = isnan(_2526) ? _2525 : (isnan(_2525) ? _2526 : max(_2525, _2526));
            bvec2 _2761 = isnan(_2524);
            bvec2 _2762 = isnan(vec2(0.0));
            vec2 _2763 = max(_2524, vec2(0.0));
            vec2 _2764 = vec2(_2761.x ? vec2(0.0).x : _2763.x, _2761.y ? vec2(0.0).y : _2763.y);
            _2532 = (isnan(0.0) ? _2527 : (isnan(_2527) ? 0.0 : min(_2527, 0.0))) + length(vec2(_2762.x ? _2524.x : _2764.x, _2762.y ? _2524.y : _2764.y));
            break;
        } while(false);
        float _2535 = -(_2532 * ((spvWorkaroundRowMajor(Uniforms.Clip[_1873])[2].w != 0.0) ? (-1.0) : 1.0));
        float _2538 = smoothstep(-0.61804687976837158203125, 0.61804687976837158203125, _2535 / fwidth(_2535));
        _1871 = vec4(_1870.xyz * _2538, _1870.w * _2538);
    }
    out_var_SV_Target = _1870;
}

)GLSL";