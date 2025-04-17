#include <Support/WinInclude.h>
#include <d3dcompiler.h>
#include <D3D/DXContext.h>
#include <Support/Window.h>
#include "DXPipeline.h"
#include <Object/MeshData.h>
#include <Object/MeshBuffer.h>
#include <Object/Material.h>
#include <Object/Renderable.h>

bool DXPipeline::Init() {
    return false;
}

void DXPipeline::AddMesh(const MeshData& meshData, const Material* material, const char* name) {
    DXContext::Get().Flush(DXWindow::Get().GetFrameCount());

    auto& cmdAlloc = DXContext::Get().GetCommandAllocator();
    cmdAlloc->Reset();

    DXContext::Get().GetCommandList()->Reset(cmdAlloc, _opaquePSO);

    UINT vertexCount = (uint32_t)(meshData.Vertices.size() & 0xffffffff);
    UINT indexCount = (uint32_t)(meshData.Indices.size() & 0xffffffff);
    UINT vertexBufferSize = (uint32_t)((meshData.Vertices.size() * sizeof(Vertex)) & 0xffffffff);
    UINT indexBufferSize = (uint32_t)((meshData.Indices.size() * sizeof(uint32_t)) & 0xffffffff);
    std::string n(name);
    MeshBuffer* buffer = new MeshBuffer(n, vertexBufferSize, sizeof(Vertex), vertexCount, indexBufferSize, indexCount, meshData);
    _renderables[buffer->Name] = new Renderable(buffer, material, _renderables.size());
}
