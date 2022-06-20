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
	void Initialize(HRESULT result,ID3D12Device* device);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList);

private://�����o�ϐ�
	
	XMFLOAT3 vertices_;
	ID3D12Resource* constBuffMaterial = nullptr;
	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vdView{};

};

