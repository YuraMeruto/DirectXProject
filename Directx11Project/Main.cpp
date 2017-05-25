#include<d3d11_2.h>
#include<stdio.h>
#include<DirectXMath.h>
#include<windows.h>
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


struct CBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

