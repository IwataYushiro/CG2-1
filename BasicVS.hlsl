float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}
//�@return pos + float4(<-X�I�t�Z�b�g+>,Y�I�t�Z�b�g,0,0) �w�肵�����������
//�@return pos * float4(X�g��, Y�g��, Y�k��, Y�k��) �w�肵�������g�k�����