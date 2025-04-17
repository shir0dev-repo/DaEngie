#include <iostream>

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <Support/Window.h>

#include <DXDebug/DXDebugLayer.h>
#include <D3D/DXContext.h>
#include <D3D/DXPipeline.h>
#include <Object/MeshData.h>
#include <Utils/GeometryGenerator.h>

int main() {
	DXDebugLayer::Get().Init();

	if (DXContext::Get().Init() && DXWindow::Get().Init() && DXPipeline::Get().Init()) {

		/*D3D12_HEAP_PROPERTIES hp{};
		hp.Type = D3D12_HEAP_TYPE_UPLOAD;
		hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		hp.CreationNodeMask = 0;
		hp.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC rd{};
		rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		rd.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		rd.Width = 1024;
		rd.Height = 1;
		rd.DepthOrArraySize = 1;
		rd.MipLevels = 1;
		rd.Format = DXGI_FORMAT_UNKNOWN;
		rd.SampleDesc.Count = 1;
		rd.SampleDesc.Quality = 0;
		rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		rd.Flags = D3D12_RESOURCE_FLAG_NONE;
		ComPointer<ID3D12Resource2> uploadBuffer;
		DXContext::Get().GetDevice()->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&uploadBuffer));*/

		MeshData box = GeometryGenerator::CreateBox(1.f, 1.f, 1.f, 0);
		Material mat = DXPipeline::Get().CreateMaterial("cum", 0, 0, 0, { 1.f, 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f }, 1.0f);
		DXPipeline::Get().AddMesh(box, &mat, "cumBox");
		
		while (!DXWindow::Get().ShouldClose()) {
			DXWindow::Get().Update();

			if (DXWindow::Get().ShouldResize()) {
				DXWindow::Get().Flush(DXWindow::Get().GetFrameCount());
				DXWindow::Get().Resize();
			}
			// Begin drawing
			DXWindow::Get().InitCommandList();
			// Begin Frame
			DXWindow::Get().BeginFrame();
			// TODO: Draw Frame
			DXPipeline::Get().DrawAllRenderables();
			// EndFrame
			DXWindow::Get().EndFrame();

			DXWindow::Get().ExecuteCommandLists();
			DXWindow::Get().Present();
		}

		DXWindow::Get().Flush(DXWindow::Get().GetFrameCount());
		DXWindow::Get().Shutdown();
		DXContext::Get().Shutdown();
		DXPipeline::Get().Shutdown();
	}

	
	DXDebugLayer::Get().Shutdown();

	return 0;
}