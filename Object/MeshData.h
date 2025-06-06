#pragma once

#include <DirectXMath.h>

#include <vector>
#include <cstdint>

struct Vertex {
	Vertex() = default;

	Vertex(
		const DirectX::XMFLOAT3& p, 
		const DirectX::XMFLOAT3& n, 
		const DirectX::XMFLOAT3& t, 
		const DirectX::XMFLOAT2& uv) :
		Position(p),
		Normal(n),
		TangentU(t),
		TexC(uv) {}

	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v) :
		Position(px, py, pz),
		Normal(nx, ny, nz),
		TangentU(tx, ty, tz),
		TexC(u, v) {}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 TangentU;
	DirectX::XMFLOAT2 TexC;
};

typedef struct MeshData {
	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;
}MeshData;