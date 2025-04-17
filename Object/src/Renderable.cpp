#include "../Renderable.h"
#include <Rendering/RenderBufferConstants.h>

#ifndef GetBufferByteSize
#define GetBufferByteSize(T) (sizeof(T) + 0xff) & ~0xff
#endif

Renderable::Renderable(MeshBuffer* meshBuffer, Material* material, uint32_t objectCBIndex) {
	this->mesh = meshBuffer;
	this->material = material;
	this->objectCBIndex = objectCBIndex;
}

void Renderable::Render(ID3D12GraphicsCommandList* cmdList, ID3D12Resource* objectCBuffer, ID3D12Resource* MaterialCBuffer) {
	uint32_t matCBByteSize = GetBufferByteSize(MaterialConstants);
}