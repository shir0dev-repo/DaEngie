#include "FrameResource.h"

FrameResource::FrameResource(ID3D12Device* device, uint32_t passCount, uint32_t objectCount, uint32_t materialCount)
{
	/*device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&CmdListAlloc));*/

	PassCB = std::make_unique<UploadBuffer<PassConstants>>(device, passCount, true);
	MatCB = std::make_unique<UploadBuffer<MaterialConstants>>(device, materialCount, true);
	ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);
}

FrameResource::~FrameResource() {}