cbuffer ConstBufferDataMaterial:register(b0)
{
	float4 color;
}
float4 main() : SV_TARGET
{
	return color;
}

//float4(��,��,��,���l(1)) �͈́@0.0f�`1.0f