#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <cstdint>

namespace MathHelper {
#ifndef GetBufferByteSize
#define GetBufferByteSize(T) (sizeof(T) + 0xff) & ~0xff
#endif

	// Math Constants:

	const float Infinity = FLT_MAX;
	const float Pi = 3.1415926535f;

	const DirectX::XMFLOAT4X4 Identity4x4(\
		1.0f, 0.0f, 0.0f, 0.0f, \
		0.0f, 1.0f, 0.0f, 0.0f, \
		0.0f, 0.0f, 1.0f, 0.0f, \
		0.0f, 0.0f, 0.0f, 1.0f); \

	// Macro Definitions
	#define Min(a, b) (a < b ? a : b)
	#define Max(a, b) (a > b ? a : b)
	#define Lerp(a, b, t) (a + (b - a) * t)
	#define Clamp(x, low, high) (x < low ? low : (x > high ? high : x))

	int RandI(int a, int b);
	float RandF();
	float RandRangeF(float a, float b);
	float AngleFromXY(float x, float y);

	DirectX::XMVECTOR NormalFromTrigon(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c);
	DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi);
	DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);
    DirectX::XMVECTOR RandUnitVec3();
    DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n);
	
	void ClampAngle(float* angle);
	void GetBasisVectors(const DirectX::XMMATRIX& m, DirectX::XMVECTOR& outBasisX, DirectX::XMVECTOR& outBasisY, DirectX::XMVECTOR& outBasisZ);
	DirectX::XMMATRIX XMMatrixOrthonormalize(const DirectX::XMMATRIX& m);
	DirectX::XMVECTOR GetRowFromFloat4x4(const DirectX::XMFLOAT4X4& m, const uint8_t row);
	DirectX::XMVECTOR GetColumnFromFloat4x4(const DirectX::XMFLOAT4X4& m, const uint8_t column);	
};

