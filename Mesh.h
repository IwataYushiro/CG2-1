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

using namespace DirectX;

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;	//�F(RGBA)

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
public:
	Mesh();
	~Mesh();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(HRESULT result, ID3D12Device* device);

	void CreateConstBufferMaterial3d(Material3d* material, ID3D12Device* device);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList);

private://�����o�ϐ�
	//���_��
	static const size_t VerticesCount_ = 3;

	XMFLOAT3 vertices_[VerticesCount_] = {
		{-0.5f,-0.5f,0.0f},//����	X��-�ō��@Y��-�ŉ�
		{-0.5f,+0.5f,0.0f},//����	X��-�ō��@Y��+�ŏ�
		{+0.5f,-0.5f,0.0f},//�E��	X��+�ŉE�@Y��-�ŉ�
	};

	ComPtr<ID3D12Resource> vertBuff = nullptr;

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vdView{};

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	XMFLOAT3* vertMap = nullptr;
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

