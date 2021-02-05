cbuffer Uniforms : register(b0)
{
  float4 State;
  matrix Transform;
  float4 Scalar4[2];
  float4 Vector[8];
  uint ClipSize;
  matrix Clip[8];
};

float Scalar(int i) { if (i < 4) return Scalar4[0][i]; else return Scalar4[1][i - 4]; }

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState sampler0 : register(s0);

struct VS_OUTPUT
{
  float4 Position    : SV_POSITION;
  float4 Color       : COLOR0;
  float2 TexCoord    : TEXCOORD0;
  float4 Data0       : COLOR1;
  float4 Data1       : COLOR2;
  float4 Data2       : COLOR3;
  float4 Data3       : COLOR4;
  float4 Data4       : COLOR5;
  float4 Data5       : COLOR6;
  float4 Data6       : COLOR7;
  float2 ObjectCoord : TEXCOORD1;
};

uint FillType(VS_OUTPUT input) { return uint(input.Data0.x + 0.5); }
float4 TileRectUV() { return Vector[0]; }
float2 TileSize() { return Vector[1].zw; }
float2 PatternTransformA() { return Vector[2].xy; }
float2 PatternTransformB() { return Vector[2].zw; }
float2 PatternTransformC() { return Vector[3].xy; }
uint Gradient_NumStops(VS_OUTPUT input) { return uint(input.Data0.y + 0.5); }
bool Gradient_IsRadial(VS_OUTPUT input) { return bool(uint(input.Data0.z + 0.5)); }
float Gradient_R0(VS_OUTPUT input) { return input.Data1.x; }
float Gradient_R1(VS_OUTPUT input) { return input.Data1.y; }
float2 Gradient_P0(VS_OUTPUT input) { return input.Data1.xy; }
float2 Gradient_P1(VS_OUTPUT input) { return input.Data1.zw; }
float SDFMaxDistance(VS_OUTPUT input) { return input.Data0.y; }

struct GradientStop { float percent; float4 color; };

GradientStop GetGradientStop(VS_OUTPUT input, uint offset) {
  GradientStop result;
  if (offset < 4) {
    result.percent = input.Data2[offset];
    if (offset == 0)
      result.color = input.Data3;
    else if (offset == 1)
      result.color = input.Data4;
    else if (offset == 2)
      result.color = input.Data5;
    else if (offset == 3)
      result.color = input.Data6;
  } else {
    result.percent = Scalar(offset - 4);
    result.color = Vector[offset - 4];
  }
  return result;
}

#define AA_WIDTH 0.354

float antialias(in float d, in float width, in float median) {
  return smoothstep(median - width, median + width, d);
}

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

float4 fillSolid(VS_OUTPUT input) {
  return input.Color;
}

float4 fillImage(VS_OUTPUT input) {
  return texture0.Sample(sampler0, input.TexCoord) * input.Color;
}

float2 transformAffine(float2 val, float2 a, float2 b, float2 c) {
  return val.x * a + val.y * b + c;
}

float4 fillPatternImage(VS_OUTPUT input) {
  float4 tile_rect_uv = TileRectUV();
  float2 tile_size = TileSize();

  float2 p = input.ObjectCoord;

  // Apply the affine matrix
  float2 transformed_coords = transformAffine(p,
    PatternTransformA(), PatternTransformB(), PatternTransformC());

  // Convert back to uv coordinate space
  transformed_coords /= tile_size;

  // Wrap UVs to [0.0, 1.0] so texture repeats properly
  float2 uv = frac(transformed_coords);

  // Clip to tile-rect UV
  uv *= tile_rect_uv.zw - tile_rect_uv.xy;
  uv += tile_rect_uv.xy;

  return texture0.Sample(sampler0, uv) * input.Color;
}

// Gradient noise from Jorge Jimenez's presentation:
// http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare
float gradientNoise(in float2 uv)
{
    const float3 magic = float3(0.06711056, 0.00583715, 52.9829189);
    return frac(magic.z * frac(dot(uv, magic.xy)));
}

