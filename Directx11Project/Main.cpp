#include<d3d11_2.h>
#include<stdio.h>
#include<DirectXMath.h>
#include<windows.h>
#include<d3d11_2.h>
#include<d3d11shader.h>
using namespace DirectX;
HINSTANCE minst = NULL;
HWND mhnd = NULL;
WCHAR m_app[] = L"sample";
WCHAR m_class[] = L"sample11";

SIZE gsize = {640,480};

ID3D11Device* mp2d11device = NULL;

ID3D11DeviceContext* mcontext = NULL;
IDXGISwapChain* mchain = NULL;
ID3D11RenderTargetView* mrendertargetview = NULL;
D3D11_VIEWPORT mViewPort[1];

D3D10_PASS_DESC passdesc;

ID3D11InputLayout* vertexlayout;
//ID3DX11Effect* peffect;
ID3D10Blob * pBlob;

ID3D11Texture2D *sampleimg;
ID3D11Texture2D *sampleimg2;
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam);
void GeteRerouceViewFromTexture(ID3D11Texture2D* tex, ID3D11ShaderResourceView **resouceview);

struct MyVertex
{
	XMVECTOR Pos;
	XMVECTOR Color;
	XMVECTOR UV;
};
