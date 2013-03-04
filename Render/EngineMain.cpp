/************************************************************************
*  Program Name: Engine main.cpp                                               
*  Name: GSP420 Rendering Core - Mike Murrell, Anthony Garza, Jesse Goldan, Christopher Carlos                                                                                         *
*  Date: January 28th - February 1st
*  Description: Mainly for the rendering of the engine
*  Update: February 8, 2013
//=============================================================================
// Modified from Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================   
************************************************************************/


#include "EngineMain.h"

cResourceManager* resMan = new cResourceManager();
static std::wstring utf8ToUTF16( const std::string &ws );

//Execution now begins in main core
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
//				   PSTR cmdLine, int showCmd)
//{
//	// Enable run-time memory check for debug builds.
//	#if defined(DEBUG) | defined(_DEBUG)
//		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//	#endif
//
//	EngineMain app(hInstance, L"GSP420 Engine", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
//	g_d3dApp = &app;
//
//	//DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
//	//gDInput = &di;
//
//    return g_d3dApp->run();
//}

EngineMain::EngineMain(HINSTANCE hInstance, std::wstring winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{

	if(!checkDeviceCaps())
	{
		MessageBox(0, L"checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	
	Shaders::InitAll();
	InitAllVertexDeclarations();
	

	m_GfxStats = new GfxStats();

	m_DirLight.Direction	= D3DXVECTOR3(0.0f, 1.0f, 2.0f);
	D3DXVec3Normalize(&m_DirLight.Direction, &m_DirLight.Direction);
	m_DirLight.Ambient		= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLight.Diffuse		= D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_DirLight.Specular		= D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);



	camera.SetPosition(D3DXVECTOR3(0.0f, 0.0f, -10.0f));
	camera.SetTarget(D3DXVECTOR3(0.0f, 3.0f, 0.0f));
	camera.SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	

	dwarf =  new Model(L"dwarf.x");
	skull =	 new Model(L"skullocc.x");
	tiny  =  new Model(L"tiny.x");

	m_GfxStats->addVertices(skull->m_Model->GetNumVertices());
	m_GfxStats->addTriangles(skull->m_Model->GetNumFaces());
	m_GfxStats->addVertices(dwarf->m_Model->GetNumVertices());
	m_GfxStats->addTriangles(dwarf->m_Model->GetNumFaces());
	m_GfxStats->addVertices(tiny->m_Model->GetNumVertices());
	m_GfxStats->addTriangles(tiny->m_Model->GetNumFaces());

	skull->InitModel(D3DXVECTOR3(-4.0f, 3.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f);
	skull->Scale(0.5f);
	dwarf->InitModel(D3DXVECTOR3(0.0f, 3.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f);
	dwarf->Scale(1.0f);
	tiny->InitModel(D3DXVECTOR3(0.0f, 3.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f);
	tiny->Scale(0.01f);

#pragma region Grid
	mLightVecW     = D3DXVECTOR3(0.0, 0.707f, -0.707f);
	mDiffuseMtrl   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mDiffuseLight  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientMtrl   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientLight  = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	mSpecularMtrl  = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	mSpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mSpecularPower = 8.0f;
	
	std::wstring groundTex = utf8ToUTF16(std::string("../Render/models/ground0.dds"));
	HR(D3DXCreateTextureFromFile(g_d3dDevice, groundTex.c_str(), &mGroundTex));

	buildFX();
	buildGridGeometry();

#pragma endregion

	onResetDevice();
}

EngineMain::~EngineMain()
{
	delete m_GfxStats;
	delete(skull);
	delete(dwarf);
	delete(tiny);

	ReleaseCOM(mGridVB);
	ReleaseCOM(mGridIB);
	ReleaseCOM(mGroundTex);
	ReleaseCOM(mFX);

	DestroyAllVertexDeclarations();
	Shaders::DestroyAll();
}

bool EngineMain::checkDeviceCaps()
{
	D3DCAPS9 caps;
	HR(g_d3dDevice->GetDeviceCaps(&caps));

	// Check for vertex shader version 3.0 support.
	if( caps.VertexShaderVersion < D3DVS_VERSION(2, 0) )
		return false;

	// Check for pixel shader version 3.0 support.
	if( caps.PixelShaderVersion < D3DPS_VERSION(2, 0) )
		return false;

	return true;
}

void EngineMain::onLostDevice()
{
	m_GfxStats->onLostDevice();
	HR(Shaders::BasicFX->m_FX->OnLostDevice());
	HR(Shaders::VBlendFX->m_FX->OnLostDevice());
}

void EngineMain::onResetDevice()
{
	m_GfxStats->onResetDevice();
	HR(Shaders::BasicFX->m_FX->OnResetDevice());
	HR(Shaders::VBlendFX->m_FX->OnResetDevice());

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
}

void EngineMain::updateScene(float dt)
{
	m_GfxStats->update(dt);
	buildViewMtx();
	skull->Rotate(sin(dt));
	tiny->Rotate(sin(dt));
}


void EngineMain::drawScene()
{
	// Clear the backbuffer and depth buffer.
	HR(g_d3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));
	
	HR(g_d3dDevice->BeginScene());

	D3DXMATRIX wVPM = camera.GetView()*camera.GetProjection();



	// Setup the rendering FX
	Shaders::BasicFX->SetDirectionalLight(&m_DirLight);
	Shaders::BasicFX->m_FX->SetTechnique(Shaders::BasicFX->m_hTech);
	// Begin passes.
	UINT numPasses = 0;


	Shaders::BasicFX->m_FX->Begin(&numPasses, 0);
	Shaders::BasicFX->m_FX->BeginPass(0);
	skull->DrawModel(wVPM);
	dwarf->DrawModel(wVPM);
	tiny->DrawModel(wVPM);

	Shaders::BasicFX->m_FX->EndPass();
	Shaders::BasicFX->m_FX->End();

	// Animation Passes //
	Shaders::VBlendFX->SetDirectionalLight(&m_DirLight);
	//HR(Shaders::VBlendFX->m_FX->SetValue(Shaders::VBlendFX->m_hLight, &m_DirLight, sizeof(DirectionalLight))); 
	Shaders::VBlendFX->m_FX->SetTechnique(Shaders::VBlendFX->m_hTech);
	numPasses = 0;
	Shaders::VBlendFX->m_FX->Begin(&numPasses, 0);
	Shaders::VBlendFX->m_FX->BeginPass(0);


	//tiny->draw(wVPM);

	Shaders::VBlendFX->m_FX->EndPass();
	Shaders::VBlendFX->m_FX->End();
	// End //

	#pragma region Draw Grid
	HR(mFX->SetTechnique(mhTech));

	HR(mFX->SetMatrix(mhWVP, &wVPM));
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &m_World);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	HR(mFX->SetMatrix(mhWorldInvTrans, &worldInvTrans));
	HR(mFX->SetValue(mhLightVecW, &mLightVecW, sizeof(D3DXVECTOR3)));
	HR(mFX->SetValue(mhDiffuseMtrl, &mDiffuseMtrl, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhDiffuseLight, &mDiffuseLight, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhAmbientMtrl, &mAmbientMtrl, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhAmbientLight, &mAmbientLight, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhSpecularLight, &mSpecularLight, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhSpecularMtrl, &mSpecularMtrl, sizeof(D3DXCOLOR)));
	HR(mFX->SetFloat(mhSpecularPower, mSpecularPower));
	HR(mFX->SetMatrix(mhWorld, &m_World));
	HR(mFX->SetTexture(mhTex, mGroundTex));

	HR(g_d3dDevice->SetVertexDeclaration(VertexPNT::Decl));
	HR(g_d3dDevice->SetStreamSource(0, mGridVB, 0, sizeof(VertexPNT)));
	HR(g_d3dDevice->SetIndices(mGridIB));

	// Begin passes.
	numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		HR(g_d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mNumGridVertices, 0, mNumGridTriangles));
		HR(mFX->EndPass());
	}
	HR(mFX->End());

#pragma endregion

	m_GfxStats->display();

	HR(g_d3dDevice->EndScene());

	// Present the backbuffer.
	HR(g_d3dDevice->Present(0, 0, 0, 0));
}

void EngineMain::buildViewMtx()
{

	camera.Render();
	Shaders::BasicFX->SetEyePosition(camera.GetPosition());
}

void EngineMain::buildProjMtx()
{
	float w = (float)m_d3dPP.BackBufferWidth;
	float h = (float)m_d3dPP.BackBufferHeight;
	camera.SetAspectRatio(w/h);
	D3DXMatrixPerspectiveFovLH(&camera.GetProjection(), camera.GetFOV(), camera.GetAspectRatio(), camera.GetNearValue(), camera.GetFarValue());
}

void EngineMain::buildFX()
{
	// Create the FX from a .fx file.
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(g_d3dDevice, L"../Render/fx/DirLightTex.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors));
	if( errors )
		MessageBox(0, (wchar_t*)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	mhTech          = mFX->GetTechniqueByName("DirLightTexTech");
	mhWVP           = mFX->GetParameterByName(0, "gWVP");
	mhWorldInvTrans = mFX->GetParameterByName(0, "gWorldInvTrans");
	mhLightVecW     = mFX->GetParameterByName(0, "gLightVecW");
	mhDiffuseMtrl   = mFX->GetParameterByName(0, "gDiffuseMtrl");
	mhDiffuseLight  = mFX->GetParameterByName(0, "gDiffuseLight");
	mhAmbientMtrl   = mFX->GetParameterByName(0, "gAmbientMtrl");
	mhAmbientLight  = mFX->GetParameterByName(0, "gAmbientLight");
	mhSpecularMtrl  = mFX->GetParameterByName(0, "gSpecularMtrl");
	mhSpecularLight = mFX->GetParameterByName(0, "gSpecularLight");
	mhSpecularPower = mFX->GetParameterByName(0, "gSpecularPower");
	mhEyePos        = mFX->GetParameterByName(0, "gEyePosW");
	mhWorld         = mFX->GetParameterByName(0, "gWorld");
	mhTex           = mFX->GetParameterByName(0, "gTex");
}

void EngineMain::buildGridGeometry()
{
	std::vector<D3DXVECTOR3> verts;
	std::vector<DWORD> indices;

	GenTriGrid(100, 100, 1.0f, 1.0f, 
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), verts, indices);

	// Save vertex count and triangle count for DrawIndexedPrimitive arguments.
	mNumGridVertices  = 100*100;
	mNumGridTriangles = 99*99*2;

	// Obtain a pointer to a new vertex buffer.
	HR(g_d3dDevice->CreateVertexBuffer(mNumGridVertices * sizeof(VertexPNT), 
		D3DUSAGE_WRITEONLY,	0, D3DPOOL_MANAGED, &mGridVB, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// grid's vertex data.
	VertexPNT* v = 0;
	HR(mGridVB->Lock(0, 0, (void**)&v, 0));

	float texScale = 0.2f;
	for(int i = 0; i < 100; ++i)
	{
		for(int j = 0; j < 100; ++j)
		{
			DWORD index = i * 100 + j;
			v[index].pos    = verts[index];
			v[index].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			v[index].tex0 = D3DXVECTOR2((float)j, (float)i) * texScale;
		}
	}

	HR(mGridVB->Unlock());


	// Obtain a pointer to a new index buffer.
	HR(g_d3dDevice->CreateIndexBuffer(mNumGridTriangles*3*sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &mGridIB, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// grid's index data.

	WORD* k = 0;
	HR(mGridIB->Lock(0, 0, (void**)&k, 0));

	for(DWORD i = 0; i < mNumGridTriangles*3; ++i)
		k[i] = (WORD)indices[i];

	HR(mGridIB->Unlock());
}

static std::wstring utf8ToUTF16( const std::string &ws )
{
 const int size = ::MultiByteToWideChar( CP_UTF8, 0, ws.c_str(), -1, NULL, 0);

    std::vector<wchar_t> buf( size );
    ::MultiByteToWideChar( CP_UTF8, 0, ws.c_str(), -1, &buf[0], size);

    return std::wstring( &buf[0] );
}