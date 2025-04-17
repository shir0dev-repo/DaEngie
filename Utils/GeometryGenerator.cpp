//***************************************************************************************
// GeometryGenerator.cpp by Frank Luna (C) 2011 All Rights Reserved.
// 
// Reworked by James Mitchell & Ethan Parker
// 
//***************************************************************************************

#include "GeometryGenerator.h"
#include <Support/WinInclude.h>
#include <Object/MeshData.h>
#include <DirectXMath.h>
#include <Utils/MathHelper.h>
#include <algorithm>
#include <winuser.h>

using namespace DirectX;

#define PI 3.1415926535f

MeshData GeometryGenerator::CreateBox(float width, float height, float depth, uint32_t numSubdivisions)
{
    MeshData meshData;

    //
	// Create the vertices.
	//

	float halfWidth = width;
	float halfHeight = height;
	float halfDepth = depth;

	Vertex v[24] = {
		// Fill in the front face vertex data.
		Vertex(-halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-halfWidth, +halfHeight, -halfDepth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(+halfWidth, +halfHeight, -halfDepth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex(+halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),

		// Fill in the back face vertex data.
		Vertex(-halfWidth, -halfHeight, +halfDepth, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex(+halfWidth, -halfHeight, +halfDepth, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		Vertex(+halfWidth, +halfHeight, +halfDepth, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(-halfWidth, +halfHeight, +halfDepth, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),

		// Fill in the top face vertex data.
		Vertex(-halfWidth, +halfHeight, -halfDepth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-halfWidth, +halfHeight, +halfDepth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(+halfWidth, +halfHeight, +halfDepth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex(+halfWidth, +halfHeight, -halfDepth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),

		// Fill in the bottom face vertex data.
		Vertex(-halfWidth, -halfHeight, -halfDepth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex(+halfWidth, -halfHeight, -halfDepth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		Vertex(+halfWidth, -halfHeight, +halfDepth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(-halfWidth, -halfHeight, +halfDepth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),

		// Fill in the left face vertex data.
		Vertex(-halfWidth, -halfHeight, +halfDepth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-halfWidth, +halfHeight, +halfDepth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
		Vertex(-halfWidth, +halfHeight, -halfDepth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
		Vertex(-halfWidth, -halfHeight, -halfDepth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),

		// Fill in the right face vertex data.
		Vertex(+halfWidth, -halfHeight, -halfDepth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(+halfWidth, +halfHeight, -halfDepth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex(+halfWidth, +halfHeight, +halfDepth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),
		Vertex(+halfWidth, -halfHeight, +halfDepth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f)
	};

	meshData.Vertices.assign(&v[0], &v[24]);
	//
	// Create the indices.
	//

	uint32_t i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.Indices.assign(&i[0], &i[36]);

    // Put a cap on the number of subdivisions.
    numSubdivisions = std::min<uint32_t>(numSubdivisions, 6u);
	
    for(uint32_t i = 0; i < numSubdivisions; ++i)
        Subdivide(meshData);

    return meshData;
}

MeshData GeometryGenerator::CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount)
{
    MeshData meshData;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.Vertices.push_back( topVertex );

	float phiStep   = XM_PI/stackCount;
	float thetaStep = 2.0f*XM_PI/sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(uint32_t i = 1; i <= stackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
        for(uint32_t j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;

			// spherical to cartesian
			v.Position.x = radius*sinf(phi)*cosf(theta);
			v.Position.y = radius*cosf(phi);
			v.Position.z = radius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.TangentU.x = -radius*sinf(phi)*sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = +radius*sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.TangentU);
			XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Position);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexC.x = theta / XM_2PI;
			v.TexC.y = phi / XM_PI;

			meshData.Vertices.push_back( v );
		}
	}

	meshData.Vertices.push_back( bottomVertex );

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

    for(uint32_t i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices.push_back(0);
		meshData.Indices.push_back(i+1);
		meshData.Indices.push_back(i);
	}
	
	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
    uint32_t baseIndex = 1;
    uint32_t ringVertexCount = sliceCount + 1;
	for(uint32_t i = 0; i < stackCount-2; ++i)
	{
		for(uint32_t j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	uint32_t southPoleIndex = (uint32_t)meshData.Vertices.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(uint32_t i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex+i);
		meshData.Indices.push_back(baseIndex+i+1);
	}

    return meshData;
}
 
void GeometryGenerator::Subdivide(MeshData& meshData)
{
	// Save a copy of the input geometry.
	MeshData inputCopy = meshData;


	meshData.Vertices.resize(0);
	meshData.Indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	uint32_t numTris = (uint32_t)inputCopy.Indices.size()/3;
	for(uint32_t i = 0; i < numTris; ++i)
	{
		Vertex v0 = inputCopy.Vertices[ inputCopy.Indices[i*3+0] ];
		Vertex v1 = inputCopy.Vertices[ inputCopy.Indices[i*3+1] ];
		Vertex v2 = inputCopy.Vertices[ inputCopy.Indices[i*3+2] ];

		//
		// Generate the midpoints.
		//

        Vertex m0 = MidPoint(v0, v1);
        Vertex m1 = MidPoint(v1, v2);
        Vertex m2 = MidPoint(v0, v2);

		//
		// Add new geometry.
		//

		meshData.Vertices.push_back(v0); // 0
		meshData.Vertices.push_back(v1); // 1
		meshData.Vertices.push_back(v2); // 2
		meshData.Vertices.push_back(m0); // 3
		meshData.Vertices.push_back(m1); // 4
		meshData.Vertices.push_back(m2); // 5
 
		meshData.Indices.push_back(i*6+0);
		meshData.Indices.push_back(i*6+3);
		meshData.Indices.push_back(i*6+5);

		meshData.Indices.push_back(i*6+3);
		meshData.Indices.push_back(i*6+4);
		meshData.Indices.push_back(i*6+5);

		meshData.Indices.push_back(i*6+5);
		meshData.Indices.push_back(i*6+4);
		meshData.Indices.push_back(i*6+2);

		meshData.Indices.push_back(i*6+3);
		meshData.Indices.push_back(i*6+1);
		meshData.Indices.push_back(i*6+4);
	}
}

Vertex GeometryGenerator::MidPoint(const Vertex& v0, const Vertex& v1)
{
    XMVECTOR p0 = XMLoadFloat3(&v0.Position);
    XMVECTOR p1 = XMLoadFloat3(&v1.Position);

    XMVECTOR n0 = XMLoadFloat3(&v0.Normal);
    XMVECTOR n1 = XMLoadFloat3(&v1.Normal);

    XMVECTOR tan0 = XMLoadFloat3(&v0.TangentU);
    XMVECTOR tan1 = XMLoadFloat3(&v1.TangentU);

    XMVECTOR tex0 = XMLoadFloat2(&v0.TexC);
    XMVECTOR tex1 = XMLoadFloat2(&v1.TexC);

    // Compute the midpoints of all the attributes.  Vectors need to be normalized
    // since linear interpolating can make them not unit length.  
    XMVECTOR pos = 0.5f*(p0 + p1);
    XMVECTOR normal = XMVector3Normalize(0.5f*(n0 + n1));
    XMVECTOR tangent = XMVector3Normalize(0.5f*(tan0+tan1));
    XMVECTOR tex = 0.5f*(tex0 + tex1);

    Vertex v;
    XMStoreFloat3(&v.Position, pos);
    XMStoreFloat3(&v.Normal, normal);
    XMStoreFloat3(&v.TangentU, tangent);
    XMStoreFloat2(&v.TexC, tex);

    return v;
}

MeshData GeometryGenerator::CreateIcosphere(float radius, uint32_t numSubdivisions)
{
    MeshData meshData;

	// Put a cap on the number of subdivisions.
    numSubdivisions = std::min<uint32_t>(numSubdivisions, 6u);

	// Approximate a sphere by tessellating an icosahedron.

	const float X = 0.525731f; 
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] = 
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),  
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),    
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X), 
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),    
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f), 
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

    uint32_t k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

    meshData.Vertices.resize(12);
    meshData.Indices.assign(&k[0], &k[60]);


	for (uint32_t i = 0; i < 12; ++i) {
		meshData.Vertices[i].Position = pos[i];
	}

	for (uint32_t i = 0; i < numSubdivisions; ++i) {
		Subdivide(meshData);
	}

	// Project vertices onto sphere and scale.
	for(uint32_t i = 0; i < meshData.Vertices.size(); ++i)
	{
		// Project onto unit sphere.
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.Vertices[i].Position));

		// Project onto sphere.
		XMVECTOR p = radius*n;

		XMStoreFloat3(&meshData.Vertices[i].Position, p);
		XMStoreFloat3(&meshData.Vertices[i].Normal, n);

		// Derive texture coordinates from spherical coordinates.
        float theta = atan2f(meshData.Vertices[i].Position.z, meshData.Vertices[i].Position.x);

        // Put in [0, 2pi].
        if(theta < 0.0f)
            theta += XM_2PI;

		float phi = acosf(meshData.Vertices[i].Position.y / radius);

		meshData.Vertices[i].TexC.x = theta/XM_2PI;
		meshData.Vertices[i].TexC.y = phi/XM_PI;

		// Partial derivative of P with respect to theta
		meshData.Vertices[i].TangentU.x = -radius*sinf(phi)*sinf(theta);
		meshData.Vertices[i].TangentU.y = 0.0f;
		meshData.Vertices[i].TangentU.z = +radius*sinf(phi)*cosf(theta);

		XMVECTOR T = XMLoadFloat3(&meshData.Vertices[i].TangentU);
		XMStoreFloat3(&meshData.Vertices[i].TangentU, XMVector3Normalize(T));
	}

    return meshData;
}

MeshData GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount)
{
    MeshData meshData;

	//
	// Build Stacks.
	// 

	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	uint32_t ringCount = stackCount+1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for(uint32_t i = 0; i < ringCount; ++i)
	{
		float y = -0.5f*height + i*stackHeight;
		float r = bottomRadius + i*radiusStep;

		// vertices of ring
		float dTheta = 2.0f*XM_PI/sliceCount;
		for(uint32_t j = 0; j <= sliceCount; ++j)
		{
			Vertex vertex;

			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);

			vertex.Position = XMFLOAT3(r*c, y, r*s);

			vertex.TexC.x = (float)j/sliceCount;
			vertex.TexC.y = 1.0f - (float)i/stackCount;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
			vertex.TangentU = XMFLOAT3(-s, 0.0f, c);

			float dr = bottomRadius-topRadius;
			XMFLOAT3 bitangent(dr*c, -height, dr*s);

			XMVECTOR T = XMLoadFloat3(&vertex.TangentU);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&vertex.Normal, N);

			meshData.Vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	uint32_t ringVertexCount = sliceCount+1;

	// Compute indices for each stack.
	for(uint32_t i = 0; i < stackCount; ++i)
	{
		for(uint32_t j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(i*ringVertexCount + j);
			meshData.Indices.push_back((i+1)*ringVertexCount + j);
			meshData.Indices.push_back((i+1)*ringVertexCount + j+1);

			meshData.Indices.push_back(i*ringVertexCount + j);
			meshData.Indices.push_back((i+1)*ringVertexCount + j+1);
			meshData.Indices.push_back(i*ringVertexCount + j+1);
		}
	}

	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);

    return meshData;
}

void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
											uint32_t sliceCount, uint32_t stackCount, MeshData& meshData)
{
	uint32_t baseIndex = (uint32_t)meshData.Vertices.size();

	float y = 0.5f*height;
	float dTheta = 2.0f*XM_PI/sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for(uint32_t i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius*cosf(i*dTheta);
		float z = topRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x/height + 0.5f;
		float v = z/height + 0.5f;

		meshData.Vertices.push_back( Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v) );
	}

	// Cap center vertex.
	meshData.Vertices.push_back( Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f) );

	// Index of center vertex.
	uint32_t centerIndex = (uint32_t)meshData.Vertices.size()-1;

	for(uint32_t i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i+1);
		meshData.Indices.push_back(baseIndex + i);
	}
}

