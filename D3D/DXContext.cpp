#include "DXContext.h"
#include <Support/Window.h>

bool DXContext::Init()
{
	if (!CreateDXGIFactory()) return false;
	if (!CreateDevice()) return false;

	return true;
	// === STOP HERE ===
	// == Everything left is frame related, and should be done in Window.h ==
	// 
	//if (FAILED(_device->CreateFence(_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)))) {
	//	return false;
	//}
	//_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
	//if (!_fenceEvent) {
	//	return false;
	//}
	//
	//if (FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAllocator)))) {
	//	return false;
	//}
	//
	//if (FAILED(_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&_cmdList)))) {
	//	return false;
	//}
	//
	//return true;
}

bool DXContext::CreateDXGIFactory()
{
	if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&_dxgiFactory)))) {
		return false;
	}

	return true;
}

bool DXContext::CreateDevice()
{
	for (UINT i = 0; ; ++i) {
		ComPointer<IDXGIAdapter1> currentAdapter;
		if (_dxgiFactory->EnumAdapters1(i, &currentAdapter) == DXGI_ERROR_NOT_FOUND) break;

		DXGI_ADAPTER_DESC1 desc;
		currentAdapter->GetDesc1(&desc);

		// ignore software adapters
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

		if (SUCCEEDED(D3D12CreateDevice(currentAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS((&_device))))) {
			_dxgiAdapter = currentAdapter;
			return true;
		}
	}

	return false;
}

void DXContext::Shutdown()
{
	_device.Release();
	_dxgiFactory.Release();
}