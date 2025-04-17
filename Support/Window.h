#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <D3D/DXContext.h>
#include <Rendering/FrameResource.h>

#include <memory>

class DXWindow {
public:
	bool Init();
	void Update();
	void Present();
	void Shutdown();
	void Resize();

	ID3D12GraphicsCommandList* InitCommandList();
	void BeginFrame();
	void EndFrame();
	void ExecuteCommandLists();

	inline bool ShouldClose() const { return _shouldClose; }
	inline bool ShouldResize() const { return _shouldResize; }

	inline ID3D12CommandQueue* GetCommandQueue() { return _cmdQueue.Get(); }
	inline ID3D12GraphicsCommandList* GetCommandList() { return _commandList.Get(); }
	inline ID3D12CommandAllocator* GetCommandAllocator() { return _allocators[_swapChain->GetCurrentBackBufferIndex()].Get(); }
	inline FrameResource* GetCurrFrameResource() { return _frameResources[_currentBufferIndex]; }

	static constexpr size_t FrameCount = 2;
	static constexpr size_t GetFrameCount() { return FrameCount; }

private:
	bool CreateCommandQueue();
	bool CreateWindowObject();
	bool CreateSwapChain();
	bool CreateRenderTargets();
	bool CreateCommandListAndAllocators();
	bool CreateFence();

	bool GetBuffers();
	void ReleaseBuffers();

	void SignalAndWait();
public:
	inline void Flush(size_t count) {
		for (size_t i = 0; i < count; i++) {
			SignalAndWait();
		}
	}
private:
	static LRESULT CALLBACK OnWindowMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	ATOM _wndClass = 0;
	HWND _window = nullptr;
	bool _shouldClose = false;

	bool _shouldResize = false;
	UINT _width = 1920;
	UINT _height = 1080;

	ComPointer<IDXGISwapChain3> _swapChain;
	ComPointer<ID3D12Resource2> _renderTargets[FrameCount];
	ComPointer<ID3D12DescriptorHeap> _rtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _rtvHandles[FrameCount];
	UINT _rtvDescriptorSize = 0;

	FrameResource* _frameResources[FrameCount];
	size_t _currentBufferIndex = 0;

	ComPointer<ID3D12CommandQueue> _cmdQueue;
	ComPointer<ID3D12CommandAllocator> _allocators[FrameCount]; // One allocator per frame
	ComPointer<ID3D12GraphicsCommandList> _commandList;

	ComPointer<ID3D12Fence> _fence;
	UINT _fenceValue = 0;
	HANDLE _fenceEvent = nullptr;

	ComPointer<ID3D12RootSignature> _rootSignature;
public:
	DXWindow(const DXWindow&) = delete;
	DXWindow& operator=(const DXWindow&) = delete;

	inline static DXWindow& Get() {
		static DXWindow instance;
		return instance;
	}

private:
	DXWindow() = default;
};