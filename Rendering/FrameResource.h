#pragma once

#include <Support/UploadBuffer.h>
#include <Rendering/RenderBufferConstants.h>
#include <Support/WinInclude.h>
#include <Support/ComPointer.h>

#include <memory>

struct FrameResource {
	ComPointer<ID3D12CommandAllocator> CmdListAlloc;
	std::unique_ptr<UploadBuffer<PassConstants>> PassCB;
	std::unique_ptr<UploadBuffer<MaterialConstants>> MatCB;
	std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB;

	FrameResource();
	FrameResource(ID3D12Device* device, uint32_t passCount, uint32_t objectCount, uint32_t materialCount);
	FrameResource(const FrameResource&) = delete;

	~FrameResource();
};