void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
									   uint32_t sliceCount, uint32_t stackCount, MeshData& meshData)
{
	// 
	// Build bottom cap.
	//

	uint32_t baseIndex = (uint32_t)meshData.Vertices.size();
	float y = -0.5f*height;

	// vertices of ring
	float dTheta = 2.0f*XM_PI/sliceCount;
	for(uint32_t i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius*cosf(i*dTheta);
		float z = bottomRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x/height + 0.5f;
		float v = z/height + 0.5f;

		meshData.Vertices.push_back( Vertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v) );
	}

	// Cap center vertex.
	meshData.Vertices.push_back( Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f) );

	// Cache the index of center vertex.
	uint32_t centerIndex = (uint32_t)meshData.Vertices.size()-1;

	for(uint32_t i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i+1);
	}
}

MeshData GeometryGenerator::CreateGrid(float width, float depth, uint32_t m, uint32_t n)
{
    MeshData meshData;

	uint32_t vertexCount = m*n;
	uint32_t faceCount   = (m-1)*(n-1)*2;

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n-1);
	float dz = depth / (m-1);

	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);

	meshData.Vertices.resize(vertexCount);
	for(uint32_t i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for(uint32_t j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			meshData.Vertices[i*n+j].Position = XMFLOAT3(x, 0.0f, z);
			meshData.Vertices[i*n+j].Normal   = XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.Vertices[i*n+j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
			meshData.Vertices[i*n+j].TexC.x = j*du;
			meshData.Vertices[i*n+j].TexC.y = i*dv;
		}
	}
 
    //
	// Create the indices.
	//

	meshData.Indices.resize(faceCount*3); // 3 indices per face

	// Iterate over each quad and compute indices.
	uint32_t k = 0;
	for(uint32_t i = 0; i < m-1; ++i)
	{
		for(uint32_t j = 0; j < n-1; ++j)
		{
			meshData.Indices[k]   = i*n+j;
			meshData.Indices[k+1] = i*n+j+1;
			meshData.Indices[k+2] = (i+1)*n+j;

			meshData.Indices[k+3] = (i+1)*n+j;
			meshData.Indices[k+4] = i*n+j+1;
			meshData.Indices[k+5] = (i+1)*n+j+1;

			k += 6; // next quad
		}
	}

    return meshData;
}

