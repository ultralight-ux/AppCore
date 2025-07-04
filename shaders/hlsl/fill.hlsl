#include "common.hlsli"
#include "clip.hlsli"

// Fill type extraction from vertex data
uint FillType(VS_OUTPUT input) { return uint(input.data0.x + 0.5); }

// Accessor functions for shader parameters
float4 TileRectUV() { return Vector[0]; }
float2 TileSize() { return Vector[1].zw; }
float2 PatternTransformA() { return Vector[2].xy; }
float2 PatternTransformB() { return Vector[2].zw; }
float2 PatternTransformC() { return Vector[3].xy; }
uint Gradient_NumStops(VS_OUTPUT input) { return uint(input.data0.y + 0.5); }
bool Gradient_IsRadial(VS_OUTPUT input) { return bool(uint(input.data0.z + 0.5)); }
float2 Gradient_P0(VS_OUTPUT input) { return input.data1.xy; }
float2 Gradient_P1(VS_OUTPUT input) { return input.data1.zw; }
float2 Gradient_R0(VS_OUTPUT input) { return input.data2.xy; }
float2 Gradient_R1(VS_OUTPUT input) { return input.data2.zw; }
float SDFMaxDistance(VS_OUTPUT input) { return input.data0.y; }

struct GradientStop { 
    float percent; 
    float4 color; 
};

GradientStop GetGradientStop(VS_OUTPUT input, uint offset) {
    GradientStop result;
    if (offset < 3) {
        result.percent = input.data3[offset];
        if (offset == 0)
            result.color = input.data4;
        else if (offset == 1)
            result.color = input.data5;
        else if (offset == 2)
            result.color = input.data6;
    } else {
        result.percent = Scalar(offset - 3);
        result.color = Vector[offset - 3];
    }
    return result;
}

// Fill implementations
float4 fillSolid(VS_OUTPUT input) {
    return input.color;
}

float4 fillImage(VS_OUTPUT input) {
    return Texture0.Sample(Sampler0, input.tex) * input.color;
}

float4 fillPatternImage(VS_OUTPUT input) {
    float4 tile_rect_uv = TileRectUV();
    float2 tile_size = TileSize();

    float2 p = input.obj;

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

    return Texture0.Sample(Sampler0, uv) * input.color;
}

float4 fillPatternGradient(VS_OUTPUT input) {
    float num_stops = Gradient_NumStops(input);
    bool is_radial = Gradient_IsRadial(input);
    float2 p0 = Gradient_P0(input);
    float2 p1 = Gradient_P1(input);
    float2 r0 = Gradient_R0(input);
    float2 r1 = Gradient_R1(input);
    float4 out_Color = float4(0.0, 0.0, 0.0, 0.0);

    float t = 0.0;
    float2 texCoord = input.tex;
    
    if (is_radial) {
        // Calculate t parameter for radial gradient
        float2 delta = texCoord - p0;  // Vector from center to current point
        
        // Calculate normalized distance in elliptical space
        // By dividing vector components by respective radii, we transform
        // the ellipse into a unit circle, simplifying distance calculations
        float2 deltaOuter = delta / r1;
        float distOuter = length(deltaOuter);
        
        if (r0.x > 0.0001 && r0.y > 0.0001) {
            // Handle gradient with inner radius (like a ring)
            float2 deltaInner = delta / r0;
            float distInner = length(deltaInner);
            
            // Map t value: 0.0 at inner edge, 1.0 at outer edge
            t = (distInner <= 1.0) ? 0.0 : 
                (distOuter >= 1.0) ? 1.0 : 
                saturate((distInner - 1.0) / (distInner - distOuter));
        } else {
            // Simple gradient from center point
            t = saturate(distOuter);
        }
        
        // Apply focal point adjustment if p1 is different from p0
        if (length(p1 - p0) > 0.0001) {
            float2 focalDir = normalize(p0 - p1);    // Direction from focal to center
            float2 pixelDir = normalize(texCoord - p1); // Direction from focal to pixel
            
            // Adjust t based on angle between directions
            // This creates a more natural gradient when focal != center
            float angleFactor = dot(pixelDir, focalDir);
            t = saturate(t * (1.0 + (1.0 - angleFactor) * 0.5));
        }
    } else {
        // Linear gradient calculation
        float2 V = p1 - p0;
        t = saturate(dot(texCoord - p0, V) / dot(V, V));
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
                        if (num_stops > 7) {
                            GradientStop stop7 = GetGradientStop(input, 7u);
                            out_Color = lerp(out_Color, stop7.color, ramp(stop6.percent, stop7.percent, t));
                            if (num_stops > 8) {
                                GradientStop stop8 = GetGradientStop(input, 8u);
                                out_Color = lerp(out_Color, stop8.color, ramp(stop7.percent, stop8.percent, t));
                                if (num_stops > 9) {
                                    GradientStop stop9 = GetGradientStop(input, 9u);
                                    out_Color = lerp(out_Color, stop9.color, ramp(stop8.percent, stop9.percent, t));
                                    if (num_stops > 10) {
                                        GradientStop stop10 = GetGradientStop(input, 10u);
                                        out_Color = lerp(out_Color, stop10.color, ramp(stop9.percent, stop10.percent, t));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return float4(out_Color.rgb, out_Color.a);
}

float4 blend(float4 src, float4 dest) {
    float4 result;
    result.rgb = src.rgb + dest.rgb * (1.0 - src.a);
    result.a = src.a + dest.a * (1.0 - src.a);
    return result;
}

float4 fillRoundedRect(VS_OUTPUT input) {
    float2 p = input.tex;
    float2 size = input.data0.zw;
    p = (p - 0.5) * size;
    float d = sdRoundRect(p, size, input.data1, input.data2);

    // Fill background
    float alpha = antialias(-d, AA_WIDTH, 0.0) * input.color.a;
    float4 outColor = float4(input.color.rgb * alpha, alpha);

    // Draw stroke
    float stroke_width = input.data3.x;
    float4 stroke_color = input.data4;

    if (stroke_width > 0.0) {
        alpha = innerStroke(stroke_width, d);
        alpha *= stroke_color.a;
        float4 stroke = float4(stroke_color.rgb * alpha, alpha);
        outColor = blend(stroke, outColor);
    }

    return outColor;
}

float4 fillBoxShadow(VS_OUTPUT input) {
    // NO-OP: deprecated function
    return input.color;
}

// Blend mode helper functions
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
    float4 dest = Texture1.Sample(Sampler0, input.obj);

    switch(uint(input.data0.y + 0.5))
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
    float alpha = Texture1.Sample(Sampler0, input.obj).a;
    return float4(col.rgb * alpha, col.a * alpha);
}

float4 fillGlyph(VS_OUTPUT input) {
    float alpha = Texture0.Sample(Sampler0, input.tex).a * input.color.a;
    float fill_color_luma = input.data0.y;
    float corrected_alpha = Texture1.Sample(Sampler0, float2(alpha, fill_color_luma)).a;

    return float4(input.color.rgb * corrected_alpha, corrected_alpha);
}

// Main pixel shader entry point
float4 main(VS_OUTPUT input) : SV_Target
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

    float4 outColor = input.color;

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

    applyClip(input.obj, outColor);

    return outColor;
}