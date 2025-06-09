#include "common.hlsli"

float4 main(VS_OUTPUT input) : SV_Target {
    return input.color;
}