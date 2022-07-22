#include "Mesh.h"


Mesh::Mesh()
{

}

Mesh::~Mesh()
{
}

void Mesh::Initialize(HRESULT result, ID3D12Device* device)
{
	for (int i = 0; i < VerticesCount_; i++)
	{
		//�O�p�`����ƂɌv�Z
		//�O�p�`�̃C���f�b�N�X�����o���A�ꎞ�I�ȕϐ��ɓ����
		unsigned short Index0 = indices[0];
		unsigned short Index1 = indices[1];
		unsigned short Index2 = indices[2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices_[Index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices_[Index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices_[Index2].pos);
		//p0=>p1,p0=>p2�x�N�g�����v�Z(�x�N�g�����Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��(������1��)
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices_[Index0].normal, normal);
		XMStoreFloat3(&vertices_[Index1].normal, normal);
		XMStoreFloat3(&vertices_[Index2].normal, normal);
	}

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * _countof(vertices_));

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���

	result = device->CreateCommittedResource(
		&heapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐���

	result = device->CreateCommittedResource(
		&heapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}
	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	//�萔�o�b�t�@����
	CreateConstBufferMaterial3d(&material3d_, device);
	CreateConstBufferObject3d(&object3d_, device);
	//�I�u�W�F�N�g�̏�����
	SetObject3d();

	//�r���[�v���W�F�N�V����������
	InitViewProjection();

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//�q���������
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vdView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vdView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vdView.StrideInBytes = sizeof(vertices_[0]);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	idView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	idView.Format = DXGI_FORMAT_R16_UINT;
	idView.SizeInBytes = sizeIB;

#pragma region ���_�V�F�[�_�[

	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",								//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,				//�C���N���[�h�\�ɂ���
		"main", "vs_5_0",								//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	//�G���[�Ȃ�
	if (FAILED(result))
	{
		//errorBlod����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize()),

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				error.begin());
		error += "\n";
		//�G���[���e���o�̓E�B���h�D�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region �s�N�Z���V�F�[�_�[
	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",								//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,				//�C���N���[�h�\�ɂ���
		"main", "ps_5_0",								//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&psBlob, &errorBlob);
	//�G���[�Ȃ�
	if (FAILED(result))
	{
		//errorBlod����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize()),

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				error.begin());
		error += "\n";
		//�G���[���e���o�̓E�B���h�D�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region ���_���C�A�E�g
	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",										//�Z�}���e�B�b�N��
			0,												//�����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X(0�ł���)
			DXGI_FORMAT_R32G32B32_FLOAT,					//�v�f���ƃr�b�g����\��(XYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT)
			0,												//���̓X���b�g�C���f�b�N�X(0�ł���)
			D3D12_APPEND_ALIGNED_ELEMENT,					//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^���(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
			0												//��x�ɕ`�悷��C���X�^���X��(0�ł���)
		},//(1�s�ŏ������������₷��)
		//���W�ȊO�ɐF�A�e�N�X�`��UV����n���ꍇ�͂���ɑ�����
		//�@���x�N�g��		
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//uv���W(1�s�ŏ������������₷��)
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,						//uv��float����Ȃ̂Ńt�H�[�}�b�g��RG����
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�
	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;			//�W���ݒ�
	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	//�J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;			//�[�x�N���b�s���O��L����
	//�u�����h�X�e�[�g�̐ݒ�
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	//�u�����h���ʐݒ�(����{�����œ���)
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100���g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�e�X�g�̒l��0���g��
	//����������(�A���t�@�u�����f�B���O�@�f�t�H���g)
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	//�}�`�̌`��̐ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;								//�`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1;								//1�s�N�Z���ɂ�1�T���v�����O

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[2] = {};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;						//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;							//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@1��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		//�萔�o�b�t�@�r���[(���)
	rootParams[1].Descriptor.ShaderRegister = 1;						//�萔�o�b�t�@�ԍ�
	rootParams[1].Descriptor.RegisterSpace = 0;							//�f�t�H���g�l
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		//�S�ẴV�F�[�_���猩����


	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;						//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);						//���[�g�p�����[�^��

	// ���[�g�V�O�l�`���̃V���A���C�Y

	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();
#pragma endregion

#pragma region �p�C�v���C���X�e�[�g�̐���

	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion
#pragma endregion
}