MeshData GeometryGenerator::CreateQuad(float x, float y, float w, float h, float depth)
{
    MeshData meshData;

	meshData.Vertices.resize(4);
	meshData.Indices.resize(6);

	// Position coordinates specified in NDC space.
	meshData.Vertices[0] = Vertex(
        x, y - h, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f);

	meshData.Vertices[1] = Vertex(
		x, y, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f);

	meshData.Vertices[2] = Vertex(
		x+w, y, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f);

	meshData.Vertices[3] = Vertex(
		x+w, y-h, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f);

	meshData.Indices[0] = 0;
	meshData.Indices[1] = 1;
	meshData.Indices[2] = 2;

	meshData.Indices[3] = 0;
	meshData.Indices[4] = 2;
	meshData.Indices[5] = 3;

    return meshData;
}

void GeometryGenerator::CreateCircle(const float radius, const float height, uint32_t sliceCount, const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& normal, const DirectX::XMVECTOR& tangent, std::vector<Vertex>& out) {
	
	out.reserve(sliceCount + 1);
	
	int slices = (sliceCount < 3) ? 3 : sliceCount;

	float circ = 2.0f * PI * radius;

	float angleDelta = 2.0f * PI / (float)slices;
	
	float uvDelta = 1.0f / (float)slices;
	
	XMFLOAT3 outPoint;
	XMFLOAT3 outNormal;
	XMFLOAT3 outTangent;
	XMFLOAT2 uv{0.0f, 0.0f};

	XMStoreFloat3(&outNormal, normal);
	XMStoreFloat3(&outTangent, tangent);

	XMVECTOR rotation = XMQuaternionRotationAxis(normal, angleDelta);
	XMVECTOR point = XMVectorScale(XMVector3Normalize(tangent), radius);

	XMStoreFloat3(&outPoint, XMVectorAdd(point, position));

	out.push_back(Vertex(outPoint, outNormal, outTangent, uv));

	for (uint32_t i = 0; i < slices - 1; i++) {
		point = XMVector3Rotate(point, rotation);
		uv.y += uvDelta;
		XMStoreFloat3(&outPoint, XMVectorAdd(point, position));
		out.push_back(Vertex(outPoint, outNormal, outTangent, uv));
	}
}