float ramp(in float inMin, in float inMax, in float val)
{
    return clamp((val - inMin) / (inMax - inMin), 0.0, 1.0);
}

float4 fillPatternGradient(VS_OUTPUT input) {
  float num_stops = Gradient_NumStops(input);
  bool is_radial = Gradient_IsRadial(input);
  float2 p0 = Gradient_P0(input);
  float2 p1 = Gradient_P1(input);
  float4 out_Color = float4(0.0, 0.0, 0.0, 0.0);

  float t = 0.0;
  if (is_radial) {
    float r0 = p1.x;
	float r1 = p1.y;
    t = distance(input.TexCoord, p0);
	float rDelta = r1 - r0;
	t = saturate((t / rDelta) - (r0 / rDelta));
  } else {
    float2 V = p1 - p0;
    t = saturate(dot(input.TexCoord - p0, V) / dot(V, V));
  }

  GradientStop stop0 = GetGradientStop(input, 0u);
  GradientStop stop1 = GetGradientStop(input, 1u);

  out_Color = lerp(stop0.color, stop1.color, ramp(stop0.percent, stop1.percent, t));
  if (num_stops > 2) {
    GradientStop stop2 = GetGradientStop(input, 2u);
    out_Color = lerp(out_Color, stop2.color, ramp(stop1.percent, stop2.percent, t));
    if (num_stops > 3) {
      GradientStop stop3 = GetGradientStop(input, 3u);
      out_Color = lerp(out_Color, stop3.color, ramp(stop2.percent, stop3.percent, t));
      if (num_stops > 4) {
        GradientStop stop4 = GetGradientStop(input, 4u);
        out_Color = lerp(out_Color, stop4.color, ramp(stop3.percent, stop4.percent, t));
        if (num_stops > 5) {
          GradientStop stop5 = GetGradientStop(input, 5u);
          out_Color = lerp(out_Color, stop5.color, ramp(stop4.percent, stop5.percent, t));
          if (num_stops > 6) {
            GradientStop stop6 = GetGradientStop(input, 6u);
            out_Color = lerp(out_Color, stop6.color, ramp(stop5.percent, stop6.percent, t));
          }
        }
      }
    }
  }

  return float4(out_Color.rgb, out_Color.a);
}

void Unpack(float4 x, out float4 a, out float4 b) {
  const float s = 65536.0;
  a = floor(x / s);
  b = floor(x - a * s);
}

float antialias2(float d) {
  return smoothstep(-AA_WIDTH, +AA_WIDTH, d);
}

// Returns two values:
// [0] = distance of p to line segment.
// [1] = closest t on line segment, clamped to [0, 1]
float2 sdSegment(in float2 p, in float2 a, in float2 b)
{
  float2 pa = p - a, ba = b - a;
  float t = dot(pa, ba) / dot(ba, ba);
  return float2(length(pa - ba * t), t);
}

float testCross(float2 a, float2 b, float2 p) {
  return (b.y - a.y) * (p.x - a.x) - (b.x - a.x) * (p.y - a.y);
}

float sdLine(in float2 a, in float2 b, in float2 p)
{
  float2 pa = p - a, ba = b - a;
  float t = dot(pa, ba) / dot(ba, ba);
  return length(pa - ba*t) * sign(testCross(a, b, p));
}


float4 blend(float4 src, float4 dest) {
  float4 result;
  result.rgb = src.rgb + dest.rgb * (1.0 - src.a);
  result.a = src.a + dest.a * (1.0 - src.a);
  return result;
}

float innerStroke(float stroke_width, float d) {
  return min(antialias(-d, AA_WIDTH, 0.0), 1.0 - antialias(-d, AA_WIDTH, stroke_width));
}

