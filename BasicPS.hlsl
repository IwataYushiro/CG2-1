#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp      : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//return float4(tex.Sample(smp,input.uv))*color;
	float3 light = normalize(float3(1.0f,-1.0f,1.0f));		//�E�����@�����̃��C�g
	float brightness = dot(-light, input.normal);			//�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	return float4(brightness, brightness, brightness, 1);	//�P�x��RGB�ɑ�����ďo��
}

//float4(��,��,��,���l(1)) �͈́@0.0f�`1.0f