#include "../Renderable.h"
#include <Rendering/RenderBufferConstants.h>
#include <Utils/MathHelper.h>

using DirectX::XMMATRIX;

Renderable::Renderable(MeshBuffer* meshBuffer, Material* material, uint32_t objectCBIndex) {
	this->mesh = meshBuffer;
	this->material = material;
	this->objectCBIndex = objectCBIndex;
}

void Renderable::Render(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* objectCBuffer, ID3D12Resource* MaterialCBuffer) {
	uint32_t matCBByteSize = GetBufferByteSize(MaterialConstants);
	uint32_t objCBByteSize = GetBufferByteSize(ObjectConstants);

	D3D12_VERTEX_BUFFER_VIEW vbv = mesh->GetVBV();
	D3D12_INDEX_BUFFER_VIEW ibv = mesh->GetIBV();
	ObjectConstants objC;
	XMMATRIX world = XMLoadFloat4x4(&Transform);

	cmdList->IASetVertexBuffers(0, 1, &vbv);
	cmdList->IASetIndexBuffer(&ibv);
	cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddr = objectCBuffer->GetGPUVirtualAddress() + objectCBIndex * static_cast<unsigned long long>(objCBByteSize);
	D3D12_GPU_VIRTUAL_ADDRESS matCBAddr = MaterialCBuffer->GetGPUVirtualAddress() + material->MatCBIndex * static_cast<unsigned long long>(matCBByteSize);

	cmdList->SetGraphicsRootConstantBufferView(0, objCBAddr);
	cmdList->SetGraphicsRootConstantBufferView(1, matCBAddr);

	cmdList->DrawIndexedInstanced(mesh->IndexCount, 1, 0, 0, 0);
}