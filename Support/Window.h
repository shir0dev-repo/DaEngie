#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <D3D/DXContext.h>

class DXWindow {
public:
	bool Init();
	void Update();
	void Present();
	void Shutdown();
	void Resize();

	void BeginFrame(ID3D12GraphicsCommandList7* cmdList);
	void EndFrame(ID3D12GraphicsCommandList7* cmdList);
	
	inline bool ShouldClose() const { return _shouldClose; }
	inline bool ShouldResize() const { return _shouldResize; }

	static constexpr size_t FrameCount = 2;
	static constexpr size_t GetFrameCount() { return FrameCount; }

private:
	bool GetBuffers();
	void ReleaseBuffers();

	static LRESULT CALLBACK OnWindowMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	ATOM _wndClass = 0;
	HWND _window = nullptr;
	bool _shouldClose = false;

	bool _shouldResize = false;
	UINT _width = 1920;
	UINT _height = 1080;

	ComPointer<IDXGISwapChain3> _swapChain;
	ComPointer<ID3D12Resource2> _buffers[FrameCount];
	size_t _currentBufferIndex = 0;

	ComPointer<ID3D12DescriptorHeap> _rtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _rtvHandles[FrameCount];

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