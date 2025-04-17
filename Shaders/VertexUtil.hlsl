struct geomIN {
    float4 clip : SV_POSITION;
    float3 world : POSITION;
    float3 normal : NORMAL;
};

struct edge
{
    float2 pointA   : EDGEP0;
    float2 dirA     : EDGEDIR0;
    
    float2 pointB   : EDGEP1;
    float2 dirB    : EDGEDIR1;
};


float4 GetMidpoint(float4 a, float4 b) {
    return a + (b - a) * 0.5;
}

float GetSqrDistance(float4 a, float4 b) {
    float4 v = a + b;
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}
uint GetTriangleCase(float z0, float z1, float z2)
{
    if (z0 > 0)
    {
        if (z1 > 0)
        {
            if (z2 > 0) return 0;
            else return 1;
        }
        else
        {
            if (z2 > 0) return 2;
            else return 3;
        }
    }
    else
    {
        if (z1 > 0)
        {
            if (z2 > 0) return 4;
            else return 5;
        }
        else
        {
            if (z2 > 0) return 6;
            else return 7;
        }
    }
}

void ComputeEdges(geomIN IN[3], out edge edgeData)
{
    uint triangleCase = GetTriangleCase(IN[0].clip.z, IN[1].clip.z, IN[2].clip.z);
    if (triangleCase == 0)
    {
        edgeData = (edge) 0;
        return;
    }
    
    float w0 = 1.0f / IN[0].clip.w;
    float w1 = 1.0f / IN[1].clip.w;
    float w2 = 1.0f / IN[2].clip.w;
    
    float2 v0 = (IN[0].clip * w0).xy;
    float2 v1 = (IN[1].clip * w1).xy;
    float2 v2 = (IN[2].clip * w2).xy;
    
    edge e = (edge)0;
    float2 v;
    
    switch (triangleCase)
    {
        case 1:
            e.pointA = v0;
            e.pointB = v1;
            v = v0.xy + (IN[2].clip * IN[2].clip).xy;
            e.dirA = normalize(v0 - v);
            e.dirB = normalize(v1 - v);
            break;
        case 2:
            e.pointA = v0;
            e.pointB = v2;
            v = v0.xy + (IN[2].clip * IN[2].clip).xy;
            e.dirA = normalize(v0 - v);
            e.dirB = normalize(v2 - v);
            break;
        case 3:
            e.pointA = v0;
            e.pointB = v0;
            e.dirA = normalize(v0 - (IN[0].clip * IN[1].clip))

    }
    
    edgeData = e;
}

void CalculateTriangleHeights(in geomIN vertexPositions[3], out float distances[3]) {    
    for (int i = 0; i < 3; i++) {
        
        float4 midpoint = GetMidpoint(vertexPositions[i].clip, vertexPositions[(i + 1) % 3].clip);
        distances[i] = GetSqrDistance(midpoint, vertexPositions[(i + 1) % 3].clip);
    }
}

float GetShortestDistanceFromEdges(in float4 points[3], in float4 clipPosition) {
    float distance = 1.175494351e38;
    for (int i = 0; i < 3; i++) {
        float d = GetSqrDistance(points[i], clipPosition);
        if (d < distance)
            distance = d;
    }

    if (distance < 0)
        distance = 1;
    else
        distance = sqrt(distance);

    return distance;
}