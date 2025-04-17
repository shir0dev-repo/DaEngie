#include <Support/WinInclude.h>
#include "../MeshBuffer.h"
#include "../MeshData.h"
#include <D3D/DXContext.h>
#include <d3dcompiler.h>
#include <Support/Window.h>

MeshBuffer::MeshBuffer(
	std::string name,
	UINT vBufferSize,
	UINT vBufferStride,
	UINT vCount,
	UINT iBufferSize,
	UINT iCount,
	const MeshData& meshData
) {
	this->Name = name;

	D3DCreateBlob(VertexBufferByteSize, &VertexBufferCPU);

	this->VertexBufferByteSize = vBufferSize;
	this->VertexByteStride = vBufferStride;
	this->VertexCount = vCount;
	memcpy(VertexBufferCPU->GetBufferPointer(), meshData.Indices.data(), vBufferSize);
	VertexBufferGPU = CreateBuffer(vBufferSize, meshData.Vertices.data(), VertexBufferUploader);

	D3DCreateBlob(iBufferSize, &IndexBufferCPU);
	IndexBufferByteSize = iBufferSize;
	IndexCount = iCount;
	this->IndexFormat = DXGI_FORMAT_R32_UINT;
	memcpy(IndexBufferCPU->GetBufferPointer(), meshData.Indices.data(), iBufferSize);
	IndexBufferGPU = CreateBuffer(iBufferSize, meshData.Indices.data(), IndexBufferUploader);
}

ComPointer<ID3D12Resource> MeshBuffer::CreateBuffer(UINT bufferSize, const void* initialData, ComPointer<ID3D12Resource>& uploader) {
	ComPointer<ID3D12Resource> defaultBuffer;
	CD3DX12_HEAP_PROPERTIES props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC buffer = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DXContext::Get().GetDevice()->CreateCommittedResource(
		&props,
		D3D12_HEAP_FLAG_NONE,
		&buffer,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&defaultBuffer));

	CD3DX12_HEAP_PROPERTIES uploadProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	DXContext::Get().GetDevice()->CreateCommittedResource(
		&uploadProps,
		D3D12_HEAP_FLAG_NONE,
		&buffer,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploader));

	D3D12_SUBRESOURCE_DATA srData{};
	srData.pData = initialData;
	srData.RowPitch = bufferSize;
	srData.SlicePitch = bufferSize;

	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	UpdateSubresources<1>(DXWindow::Get().GetCommandList(), defaultBuffer, uploader, 0, 0, 1, &srData);
	CD3DX12_RESOURCE_BARRIER transition2 = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	DXWindow::Get().GetCommandList()->ResourceBarrier(1, &transition2);

	return defaultBuffer;
}