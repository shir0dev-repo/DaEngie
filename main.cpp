#include <iostream>

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <Support/Window.h>

#include <Debug/DXDebugLayer.h>
#include <D3D/DXContext.h>

int main() {
	DXDebugLayer::Get().Init();

	if (DXContext::Get().Init() && DXWindow::Get().Init()) {

		D3D12_HEAP_PROPERTIES hp{};
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
		DXContext::Get().GetDevice()->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&uploadBuffer));

		D3D12_INPUT_ELEMENT_DESC vertexLayout[] = {
			{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		// === Pipeline State ===
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gfxPsod{};
		gfxPsod.InputLayout.NumElements = _countof(vertexLayout);
		gfxPsod.InputLayout.pInputElementDescs = vertexLayout;
		gfxPsod.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		while (!DXWindow::Get().ShouldClose()) {
			DXWindow::Get().Update();

			if (DXWindow::Get().ShouldResize()) {
				DXContext::Get().Flush(DXWindow::Get().GetFrameCount());
				DXWindow::Get().Resize();
			}
			// Begin drawing
			auto* cmdList = DXContext::Get().InitCommandList();
			// Begin Frame
			DXWindow::Get().BeginFrame(cmdList);
			// TODO: Draw Frame

			// EndFrame
			DXWindow::Get().EndFrame(cmdList);

			DXContext::Get().ExecuteCommandList();
			DXWindow::Get().Present();
		}

		DXContext::Get().Flush(DXWindow::Get().GetFrameCount());

		DXWindow::Get().Shutdown();
		//DXContext::Get().Shutdown();
	}

	
	DXDebugLayer::Get().Shutdown();

	return 0;
}