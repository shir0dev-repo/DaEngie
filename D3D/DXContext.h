#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>

class DXContext {
public:
	bool Init();
	void Shutdown();

	inline ComPointer<IDXGIFactory7>& GetFactory() { return _dxgiFactory; }
	inline ComPointer<ID3D12Device10>& GetDevice()  { return _device; }
	
private:
	bool CreateDXGIFactory();
	bool CreateDevice();
	
	ComPointer<IDXGIFactory7> _dxgiFactory;
	ComPointer<IDXGIAdapter1> _dxgiAdapter;
	ComPointer<ID3D12Device10> _device;
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