#include "Basic.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
	return float4(input.uv,0,1);
}

//float4(��,��,��,���l(1)) �͈́@0.0f�`1.0f