void Mesh::CreateConstBufferMaterial3d(Material3d* material, ID3D12Device* device)
{
	HRESULT result;

	//�萔�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};		//�q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourseDesc{};
	cbResourseDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourseDesc.Width = (sizeof(material->cbdm) + 0xff) & ~0xff;//256�o�C�g�A���C�������g
	cbResourseDesc.Height = 1;
	cbResourseDesc.DepthOrArraySize = 1;
	cbResourseDesc.MipLevels = 1;
	cbResourseDesc.SampleDesc.Count = 1;
	cbResourseDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourseDesc, //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material->constBuffMaterial));
	assert(SUCCEEDED(result));


	//�萔�o�b�t�@�̃}�b�s���O
	result = material->constBuffMaterial->Map(0, nullptr, (void**)&material->constMapMaterial);//�}�b�s���O
	assert(SUCCEEDED(result));

}
void Mesh::CreateConstBufferObject3d(Object3d* object, ID3D12Device* device)
{
	HRESULT result;

	D3D12_HEAP_PROPERTIES cbHeapProp{};		//�q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourseDesc{};
	cbResourseDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourseDesc.Width = (sizeof(object->cbdt) + 0xff) & ~0xff;//256�o�C�g�A���C�������g
	cbResourseDesc.Height = 1;
	cbResourseDesc.DepthOrArraySize = 1;
	cbResourseDesc.MipLevels = 1;
	cbResourseDesc.SampleDesc.Count = 1;
	cbResourseDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourseDesc, //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffTransform));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = object->constBuffTransform->Map(0, nullptr, (void**)&object->constMapTransform);//�}�b�s���O
	assert(SUCCEEDED(result));
}

void Mesh::SetObject3d()
{
	//�傫��
	object3d_.scale = { 1.0f,1.0f,1.0f };
	//��]
	object3d_.rotation = { 0.0f,0.0f,0.0f };
	//����
	object3d_.position = { 0.0f,0.0f,0.0f };
}
void Mesh::InitViewProjection()
{
	//�E�B���h�D�T�C�Y
	const int windowWidth = 1280; //����
	const int windowHeight = 720; //�c��

	//�������e�ϊ��s��̌v�Z
	matprojection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		(float)windowWidth / windowHeight,
		0.1f, 1000.0f
	);

	//�r���[�ϊ��s��

	//���_
	eye = { 0.0f, 0.0f, -100.0f };
	//�����_
	target = { 0.0f, 0.0f, 0.0f };
	//������x�N�g��
	up = { 0.0f, 1.0f, 0.0f };

	matview = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

}

void Mesh::Update(BYTE* keys, BYTE* preKeys, ID3D12Device* device)
{
	//�F�����X�ɕς��
	UpdateMaterial3d(&material3d_);
	
//����
	ControlView(keys);

	UpdateObject3d(&object3d_, matview, matprojection);
	
	ControlObject3d(&object3d_, keys);
}

