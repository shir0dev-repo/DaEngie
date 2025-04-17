#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <D3D/DXContext.h>
#include <Object/Renderable.h>
#include <map>

class DXPipeline {
public:
	bool Init();
	void Shutdown();

	void AddMesh(const struct MeshData& meshData, const struct Material* material, const char* name);

private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC _opaquePSODesc;
	ComPointer<ID3D12PipelineState> _opaquePSO;
	ComPointer<ID3D12PipelineState> _transparentPSO;

	std::map<std::string, Renderable*> _renderables;
private:
	
	DXPipeline(const DXPipeline&) = delete;
	DXPipeline& operator=(const DXPipeline&) = delete;

	inline static DXPipeline& Get() {
		static DXPipeline instance;
		return instance;
	}

private:
	DXPipeline() = default;
};