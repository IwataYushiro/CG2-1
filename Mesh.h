#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl.h>

using namespace DirectX;

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
	XMFLOAT3 vertices[];
};

