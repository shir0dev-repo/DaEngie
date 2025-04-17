#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>

#include <string>

struct Texture
{
	std::string Name;
	std::wstring Filename;

	ComPointer<ID3D12Resource> Resource = nullptr;
	ComPointer<ID3D12Resource> UploadHeap = nullptr;
};