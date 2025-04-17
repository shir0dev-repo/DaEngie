#include "Shader.h"
#include "d3dcompiler.h"

Shader::Shader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entryPoint, const std::string& target)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPointer<ID3DBlob> byteCode = nullptr;
	ComPointer<ID3DBlob> errorBlob = nullptr;
	HRESULT result = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errorBlob);

	if (errorBlob) {
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

	if (FAILED(result)) throw;

	_resource = byteCode;
}
