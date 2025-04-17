#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>

class DXContext {
public:
	bool Init();
	void Shutdown();

	ID3D12GraphicsCommandList7* InitCommandList();
	void ExecuteCommandList();
	void SignalAndWait();

	inline void Flush(size_t count) {
		for (size_t i = 0; i < count; i++) {
			SignalAndWait();
		}
	}

	inline ComPointer<IDXGIFactory7>& GetFactory() { return _dxgiFactory; }
	inline ComPointer<ID3D12Device10>& GetDevice()  { return _device; }
	inline ComPointer<ID3D12CommandQueue>& GetCommandQueue() { return _cmdQueue; }
	inline ComPointer<ID3D12GraphicsCommandList7>& GetCommandList() { return _cmdList; }
	inline ComPointer<ID3D12CommandAllocator>& GetCommandAllocator() { return _cmdAllocator; }
	
private:
	ComPointer<IDXGIFactory7> _dxgiFactory;
	ComPointer<ID3D12Device10> _device;
	ComPointer<ID3D12CommandQueue> _cmdQueue;

	ComPointer<ID3D12CommandAllocator> _cmdAllocator;
	ComPointer<ID3D12GraphicsCommandList7> _cmdList;

	ComPointer<ID3D12Fence1> _fence;
	UINT64 _fenceValue = 0;
	HANDLE _fenceEvent = nullptr;

public:
	DXContext(const DXContext&) = delete;

	DXContext& operator=(const DXContext&) = delete;

	inline static DXContext& Get() {
		static DXContext instance;
		return instance;
	}

private:
	DXContext() = default;
};