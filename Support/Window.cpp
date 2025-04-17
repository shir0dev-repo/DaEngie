#include "Window.h"
#include <Support/WinInclude.h>
#include <D3D/DXPipeline.h>

bool DXWindow::Init()
{
	if (!CreateCommandQueue()) return false;
	// Init Window Handle
	if (!CreateWindowObject()) return false;
	// Create Swap Chain
	if (!CreateSwapChain()) return false;
	// Create Render Target View Heap
	if (!CreateRenderTargets()) return false;
	// Create Command List and Allocators
	if (!CreateCommandListAndAllocators()) return false;
	// Create Fence for GPU Syncing
	if (!CreateFence()) return false;

	return true;
	
	//auto firstHandle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	//UINT handleIncrement = DXContext::Get().GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//for (size_t i = 0; i < FrameCount; ++i) {
	//	_rtvHandles[i] = firstHandle;
	//	_rtvHandles[i].ptr += handleIncrement * i;
	//}
	//
	//if (!GetBuffers()) {
	//	return false;
	//}
	//
	//return true;
}

bool DXWindow::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;

	if (FAILED(DXContext::Get().GetDevice()->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&_cmdQueue)))) {
		return false;
	}

	return true;
}

bool DXWindow::CreateWindowObject()
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

	return _window != nullptr;
}

bool DXWindow::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 swd{};

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
	swd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	auto& factory = DXContext::Get().GetFactory();
	ComPointer<IDXGISwapChain1> sc1;
	factory->CreateSwapChainForHwnd(DXWindow::Get().GetCommandQueue(), _window, &swd, nullptr, nullptr, &sc1);
	if (!sc1.QueryInterface(_swapChain)) {
		return false;
	}

	factory->MakeWindowAssociation(_window, DXGI_MWA_NO_ALT_ENTER);
	return true;
}

bool DXWindow::CreateRenderTargets()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = FrameCount;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	if (FAILED(DXContext::Get().GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap)))) {
		return false;
	}

	ID3D12Device* device = DXContext::Get().GetDevice();
	_rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());

	for (UINT i = 0; i < FrameCount; ++i) {
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]));
		
		_rtvHandles[i] = rtvHandle;
		device->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, _rtvHandles[i]);

		rtvHandle.Offset(1, _rtvDescriptorSize);
	}

	return true;
}

bool DXWindow::CreateCommandListAndAllocators()
{
	ID3D12Device* device = DXContext::Get().GetDevice();

	for (UINT i = 0; i < FrameCount; ++i) {
		if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_allocators[i])))) {
			return false;
		}
	}

	
	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _allocators[0].Get(), nullptr, IID_PPV_ARGS(&_commandList)))) {
		return false;
	
	}

	_commandList->Close();
	return true;
}

bool DXWindow::CreateFence()
{
	ID3D12Device* device = DXContext::Get().GetDevice();
	if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)))) {
		return false;
	}

	_fenceValue = 1;
	_fenceEvent = CreateEvent(nullptr, false, false, nullptr);

	return _fenceEvent != nullptr;
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
		if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i])))) {
			return false;
		}

		D3D12_RENDER_TARGET_VIEW_DESC rtv{};
		rtv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtv.Texture2D.MipSlice = 0;
		rtv.Texture2D.PlaneSlice = 0;

		DXContext::Get().GetDevice()->CreateRenderTargetView(_renderTargets[i], &rtv, _rtvHandles[i]);
	}

	return true;
}

void DXWindow::ReleaseBuffers()
{
	for (size_t i = 0; i < FrameCount; i++) {
		_renderTargets[i].Release();
	}
}

void DXWindow::SignalAndWait()
{
	_cmdQueue->Signal(_fence, ++_fenceValue);
	if (SUCCEEDED(_fence->SetEventOnCompletion(_fenceValue, _fenceEvent))) {
		if (WaitForSingleObject(_fenceEvent, 20000) != WAIT_OBJECT_0) {
			exit(-1);
		}
	}
	else {
		exit(-1);
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

ID3D12GraphicsCommandList* DXWindow::InitCommandList()
{
	UINT currBufferIndex = _swapChain->GetCurrentBackBufferIndex();
	_commandList->Close();
	_allocators[currBufferIndex]->Reset();
	HRESULT r2 = _commandList->Reset(_allocators[currBufferIndex], nullptr);

	return _commandList;
}

void DXWindow::BeginFrame()
{
	_currentBufferIndex = _swapChain->GetCurrentBackBufferIndex();
	auto& currCmdAlloc = _allocators[_currentBufferIndex];

	_commandList->Reset(currCmdAlloc, DXPipeline::Get().GetOpaquePSO());

	D3D12_RESOURCE_BARRIER barr;
	barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barr.Transition.pResource = _renderTargets[_currentBufferIndex];
	barr.Transition.Subresource = 0;
	barr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	_commandList->ResourceBarrier(1, &barr);
	float clearColor[] = {.4f, .4f, .8f, 1.f};

	_commandList->ClearRenderTargetView(_rtvHandles[_currentBufferIndex], clearColor, 0, nullptr);
	_commandList->OMSetRenderTargets(1, &_rtvHandles[_currentBufferIndex], false, nullptr);
}

void DXWindow::EndFrame()
{
	D3D12_RESOURCE_BARRIER barr;
	barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barr.Transition.pResource = _renderTargets[_currentBufferIndex];
	barr.Transition.Subresource = 0;
	barr.Transition.StateBefore= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	_commandList->ResourceBarrier(1, &barr);
}

void DXWindow::ExecuteCommandLists()
{
	if (SUCCEEDED(_commandList->Close())) {
		ID3D12CommandList* lists[] = { _commandList };
		_cmdQueue->ExecuteCommandLists(1, lists);
		SignalAndWait();
	}
}

void DXWindow::Present()
{
	_swapChain->Present(1, 0);
}

void DXWindow::Shutdown()
{
	_commandList.Release();
	for (UINT i = 0; i < FrameCount; ++i) {
		_allocators[i].Release();
	}

	if (_fenceEvent) {
		CloseHandle(_fenceEvent);
	}

	_fence.Release();
	_cmdQueue.Release();
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


