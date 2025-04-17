#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <Utils/MathHelper.h>

template<typename T> class UploadBuffer {
public:
	UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : _isConstantBuffer(isConstantBuffer) {
		_elementByteSize = isConstantBuffer ? GetBufferByteSize(T) : sizeof(T);

		auto upload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(_elementByteSize * elementCount);
		device->CreateCommittedResource(
			&upload,
			D3D12_HEAP_FLAG_NONE,
			&buffer,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_uploadBuffer));

		_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData));
	}

	~UploadBuffer() {
		if (_uploadBuffer) {
			_uploadBuffer->Unmap(0, nullptr);
		}
	}

private:
	ComPointer<ID3D12Resource> _uploadBuffer;
	BYTE* _mappedData = nullptr;
	UINT _elementByteSize;
	bool _isConstantBuffer;
};