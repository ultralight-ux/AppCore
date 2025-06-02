#ifndef CLIP_HLSLI
#define CLIP_HLSLI

#include "common.hlsli"

// Anti-aliasing width for SDF edges
#define AA_WIDTH 0.354

// Helper functions
float2 transformAffine(float2 val, float2 a, float2 b, float2 c) {
    return val.x * a + val.y * b + c;
}

void Unpack(float4 x, out float4 a, out float4 b) {
    const float s = 65536.0;
    a = floor(x / s);
    b = floor(x - a * s);
}

float4 GetCol(in float4x4 m, uint i) { 
    return float4(m[0][i], m[1][i], m[2][i], m[3][i]); 
}

// Anti-aliasing functions
float antialias(in float d, in float width, in float median) {
    return smoothstep(median - width, median + width, d);
}

float antialias2(float d) {
    return smoothstep(-0.6180469, 0.6180469, d/fwidth(d));
}

// SDF (Signed Distance Field) functions
float sdRect(float2 p, float2 size) {
    float2 d = abs(p) - size;
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

// The below function "sdEllipse" is MIT licensed with following text:
//
// The MIT License
// Copyright 2013 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions: The above copyright
// notice and this permission notice shall be included in all copies or substantial
// portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
// EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
float sdEllipse(float2 p, float2 ab) {
    if (abs(ab.x - ab.y) < 0.1)
        return length(p) - ab.x;

    p = abs(p); if (p.x > p.y) { p = p.yx; ab = ab.yx; }

    float l = ab.y*ab.y - ab.x*ab.x;

    float m = ab.x*p.x / l;
    float n = ab.y*p.y / l;
    float m2 = m*m;
    float n2 = n*n;

    float c = (m2 + n2 - 1.0) / 3.0;
    float c3 = c*c*c;

    float q = c3 + m2*n2*2.0;
    float d = c3 + m2*n2;
    float g = m + m*n2;

    float co;

    if (d < 0.0) {
        float p = acos(q / c3) / 3.0;
        float s = cos(p);
        float t = sin(p)*sqrt(3.0);
        float rx = sqrt(-c*(s + t + 2.0) + m2);
        float ry = sqrt(-c*(s - t + 2.0) + m2);
        co = (ry + sign(l)*rx + abs(g) / (rx*ry) - m) / 2.0;
    } else {
        float h = 2.0*m*n*sqrt(d);
        float s = sign(q + h)*pow(abs(q + h), 1.0 / 3.0);
        float u = sign(q - h)*pow(abs(q - h), 1.0 / 3.0);
        float rx = -s - u - c*4.0 + 2.0*m2;
        float ry = (s - u)*sqrt(3.0);
        float rm = sqrt(rx*rx + ry*ry);
        float p = ry / sqrt(rm - rx);
        co = (p + 2.0*g / rm - m) / 2.0;
    }

    float si = sqrt(1.0 - co*co);

    float2 r = float2(ab.x*co, ab.y*si);

    return length(r - p) * sign(p.y - r.y);
}

float sdRoundRect(float2 p, float2 size, float4 rx, float4 ry) {
    size *= 0.5;
    float2 corner;

    corner = float2(-size.x + rx.x, -size.y + ry.x);  // Top-Left
    float2 local = p - corner;
    if (dot(rx.x, ry.x) > 0.0 && p.x < corner.x && p.y <= corner.y)
        return sdEllipse(local, float2(rx.x, ry.x));

    corner = float2(size.x - rx.y, -size.y + ry.y);   // Top-Right
    local = p - corner;
    if (dot(rx.y, ry.y) > 0.0 && p.x >= corner.x && p.y <= corner.y)
        return sdEllipse(local, float2(rx.y, ry.y));

    corner = float2(size.x - rx.z, size.y - ry.z);  // Bottom-Right
    local = p - corner;
    if (dot(rx.z, ry.z) > 0.0 && p.x >= corner.x && p.y >= corner.y)
        return sdEllipse(local, float2(rx.z, ry.z));

    corner = float2(-size.x + rx.w, size.y - ry.w); // Bottom-Left
    local = p - corner;
    if (dot(rx.w, ry.w) > 0.0 && p.x < corner.x && p.y > corner.y)
        return sdEllipse(local, float2(rx.w, ry.w));

    return sdRect(p, size);
}

// Line segment distance functions
float testCross(float2 a, float2 b, float2 p) {
    return (b.y - a.y) * (p.x - a.x) - (b.x - a.x) * (p.y - a.y);
}

// Returns two values:
// [0] = distance of p to line segment.
// [1] = closest t on line segment, clamped to [0, 1]
float2 sdSegment(in float2 p, in float2 a, in float2 b) {
    float2 pa = p - a, ba = b - a;
    float t = dot(pa, ba) / dot(ba, ba);
    return float2(length(pa - ba * t), t);
}

float sdLine(in float2 a, in float2 b, in float2 p) {
    float2 pa = p - a, ba = b - a;
    float t = dot(pa, ba) / dot(ba, ba);
    return length(pa - ba*t) * sign(testCross(a, b, p));
}

// Clipping function
// This should be called from pixel shaders with object coordinates
void applyClip(float2 objectCoord, inout float4 outColor) {
    for (uint i = 0; i < ClipData.x; i++) {
        float4x4 data = Clip[i];
        float2 origin = GetCol(data, 0).xy;
        float2 size = GetCol(data, 0).zw;
        float4 radii_x, radii_y;
        Unpack(GetCol(data, 1), radii_x, radii_y);
        bool inverse = bool(GetCol(data, 3).z);

        float2 p = objectCoord;
        p = transformAffine(p, GetCol(data, 2).xy, GetCol(data, 2).zw, GetCol(data, 3).xy);
        p -= origin;
        float d_clip = sdRoundRect(p, size, radii_x, radii_y) * (inverse ? -1.0 : 1.0);

        float alpha = antialias2(-d_clip);
        outColor = float4(outColor.rgb * alpha, outColor.a * alpha);
    }
}

// Stroke helper functions
float innerStroke(float stroke_width, float d) {
    return min(antialias(-d, AA_WIDTH, 0.0), 1.0 - antialias(-d, AA_WIDTH, stroke_width));
}

// Gradient noise from Jorge Jimenez's presentation:
// http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare
float gradientNoise(in float2 uv) {
    const float3 magic = float3(0.06711056, 0.00583715, 52.9829189);
    return frac(magic.z * frac(dot(uv, magic.xy)));
}

// Ramp function for gradients
float ramp(in float inMin, in float inMax, in float val) {
    return clamp((val - inMin) / (inMax - inMin), 0.0, 1.0);
}

#endif // CLIP_HLSLI