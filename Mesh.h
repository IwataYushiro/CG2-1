#pragma once
#include <Windows.h>
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
class Mesh
{
public: // �����o�֐�
	Mesh();
	~Mesh();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private://�����o�ϐ�
	HRESULT result;
	ID3D12Device* device = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	
	XMFLOAT3 vertices[3][10] = {
		{
		{-0.5f,-0.5f,0.0f},//����	X��-�ō��@Y��-�ŉ�
		{-0.5f,+0.5f,0.0f},//����	X��-�ō��@Y��+�ŏ�
		{+0.5f,-0.5f,0.0f},
		}
		//�E��	X��+�ŉE�@Y��-�ŉ�
	};
	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB;

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};

	//���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;

	//�萔�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};		//�q�[�v�ݒ�

	ID3D12Resource* constBuffMaterial = nullptr;
	
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourseDesc{};

	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	XMFLOAT3* vertMap = nullptr;

	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC* inputLayout = nullptr;

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vdView{};

	ID3DBlob* vsBlob = nullptr;		//���_�V�F�[�_�[�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr;		//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr;	//�G���[�I�u�W�F�N�g

};