MeshData GeometryGenerator::CreateCone(float radius, float height, uint32_t sliceCount) {
	if (sliceCount < 3) sliceCount = 3;

	float circ = 2.0f * PI * radius;
	float angleInc = circ / (float) sliceCount;

	float baseY = -height * 0.5f;
	XMFLOAT3 normal(0.0f, -1.0f, 0.0f);
	XMFLOAT3 tangent(0.0f, 0.0f, 0.0f);
	XMFLOAT2 uv(0.0f, 0.0f);

	MeshData cone;
	XMFLOAT3 point(0, baseY, 0);

	// Store bottom center vertex:
	cone.Vertices.push_back(Vertex(point, normal, tangent, uv));

	for (uint32_t i = 0; i < sliceCount; i++) {
		normal = { cosf(angleInc * (float)i), 0.0f, sinf(angleInc * (float)i) };
		point = { normal.x * radius, baseY, normal.z * radius };

		XMStoreFloat3(&tangent, XMVector3Cross(XMLoadFloat3(&normal), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		cone.Vertices.push_back(Vertex(point, normal, tangent, uv));
	}

	cone.Vertices.push_back(cone.Vertices[1]);

	// Store the top vertex.
	point = { 0.0f, -baseY, 0.0f };
	normal = { 0.0f, 1.0f, 0.0f };

	cone.Vertices.push_back(Vertex(point, normal, tangent, uv));

	uint32_t topIndex = cone.Vertices.size() - 1;
	uint32_t bottomIndex = 0;

	// do indices
	for (uint32_t i = 1; i < cone.Vertices.size() - 2; i++) {
		// base tris
		cone.Indices.push_back(bottomIndex);
		cone.Indices.push_back(i);
		cone.Indices.push_back(i + 1);
		// side tris
		cone.Indices.push_back(i);
		cone.Indices.push_back(topIndex);
		cone.Indices.push_back(i + 1);
	}

	return cone;
}
MeshData GeometryGenerator::CreatePyramid(float width, float height, float depth, float pointBevel) {
	MeshData meshData;

	//
	// Create the vertices.
	//
	
	Vertex vertices[24];

	float halfWidth = 0.5f * width;
	float halfHeight = 0.5f * height;
	float halfDepth = 0.5f * depth;
	float topWidth = halfWidth * pointBevel;
	float topDepth = halfDepth * pointBevel;

	// Fill in the front face vertex data.
	vertices[0] = Vertex(-halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[1] = Vertex(-topWidth, +halfHeight, -topDepth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[2] = Vertex(+topWidth, +halfHeight, -topDepth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[3] = Vertex(+halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	vertices[4] = Vertex(-halfWidth, -halfHeight, +halfDepth, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	vertices[5] = Vertex(+halfWidth, -halfHeight, +halfDepth, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[6] = Vertex(+topWidth, +halfHeight, +topDepth, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[7] = Vertex(-topWidth, +halfHeight, +topDepth, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	vertices[8] = Vertex(-topWidth, +halfHeight, -topDepth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[9] = Vertex(-topWidth, +halfHeight, +topDepth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[10] = Vertex(+topWidth, +halfHeight, +topDepth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[11] = Vertex(+topWidth, +halfHeight, -topDepth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	vertices[12] = Vertex(-halfWidth, -halfHeight, -halfDepth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	vertices[13] = Vertex(+halfWidth, -halfHeight, -halfDepth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[14] = Vertex(+halfWidth, -halfHeight, +halfDepth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[15] = Vertex(-halfWidth, -halfHeight, +halfDepth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	vertices[16] = Vertex(-halfWidth, -halfHeight, +halfDepth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[17] = Vertex(-topWidth, +halfHeight, +topDepth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[18] = Vertex(-topWidth, +halfHeight, -topDepth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[19] = Vertex(-halfWidth, -halfHeight, -halfDepth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	vertices[20] = Vertex(+halfWidth, -halfHeight, -halfDepth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices[21] = Vertex(+topWidth, +halfHeight, -topDepth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[22] = Vertex(+topWidth, +halfHeight, +topDepth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	vertices[23] = Vertex(+halfWidth, -halfHeight, +halfDepth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshData.Vertices.assign(&vertices[0], &vertices[24]);

	//
	// Create the indices.
	//

	uint32_t indices[36];

	// Fill in the front face index data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Fill in the back face index data
	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// Fill in the top face index data
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// Fill in the bottom face index data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// Fill in the left face index data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// Fill in the right face index data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	meshData.Indices.assign(&indices[0], &indices[36]);

	return meshData;
}

MeshData GeometryGenerator::CreateTriangularPrism(float barycentricDistance) {	
	XMVECTOR bottomPointRotation = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), 2.0944f); // 120°
	
	float pointDst = barycentricDistance * 0.5f;
	XMFLOAT3 topPoint(0, pointDst, 0);
	XMFLOAT3 forwardPoint(0, -pointDst, pointDst);

	XMVECTOR result = XMVector3Rotate(XMLoadFloat3(&forwardPoint), bottomPointRotation);
	XMFLOAT3 backLeftPoint;
	XMStoreFloat3(&backLeftPoint, result);

	XMFLOAT3 backRightPoint;
	result = XMVector3Rotate(result, bottomPointRotation);
	XMStoreFloat3(&backRightPoint, result);

	XMFLOAT3 normal(0, 0, 0);
	XMFLOAT3 tangent(0, 0, 0);
	XMFLOAT2 uv(0, 0);

	MeshData mesh;
	mesh.Vertices.push_back(Vertex(topPoint, normal, tangent, uv));
	mesh.Vertices.push_back(Vertex(forwardPoint, normal, tangent, uv));
	mesh.Vertices.push_back(Vertex(backLeftPoint, normal, tangent, uv));
	mesh.Vertices.push_back(Vertex(backRightPoint, normal, tangent, uv));
	
	//front-left face
	mesh.Indices.push_back(0);
	mesh.Indices.push_back(1);
	mesh.Indices.push_back(2);
	//back face
	mesh.Indices.push_back(0);
	mesh.Indices.push_back(2);
	mesh.Indices.push_back(3);
	// front right face
	mesh.Indices.push_back(0);
	mesh.Indices.push_back(3);
	mesh.Indices.push_back(1);
	// bottom face
	mesh.Indices.push_back(1);
	mesh.Indices.push_back(3);
	mesh.Indices.push_back(2);

	return mesh;
}
MeshData GeometryGenerator::CreateWedge(float length, float width, float height, uint32_t numSubdivisions) {
	XMFLOAT3 halfExtents(length * 0.5f, height * 0.5f, width * 0.5f);

	const XMFLOAT3 bottomLeftBack(	-halfExtents.x, -halfExtents.y, -halfExtents.z);
	const XMFLOAT3 topLeftBack(		-halfExtents.x,  halfExtents.y, -halfExtents.z);
	const XMFLOAT3 bottomRightBack(	 halfExtents.x, -halfExtents.y, -halfExtents.z);
	/*
	wedgeVertices.Add(new Vector3(-halfLength, 0, -halfWidth)); // Bottom-left front
	wedgeVertices.Add(new Vector3(-halfLength, 0, halfWidth));  // Bottom-right front
	wedgeVertices.Add(new Vector3(halfLength, 0, -halfWidth));  // Bottom-left back
	wedgeVertices.Add(new Vector3(halfLength, 0, halfWidth));   // Bottom-right back
	wedgeVertices.Add(new Vector3(halfLength, slopeHeight, -halfWidth)); // Top-left back
	wedgeVertices.Add(new Vector3(halfLength, slopeHeight, halfWidth));  // Top-right back
	*/
	const XMFLOAT3 bottomLeftForward(	-halfExtents.x,  halfExtents.y, halfExtents.z);
	const XMFLOAT3 topLeftForward(	-halfExtents.x,  halfExtents.y, halfExtents.z);
	const XMFLOAT3 bottomRightForward( halfExtents.x, -halfExtents.y, halfExtents.z);

	XMFLOAT3 normal(0, 0, 0);
	XMFLOAT3 tangent(0, 0, 0);
	XMFLOAT2 uv(0, 0);

	MeshData mesh;
	mesh.Vertices.push_back(Vertex(bottomLeftBack, normal, tangent, uv));		// 0
	mesh.Vertices.push_back(Vertex(topLeftBack, normal, tangent, uv));			// 1
	mesh.Vertices.push_back(Vertex(bottomRightBack, normal, tangent, uv));		// 2
	
	mesh.Vertices.push_back(Vertex(bottomLeftForward, normal, tangent, uv));	// 3
	mesh.Vertices.push_back(Vertex(topLeftForward, normal, tangent, uv));		// 4
	mesh.Vertices.push_back(Vertex(bottomRightForward, normal, tangent, uv));	// 5

	// back tri
	mesh.Indices.push_back(0);
	mesh.Indices.push_back(1);
	mesh.Indices.push_back(2);

	// front tri
	mesh.Indices.push_back(3);
	mesh.Indices.push_back(5);
	mesh.Indices.push_back(4);
	//
	//// top square
	//mesh.Indices.push_back(1);
	//mesh.Indices.push_back(5);
	//mesh.Indices.push_back(2);
	//mesh.Indices.push_back(4);
	//mesh.Indices.push_back(5);
	//mesh.Indices.push_back(1);
	//
	//// back square
	//mesh.Indices.push_back(0);
	//mesh.Indices.push_back(3);
	//mesh.Indices.push_back(1);
	//mesh.Indices.push_back(4);
	//mesh.Indices.push_back(1);
	//mesh.Indices.push_back(3);

	//bottom square
	//mesh.Indices.push_back(0);
	//mesh.Indices.push_back(5);
	//mesh.Indices.push_back(2);
	


	for (uint32_t i = 0; i < numSubdivisions; i++) {
		if (mesh.Indices.size() >= 0x8000) break;
		//Subdivide(mesh);
	}

	return mesh;
}

MeshData GeometryGenerator::CreateTorus(const float innerRadius, const float outerRadius, const uint32_t sliceCount, const uint32_t stackCount) {
	
	// Torus's have the same number of faces as they do vertices (triangles double this count).
	uint32_t TotalVertices = sliceCount * stackCount;
	uint32_t TotalIndices = sliceCount * stackCount * 6;

	float stackRadius = (outerRadius - innerRadius) * 0.5;
	float centerRadius = stackRadius + innerRadius;
	float uvIncrement = 1.0f / static_cast<float>(stackCount);

	// Generate circle, for each vertex on circle, generate circle at point, then connect faces. 

	XMVECTOR position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR tangent = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);		
	XMVECTOR sliceNormal;

	std::vector<Vertex>guideRing;
	std::vector<Vertex>currentStack;
	
	GeometryGenerator::CreateCircle(centerRadius, 0.0f, stackCount, position, normal, tangent, guideRing);

	// C is better anyway -Ethan
	Vertex* V = (Vertex*)malloc(sizeof(Vertex) * TotalVertices);
	assert(V);

	float uvOffset = 0.0f;

	tangent = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Generate Vertices:
	for (uint32_t i = 0; i < stackCount; i++) {
		// Load the current position in the guide ring:
		position = XMLoadFloat3(&guideRing[i].Position);
		normal = XMVector3Cross(XMVector3Normalize(position), tangent);
		
		GeometryGenerator::CreateCircle(stackRadius, 0.0f, sliceCount, position, normal, tangent, currentStack);
		
		for (uint32_t sliceIndex = 0; sliceIndex < sliceCount; sliceIndex++) {
			// Calculate the normal of the current point (vector from guide ring to point, normalized).
			sliceNormal = XMLoadFloat3(&currentStack[sliceIndex].Position);
			sliceNormal = XMVectorSubtract(sliceNormal, position);
			sliceNormal = XMVector3Normalize(sliceNormal);

			// Write tangent and normal to the vertex.
			XMStoreFloat3(&currentStack[sliceIndex].TangentU, normal);
			XMStoreFloat3(&currentStack[sliceIndex].Normal, sliceNormal);
			currentStack[sliceIndex].TexC.x = uvOffset;
		}

		memcpy(V + (i * sliceCount), currentStack.data(), sliceCount * sizeof(Vertex));
		currentStack.clear();
		uvOffset += uvIncrement;
	}

	// Create indices:
	uint32_t* I = (uint32_t*)calloc(sizeof(uint32_t), TotalIndices);
	assert(I);

	uint32_t vi = 0;
	for (uint32_t i = 0; i < TotalIndices; i += 6) {
		// First triangle:
		I[i + 0] = (vi + sliceCount) % TotalVertices;
		I[i + 1] = (vi + 1) % TotalVertices;
		I[i + 2] = vi % TotalVertices;

		// Second triangle:
		I[i + 3] = (vi + 1) % TotalVertices;
		I[i + 4] = (vi + sliceCount) % TotalVertices;
		I[i + 5] = (vi + sliceCount + 1) % TotalVertices;
		vi++;

		
	}

	//Copy the indices and vertices to meshData.
	MeshData meshData;
	meshData.Vertices.assign(V, V + TotalVertices);
	meshData.Indices.assign(I, I + TotalIndices);

	free(I);
	free(V);

	return meshData;
}

MeshData GeometryGenerator::CreateDiamond(const float width, const float height, const float depth, XMFLOAT3 skew) {

	MeshData meshData;

	float skewX = skew.x * 0.5f;
	float skewY = skew.y * 0.5f;
	float skewZ = skew.x * 0.5f;

	float w2 = 0.5f * width;
	float h2 = 0.5f * height;
	float d2 = 0.5f * depth;
	
	// Calculate unique vertices
	XMFLOAT3 RightBottomBack( -w2, -h2, -d2 );
	XMFLOAT3 RightBottomFront( -w2, -h2, +d2 );
	XMFLOAT3 RightTopBack( -w2, +h2, -d2 );
	XMFLOAT3 RightTopFront( -w2, +h2, +d2 );
	XMFLOAT3 LeftTopBack( +w2, +h2, -d2 );
	XMFLOAT3 LeftTopFront( +w2, +h2, +d2 );
	XMFLOAT3 LeftBottomBack( +w2, -h2, -d2 );
	XMFLOAT3 LeftBottomFront( +w2, -h2, +d2 );


	// Apply X skew
	RightBottomBack.x += skewX;
	RightBottomFront.x += skewX;
	RightTopBack.x += skewX;
	RightTopFront.x += skewX;

	LeftTopBack.x -= skewX;
	LeftTopFront.x -= skewX;
	LeftBottomBack.x -= skewX;
	LeftBottomFront.x -= skewX;


	// Apply Y skew
	LeftTopFront.y += skewY;
	LeftTopBack.y += skewY;
	RightTopBack.y += skewY;
	RightTopFront.y += skewY;

	RightBottomBack.y -= skewY;
	RightBottomFront.y -= skewY;
	LeftBottomBack.y -= skewY;
	LeftBottomFront.y -= skewY;


	// Apply Z skew
	LeftTopFront.z += skewZ;
	RightTopFront.z += skewZ;
	LeftBottomFront.z += skewZ;
	RightBottomFront.z += skewZ;

	LeftTopBack.z -= skewZ;
	RightTopBack.z -= skewZ;
	LeftBottomBack.z -= skewZ;
	RightBottomBack.z -= skewZ;


	// Calculate face normals
	// ngl, this is kinda not efficient memory wise but who cares it's DirectX on Windows everyone!! We can be stupid!!! -Ethan
	XMFLOAT3 FrontNormal;
	XMFLOAT3 BackNormal;
	XMFLOAT3 TopNormal;
	XMFLOAT3 BottomNormal;
	XMFLOAT3 RightNormal;
	XMFLOAT3 LeftNormal;

	XMFLOAT3 FrontTangent;
	XMFLOAT3 BackTangent;
	XMFLOAT3 TopTangent;
	XMFLOAT3 BottomTangent;
	XMFLOAT3 RightTangent;
	XMFLOAT3 LeftTangent;

	// Reuse this one XMVECTOR for calculating the normal and tangent because why bother making 6 more definitions.
	XMVECTOR result;


	// Front Back
	result = MathHelper::NormalFromTrigon(RightBottomBack, RightTopBack, LeftTopBack);
	XMStoreFloat3(&FrontNormal, result);
	XMStoreFloat3(&BackNormal, XMVectorNegate(result));
	
	result = XMVector3Orthogonal(result);
	XMStoreFloat3(&FrontTangent, result);
	XMStoreFloat3(&BackTangent, XMVectorNegate(result));


	// Top Bottom
	result = MathHelper::NormalFromTrigon(RightTopBack, RightTopFront, LeftTopFront);
	XMStoreFloat3(&TopNormal, result);
	XMStoreFloat3(&BottomNormal, XMVectorNegate(result));

	result = XMVector3Orthogonal(result);
	XMStoreFloat3(&TopTangent, result);
	XMStoreFloat3(&BottomTangent, XMVectorNegate(result));
	

	// Left Right
	result = MathHelper::NormalFromTrigon(RightBottomFront, RightTopFront, RightTopBack);
	XMStoreFloat3(&LeftNormal, result);
	XMStoreFloat3(&RightNormal, XMVectorNegate(result));

	result = XMVector3Orthogonal(result);
	XMStoreFloat3(&LeftTangent, result);
	XMStoreFloat3(&RightTangent, XMVectorNegate(result));


	Vertex v[24]{
		/* Front */
		Vertex(RightBottomBack, FrontNormal,    FrontTangent,   XMFLOAT2(0.0f, 1.0f)),
		Vertex(RightTopBack,    FrontNormal,    FrontTangent,   XMFLOAT2(0.0f, 0.0f)),
		Vertex(LeftTopBack,     FrontNormal,    FrontTangent,   XMFLOAT2(1.0f, 0.0f)),
		Vertex(LeftBottomBack,  FrontNormal,    FrontTangent,   XMFLOAT2(1.0f, 1.0f)),
		/* Back */
		Vertex(RightBottomFront,BackNormal,     BackTangent,    XMFLOAT2(1.0f, 1.0f)),
		Vertex(LeftBottomFront, BackNormal,     BackTangent,    XMFLOAT2(0.0f, 1.0f)),
		Vertex(LeftTopFront,    BackNormal,     BackTangent,    XMFLOAT2(0.0f, 0.0f)),
		Vertex(RightTopFront,   BackNormal,     BackTangent,    XMFLOAT2(1.0f, 0.0f)),
		/* Top */
		Vertex(RightTopBack,    TopNormal,      TopTangent,     XMFLOAT2(0.0f, 1.0f)),
		Vertex(RightTopFront,   TopNormal,      TopTangent,     XMFLOAT2(0.0f, 0.0f)),
		Vertex(LeftTopFront,    TopNormal,      TopTangent,     XMFLOAT2(1.0f, 0.0f)),
		Vertex(LeftTopBack,     TopNormal,      TopTangent,     XMFLOAT2(1.0f, 1.0f)),
		/* Bottom */
		Vertex(RightBottomBack, BottomNormal,   BottomTangent,  XMFLOAT2(1.0f, 1.0f)),
		Vertex(LeftBottomBack,  BottomNormal,   BottomTangent,  XMFLOAT2(0.0f, 1.0f)),
		Vertex(LeftBottomFront, BottomNormal,   BottomTangent,  XMFLOAT2(0.0f, 0.0f)),
		Vertex(RightBottomFront,BottomNormal,   BottomTangent,  XMFLOAT2(1.0f, 0.0f)),
		/* Left */
		Vertex(RightBottomFront,LeftNormal,     LeftTangent,    XMFLOAT2(0.0f, 1.0f)),
		Vertex(RightTopFront,   LeftNormal,     LeftTangent,    XMFLOAT2(0.0f, 0.0f)),
		Vertex(RightTopBack,    LeftNormal,     LeftTangent,    XMFLOAT2(1.0f, 0.0f)),
		Vertex(RightBottomBack, LeftNormal,     LeftTangent,    XMFLOAT2(1.0f, 1.0f)),
		/* Right */
		Vertex(LeftBottomBack,  RightNormal,    RightTangent,   XMFLOAT2(0.0f, 1.0f)),
		Vertex(LeftTopBack,     RightNormal,    RightTangent,   XMFLOAT2(0.0f, 0.0f)),
		Vertex(LeftTopFront,    RightNormal,    RightTangent,   XMFLOAT2(1.0f, 0.0f)),
		Vertex(LeftBottomFront, RightNormal,    RightTangent,   XMFLOAT2(1.0f, 1.0f)),
	};
	

	meshData.Vertices.assign(&v[0], &v[24]);

	// Create the indices.
	//
	//

	uint32_t i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.Indices.assign(&i[0], &i[36]);

	return meshData;
}