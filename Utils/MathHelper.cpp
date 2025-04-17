//***************************************************************************************
// MathHelper.cpp by Frank Luna (C) 2011 All Rights Reserved.
// 
// HEAVILY modified by Ethan Parker . (C) 2025 All Rights Revoked.
// Seriously what the fuck was Frank smoking?
// 
//***************************************************************************************

#include "MathHelper.h"
#include <float.h>
#include <cmath>

float MathHelper::RandF() {
	// Returns random float in [0, 1).
	return (float)(rand()) / (float)RAND_MAX;
}

float MathHelper::RandRangeF(float a, float b) {
	// Returns random float in [a, b).
	return a + MathHelper::RandF() * (b - a);
}

int MathHelper::RandI(int a, int b) {
	return a + rand() % ((b - a) + 1);
}

DirectX::XMVECTOR MathHelper::NormalFromTrigon(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, DirectX::XMFLOAT3 c) {
	DirectX::XMVECTOR ab = DirectX::XMLoadFloat3(&a);
	DirectX::XMVECTOR ac = DirectX::XMLoadFloat3(&a);
	DirectX::XMVECTOR normal = DirectX::XMVector3Cross(ab, ac);
	return DirectX::XMVector3Normalize(normal);
}

DirectX::XMVECTOR MathHelper::SphericalToCartesian(float radius, float theta, float phi) {
	return DirectX::XMVectorSet(
		radius * sinf(phi) * cosf(theta),
		radius * cosf(phi),
		radius * sinf(phi) * sinf(theta),
		1.0f);
}

DirectX::XMMATRIX MathHelper::InverseTranspose(DirectX::CXMMATRIX M) {
	// Inverse-transpose is just applied to normals.  So zero out 
	// translation row so that it doesn't get into our inverse-transpose
	// calculation--we don't want the inverse-transpose of the translation.
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
}

float MathHelper::AngleFromXY(float x, float y) {

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	float theta = 0.0f;
 
	// Quadrant I or IV
	if(x >= 0.0f) 
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if(theta < 0.0f)
			theta += 2.0f*Pi; // in [0, 2*pi).
	}

	// Quadrant II or III
	else      
		theta = atanf(y/x) + Pi; // in [0, 2*pi).

	return theta;
}

void MathHelper::ClampAngle(float* angle) {
	static float twoPI = 2 * Pi;
	if (*angle <= 0) {
		*angle += twoPI;
	}
	else {
		*angle = fmodf(*angle, twoPI);
	}
	
}

DirectX::XMVECTOR MathHelper::RandUnitVec3()
{
	DirectX::XMVECTOR One  = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR Zero = DirectX::XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while(true)
	{
		// Generate random point in the cube [-1,1]^3.
		DirectX::XMVECTOR v = DirectX::XMVectorSet(RandRangeF(-1.0f, 1.0f), RandRangeF(-1.0f, 1.0f), RandRangeF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.

		if(DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(v), One) )
			continue;

		return DirectX::XMVector3Normalize(v);
	}
}

DirectX::XMVECTOR MathHelper::RandHemisphereUnitVec3(DirectX::XMVECTOR n)
{
	DirectX::XMVECTOR One = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR Zero = DirectX::XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while(true)
	{
		// Generate random point in the cube [-1,1]^3.
		DirectX::XMVECTOR v = DirectX::XMVectorSet(RandRangeF(-1.0f, 1.0f), RandRangeF(-1.0f, 1.0f), RandRangeF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.
		
		if(DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(v), One) )
			continue;

		// Ignore points in the bottom hemisphere.
		if(DirectX::XMVector3Less(DirectX::XMVector3Dot(n, v), Zero ) )
			continue;

		return DirectX::XMVector3Normalize(v);
	}
}

void MathHelper::GetBasisVectors(const DirectX::XMMATRIX& m, DirectX::XMVECTOR& outBasisX, DirectX::XMVECTOR& outBasisY, DirectX::XMVECTOR& outBasisZ) {
	// DirectX uses row-major for some fuckass reason -James
	DirectX::XMFLOAT4X4 matf;
	DirectX::XMStoreFloat4x4(&matf, m);

	outBasisX = GetRowFromFloat4x4(matf, 0);
	outBasisY = GetRowFromFloat4x4(matf, 1);
	outBasisZ = GetRowFromFloat4x4(matf, 2);
}

DirectX::XMMATRIX MathHelper::XMMatrixOrthonormalize(const DirectX::XMMATRIX& m) {
	DirectX::XMFLOAT4X4 result;
	// Cache the matrices 4th row immediately, since it's unused. -James
	DirectX::XMStoreFloat4x4(&result, m);

	DirectX::XMVECTOR x, y, z;
	GetBasisVectors(m, x, y, z);

	z = DirectX::XMVector3Normalize(z);
	x = DirectX::XMVector3Cross(y, z);
	x = DirectX::XMVector3Normalize(x);
	y = DirectX::XMVector3Cross(z, x);
	result = {
		x.m128_f32[0], x.m128_f32[1], x.m128_f32[2], 0,
		y.m128_f32[0], y.m128_f32[1], y.m128_f32[2], 0,
		z.m128_f32[0], z.m128_f32[1], z.m128_f32[2], 0,
		result._31, result._32, result._33, result._34
	};
	
	return DirectX::XMLoadFloat4x4(&result);
}

DirectX::XMVECTOR MathHelper::GetRowFromFloat4x4(const DirectX::XMFLOAT4X4& m, const uint8_t row) {
	return DirectX::XMVectorSet(
		m(row, 0),
		m(row, 1),
		m(row, 2),
		m(row, 3)
	);
}

DirectX::XMVECTOR MathHelper::GetColumnFromFloat4x4(const DirectX::XMFLOAT4X4& m, const uint8_t col) {
	return DirectX::XMVectorSet(
		m(0, col),
		m(1, col),
		m(2, col),
		m(3, col));
}