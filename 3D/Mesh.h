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
struct Material3d
{
	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	ID3D12Resource* constBuffMaterial = nullptr;
	
	//�\���̂�ϐ���
	ConstBufferDataMaterial* cbdm = nullptr;
	
	//�}�b�s���O�p�̃|�C���^
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	
};
//3D�I�u�W�F�N�g�^
struct Object3d
{
	//�萔�o�b�t�@(�s��p)
	ID3D12Resource* constBuffTransform;
	//�\���̂�ϐ���
	ConstBufferDataTransform* cbdt = nullptr;
	//�萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataTransform* constMapTransform;
	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	//���[���h�ϊ��s��
	XMMATRIX matWorld;
	//�e�I�u�W�F�N�g�ւ̃|�C���^
	Object3d* parent = nullptr;
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
	//�萔�o�b�t�@�̐ݒ�
	void CreateConstBufferMaterial3d(Material3d* material, ID3D12Device* device);
	void CreateConstBufferObject3d(Object3d* object, ID3D12Device* device);
	/// <summary>
	/// ���t���[������
	
	/// </summary>

	//�����_�[�^�[�Q�b�g�ݒ�
	void GetRenderTargetView(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);
	//��ʃN���A�ݒ�
	void ClearScreen(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);

	void Update(BYTE* keys);
	void UpdateObject3d(Object3d* object, XMMATRIX& matview, XMMATRIX& matprojection);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList);
	//�����I�u�W�F�N�g�`��p
	void DrawObject3d(Object3d* object,ID3D12GraphicsCommandList* commandList,
		D3D12_VERTEX_BUFFER_VIEW& vdView,D3D12_INDEX_BUFFER_VIEW& idView,UINT numIndices);
private://�����o�ϐ�
	//���_��
	static const int VerticesCount_ = 24;
	//�C���f�b�N�X��
	static const int indicesCount_ = 36;
	//�I�u�W�F�N�g��
	static const size_t kObjectCount_ = 50;
	//3D�}�e���A��
	Material3d material3d_;
	//3D�I�u�W�F�N�g�̔z��
	Object3d object3ds_[kObjectCount_];
	
	//�E�B���h�D�T�C�Y
	const int windowWidth = 1280; //����
	const int windowHeight = 720; //�c��

	struct Vertex
	{
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		//uv���W
	};

	//���_�f�[�^
	Vertex vertices_[VerticesCount_] = {
		//�O(Z�Œ�)
		// x	 y	  z		  �@��u	  v
		{{-5.0f ,-5.0f ,-5.0f},{},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f,  5.0f ,-5.0f},{},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f ,-5.0f ,-5.0f},{},{1.0f,1.0f}},//�E��					
		{{ 5.0f , 5.0f ,-5.0f},{},{1.0f,0.0f}},//�E��	
		//��(�O�ʂ�Z���W�̕������t)
		// x	 y	  z		 u	  v
		{{-5.0f ,-5.0f ,5.0f},{},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f,  5.0f ,5.0f},{},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f ,-5.0f ,5.0f},{},{1.0f,1.0f}},//�E��					
		{{ 5.0f , 5.0f ,5.0f},{}, {1.0f,0.0f}},//�E��

		//��(X�Œ�)
		// x	 y	  z		 u	  v
		{{-5.0f ,-5.0f ,-5.0f},{},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f ,5.0f ,-5.0f},{},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{-5.0f ,-5.0f ,5.0f},{},{1.0f,1.0f}},//�E��					
		{{-5.0f , 5.0f , 5.0f},{},{1.0f,0.0f}},//�E��	
		//�E(���ʂ�X���W�̕������t)
		// x	 y	  z		 u	  v
		{{ 5.0f ,-5.0f ,-5.0f},{}, {0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{ 5.0f ,5.0f , -5.0f},{},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f ,-5.0f ,5.0f},{},{1.0f,1.0f}},//�E��					
		{{ 5.0f , 5.0f , 5.0f},{},{1.0f,0.0f}},//�E��

		//��(Y�Œ�)
		// x	 y	  z		 u	  v
		{{-5.0f ,-5.0f ,-5.0f},{},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f ,-5.0f , 5.0f},{},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f ,-5.0f ,-5.0f},{},{1.0f,1.0f}},//�E��					
		{{ 5.0f ,-5.0f , 5.0f},{},{1.0f,0.0f}},//�E��	
		//��(���ʂ�Y���W�̕������t)
		// x	 y	  z		 u	  v
		{{-5.0f , 5.0f ,-5.0f},{},{0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-5.0f , 5.0f , 5.0f},{},{0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 5.0f , 5.0f ,-5.0f},{},{1.0f,1.0f}},//�E��					
		{{ 5.0f , 5.0f , 5.0f},{}, {1.0f,0.0f}},//�E��
	};

	//�C���f�b�N�X�f�[�^
	unsigned short indices[indicesCount_] =
	{
		//�O
		0,1,2,//�O�p�`1��
		2,1,3,//�O�p�`2��

		//��(�O�̖ʂ�4���Z)
		6,5,4,//�O�p�`3��
		7,5,6,//�O�p�`4��

		//��
		10,9,8,//�O�p�`5��
		10,11,9,//�O�p�`6��

		//�E
		14,12,13,//�O�p�`7��
		15,14,13,//�O�p�`8��

		//��
		18,17,16,//�O�p�`9��
		19,17,18,//�O�p�`10��

		//��
		22,20,21,//�O�p�`11��
		22,21,23,//�O�p�`12��

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

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vdView{};

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW idView{};

	//�[�x�r���[�p�̃f�X�N���v�^�q�[�v
	ID3D12DescriptorHeap* dsvHeap = nullptr;

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;

	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;

	//�p�C�v���C���X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	float angle = 0.0f;//�J�����̉�]�p
};
