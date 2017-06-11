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
	XMFLOAT3 Pos;
	XMFLOAT3 Color;
	XMFLOAT2 UV;
};


MyVertex vtx[] = {
	XMFLOAT3(-0.8f,-0.8f,0.5f),
	XMFLOAT3(1.0f,1.0f,1.0f),
	XMFLOAT2(0.0f,1.0f),

	XMFLOAT3(-0.8f,-0.8f,0.5f),
	XMFLOAT3(1.0f,1.0f,1.0f),
	XMFLOAT2(0.0f,0.0f),

	XMFLOAT3(0.8f,-0.8f,0.5f),
	XMFLOAT3(1.0f,1.0f,1.0f),
	XMFLOAT2(1.0f,1.0f),

	XMFLOAT3(-0.8f,-0.8f,0.5f),
	XMFLOAT3(1.0f,1.0f,1.0f),
	XMFLOAT2(0.0f,0.0f),

	XMFLOAT3(0.8f,0.8f,0.5f),
	XMFLOAT3(1.0f,1.0f,1.0f),
	XMFLOAT2(1.0f,0.0f),

	XMFLOAT3(0.8f,-0.8f,0.5f),
	XMFLOAT3(1.0f,1.0f,1.0f),
	XMFLOAT2(1.0f,1.0f),
};

//アプリの初期化

HRESULT InitApp(HINSTANCE hInst)
{
	minst = hInst;
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	//	wc.lpszClassName = m_class;

	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = 640;
	rc.bottom = 480;
	AdjustWindowRect(&rc,WS_OVERLAPPEDWINDOW,TRUE);
	mhnd = CreateWindow(
		m_class,
		m_app,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInst,
		NULL
	);
	ShowWindow(mhnd,SW_SHOWNORMAL);
	UpdateWindow(mhnd);
	return S_OK;
}

HRESULT InitDirect3D(void)
{
	RECT rc;
	GetClientRect(mhnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = mhnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	D3D_FEATURE_LEVEL futurelevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pfturelevel = NULL;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
	NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&futurelevel,
		1,
		D3D11_SDK_LAYERS_VERSION,
		&sd,
		&mchain,
		&mp2d11device,
		pfturelevel,
		&mcontext
	);

	if (FAILED(hr))
	{
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_REFERENCE,
			NULL,
			0,
			&futurelevel,
			1,
			D3D11_SDK_VERSION,
			&sd,
			&mchain,
			&mp2d11device,
			pfturelevel,
			&mcontext
		);	
	}
	InitBackBuffer(width, height);
	return S_OK;
}


HRESULT InitBackBuffer(UINT width, UINT height)
{
	HRESULT hr;

	ID3D11Texture2D * pBackBuffer;
	hr = mchain->GetBuffer(0, _uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	hr = mp2d11device->CreateRenderTargetView(
		pBackBuffer,
		NULL,
		&mrendertargetview
	);
	pBackBuffer->Release();
	mViewPort[0].TopLeftX = 0;//左X座標
	mViewPort[0].TopLeftY = 0;//左X座標
	mViewPort[0].Width = width;
	mViewPort[0].Height = height;
	mViewPort[0].MinDepth = 0.0f;
	mViewPort[0].MaxDepth = 1.0f;
	return S_OK;
}


HRESULT Render(void)
{
	HRESULT hr;
	XMMATRIX matX, matZ, matInit, matScalling;

	float rotate = (float)(XM_PI * (timeGetTime() % 2000)) / 2000.0f;
	matZ = XMMatrixRotationZ(rotate);
	rotate = (float)(XM_PI * (timeGetTime() % 1500)) / 5000.0f;
	matX = XMMatrixRotationX(rotate);
	matInit = XMMatrixIdentity();
	matScalling = XMMatrixScaling(4.0f, 3.0f, 14.0f);
	matInit = XMMatrixMultiply(matInit, matScalling);


	XMMATRIX matView;
	matView = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1), XMVectorSet(0.0f, 0.0f, 1.0f, 1), XMVectorSet(0.0f, 1.0f, 0.0f, 1));

	XMMATRIX mat;
	XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)640 / (float)480, -10.0f, 800.0f);
	float ClearColor[4] = { 0.0f,0.5f,0.7f,1.0f };
	mcontext->ClearRenderTargetView(mrendertargetview, ClearColor);
	mcontext->RSSetViewports(1, mViewPort);
	mcontext->OMSetRenderTargets(1,&mrendertargetview,NULL);
	
	hr = mchain->Present(0, 0);
	return hr;
}


bool CleanupDirect3D(void)
{
	if (mp2d11device) mcontext->ClearState();
	return true;
}

bool CleanAPP(void)
{
	UnregisterClass(m_class, minst);
	return true;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, UINT wparam, LONG lparam)
{
	HRESULT hr = S_OK;

	switch (msg)
	{
	case WM_DESTROY:
		CleanupDirect3D();
		PostQuitMessage(0);
		mhnd = NULL;
		return 0;

	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
		break;
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}

HRESULT IsDeviceRemoved(void)
{
	HRESULT hr;
	hr = mp2d11device->GetDeviceRemovedReason();
	switch (hr) {
	case S_OK:
		break;         // 正常

	case DXGI_ERROR_DEVICE_HUNG:
	case DXGI_ERROR_DEVICE_RESET:
		CleanupDirect3D();				// Direct3Dの解放(アプリケーション定義)
		hr = InitDirect3D();			// Direct3Dの初期化(アプリケーション定義)
		if (FAILED(hr))	return hr; // 失敗。アプリケーションを終了
		break;

	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
	case E_OUTOFMEMORY:
	default:
		return hr;   // どうしようもないので、アプリケーションを終了。
	};
	return S_OK;

}


bool AppIdle(void)
{
	if (!mp2d11device)	return false;
	HRESULT hr;
	hr = IsDeviceRemoved();
	if (FAILED(hr))	return false;

	Render();
	return true;
}

int WINAPI wWinMain(HINSTANCE hInst,HINSTANCE, LPWSTR, int)
{
	HRESULT hr = InitApp(hInst);

}
