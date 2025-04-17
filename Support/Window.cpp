#include "Window.h"

bool DXWindow::Init()
{
	WNDCLASSEXW wcex{};
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = &OnWindowMessage;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(nullptr);
	wcex.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"D3D12ExWndCls";
	wcex.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);
	
	_wndClass = RegisterClassExW(&wcex);
	if (_wndClass == 0) {
		return false;
	}

	_window = CreateWindowExW(
		WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW,
		(LPCWSTR)_wndClass,
		L"GAME3111 - Engine",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100,
		1920, 1080,
		nullptr,
		nullptr,
		wcex.hInstance,
		nullptr
	);

	if (_window == nullptr) {
		return false;
	}

	// Describe swap chain
	DXGI_SWAP_CHAIN_DESC1 swd{};
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC sfd{};

	swd.Width = 1920;
	swd.Height = 1080;
	swd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swd.Stereo = false;
	swd.SampleDesc.Count = 1;
	swd.SampleDesc.Quality = 0;
	swd.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swd.BufferCount = GetFrameCount();
	swd.Scaling = DXGI_SCALING_STRETCH;
	swd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	swd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	sfd.Windowed = true;

	//Create swap chain
	auto& factory = DXContext::Get().GetFactory();
	ComPointer<IDXGISwapChain1> sc1;
	factory->CreateSwapChainForHwnd(DXContext::Get().GetCommandQueue(), _window, &swd, &sfd, nullptr, &sc1);
	if (!sc1.QueryInterface(_swapChain)) {
		return false;
	}

	// Create Render Target View Heap
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = FrameCount;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	if (FAILED(DXContext::Get().GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap)))) {
		return false;
	}

	// Create handles to view
	auto firstHandle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	UINT handleIncrement = DXContext::Get().GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (size_t i = 0; i < FrameCount; ++i) {
		_rtvHandles[i] = firstHandle;
		_rtvHandles[i].ptr += handleIncrement * i;
	}

	if (!GetBuffers()) {
		return false;
	}

	return true;
}

void DXWindow::Update()
{
	MSG msg;
	while (PeekMessageW(&msg, _window, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

bool DXWindow::GetBuffers()
{
	for (size_t i = 0; i < FrameCount; i++) {
		if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_buffers[i])))) {
			return false;
		}

		D3D12_RENDER_TARGET_VIEW_DESC rtv{};
		rtv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtv.Texture2D.MipSlice = 0;
		rtv.Texture2D.PlaneSlice = 0;

		DXContext::Get().GetDevice()->CreateRenderTargetView(_buffers[i], &rtv, _rtvHandles[i]);
	}

	return true;
}

void DXWindow::ReleaseBuffers()
{
	for (size_t i = 0; i < FrameCount; i++) {
		_buffers[i].Release();
	}
}

LRESULT DXWindow::OnWindowMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		
		case WM_SIZE:
			if (lParam != 0 && (HIWORD(lParam) != Get()._height || LOWORD(lParam) != Get()._width))
				Get()._shouldResize = true;
			break;
		case WM_CLOSE:
			Get()._shouldClose = true;
			return 0;
			
	}

	return DefWindowProcW(window, msg, wParam, lParam);
}

void DXWindow::Resize()
{
	ReleaseBuffers();

	RECT cr{};
	if (GetClientRect(_window, &cr)) {
		UINT width = cr.right - cr.left;
		UINT height = cr.bottom - cr.top;

		_swapChain->ResizeBuffers(GetFrameCount(), _width, _height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
		_shouldResize = false;
	}

	GetBuffers();
}

void DXWindow::BeginFrame(ID3D12GraphicsCommandList7* cmdList)
{
	_currentBufferIndex = _swapChain->GetCurrentBackBufferIndex();

	D3D12_RESOURCE_BARRIER barr;
	barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barr.Transition.pResource = _buffers[_currentBufferIndex];
	barr.Transition.Subresource = 0;
	barr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	cmdList->ResourceBarrier(1, &barr);
	float clearColor[] = {.4f, .4f, .8f, 1.f};

	cmdList->ClearRenderTargetView(_rtvHandles[_currentBufferIndex], clearColor, 0, nullptr);
	
		cmdList->OMSetRenderTargets(1, &_rtvHandles[_currentBufferIndex], false, nullptr);
}

void DXWindow::EndFrame(ID3D12GraphicsCommandList7* cmdList)
{
	D3D12_RESOURCE_BARRIER barr;
	barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barr.Transition.pResource = _buffers[_currentBufferIndex];
	barr.Transition.Subresource = 0;
	barr.Transition.StateBefore= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	cmdList->ResourceBarrier(1, &barr);
}

void DXWindow::Present()
{
	_swapChain->Present(1, 0);
}

void DXWindow::Shutdown()
{
	ReleaseBuffers();

	_rtvHeap.Release();

	_swapChain.Release();

	if (_window) {
		DestroyWindow(_window);
	}

	if (_wndClass) {
		UnregisterClassW((LPCWSTR)_wndClass, GetModuleHandleW((nullptr)));
	}
}