void Mesh::UpdateMaterial3d(Material3d* material)
{
	//�����_��
	//�����V�[�h�𐶐�
	std::random_device seed_gen;
	//�����Z���k�E�c�C�X�^�[�̗����G���W���𐶐�
	std::mt19937_64 engine(seed_gen());
	//�����͈͂��w��
	std::uniform_real_distribution<float> dist(0.0f, 0.99f);

	red += 1.0f / 255.0f;
	if (red >= 1.0f)
	{
		red = 1.0f;
		green += 1.0f / 255.0f;
	}
	if (green >= 1.0f)
	{
		green = 1.0f / 255.0f;
		blue += dist(engine);
	}
	if (blue >= 1.0f)
	{
		red = 0.0f;
		green = 0.0f;
		blue = dist(engine);
		alpha = dist(engine);
	}

	//�l���������ނƎ����I�ɓ]�������
	material->constMapMaterial->color = XMFLOAT4(red, green, blue, alpha);

}
void Mesh::UpdateObject3d(Object3d* object, XMMATRIX& matview, XMMATRIX& matprojection)
{
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[�����O���v�Z
	matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(object->rotation.z);
	matRot *= XMMatrixRotationX(object->rotation.x);
	matRot *= XMMatrixRotationY(object->rotation.y);

	matTrans = XMMatrixTranslation(object->position.x, object->position.y, object->position.z);
	//���[���h�s�񍇐�
	object->matWorld = XMMatrixIdentity();
	object->matWorld *= matScale;	//�X�P�[�����O���f
	object->matWorld *= matRot;		//��]���f
	object->matWorld *= matTrans;	//���s�ړ����f

	//�萔�o�b�t�@�փf�[�^�]��
	object->constMapTransform->mat = object->matWorld * matview * matprojection;
}

void Mesh::ControlObject3d(Object3d* object, BYTE* keys)
{
	if (keys[DIK_UP] || keys[DIK_DOWN] || keys[DIK_RIGHT] || keys[DIK_LEFT])
	{
		if (keys[DIK_UP]) { object->position.y += 1.0f; }
		else if (keys[DIK_DOWN]) { object->position.y -= 1.0f; }
		if (keys[DIK_RIGHT]) { object->position.x += 1.0f; }
		else if (keys[DIK_LEFT]) { object->position.x -= 1.0f; }
	}

	if (keys[DIK_W] || keys[DIK_A] || keys[DIK_S] || keys[DIK_D] || keys[DIK_SPACE])
	{
		if (keys[DIK_W]) { object->rotation.y += XMConvertToRadians(1.0f);}
		else if (keys[DIK_S]) { object->rotation.y -= XMConvertToRadians(1.0f); }
		if (keys[DIK_A]) { object->rotation.x += XMConvertToRadians(1.0f); }
		else if (keys[DIK_D]) { object->rotation.x -= XMConvertToRadians(1.0f); }
		if (keys[DIK_SPACE]) { object->rotation.z -= XMConvertToRadians(1.0f); }
	}
}
void Mesh::ControlView(BYTE* keys)
{
	//���_�𑀍�
	if (keys[DIK_Z] || keys[DIK_X])
	{
		if (keys[DIK_Z]) { angle += XMConvertToRadians(1.0f); }
		else if (keys[DIK_X]) { angle -= XMConvertToRadians(1.0f); }

		//angle���W�A������y������ɉ�]�A���a��-100
		eye.x = -100.0f * sinf(angle);
		eye.z = -100.0f * cosf(angle);

		matview = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	}
}

void Mesh::Draw(ID3D12GraphicsCommandList* commandList)
{

	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices_); i++)
	{
		vertMap[i] = vertices_[i];		//���W���R�s�[
	}

//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	commandList->SetPipelineState(pipelineState.Get());
	commandList->SetGraphicsRootSignature(rootSignature.Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	commandList->IASetVertexBuffers(0, 1, &vdView);
	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	commandList->IASetIndexBuffer(&idView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(0, material3d_.constBuffMaterial->GetGPUVirtualAddress());

	//�`��R�}���h
	DrawObject3d(&material3d_,&object3d_, commandList, vdView, idView, _countof(indices));
}

void Mesh::DrawObject3d(Material3d* material,Object3d* object, ID3D12GraphicsCommandList* commandList,
	D3D12_VERTEX_BUFFER_VIEW& vdView, D3D12_INDEX_BUFFER_VIEW& idView, UINT numIndices)
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	commandList->IASetVertexBuffers(0, 1, &vdView);
	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	commandList->IASetIndexBuffer(&idView);
	commandList->SetGraphicsRootConstantBufferView(1, object->constBuffTransform->GetGPUVirtualAddress());

	//�C���f�b�N�X�o�b�t�@���g���ꍇ
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
	
	//commandList->DrawInstanced(_countof(vertices_), 1, 0, 0);	//�S�Ă̒��_���g���ĕ`��
}