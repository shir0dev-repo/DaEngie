#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>

#include <string>

struct Shader {
	Shader() = default;
	Shader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entryPoint, const std::string& target);
	
	ID3DBlob* GetResource() { return _resource.Get(); }
private:
	ComPointer<ID3DBlob> _resource;
};