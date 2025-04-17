#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <D3D/DXContext.h>
#include <Object/Renderable.h>
#include <Rendering/Shader.h>
#include <map>

class DXPipeline {
public:
	bool Init();
	void Shutdown();

	void DrawAllRenderables();
	void AddMesh(const struct MeshData& meshData, struct Material* material, const char* name);
	struct Material CreateMaterial(const char* name, uint32_t CBIndex, uint32_t diffuseSRVHeapIndex, uint32_t normalSRVHeapIndex, DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 fresnelR, float roughness);
	void AddRenderable(struct MeshBuffer* meshBuffer, struct Material* material, uint32_t objCBIndex);

	ID3D12PipelineState* GetOpaquePSO() { return _opaquePSO.Get(); }

private:
	bool CreateRootSignature();
	void CreateShaders();
	bool CreatePSOs();
	bool CreateDescriptorHeaps();
	bool CreateBufferResources();

	ComPointer<ID3D12RootSignature> _rootSignature;
	ComPointer<ID3DBlob> _serializedRootSignature;
	CD3DX12_ROOT_PARAMETER slotRootParams[4];

	D3D12_GRAPHICS_PIPELINE_STATE_DESC _opaquePSODesc;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC _waterPSODesc;
	ComPointer<ID3D12PipelineState> _opaquePSO;
	ComPointer<ID3D12PipelineState> _waterPSO;

	ComPointer<ID3D12Resource> _objectCB;
	ComPointer<ID3D12Resource> _materialCB;
	ComPointer<ID3D12DescriptorHeap> _CBVSRVHeap;

	std::map<std::string, Renderable*> _renderables;
	std::map<std::string, Material*> _materials;
	std::map<std::string, Shader> _shaders;
private:
	DXPipeline(const DXPipeline&) = delete;
	DXPipeline& operator=(const DXPipeline&) = delete;

public:
	inline static DXPipeline& Get() {
		static DXPipeline instance;
		return instance;
	}

private:
	DXPipeline() = default;

	const CD3DX12_STATIC_SAMPLER_DESC staticSamplers[6] = {
		CD3DX12_STATIC_SAMPLER_DESC(
			0, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP), // addressW

	CD3DX12_STATIC_SAMPLER_DESC(
			1, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP), // addressW

	CD3DX12_STATIC_SAMPLER_DESC(
			2, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP), // addressW

	CD3DX12_STATIC_SAMPLER_DESC(
			3, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP), // addressW

	CD3DX12_STATIC_SAMPLER_DESC(
			4, // shaderRegister
			D3D12_FILTER_ANISOTROPIC, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
			0.0f,                             // mipLODBias
			8),                               // maxAnisotropy

	CD3DX12_STATIC_SAMPLER_DESC(
			5, // shaderRegister
			D3D12_FILTER_ANISOTROPIC, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
			0.0f,                              // mipLODBias
			8)
	};
};