float4 fillRoundedRect(VS_OUTPUT input) {
  float2 p = input.TexCoord;
  float2 size = input.Data0.zw;
  p = (p - 0.5) * size;
  float d = sdRoundRect(p, size, input.Data1, input.Data2);

  // Fill background
  float alpha = antialias(-d, AA_WIDTH, 0.0) * input.Color.a;
  float4 outColor = float4(input.Color.rgb * alpha, alpha);

  // Draw stroke
  float stroke_width = input.Data3.x;
  float4 stroke_color = input.Data4;

  if (stroke_width > 0.0) {
    alpha = innerStroke(stroke_width, d);
    alpha *= stroke_color.a;
    float4 stroke = float4(stroke_color.rgb * alpha, alpha);
    outColor = blend(stroke, outColor);
  }

  return outColor;
}

float4 fillBoxShadow(VS_OUTPUT input) {
  float2 p = input.ObjectCoord;
  bool inset = bool(uint(input.Data0.y + 0.5));
  float radius = input.Data0.z;
  float2 origin = input.Data1.xy;
  float2 size = input.Data1.zw;
  float2 clip_origin = input.Data4.xy;
  float2 clip_size = input.Data4.zw;
  
  float sdClip = sdRoundRect(p - clip_origin, clip_size, input.Data5, input.Data6);
  float sdRect = sdRoundRect(p - origin, size, input.Data2, input.Data3);
  
  float clip = inset ? -sdRect : sdClip;
  float d = inset ? -sdClip : sdRect;

  if (clip < 0.0) {
    discard;
    return float4(0.0, 0.0, 0.0, 0.0);
  }

  float alpha = radius >= 1.0? pow(antialias(-d, radius * 2 + 0.2, 0.0), 1.9) * 3.3 / pow(radius * 1.2, 0.15) :
                               antialias(-d, AA_WIDTH, inset ? -1.0 : 1.0);

  alpha = clamp(alpha, 0.0, 1.0) * input.Color.a;
  return float4(input.Color.rgb * alpha, alpha);
}

float3 blendOverlay(float3 src, float3 dest) {
  float3 col;
  for (uint i = 0; i < 3; ++i)
    col[i] = dest[i] < 0.5 ? (2.0 * dest[i] * src[i]) : (1.0 - 2.0 * (1.0 - dest[i]) * (1.0 - src[i]));
  return col;
}

float3 blendColorDodge(float3 src, float3 dest) {
  float3 col;
  for (uint i = 0; i < 3; ++i)
    col[i] = (src[i] == 1.0) ? src[i] : min(dest[i] / (1.0 - src[i]), 1.0);
  return col;
}

float3 blendColorBurn(float3 src, float3 dest) {
  float3 col;
  for (uint i = 0; i < 3; ++i)
    col[i] = (src[i] == 0.0) ? src[i] : max((1.0 - ((1.0 - dest[i]) / src[i])), 0.0);
  return col;
}

float3 blendHardLight(float3 src, float3 dest) {
  float3 col;
  for (uint i = 0; i < 3; ++i)
    col[i] = dest[i] < 0.5 ? (2.0 * dest[i] * src[i]) : (1.0 - 2.0 * (1.0 - dest[i]) * (1.0 - src[i]));
  return col;
}

float3 blendSoftLight(float3 src, float3 dest) {
  float3 col;
  for (uint i = 0; i < 3; ++i)
    col[i] = (src[i] < 0.5) ? (2.0 * dest[i] * src[i] + dest[i] * dest[i] * (1.0 - 2.0 * src[i])) : (sqrt(dest[i]) * (2.0 * src[i] - 1.0) + 2.0 * dest[i] * (1.0 - src[i]));
  return col;
}

float3 rgb2hsl( float3 col )
{
  const float eps = 0.0000001;
  float minc = min( col.r, min(col.g, col.b) );
  float maxc = max( col.r, max(col.g, col.b) );
  float3 mask = step(col.grr,col.rgb) * step(col.bbg,col.rgb);
  float3 h = mask * (float3(0.0,2.0,4.0) + (col.gbr-col.brg)/(maxc-minc + eps)) / 6.0;
  return float3(frac( 1.0 + h.x + h.y + h.z ),                // H
                (maxc-minc)/(1.0-abs(minc+maxc-1.0) + eps),   // S
                (minc+maxc)*0.5 );                            // L
}

