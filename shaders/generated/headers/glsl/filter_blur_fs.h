// Generated shader header for filter_blur_fs
// Generated from: filter_blur.glsl

#pragma once

static const char* filter_blur_fs_source = R"GLSL(#version 420

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

layout(binding = 0) uniform sampler2D SPIRV_Cross_CombinedTexture0Sampler0;

layout(location = 0) in vec4 in_var_COLOR0;
layout(location = 1) in vec2 in_var_TEXCOORD0;
layout(location = 0) out vec4 out_var_SV_Target;

void main()
{
    vec4 _1431;
    if (Uniforms.Integer4[0].y != 0)
    {
        vec2 _743 = vec2(Uniforms.Scalar4[0].x, Uniforms.Scalar4[0].y);
        vec2 _748 = vec2(Uniforms.Scalar4[0].z, Uniforms.Scalar4[0].w);
        float _1429;
        do
        {
            bool _759;
            if (Uniforms.Scalar4[0].x < 0.001000000047497451305389404296875)
            {
                _759 = Uniforms.Scalar4[0].y < 0.001000000047497451305389404296875;
            }
            else
            {
                _759 = false;
            }
            if (_759)
            {
                vec2 _762 = in_var_TEXCOORD0 + _748;
                float _763 = _762.x;
                bool _769;
                if (_763 >= 0.0)
                {
                    _769 = _762.y >= 0.0;
                }
                else
                {
                    _769 = false;
                }
                bool _773;
                if (_769)
                {
                    _773 = _763 <= Uniforms.Scalar4[1].x;
                }
                else
                {
                    _773 = false;
                }
                bool _778;
                if (_773)
                {
                    _778 = _762.y <= Uniforms.Scalar4[1].y;
                }
                else
                {
                    _778 = false;
                }
                if (_778)
                {
                    _1429 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _762).w;
                    break;
                }
                _1429 = 0.0;
                break;
            }
            vec2 _786 = in_var_TEXCOORD0 + _748;
            float _787 = _786.x;
            bool _793;
            if (_787 >= 0.0)
            {
                _793 = _786.y >= 0.0;
            }
            else
            {
                _793 = false;
            }
            bool _797;
            if (_793)
            {
                _797 = _787 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _797 = false;
            }
            bool _802;
            if (_797)
            {
                _802 = _786.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _802 = false;
            }
            float _811;
            if (_802)
            {
                _811 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _786).w * 0.0827184617519378662109375;
            }
            else
            {
                _811 = 0.0;
            }
            float _812 = _802 ? 0.0827184617519378662109375 : 0.0;
            vec2 _813 = _743 * 0.20000000298023223876953125;
            vec2 _815 = (in_var_TEXCOORD0 + _813) + _748;
            float _816 = _815.x;
            bool _822;
            if (_816 >= 0.0)
            {
                _822 = _815.y >= 0.0;
            }
            else
            {
                _822 = false;
            }
            bool _826;
            if (_822)
            {
                _826 = _816 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _826 = false;
            }
            bool _831;
            if (_826)
            {
                _831 = _815.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _831 = false;
            }
            float _842;
            float _843;
            if (_831)
            {
                _842 = _812 + 0.081080533564090728759765625;
                _843 = _811 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _815).w * 0.081080533564090728759765625);
            }
            else
            {
                _842 = _812;
                _843 = _811;
            }
            vec2 _845 = (in_var_TEXCOORD0 - _813) + _748;
            float _846 = _845.x;
            bool _852;
            if (_846 >= 0.0)
            {
                _852 = _845.y >= 0.0;
            }
            else
            {
                _852 = false;
            }
            bool _856;
            if (_852)
            {
                _856 = _846 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _856 = false;
            }
            bool _861;
            if (_856)
            {
                _861 = _845.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _861 = false;
            }
            float _872;
            float _873;
            if (_861)
            {
                _872 = _843 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _845).w * 0.081080533564090728759765625);
                _873 = _842 + 0.081080533564090728759765625;
            }
            else
            {
                _872 = _843;
                _873 = _842;
            }
            vec2 _874 = _743 * 0.4000000059604644775390625;
            vec2 _876 = (in_var_TEXCOORD0 + _874) + _748;
            float _877 = _876.x;
            bool _883;
            if (_877 >= 0.0)
            {
                _883 = _876.y >= 0.0;
            }
            else
            {
                _883 = false;
            }
            bool _887;
            if (_883)
            {
                _887 = _877 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _887 = false;
            }
            bool _892;
            if (_887)
            {
                _892 = _876.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _892 = false;
            }
            float _903;
            float _904;
            if (_892)
            {
                _903 = _873 + 0.0763587653636932373046875;
                _904 = _872 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _876).w * 0.0763587653636932373046875);
            }
            else
            {
                _903 = _873;
                _904 = _872;
            }
            vec2 _906 = (in_var_TEXCOORD0 - _874) + _748;
            float _907 = _906.x;
            bool _913;
            if (_907 >= 0.0)
            {
                _913 = _906.y >= 0.0;
            }
            else
            {
                _913 = false;
            }
            bool _917;
            if (_913)
            {
                _917 = _907 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _917 = false;
            }
            bool _922;
            if (_917)
            {
                _922 = _906.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _922 = false;
            }
            float _933;
            float _934;
            if (_922)
            {
                _933 = _904 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _906).w * 0.0763587653636932373046875);
                _934 = _903 + 0.0763587653636932373046875;
            }
            else
            {
                _933 = _904;
                _934 = _903;
            }
            vec2 _935 = _743 * 0.60000002384185791015625;
            vec2 _937 = (in_var_TEXCOORD0 + _935) + _748;
            float _938 = _937.x;
            bool _944;
            if (_938 >= 0.0)
            {
                _944 = _937.y >= 0.0;
            }
            else
            {
                _944 = false;
            }
            bool _948;
            if (_944)
            {
                _948 = _938 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _948 = false;
            }
            bool _953;
            if (_948)
            {
                _953 = _937.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _953 = false;
            }
            float _964;
            float _965;
            if (_953)
            {
                _964 = _934 + 0.069092273712158203125;
                _965 = _933 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _937).w * 0.069092273712158203125);
            }
            else
            {
                _964 = _934;
                _965 = _933;
            }
            vec2 _967 = (in_var_TEXCOORD0 - _935) + _748;
            float _968 = _967.x;
            bool _974;
            if (_968 >= 0.0)
            {
                _974 = _967.y >= 0.0;
            }
            else
            {
                _974 = false;
            }
            bool _978;
            if (_974)
            {
                _978 = _968 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _978 = false;
            }
            bool _983;
            if (_978)
            {
                _983 = _967.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _983 = false;
            }
            float _994;
            float _995;
            if (_983)
            {
                _994 = _965 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _967).w * 0.069092273712158203125);
                _995 = _964 + 0.069092273712158203125;
            }
            else
            {
                _994 = _965;
                _995 = _964;
            }
            vec2 _996 = _743 * 0.800000011920928955078125;
            vec2 _998 = (in_var_TEXCOORD0 + _996) + _748;
            float _999 = _998.x;
            bool _1005;
            if (_999 >= 0.0)
            {
                _1005 = _998.y >= 0.0;
            }
            else
            {
                _1005 = false;
            }
            bool _1009;
            if (_1005)
            {
                _1009 = _999 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1009 = false;
            }
            bool _1014;
            if (_1009)
            {
                _1014 = _998.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1014 = false;
            }
            float _1025;
            float _1026;
            if (_1014)
            {
                _1025 = _995 + 0.060065932571887969970703125;
                _1026 = _994 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _998).w * 0.060065932571887969970703125);
            }
            else
            {
                _1025 = _995;
                _1026 = _994;
            }
            vec2 _1028 = (in_var_TEXCOORD0 - _996) + _748;
            float _1029 = _1028.x;
            bool _1035;
            if (_1029 >= 0.0)
            {
                _1035 = _1028.y >= 0.0;
            }
            else
            {
                _1035 = false;
            }
            bool _1039;
            if (_1035)
            {
                _1039 = _1029 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1039 = false;
            }
            bool _1044;
            if (_1039)
            {
                _1044 = _1028.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1044 = false;
            }
            float _1055;
            float _1056;
            if (_1044)
            {
                _1055 = _1026 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1028).w * 0.060065932571887969970703125);
                _1056 = _1025 + 0.060065932571887969970703125;
            }
            else
            {
                _1055 = _1026;
                _1056 = _1025;
            }
            vec2 _1057 = _743 * 1.0;
            vec2 _1059 = (in_var_TEXCOORD0 + _1057) + _748;
            float _1060 = _1059.x;
            bool _1066;
            if (_1060 >= 0.0)
            {
                _1066 = _1059.y >= 0.0;
            }
            else
            {
                _1066 = false;
            }
            bool _1070;
            if (_1066)
            {
                _1070 = _1060 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1070 = false;
            }
            bool _1075;
            if (_1070)
            {
                _1075 = _1059.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1075 = false;
            }
            float _1086;
            float _1087;
            if (_1075)
            {
                _1086 = _1056 + 0.0501712858676910400390625;
                _1087 = _1055 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1059).w * 0.0501712858676910400390625);
            }
            else
            {
                _1086 = _1056;
                _1087 = _1055;
            }
            vec2 _1089 = (in_var_TEXCOORD0 - _1057) + _748;
            float _1090 = _1089.x;
            bool _1096;
            if (_1090 >= 0.0)
            {
                _1096 = _1089.y >= 0.0;
            }
            else
            {
                _1096 = false;
            }
            bool _1100;
            if (_1096)
            {
                _1100 = _1090 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1100 = false;
            }
            bool _1105;
            if (_1100)
            {
                _1105 = _1089.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1105 = false;
            }
            float _1116;
            float _1117;
            if (_1105)
            {
                _1116 = _1087 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1089).w * 0.0501712858676910400390625);
                _1117 = _1086 + 0.0501712858676910400390625;
            }
            else
            {
                _1116 = _1087;
                _1117 = _1086;
            }
            vec2 _1118 = _743 * 1.2000000476837158203125;
            vec2 _1120 = (in_var_TEXCOORD0 + _1118) + _748;
            float _1121 = _1120.x;
            bool _1127;
            if (_1121 >= 0.0)
            {
                _1127 = _1120.y >= 0.0;
            }
            else
            {
                _1127 = false;
            }
            bool _1131;
            if (_1127)
            {
                _1131 = _1121 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1131 = false;
            }
            bool _1136;
            if (_1131)
            {
                _1136 = _1120.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1136 = false;
            }
            float _1147;
            float _1148;
            if (_1136)
            {
                _1147 = _1117 + 0.04026339948177337646484375;
                _1148 = _1116 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1120).w * 0.04026339948177337646484375);
            }
            else
            {
                _1147 = _1117;
                _1148 = _1116;
            }
            vec2 _1150 = (in_var_TEXCOORD0 - _1118) + _748;
            float _1151 = _1150.x;
            bool _1157;
            if (_1151 >= 0.0)
            {
                _1157 = _1150.y >= 0.0;
            }
            else
            {
                _1157 = false;
            }
            bool _1161;
            if (_1157)
            {
                _1161 = _1151 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1161 = false;
            }
            bool _1166;
            if (_1161)
            {
                _1166 = _1150.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1166 = false;
            }
            float _1177;
            float _1178;
            if (_1166)
            {
                _1177 = _1148 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1150).w * 0.04026339948177337646484375);
                _1178 = _1147 + 0.04026339948177337646484375;
            }
            else
            {
                _1177 = _1148;
                _1178 = _1147;
            }
            vec2 _1179 = _743 * 1.39999997615814208984375;
            vec2 _1181 = (in_var_TEXCOORD0 + _1179) + _748;
            float _1182 = _1181.x;
            bool _1188;
            if (_1182 >= 0.0)
            {
                _1188 = _1181.y >= 0.0;
            }
            else
            {
                _1188 = false;
            }
            bool _1192;
            if (_1188)
            {
                _1192 = _1182 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1192 = false;
            }
            bool _1197;
            if (_1192)
            {
                _1197 = _1181.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1197 = false;
            }
            float _1208;
            float _1209;
            if (_1197)
            {
                _1208 = _1178 + 0.0310451574623584747314453125;
                _1209 = _1177 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1181).w * 0.0310451574623584747314453125);
            }
            else
            {
                _1208 = _1178;
                _1209 = _1177;
            }
            vec2 _1211 = (in_var_TEXCOORD0 - _1179) + _748;
            float _1212 = _1211.x;
            bool _1218;
            if (_1212 >= 0.0)
            {
                _1218 = _1211.y >= 0.0;
            }
            else
            {
                _1218 = false;
            }
            bool _1222;
            if (_1218)
            {
                _1222 = _1212 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1222 = false;
            }
            bool _1227;
            if (_1222)
            {
                _1227 = _1211.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1227 = false;
            }
            float _1238;
            float _1239;
            if (_1227)
            {
                _1238 = _1209 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1211).w * 0.0310451574623584747314453125);
                _1239 = _1208 + 0.0310451574623584747314453125;
            }
            else
            {
                _1238 = _1209;
                _1239 = _1208;
            }
            vec2 _1240 = _743 * 1.60000002384185791015625;
            vec2 _1242 = (in_var_TEXCOORD0 + _1240) + _748;
            float _1243 = _1242.x;
            bool _1249;
            if (_1243 >= 0.0)
            {
                _1249 = _1242.y >= 0.0;
            }
            else
            {
                _1249 = false;
            }
            bool _1253;
            if (_1249)
            {
                _1253 = _1243 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1253 = false;
            }
            bool _1258;
            if (_1253)
            {
                _1258 = _1242.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1258 = false;
            }
            float _1269;
            float _1270;
            if (_1258)
            {
                _1269 = _1239 + 0.02299881912767887115478515625;
                _1270 = _1238 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1242).w * 0.02299881912767887115478515625);
            }
            else
            {
                _1269 = _1239;
                _1270 = _1238;
            }
            vec2 _1272 = (in_var_TEXCOORD0 - _1240) + _748;
            float _1273 = _1272.x;
            bool _1279;
            if (_1273 >= 0.0)
            {
                _1279 = _1272.y >= 0.0;
            }
            else
            {
                _1279 = false;
            }
            bool _1283;
            if (_1279)
            {
                _1283 = _1273 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1283 = false;
            }
            bool _1288;
            if (_1283)
            {
                _1288 = _1272.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1288 = false;
            }
            float _1299;
            float _1300;
            if (_1288)
            {
                _1299 = _1270 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1272).w * 0.02299881912767887115478515625);
                _1300 = _1269 + 0.02299881912767887115478515625;
            }
            else
            {
                _1299 = _1270;
                _1300 = _1269;
            }
            vec2 _1301 = _743 * 1.80000007152557373046875;
            vec2 _1303 = (in_var_TEXCOORD0 + _1301) + _748;
            float _1304 = _1303.x;
            bool _1310;
            if (_1304 >= 0.0)
            {
                _1310 = _1303.y >= 0.0;
            }
            else
            {
                _1310 = false;
            }
            bool _1314;
            if (_1310)
            {
                _1314 = _1304 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1314 = false;
            }
            bool _1319;
            if (_1314)
            {
                _1319 = _1303.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1319 = false;
            }
            float _1330;
            float _1331;
            if (_1319)
            {
                _1330 = _1300 + 0.01636987738311290740966796875;
                _1331 = _1299 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1303).w * 0.01636987738311290740966796875);
            }
            else
            {
                _1330 = _1300;
                _1331 = _1299;
            }
            vec2 _1333 = (in_var_TEXCOORD0 - _1301) + _748;
            float _1334 = _1333.x;
            bool _1340;
            if (_1334 >= 0.0)
            {
                _1340 = _1333.y >= 0.0;
            }
            else
            {
                _1340 = false;
            }
            bool _1344;
            if (_1340)
            {
                _1344 = _1334 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1344 = false;
            }
            bool _1349;
            if (_1344)
            {
                _1349 = _1333.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1349 = false;
            }
            float _1360;
            float _1361;
            if (_1349)
            {
                _1360 = _1331 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1333).w * 0.01636987738311290740966796875);
                _1361 = _1330 + 0.01636987738311290740966796875;
            }
            else
            {
                _1360 = _1331;
                _1361 = _1330;
            }
            vec2 _1362 = _743 * 2.0;
            vec2 _1364 = (in_var_TEXCOORD0 + _1362) + _748;
            float _1365 = _1364.x;
            bool _1371;
            if (_1365 >= 0.0)
            {
                _1371 = _1364.y >= 0.0;
            }
            else
            {
                _1371 = false;
            }
            bool _1375;
            if (_1371)
            {
                _1375 = _1365 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1375 = false;
            }
            bool _1380;
            if (_1375)
            {
                _1380 = _1364.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1380 = false;
            }
            float _1391;
            float _1392;
            if (_1380)
            {
                _1391 = _1361 + 0.011194727383553981781005859375;
                _1392 = _1360 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1364).w * 0.011194727383553981781005859375);
            }
            else
            {
                _1391 = _1361;
                _1392 = _1360;
            }
            vec2 _1394 = (in_var_TEXCOORD0 - _1362) + _748;
            float _1395 = _1394.x;
            bool _1401;
            if (_1395 >= 0.0)
            {
                _1401 = _1394.y >= 0.0;
            }
            else
            {
                _1401 = false;
            }
            bool _1405;
            if (_1401)
            {
                _1405 = _1395 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _1405 = false;
            }
            bool _1410;
            if (_1405)
            {
                _1410 = _1394.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _1410 = false;
            }
            float _1421;
            float _1422;
            if (_1410)
            {
                _1421 = _1392 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _1394).w * 0.011194727383553981781005859375);
                _1422 = _1391 + 0.011194727383553981781005859375;
            }
            else
            {
                _1421 = _1392;
                _1422 = _1391;
            }
            float _1428;
            if (_1422 > 0.0)
            {
                _1428 = _1421 / _1422;
            }
            else
            {
                _1428 = 0.0;
            }
            _1429 = _1428;
            break;
        } while(false);
        _1431 = vec4(_1429);
    }
    else
    {
        vec2 _81 = vec2(Uniforms.Scalar4[0].x, Uniforms.Scalar4[0].y);
        vec2 _86 = vec2(Uniforms.Scalar4[0].z, Uniforms.Scalar4[0].w);
        vec4 _738;
        do
        {
            bool _97;
            if (Uniforms.Scalar4[0].x < 0.001000000047497451305389404296875)
            {
                _97 = Uniforms.Scalar4[0].y < 0.001000000047497451305389404296875;
            }
            else
            {
                _97 = false;
            }
            if (_97)
            {
                _738 = texture(SPIRV_Cross_CombinedTexture0Sampler0, in_var_TEXCOORD0 + _86);
                break;
            }
            vec2 _105 = in_var_TEXCOORD0 + _86;
            float _106 = _105.x;
            bool _112;
            if (_106 >= 0.0)
            {
                _112 = _105.y >= 0.0;
            }
            else
            {
                _112 = false;
            }
            bool _116;
            if (_112)
            {
                _116 = _106 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _116 = false;
            }
            bool _121;
            if (_116)
            {
                _121 = _105.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _121 = false;
            }
            vec4 _129;
            if (_121)
            {
                _129 = texture(SPIRV_Cross_CombinedTexture0Sampler0, _105) * 0.0827184617519378662109375;
            }
            else
            {
                _129 = vec4(0.0);
            }
            float _130 = _121 ? 0.0827184617519378662109375 : 0.0;
            vec2 _131 = _81 * 0.20000000298023223876953125;
            vec2 _134 = (in_var_TEXCOORD0 + _131) + _86;
            vec2 _136 = (in_var_TEXCOORD0 + (-_131)) + _86;
            float _137 = _134.x;
            bool _143;
            if (_137 >= 0.0)
            {
                _143 = _134.y >= 0.0;
            }
            else
            {
                _143 = false;
            }
            bool _147;
            if (_143)
            {
                _147 = _137 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _147 = false;
            }
            bool _152;
            if (_147)
            {
                _152 = _134.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _152 = false;
            }
            float _162;
            vec4 _163;
            if (_152)
            {
                _162 = _130 + 0.081080533564090728759765625;
                _163 = _129 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _134) * 0.081080533564090728759765625);
            }
            else
            {
                _162 = _130;
                _163 = _129;
            }
            float _164 = _136.x;
            bool _170;
            if (_164 >= 0.0)
            {
                _170 = _136.y >= 0.0;
            }
            else
            {
                _170 = false;
            }
            bool _174;
            if (_170)
            {
                _174 = _164 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _174 = false;
            }
            bool _179;
            if (_174)
            {
                _179 = _136.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _179 = false;
            }
            vec4 _189;
            float _190;
            if (_179)
            {
                _189 = _163 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _136) * 0.081080533564090728759765625);
                _190 = _162 + 0.081080533564090728759765625;
            }
            else
            {
                _189 = _163;
                _190 = _162;
            }
            vec2 _191 = _81 * 0.4000000059604644775390625;
            vec2 _194 = (in_var_TEXCOORD0 + _191) + _86;
            vec2 _196 = (in_var_TEXCOORD0 + (-_191)) + _86;
            float _197 = _194.x;
            bool _203;
            if (_197 >= 0.0)
            {
                _203 = _194.y >= 0.0;
            }
            else
            {
                _203 = false;
            }
            bool _207;
            if (_203)
            {
                _207 = _197 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _207 = false;
            }
            bool _212;
            if (_207)
            {
                _212 = _194.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _212 = false;
            }
            float _222;
            vec4 _223;
            if (_212)
            {
                _222 = _190 + 0.0763587653636932373046875;
                _223 = _189 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _194) * 0.0763587653636932373046875);
            }
            else
            {
                _222 = _190;
                _223 = _189;
            }
            float _224 = _196.x;
            bool _230;
            if (_224 >= 0.0)
            {
                _230 = _196.y >= 0.0;
            }
            else
            {
                _230 = false;
            }
            bool _234;
            if (_230)
            {
                _234 = _224 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _234 = false;
            }
            bool _239;
            if (_234)
            {
                _239 = _196.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _239 = false;
            }
            vec4 _249;
            float _250;
            if (_239)
            {
                _249 = _223 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _196) * 0.0763587653636932373046875);
                _250 = _222 + 0.0763587653636932373046875;
            }
            else
            {
                _249 = _223;
                _250 = _222;
            }
            vec2 _251 = _81 * 0.60000002384185791015625;
            vec2 _254 = (in_var_TEXCOORD0 + _251) + _86;
            vec2 _256 = (in_var_TEXCOORD0 + (-_251)) + _86;
            float _257 = _254.x;
            bool _263;
            if (_257 >= 0.0)
            {
                _263 = _254.y >= 0.0;
            }
            else
            {
                _263 = false;
            }
            bool _267;
            if (_263)
            {
                _267 = _257 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _267 = false;
            }
            bool _272;
            if (_267)
            {
                _272 = _254.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _272 = false;
            }
            float _282;
            vec4 _283;
            if (_272)
            {
                _282 = _250 + 0.069092273712158203125;
                _283 = _249 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _254) * 0.069092273712158203125);
            }
            else
            {
                _282 = _250;
                _283 = _249;
            }
            float _284 = _256.x;
            bool _290;
            if (_284 >= 0.0)
            {
                _290 = _256.y >= 0.0;
            }
            else
            {
                _290 = false;
            }
            bool _294;
            if (_290)
            {
                _294 = _284 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _294 = false;
            }
            bool _299;
            if (_294)
            {
                _299 = _256.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _299 = false;
            }
            vec4 _309;
            float _310;
            if (_299)
            {
                _309 = _283 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _256) * 0.069092273712158203125);
                _310 = _282 + 0.069092273712158203125;
            }
            else
            {
                _309 = _283;
                _310 = _282;
            }
            vec2 _311 = _81 * 0.800000011920928955078125;
            vec2 _314 = (in_var_TEXCOORD0 + _311) + _86;
            vec2 _316 = (in_var_TEXCOORD0 + (-_311)) + _86;
            float _317 = _314.x;
            bool _323;
            if (_317 >= 0.0)
            {
                _323 = _314.y >= 0.0;
            }
            else
            {
                _323 = false;
            }
            bool _327;
            if (_323)
            {
                _327 = _317 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _327 = false;
            }
            bool _332;
            if (_327)
            {
                _332 = _314.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _332 = false;
            }
            float _342;
            vec4 _343;
            if (_332)
            {
                _342 = _310 + 0.060065932571887969970703125;
                _343 = _309 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _314) * 0.060065932571887969970703125);
            }
            else
            {
                _342 = _310;
                _343 = _309;
            }
            float _344 = _316.x;
            bool _350;
            if (_344 >= 0.0)
            {
                _350 = _316.y >= 0.0;
            }
            else
            {
                _350 = false;
            }
            bool _354;
            if (_350)
            {
                _354 = _344 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _354 = false;
            }
            bool _359;
            if (_354)
            {
                _359 = _316.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _359 = false;
            }
            vec4 _369;
            float _370;
            if (_359)
            {
                _369 = _343 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _316) * 0.060065932571887969970703125);
                _370 = _342 + 0.060065932571887969970703125;
            }
            else
            {
                _369 = _343;
                _370 = _342;
            }
            vec2 _371 = _81 * 1.0;
            vec2 _374 = (in_var_TEXCOORD0 + _371) + _86;
            vec2 _376 = (in_var_TEXCOORD0 + (-_371)) + _86;
            float _377 = _374.x;
            bool _383;
            if (_377 >= 0.0)
            {
                _383 = _374.y >= 0.0;
            }
            else
            {
                _383 = false;
            }
            bool _387;
            if (_383)
            {
                _387 = _377 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _387 = false;
            }
            bool _392;
            if (_387)
            {
                _392 = _374.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _392 = false;
            }
            float _402;
            vec4 _403;
            if (_392)
            {
                _402 = _370 + 0.0501712858676910400390625;
                _403 = _369 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _374) * 0.0501712858676910400390625);
            }
            else
            {
                _402 = _370;
                _403 = _369;
            }
            float _404 = _376.x;
            bool _410;
            if (_404 >= 0.0)
            {
                _410 = _376.y >= 0.0;
            }
            else
            {
                _410 = false;
            }
            bool _414;
            if (_410)
            {
                _414 = _404 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _414 = false;
            }
            bool _419;
            if (_414)
            {
                _419 = _376.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _419 = false;
            }
            vec4 _429;
            float _430;
            if (_419)
            {
                _429 = _403 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _376) * 0.0501712858676910400390625);
                _430 = _402 + 0.0501712858676910400390625;
            }
            else
            {
                _429 = _403;
                _430 = _402;
            }
            vec2 _431 = _81 * 1.2000000476837158203125;
            vec2 _434 = (in_var_TEXCOORD0 + _431) + _86;
            vec2 _436 = (in_var_TEXCOORD0 + (-_431)) + _86;
            float _437 = _434.x;
            bool _443;
            if (_437 >= 0.0)
            {
                _443 = _434.y >= 0.0;
            }
            else
            {
                _443 = false;
            }
            bool _447;
            if (_443)
            {
                _447 = _437 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _447 = false;
            }
            bool _452;
            if (_447)
            {
                _452 = _434.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _452 = false;
            }
            float _462;
            vec4 _463;
            if (_452)
            {
                _462 = _430 + 0.04026339948177337646484375;
                _463 = _429 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _434) * 0.04026339948177337646484375);
            }
            else
            {
                _462 = _430;
                _463 = _429;
            }
            float _464 = _436.x;
            bool _470;
            if (_464 >= 0.0)
            {
                _470 = _436.y >= 0.0;
            }
            else
            {
                _470 = false;
            }
            bool _474;
            if (_470)
            {
                _474 = _464 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _474 = false;
            }
            bool _479;
            if (_474)
            {
                _479 = _436.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _479 = false;
            }
            vec4 _489;
            float _490;
            if (_479)
            {
                _489 = _463 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _436) * 0.04026339948177337646484375);
                _490 = _462 + 0.04026339948177337646484375;
            }
            else
            {
                _489 = _463;
                _490 = _462;
            }
            vec2 _491 = _81 * 1.39999997615814208984375;
            vec2 _494 = (in_var_TEXCOORD0 + _491) + _86;
            vec2 _496 = (in_var_TEXCOORD0 + (-_491)) + _86;
            float _497 = _494.x;
            bool _503;
            if (_497 >= 0.0)
            {
                _503 = _494.y >= 0.0;
            }
            else
            {
                _503 = false;
            }
            bool _507;
            if (_503)
            {
                _507 = _497 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _507 = false;
            }
            bool _512;
            if (_507)
            {
                _512 = _494.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _512 = false;
            }
            float _522;
            vec4 _523;
            if (_512)
            {
                _522 = _490 + 0.0310451574623584747314453125;
                _523 = _489 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _494) * 0.0310451574623584747314453125);
            }
            else
            {
                _522 = _490;
                _523 = _489;
            }
            float _524 = _496.x;
            bool _530;
            if (_524 >= 0.0)
            {
                _530 = _496.y >= 0.0;
            }
            else
            {
                _530 = false;
            }
            bool _534;
            if (_530)
            {
                _534 = _524 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _534 = false;
            }
            bool _539;
            if (_534)
            {
                _539 = _496.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _539 = false;
            }
            vec4 _549;
            float _550;
            if (_539)
            {
                _549 = _523 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _496) * 0.0310451574623584747314453125);
                _550 = _522 + 0.0310451574623584747314453125;
            }
            else
            {
                _549 = _523;
                _550 = _522;
            }
            vec2 _551 = _81 * 1.60000002384185791015625;
            vec2 _554 = (in_var_TEXCOORD0 + _551) + _86;
            vec2 _556 = (in_var_TEXCOORD0 + (-_551)) + _86;
            float _557 = _554.x;
            bool _563;
            if (_557 >= 0.0)
            {
                _563 = _554.y >= 0.0;
            }
            else
            {
                _563 = false;
            }
            bool _567;
            if (_563)
            {
                _567 = _557 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _567 = false;
            }
            bool _572;
            if (_567)
            {
                _572 = _554.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _572 = false;
            }
            float _582;
            vec4 _583;
            if (_572)
            {
                _582 = _550 + 0.02299881912767887115478515625;
                _583 = _549 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _554) * 0.02299881912767887115478515625);
            }
            else
            {
                _582 = _550;
                _583 = _549;
            }
            float _584 = _556.x;
            bool _590;
            if (_584 >= 0.0)
            {
                _590 = _556.y >= 0.0;
            }
            else
            {
                _590 = false;
            }
            bool _594;
            if (_590)
            {
                _594 = _584 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _594 = false;
            }
            bool _599;
            if (_594)
            {
                _599 = _556.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _599 = false;
            }
            vec4 _609;
            float _610;
            if (_599)
            {
                _609 = _583 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _556) * 0.02299881912767887115478515625);
                _610 = _582 + 0.02299881912767887115478515625;
            }
            else
            {
                _609 = _583;
                _610 = _582;
            }
            vec2 _611 = _81 * 1.80000007152557373046875;
            vec2 _614 = (in_var_TEXCOORD0 + _611) + _86;
            vec2 _616 = (in_var_TEXCOORD0 + (-_611)) + _86;
            float _617 = _614.x;
            bool _623;
            if (_617 >= 0.0)
            {
                _623 = _614.y >= 0.0;
            }
            else
            {
                _623 = false;
            }
            bool _627;
            if (_623)
            {
                _627 = _617 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _627 = false;
            }
            bool _632;
            if (_627)
            {
                _632 = _614.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _632 = false;
            }
            float _642;
            vec4 _643;
            if (_632)
            {
                _642 = _610 + 0.01636987738311290740966796875;
                _643 = _609 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _614) * 0.01636987738311290740966796875);
            }
            else
            {
                _642 = _610;
                _643 = _609;
            }
            float _644 = _616.x;
            bool _650;
            if (_644 >= 0.0)
            {
                _650 = _616.y >= 0.0;
            }
            else
            {
                _650 = false;
            }
            bool _654;
            if (_650)
            {
                _654 = _644 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _654 = false;
            }
            bool _659;
            if (_654)
            {
                _659 = _616.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _659 = false;
            }
            vec4 _669;
            float _670;
            if (_659)
            {
                _669 = _643 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _616) * 0.01636987738311290740966796875);
                _670 = _642 + 0.01636987738311290740966796875;
            }
            else
            {
                _669 = _643;
                _670 = _642;
            }
            vec2 _671 = _81 * 2.0;
            vec2 _674 = (in_var_TEXCOORD0 + _671) + _86;
            vec2 _676 = (in_var_TEXCOORD0 + (-_671)) + _86;
            float _677 = _674.x;
            bool _683;
            if (_677 >= 0.0)
            {
                _683 = _674.y >= 0.0;
            }
            else
            {
                _683 = false;
            }
            bool _687;
            if (_683)
            {
                _687 = _677 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _687 = false;
            }
            bool _692;
            if (_687)
            {
                _692 = _674.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _692 = false;
            }
            float _702;
            vec4 _703;
            if (_692)
            {
                _702 = _670 + 0.011194727383553981781005859375;
                _703 = _669 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _674) * 0.011194727383553981781005859375);
            }
            else
            {
                _702 = _670;
                _703 = _669;
            }
            float _704 = _676.x;
            bool _710;
            if (_704 >= 0.0)
            {
                _710 = _676.y >= 0.0;
            }
            else
            {
                _710 = false;
            }
            bool _714;
            if (_710)
            {
                _714 = _704 <= Uniforms.Scalar4[1].x;
            }
            else
            {
                _714 = false;
            }
            bool _719;
            if (_714)
            {
                _719 = _676.y <= Uniforms.Scalar4[1].y;
            }
            else
            {
                _719 = false;
            }
            vec4 _729;
            float _730;
            if (_719)
            {
                _729 = _703 + (texture(SPIRV_Cross_CombinedTexture0Sampler0, _676) * 0.011194727383553981781005859375);
                _730 = _702 + 0.011194727383553981781005859375;
            }
            else
            {
                _729 = _703;
                _730 = _702;
            }
            vec4 _737;
            if (_730 > 0.0)
            {
                _737 = _729 / vec4(_730);
            }
            else
            {
                _737 = vec4(0.0);
            }
            _738 = _737;
            break;
        } while(false);
        _1431 = _738;
    }
    vec4 _1438;
    if (Uniforms.Integer4[0].x == 1)
    {
        _1438 = _1431 * in_var_COLOR0;
    }
    else
    {
        _1438 = _1431;
    }
    out_var_SV_Target = _1438;
}

)GLSL";