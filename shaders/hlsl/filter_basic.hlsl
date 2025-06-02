#include "common.hlsli"

// Basic CSS/SVG filter pixel shader
// Used for ShaderType::FilterBasic

// Helper functions
uint FilterType(VS_OUTPUT input) { return uint(input.data0.x + 0.5); }
float Amount(VS_OUTPUT input) { return input.data0.y; }

// Filter implementations
void filterGrayscale(inout float4 color, float amount) {
    amount = 1.0 - amount;
    color = float4((0.2126 + 0.7874 * amount) * color.r + (0.7152 - 0.7152 * amount) * color.g + (0.0722 - 0.0722 * amount) * color.b,
        (0.2126 - 0.2126 * amount) * color.r + (0.7152 + 0.2848 * amount) * color.g + (0.0722 - 0.0722 * amount) * color.b,
        (0.2126 - 0.2126 * amount) * color.r + (0.7152 - 0.7152 * amount) * color.g + (0.0722 + 0.9278 * amount) * color.b,
        color.a);
}

void filterSepia(inout float4 color, float amount) {
    amount = 1.0 - amount;
    color = float4((0.393 + 0.607 * amount) * color.r + (0.769 - 0.769 * amount) * color.g + (0.189 - 0.189 * amount) * color.b,
        (0.349 - 0.349 * amount) * color.r + (0.686 + 0.314 * amount) * color.g + (0.168 - 0.168 * amount) * color.b,
        (0.272 - 0.272 * amount) * color.r + (0.534 - 0.534 * amount) * color.g + (0.131 + 0.869 * amount) * color.b,
        color.a);
}

void filterSaturate(inout float4 color, float amount) {
    color = float4((0.213 + 0.787 * amount) * color.r + (0.715 - 0.715 * amount) * color.g + (0.072 - 0.072 * amount) * color.b,
        (0.213 - 0.213 * amount) * color.r + (0.715 + 0.285 * amount) * color.g + (0.072 - 0.072 * amount) * color.b,
        (0.213 - 0.213 * amount) * color.r + (0.715 - 0.715 * amount) * color.g + (0.072 + 0.928 * amount) * color.b,
        color.a);
}

void filterHueRotate(inout float4 color, float amount) {
    float c = cos(amount * PI / 180.0);
    float s = sin(amount * PI / 180.0);
    color = float4(color.r * (0.213 + c * 0.787 - s * 0.213) + color.g * (0.715 - c * 0.715 - s * 0.715) + color.b * (0.072 - c * 0.072 + s * 0.928),
        color.r * (0.213 - c * 0.213 + s * 0.143) + color.g * (0.715 + c * 0.285 + s * 0.140) + color.b * (0.072 - c * 0.072 - s * 0.283),
        color.r * (0.213 - c * 0.213 - s * 0.787) + color.g * (0.715 - c * 0.715 + s * 0.715) + color.b * (0.072 + c * 0.928 + s * 0.072),
        color.a);
}

float invert(float n, float a, float amount) { 
    return (a - n) * amount + n * (1.0 - amount); 
}

void filterInvert(inout float4 color, float amount) {
    color = float4(invert(color.r, 1.0, amount), invert(color.g, 1.0, amount), invert(color.b, 1.0, amount), color.a);
}

void filterBrightness(inout float4 color, float amount) {
    color = float4(color.r * amount, color.g * amount, color.b * amount, color.a);
}

float contrast(float n, float amount) { 
    return (n - 0.5) * amount + 0.5; 
}

void filterContrast(inout float4 color, float amount) {
    color = float4(contrast(color.r, amount), contrast(color.g, amount), contrast(color.b, amount), color.a);
}

void filterOpacity(inout float4 color, float amount) {
    color *= amount;
}

float4 main(VS_OUTPUT input) : SV_Target {
    const uint FilterType_Grayscale = 0u;
    const uint FilterType_Sepia = 1u;
    const uint FilterType_Saturate = 2u;
    const uint FilterType_HueRotate = 3u;
    const uint FilterType_Invert = 4u;
    const uint FilterType_Opacity = 5u;
    const uint FilterType_Brightness = 6u;
    const uint FilterType_Contrast = 7u;

    float4 color = Texture0.Sample(Sampler0, input.tex) * input.color;
    float amount = Amount(input);

    switch(FilterType(input)) {
        case FilterType_Grayscale: filterGrayscale(color, amount); break;
        case FilterType_Sepia: filterSepia(color, amount); break;
        case FilterType_Saturate: filterSaturate(color, amount); break;
        case FilterType_HueRotate: filterHueRotate(color, amount); break;
        case FilterType_Invert: filterInvert(color, amount); break;
        case FilterType_Opacity: filterOpacity(color, amount); break;
        case FilterType_Brightness: filterBrightness(color, amount); break;
        case FilterType_Contrast: filterContrast(color, amount); break;
    }

    return color;
}