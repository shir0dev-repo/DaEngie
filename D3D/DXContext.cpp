#include "DXContext.h"

bool DXContext::Init()
{
	if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&_dxgiFactory)))) {
		return false;
	}

	if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device)))) {
		return false;
	}

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	
	if (FAILED(_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&_cmdQueue)))) {
		return false;
	}

	if (FAILED(_device->CreateFence(_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)))) {
		return false;
	}

	_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
	if (!_fenceEvent) {
		return false;
	}
	
	if (FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAllocator)))) {
		return false;
	}

	if (FAILED(_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&_cmdList)))) {
		return false;
	}

	return true;
}

void DXContext::SignalAndWait()
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

ID3D12GraphicsCommandList7* DXContext::InitCommandList()
{
	_cmdAllocator->Reset();
	_cmdList->Reset(_cmdAllocator, nullptr);

	return _cmdList;
}

void DXContext::ExecuteCommandList()
{
	if (SUCCEEDED(_cmdList->Close())) {
		ID3D12CommandList* lists[] = { _cmdList };
		_cmdQueue->ExecuteCommandLists(1, lists);
		SignalAndWait();
	}
}

void DXContext::Shutdown()
{
	_cmdList.Release();
	_cmdAllocator.Release();

	if (_fenceEvent) {
		CloseHandle(_fenceEvent);
	}
	
	_fence.Release();
	_device.Release();
	_cmdQueue.Release();

	_dxgiFactory.Release();
}