#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dinput.h>
#include <math.h>
#include <string>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <random>

using namespace DirectX;

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//�F(RGBA)

};
//���W
struct ConstBufferDataTransform
{
	XMMATRIX mat; // 3D�ϊ��s��

};
class Mesh
{
public: // �����o�֐�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	
	struct Material3d
	{
		//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
		ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

		//�\���̂�ϐ���
		ConstBufferDataMaterial* cbdm = nullptr;

		//�}�b�s���O�p�̃|�C���^
		ConstBufferDataMaterial* constMapMaterial = nullptr;

	};
	//3D�}�e���A��
	Material3d material3d_;
	
	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	float alpha = 1.0f;

	//3D�I�u�W�F�N�g�^
	struct Object3d
	{
		//�萔�o�b�t�@(�s��p)
		ComPtr<ID3D12Resource> constBuffTransform = nullptr;
		//�\���̂�ϐ���
		ConstBufferDataTransform* cbdt = nullptr;
		//�萔�o�b�t�@�}�b�v(�s��p)
		ConstBufferDataTransform* constMapTransform = nullptr;
		//�A�t�B���ϊ����
		XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
		XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
		XMFLOAT3 position = { 0.0f,0.0f,0.0f };
		//���[���h�ϊ��s��
		XMMATRIX matWorld = XMMatrixIdentity();
		//�e�I�u�W�F�N�g�ւ̃|�C���^
		Object3d* parent = nullptr;
	};
	Object3d object3d_;
public:
	Mesh();
	~Mesh();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(HRESULT result, ID3D12Device* device);

	//�萔�o�b�t�@�쐬
	void CreateConstBufferMaterial3d(Material3d* material, ID3D12Device* device);
	void CreateConstBufferObject3d(Object3d* object, ID3D12Device* device);
	//�I�u�W�F�N�g�̏�����
	void SetObject3d();
	void InitViewProjection();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(BYTE* keys, BYTE* preKeys, ID3D12Device* device);
	//�}�e���A���̍X�V
	void UpdateMaterial3d(Material3d* material);

	//�I�u�W�F�N�g�̍X�V
	void UpdateObject3d(Object3d* object, XMMATRIX& matview, XMMATRIX& matprojection);
	void ControlObject3d(Object3d* object, BYTE* keys);
	//�r���[����
	void ControlView(BYTE* keys);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList);

	void DrawObject3d(Material3d* material, Object3d* object,
		ID3D12GraphicsCommandList* commandList,
		D3D12_VERTEX_BUFFER_VIEW& vdView, D3D12_INDEX_BUFFER_VIEW& idView, UINT numIndices);

private://�����o�ϐ�
	//���_��
	static const size_t VerticesCount_ = 4;
	//�C���f�b�N�X��
	static const size_t indicesCount_ = 3;

	struct Vertex
	{
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		//uv���W
	};

	Vertex vertices_[VerticesCount_] = {
		//�O(Z�Œ�)
		// x	 y	  z		  �@��  u	  v
		{{-50.0f ,-50.0f ,50.0f},{}, {0.0f,1.0f}},//����	X��-�ō��@Y��-�ŉ�
		{{-50.0f,  50.0f ,50.0f},{}, {0.0f,0.0f}},//����	X��+�ŉE�@Y��+�ŏ�
		{{ 50.0f ,-50.0f ,50.0f},{}, {1.0f,1.0f}},//�E��	
		{{ 50.0f , 50.0f ,50.0f},{},{1.0f,0.0f}},//�E��	
		
	};
	ComPtr<ID3D12Resource> vertBuff = nullptr;

	//�C���f�b�N�X�f�[�^
	unsigned short indices[indicesCount_] =
	{
		//�O
		0,1,2,//�O�p�`1��
		
	};
	//�C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff = nullptr;

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
	float angle = 0.0f;//�J�����̉�]�p
	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vdView{};
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW idView{};

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	//���_�V�F�[�_��
	ComPtr<ID3DBlob> vsBlob = nullptr;		//���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr;		//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr;	//�G���[�I�u�W�F�N�g
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	//�p�C�v���C���X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
};

