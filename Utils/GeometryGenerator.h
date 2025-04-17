//***************************************************************************************
// GeometryGenerator.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Reworked by James Mitchell & Ethan Parker
//
// All triangles are generated "outward" facing.  If you want "inward" 
// facing triangles (for example, if you want to place the camera inside
// a sphere to simulate a sky), you will need to:
//   1. Change the Direct3D cull mode or manually reverse the winding order.
//   2. Invert the normal.
//   3. Update the texture coordinates and tangent vectors.
// 
//***************************************************************************************

#pragma once

#include "DirectXMath.h"

#include <cstdint>
#include <vector>

struct MeshData;
struct Vertex;
namespace GeometryGenerator {
  
	///<summary>
	/// Creates a box centered at the origin with the given dimensions, where each
    /// face has m rows and n columns of vertices.
	///</summary>
    MeshData CreateBox(float width, float height, float depth, uint32_t numSubdivisions);

	///<summary>
	/// Creates a sphere centered at the origin with the given radius.  The
	/// slices and stacks parameters control the degree of tessellation.
	///</summary>
    MeshData CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount);

	///<summary>
	/// Creates a geosphere centered at the origin with the given radius.  The
	/// depth controls the level of tessellation.
	///</summary>
    MeshData CreateIcosphere(float radius, uint32_t numSubdivisions);

	///<summary>
	/// Creates a cylinder parallel to the y-axis, and centered about the origin.  
	/// The bottom and top radius can vary to form various cone shapes rather than true
	// cylinders.  The slices and stacks parameters control the degree of tessellation.
	///</summary>
    MeshData CreateCylinder(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount);

	///<summary>
	/// Creates an mxn grid in the xz-plane with m rows and n columns, centered
	/// at the origin with the specified width and depth.
	///</summary>
    MeshData CreateGrid(float width, float depth, uint32_t m, uint32_t n);

	///<summary>
	/// Creates a quad aligned with the screen.  This is useful for post-processing and screen effects.
	///</summary>
	
    MeshData CreateQuad(float x, float y, float w, float h, float depth);
	MeshData CreateCone(float radius, float height, uint32_t sliceCount);
	MeshData CreatePyramid(float width, float height, float depth, float pointBevel);
	MeshData CreateTriangularPrism(float barycentricDistance);
	MeshData CreateWedge(float length, float width, float height, uint32_t numSubdivisions);
	MeshData CreateTorus(const float innerRadius, const float outerRadius, const uint32_t sliceCount, const uint32_t stackCount);
	MeshData CreateDiamond(const float width, const float height, const float depth, DirectX::XMFLOAT3 skew);

	void Subdivide(MeshData& meshData);
    Vertex MidPoint(const Vertex& v0, const Vertex& v1);
	
	void CreateCircle(const float radius, const float height, uint32_t sliceCount, const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& normal, const DirectX::XMVECTOR& tangent, std::vector<Vertex>& out);

	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);
};