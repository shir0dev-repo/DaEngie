#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <Object/MeshData.h>

#include <string>

struct MeshBuffer
{
	MeshBuffer(std::string name,UINT vBufferSize,UINT vBufferStride,UINT vCount,UINT iBufferSize,UINT iCount,const MeshData& meshData);

	// Give it a name so we can look it up by name.
	std::string Name;

	inline D3D12_VERTEX_BUFFER_VIEW GetVBV() {
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}

	inline D3D12_INDEX_BUFFER_VIEW GetIBV() {
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}

	inline D3D12_VERTEX_BUFFER_VIEW GetCBV() {
		D3D12_VERTEX_BUFFER_VIEW cbv;
		cbv.BufferLocation = ColorBufferGPU->GetGPUVirtualAddress();
		cbv.StrideInBytes = ColorByteStride;
		cbv.SizeInBytes = ColorBufferByteSize;

		return cbv;
	}

	ComPointer<ID3DBlob> VertexBufferCPU = nullptr;
	ComPointer<ID3DBlob> IndexBufferCPU = nullptr;
	ComPointer<ID3DBlob> ColorBufferCPU = nullptr;

	ComPointer<ID3D12Resource> VertexBufferGPU = nullptr;
	ComPointer<ID3D12Resource> IndexBufferGPU = nullptr;
	ComPointer<ID3D12Resource> ColorBufferGPU = nullptr;

	ComPointer<ID3D12Resource> VertexBufferUploader = nullptr;
	ComPointer<ID3D12Resource> IndexBufferUploader = nullptr;
	ComPointer<ID3D12Resource> ColorBufferUploader = nullptr;

	// Data about the buffers.
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;

	uint32_t VertexBufferByteSize = 0;
	uint32_t VertexByteStride = 0;
	uint32_t VertexCount = 0;

	uint32_t IndexBufferByteSize = 0;
	uint32_t IndexCount = 0;

	uint32_t ColorByteStride = 0;
	uint32_t ColorBufferByteSize = 0;

private:
	ComPointer<ID3D12Resource> CreateBuffer(UINT bufferSize, const void* initialData, ComPointer<ID3D12Resource>& uploader);
};