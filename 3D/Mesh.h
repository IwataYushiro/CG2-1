#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dinput.h>
#include <DirectXMath.h>
#include <wrl.h>

using namespace DirectX;

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//�F(RGBA)

};
struct ConstBufferDataTransform
{
	XMMATRIX mat; // 3D�ϊ��s��

};
class Mesh
{
public: // �����o�֐�
	Mesh();
	~Mesh();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(HRESULT result, ID3D12Device* device);

	template<typename T1, typename T2>
	void CreateConstBuffer(T1* cb, ID3D12Device* device, ID3D12Resource*& buffer, T2*& cbm);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(BYTE* keys);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList);

private://�����o�ϐ�

	//�E�B���h�D�T�C�Y
	const int windowWidth = 1280; //����
	const int windowHeight = 720; //�c��

	struct Vertex
	{
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT2 uv;		//uv���W
	};

	//���_�f�[�^
	Vertex vertices_[24] = {
		//�O(Z�Œ�)
		// x	 y	  z		 u	  v
		{{-5.0f ,-5.0f ,-5.0f},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f,  5.0f ,-5.0f},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f ,-5.0f ,-5.0f},{1.0f,1.0f}},//�E��					
		{{ 5.0f , 5.0f ,-5.0f},{1.0f,0.0f}},//�E��	
		//��(�O�ʂ�Z���W�̕������t)
		// x	 y	  z		 u	  v
		{{-5.0f ,-5.0f ,5.0f},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f,  5.0f ,5.0f},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f ,-5.0f ,5.0f},{1.0f,1.0f}},//�E��					
		{{ 5.0f , 5.0f ,5.0f},{1.0f,0.0f}},//�E��

		//��(X�Œ�)
		// x	 y	  z		 u	  v
		{{-5.0f ,-5.0f ,-5.0f},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f ,-5.0f , 5.0f},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{-5.0f , 5.0f ,-5.0f},{1.0f,1.0f}},//�E��					
		{{-5.0f , 5.0f , 5.0f},{1.0f,0.0f}},//�E��	
		//�E(���ʂ�X���W�̕������t)
		// x	 y	  z		 u	  v
		{{ 5.0f ,-5.0f ,-5.0f},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{ 5.0f ,-5.0f , 5.0f},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f , 5.0f ,-5.0f},{1.0f,1.0f}},//�E��					
		{{ 5.0f , 5.0f , 5.0f},{1.0f,0.0f}},//�E��

		//��(Y�Œ�)
		// x	 y	  z		 u	  v
		{{-5.0f ,-5.0f ,-5.0f},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f ,-5.0f , 5.0f},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f ,-5.0f ,-5.0f},{1.0f,1.0f}},//�E��					
		{{ 5.0f ,-5.0f , 5.0f},{1.0f,0.0f}},//�E��	
		//��(���ʂ�Y���W�̕������t)
		// x	 y	  z		 u	  v
		{{-5.0f , 5.0f ,-5.0f},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f , 5.0f , 5.0f},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f , 5.0f ,-5.0f},{1.0f,1.0f}},//�E��					
		{{ 5.0f , 5.0f , 5.0f},{1.0f,0.0f}},//�E��
	};

	//�C���f�b�N�X�f�[�^
	unsigned short indices[36] =
	{
		//�O
		0,1,2,//�O�p�`1��
		1,2,3,//�O�p�`2��
		
		//��(�O�̖ʂ�4���Z)
		4,5,6,//�O�p�`3��
		5,6,7,//�O�p�`4��
		
		//��
		8,9,10,//�O�p�`5��
		9,10,11,//�O�p�`6��
		
		//�E
		12,13,14,//�O�p�`7��
		13,14,15,//�O�p�`8��
		
		//��
		16,17,18,//�O�p�`9��
		17,18,19,//�O�p�`10��
		
		//��
		20,21,22,//�O�p�`11��
		21,22,23,//�O�p�`12��
	};

	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	ID3D12Resource* constBuffMaterial = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;

	//�\���̂�ϐ���
	ConstBufferDataMaterial* cbdm = nullptr;
	ConstBufferDataTransform* cbdt = nullptr;

	//�}�b�s���O�p�̃|�C���^
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;

	//�ˉe�ϊ��s��
	XMMATRIX matprojection;

	//�r���[�ϊ��s��
	XMMATRIX matview;
	//���_
	XMFLOAT3 eye;
	//�����_
	XMFLOAT3 target;
	//������x�N�g��
	XMFLOAT3 up;
	
	//���[���h�ϊ��s��
	XMMATRIX matWorld;
	//�X�P�[�����O�s��
	XMMATRIX matScale;
	//�X�P�[�����O�{��
	XMFLOAT3 scale;
	 
	//��]�s��
	XMMATRIX matRot;
	//��]�p
	XMFLOAT3 rotation;
	
	//���s�ړ��s��
	XMMATRIX matTrans;
	//���W
	XMFLOAT3 position;

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vdView{};

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW idView{};

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;

	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;

	//�p�C�v���C���X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;



	float angle = 0.0f;//�J�����̉�]�p
};
