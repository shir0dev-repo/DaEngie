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
    if (!CreateRootSignature()) return false;
    CreateShaders();
    if (!CreatePSOs()) return false;
    if (!CreateDescriptorHeaps()) return false;
    if (!CreateBufferResources()) return false;

    return true;
}

bool DXPipeline::CreateRootSignature() {
    CD3DX12_ROOT_PARAMETER rootParams[4];
    D3D12_DESCRIPTOR_RANGE textureRange{};
    textureRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    textureRange.NumDescriptors = 1;
    textureRange.BaseShaderRegister = 0;
    textureRange.RegisterSpace = 0;
    textureRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // Descriptor table for textures
    rootParams[0].InitAsDescriptorTable(1, &textureRange, D3D12_SHADER_VISIBILITY_PIXEL);
    rootParams[1].InitAsConstantBufferView(0);
    rootParams[2].InitAsConstantBufferView(1);
    rootParams[3].InitAsConstantBufferView(2);

    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Init(
        _countof(rootParams), rootParams,
        0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPointer<ID3DBlob> serializedSig;
    ComPointer<ID3DBlob> errorBlob;
    HRESULT result = D3D12SerializeRootSignature(
        &rootSigDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &serializedSig,
        &errorBlob);

    if (FAILED(result)) {
        OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        return false;
    }

    result = DXContext::Get().GetDevice()->CreateRootSignature(
        0,
        serializedSig->GetBufferPointer(),
        serializedSig->GetBufferSize(),
        IID_PPV_ARGS(&_rootSignature));

    if (FAILED(result)) {
        return false;
    }

    return true;
}

void DXPipeline::CreateShaders()
{
    _shaders["standardVS"] = Shader(L"Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
    _shaders["standardPS"] = Shader(L"Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");
}

bool DXPipeline::CreatePSOs()
{
    D3D12_INPUT_ELEMENT_DESC vertexLayout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
    ID3D12Device* device = DXContext::Get().GetDevice();
    
    D3D12_GRAPHICS_PIPELINE_STATE_DESC opqDesc{};
    memset(&opqDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

    opqDesc.InputLayout = { &vertexLayout[0], _countof(vertexLayout)};
    opqDesc.pRootSignature = _rootSignature.Get();
    // TODO: add shaders ?
    opqDesc.VS = { _shaders["standardVS"].GetResource()->GetBufferPointer(), _shaders["standardVS"].GetResource()->GetBufferSize()};
    opqDesc.PS = { _shaders["standardPS"].GetResource()->GetBufferPointer(), _shaders["standardPS"].GetResource()->GetBufferSize()};
    opqDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    opqDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    opqDesc.DepthStencilState.DepthEnable = false;
    opqDesc.DepthStencilState.StencilEnable = false;
    opqDesc.SampleMask = 0;
    opqDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    opqDesc.NumRenderTargets = 1;
    opqDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    opqDesc.SampleDesc.Count = 1;

    if (FAILED(device->CreateGraphicsPipelineState(&opqDesc, IID_PPV_ARGS(&_opaquePSO)))) {
        return false;
    }

    return true;
}

bool DXPipeline::CreateDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvSrvHeap{};
    cbvSrvHeap.NumDescriptors = 1;
    cbvSrvHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvSrvHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    ID3D12Device* device = DXContext::Get().GetDevice();
    HRESULT result = device->CreateDescriptorHeap(&cbvSrvHeap, IID_PPV_ARGS(&_CBVSRVHeap));
    if (FAILED(result)) {
        return false;
    }

    return true;
}

bool DXPipeline::CreateBufferResources()
{
    UINT descriptorSize = DXContext::Get().GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
    cbvDesc.BufferLocation;
    return true;
}

void DXPipeline::Shutdown()
{
    _serializedRootSignature.Release();
    _rootSignature.Release();
}

void DXPipeline::DrawAllRenderables()
{
    auto cmdList = DXWindow::Get().GetCommandList();
    for (auto r : _renderables) {
    }
}

void DXPipeline::AddMesh(const MeshData& meshData, Material* material, const char* name) {
    DXWindow::Get().Flush(DXWindow::Get().GetFrameCount());

    auto cmdAlloc = DXWindow::Get().GetCommandAllocator();
    cmdAlloc->Reset();

    DXWindow::Get().GetCommandList()->Reset(cmdAlloc, _opaquePSO);

    UINT vertexCount = (uint32_t)(meshData.Vertices.size() & 0xffffffff);
    UINT indexCount = (uint32_t)(meshData.Indices.size() & 0xffffffff);
    UINT vertexBufferSize = (uint32_t)((meshData.Vertices.size() * sizeof(Vertex)) & 0xffffffff);
    UINT indexBufferSize = (uint32_t)((meshData.Indices.size() * sizeof(uint32_t)) & 0xffffffff);
    std::string n(name);
    MeshBuffer* buffer = new MeshBuffer(n, vertexBufferSize, sizeof(Vertex), vertexCount, indexBufferSize, indexCount, meshData);
    _renderables[buffer->Name] = new Renderable(buffer, material, _renderables.size());
}

void DXPipeline::AddRenderable(MeshBuffer* meshBuffer, Material* material, uint32_t objCBIndex) {
    _renderables[meshBuffer->Name] = new Renderable(meshBuffer, material, objCBIndex);
}



Material DXPipeline::CreateMaterial(const char* name, uint32_t CBIndex, uint32_t diffuseSRVHeapIndex, uint32_t normalSRVHeapIndex, DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 fresnelR, float roughness)
{
    Material* m = new Material();
    m->Name = name;
    m->MatCBIndex = CBIndex;
    m->DiffuseSrvHeapIndex = diffuseSRVHeapIndex;
    m->NormalSrvHeapIndex = normalSRVHeapIndex;
    m->DiffuseAlbedo = color;
    m->FresnelR0 = fresnelR;
    m->Roughness = roughness;

    _materials[m->Name] = m;

    // TODO: rebuild frame resources
    return *m;
}
