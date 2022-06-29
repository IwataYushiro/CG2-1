#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <DirectXMath.h>
#include <wrl.h>

using namespace DirectX;

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//�F(RGBA)

};
class Sprite
{
public: // �����o�֐�
	Sprite();
	~Sprite();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(HRESULT result, ID3D12Device* device);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList);

private://�����o�ϐ�
	struct Vertex
	{
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT2 uv;		//uv���W
	};

	//���_�f�[�^
	Vertex vertices_[4] = {
		// x	 y	  z		 u	  v
		{{-0.5f,-0.7f,0.0f},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-0.5f,+0.7f,0.0f},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{+0.5f,-0.7f,0.0f},{1.0f,1.0f}},//�E��					
		{{+0.5f,+0.7f,0.0f},{1.0f,0.0f}},//�E��					
	};
	
	//�C���f�b�N�X�f�[�^
	unsigned short indices[6] =
	{
		0,1,2,//�O�p�`1��
		1,2,3,//�O�p�`2��
	};

	ID3D12Resource* constBuffMaterial = nullptr;

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
};