float3 hsl2rgb( float3 c )
{
  float3 rgb = clamp( abs(fmod(c.x*6.0+float3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
  return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}

float3 blendHue(float3 src, float3 dest) {
  float3 baseHSL = rgb2hsl(dest);
  return hsl2rgb(float3(rgb2hsl(src).r, baseHSL.g, baseHSL.b));
}

float3 blendSaturation(float3 src, float3 dest) {
  float3 baseHSL = rgb2hsl(dest);
  return hsl2rgb(float3(baseHSL.r, rgb2hsl(src).g, baseHSL.b));
}

float3 blendColor(float3 src, float3 dest) {
  float3 blendHSL = rgb2hsl(src);
  return hsl2rgb(float3(blendHSL.r, blendHSL.g, rgb2hsl(dest).b));
}

float3 blendLuminosity(float3 src, float3 dest) {
  float3 baseHSL = rgb2hsl(dest);
  return hsl2rgb(float3(baseHSL.r, baseHSL.g, rgb2hsl(src).b));
}

float4 fillBlend(VS_OUTPUT input) { 
  const uint BlendOp_Clear = 0u;
  const uint BlendOp_Source = 1u;
  const uint BlendOp_Over = 2u;
  const uint BlendOp_In = 3u;
  const uint BlendOp_Out = 4u;
  const uint BlendOp_Atop = 5u;
  const uint BlendOp_DestOver = 6u;
  const uint BlendOp_DestIn = 7u;
  const uint BlendOp_DestOut = 8u;
  const uint BlendOp_DestAtop = 9u;
  const uint BlendOp_XOR = 10u;
  const uint BlendOp_Darken = 11u;
  const uint BlendOp_Add = 12u;
  const uint BlendOp_Difference = 13u;
  const uint BlendOp_Multiply = 14u;
  const uint BlendOp_Screen = 15u;
  const uint BlendOp_Overlay = 16u;
  const uint BlendOp_Lighten = 17u;
  const uint BlendOp_ColorDodge = 18u;
  const uint BlendOp_ColorBurn = 19u;
  const uint BlendOp_HardLight = 20u;
  const uint BlendOp_SoftLight = 21u;
  const uint BlendOp_Exclusion = 22u;
  const uint BlendOp_Hue = 23u;
  const uint BlendOp_Saturation = 24u;
  const uint BlendOp_Color = 25u;
  const uint BlendOp_Luminosity = 26u;

  float4 src = fillImage(input);
  float4 dest = texture1.Sample(sampler0, input.ObjectCoord);

  switch(uint(input.Data0.y + 0.5))
  {
  case BlendOp_Clear: return float4(0.0, 0.0, 0.0, 0.0);
  case BlendOp_Source: return src;
  case BlendOp_Over: return src + dest * (1.0 - src.a);
  case BlendOp_In: return src * dest.a;
  case BlendOp_Out: return src * (1.0 - dest.a);
  case BlendOp_Atop: return src * dest.a + dest * (1.0 - src.a);
  case BlendOp_DestOver: return src * (1.0 - dest.a) + dest;
  case BlendOp_DestIn: return dest * src.a;
  case BlendOp_DestOut: return dest * (1.0 - src.a);
  case BlendOp_DestAtop: return src * (1.0 - dest.a) + dest * src.a;
  case BlendOp_XOR: return saturate(src * (1.0 - dest.a) + dest * (1.0 - src.a));
  case BlendOp_Darken: return float4(min(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  case BlendOp_Add: return saturate(src + dest);
  case BlendOp_Difference: return float4(abs(dest.rgb - src.rgb) * src.a, dest.a * src.a);
  case BlendOp_Multiply: return float4(src.rgb * dest.rgb * src.a, dest.a * src.a);
  case BlendOp_Screen: return float4((1.0 - ((1.0 - dest.rgb) * (1.0 - src.rgb))) * src.a, dest.a * src.a);
  case BlendOp_Overlay: return float4(blendOverlay(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  case BlendOp_Lighten: return float4(max(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  case BlendOp_ColorDodge: return float4(blendColorDodge(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  case BlendOp_ColorBurn: return float4(blendColorBurn(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  case BlendOp_HardLight: return float4(blendOverlay(dest.rgb, src.rgb) * src.a, dest.a * src.a);
  case BlendOp_SoftLight: return float4(blendSoftLight(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  case BlendOp_Exclusion: return float4((dest.rgb + src.rgb - 2.0 * dest.rgb * src.rgb) * src.a, dest.a * src.a);
  case BlendOp_Hue: return float4(blendHue(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  case BlendOp_Saturation: return float4(blendSaturation(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  case BlendOp_Color: return float4(blendColor(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  case BlendOp_Luminosity: return float4(blendLuminosity(src.rgb, dest.rgb) * src.a, dest.a * src.a);
  }

  return src;
}

float4 fillMask(VS_OUTPUT input) {
  float4 col = fillImage(input);
  float alpha = texture1.Sample(sampler0, input.ObjectCoord).a;
  return float4(col.rgb * alpha, col.a * alpha);
}

float4 GetCol(in matrix m, uint i) { return float4(m[0][i], m[1][i], m[2][i], m[3][i]); }

#define VISUALIZE_CLIP 0

void applyClip(VS_OUTPUT input, inout float4 outColor) {
  for (uint i = 0; i < ClipSize; i++) {
    matrix data = Clip[i];
    float2 origin = GetCol(data, 0).xy;
    float2 size = GetCol(data, 0).zw;
    float4 radii_x, radii_y;
    Unpack(GetCol(data, 1), radii_x, radii_y);
    bool inverse = bool(GetCol(data, 3).z);

    float2 p = input.ObjectCoord;
    p = transformAffine(p, GetCol(data, 2).xy, GetCol(data, 2).zw, GetCol(data, 3).xy);
    p -= origin;
    float d_clip = sdRoundRect(p, size, radii_x, radii_y) * (inverse ? -1.0 : 1.0);

#if VISUALIZE_CLIP
    if (abs(d_clip) < 3.0)
      outColor = float4(0.9, 1.0, 0.0, 1.0);
#else
    float alpha = antialias2(-d_clip);
    outColor = float4(outColor.rgb * alpha, outColor.a * alpha);
#endif
  }
}

float4 fillGlyph(VS_OUTPUT input) {
  float alpha = texture0.Sample(sampler0, input.TexCoord).a * input.Color.a;
  float fill_color_luma = input.Data0.y;
  float corrected_alpha = texture1.Sample(sampler0, float2(alpha, fill_color_luma)).a;

  return float4(input.Color.rgb * corrected_alpha, corrected_alpha);
}

float4 PS(VS_OUTPUT input) : SV_Target
{
  const uint FillType_Solid = 0u;
  const uint FillType_Image = 1u;
  const uint FillType_Pattern_Image = 2u;
  const uint FillType_Pattern_Gradient = 3u;
  const uint FillType_RESERVED_1 = 4u;
  const uint FillType_RESERVED_2 = 5u;
  const uint FillType_RESERVED_3 = 6u;
  const uint FillType_Rounded_Rect = 7u;
  const uint FillType_Box_Shadow = 8u;
  const uint FillType_Blend = 9u;
  const uint FillType_Mask = 10u;
  const uint FillType_Glyph = 11u;

  float4 outColor = input.Color;

  switch (FillType(input))
  {
  case FillType_Solid: outColor = fillSolid(input); break;
  case FillType_Image: outColor = fillImage(input); break;
  case FillType_Pattern_Image: outColor = fillPatternImage(input); break;
  case FillType_Pattern_Gradient: outColor = fillPatternGradient(input); break;
  case FillType_Rounded_Rect: outColor = fillRoundedRect(input); break;
  case FillType_Box_Shadow: outColor = fillBoxShadow(input); break;
  case FillType_Blend: outColor = fillBlend(input); break;
  case FillType_Mask: outColor = fillMask(input); break;
  case FillType_Glyph: outColor = fillGlyph(input); break;
  }

  applyClip(input, outColor);

  return outColor;
}
