#pragma once

#include <Support/WinInclude.h>
#include <Object/MeshBuffer.h>
#include <DirectXMath.h>
#include <Object/Material.h>


struct Renderable {
	Renderable(MeshBuffer* meshBuffer, const Material* material, uint32_t objectCBIndex);

	DirectX::XMFLOAT4X4 Transform;
	DirectX::XMFLOAT4X4 TexTransform;

	MeshBuffer* mesh;
	Material* material;
	uint32_t objectCBIndex;

	void Render(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* objectCBuffer, ID3D12Resource* MaterialCBuffer);
};