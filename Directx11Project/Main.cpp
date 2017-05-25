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

HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* g_pd3Device = NULL;
ID3D11DeviceContext* g_pImmediateContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* RenderTarget = NULL;
ID3D11Texture2D* DepthStencil = NULL;
ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
ID3D11VertexShader* VertexShader = NULL;
ID3D11PixelShader* PixselShader = NULL;
ID3D11InputLayout* VertexLayout = NULL;
ID3D10Buffer*IndexBuffer = NULL;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;
ID3D11ShaderResourceView*           g_pTextureRV = NULL;
ID3D11SamplerState*                 g_pSamplerLinear = NULL;
XMMATRIX g_world;
XMMATRIX g_view;
XMMATRIX g_project;
XMFLOAT4 g_mesh(0.7f, 0.7f, 0.7f,1.0f);