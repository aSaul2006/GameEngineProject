//=============================================================================
// d3dUtil.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "d3dUtil.h"
#include "Vertex.h"
#define SAFE_DELETE(a) { delete (a); (a) = NULL; }

void GenTriGrid(int numVertRows, int numVertCols,
				float dx, float dy, 
				const D3DXVECTOR3& center, 
				std::vector<D3DXVECTOR3>& verts,
				std::vector<DWORD>& indices)
{
	int numVertices = numVertRows*numVertCols;
	int numCellRows = numVertRows-1;
	int numCellCols = numVertCols-1;

	int numTris = numCellRows*numCellCols*2;

	float width = (float)numCellCols * dx;
	float height = (float)numCellRows * dy;

	//===========================================
	// Build vertices.

	// We first build the grid geometry centered about the origin and on
	// the xz-plane, row-by-row and in a top-down fashion.  We then translate
	// the grid vertices so that they are centered about the specified 
	// parameter 'center'.

	verts.resize( numVertices );

	// Offsets to translate grid from quadrant 4 to center of 
	// coordinate system.
	float xOffset = -width * 0.5f; 
	float yOffset = height * 0.5f;

	int k = 0;
	for(float i = 0; i < numVertRows; ++i)
	{
		for(float j = 0; j < numVertCols; ++j)
		{
			// Negate the depth coordinate to put in quadrant four.  
			// Then offset to center about coordinate system.
			verts[k].x =  j * dx + xOffset;
			verts[k].y = -i * dy + yOffset;
			verts[k].z =  0.0f;

			// Translate so that the center of the grid is at the
			// specified 'center' parameter.
			D3DXMATRIX T;
			D3DXMatrixTranslation(&T, center.x, center.y, center.z);
			D3DXVec3TransformCoord(&verts[k], &verts[k], &T);
			
			++k; // Next vertex
		}
	}

	//===========================================
	// Build indices.

	indices.resize(numTris * 3);
	 
	// Generate indices for each quad.
	k = 0;
	for(DWORD i = 0; i < (DWORD)numCellRows; ++i)
	{
		for(DWORD j = 0; j < (DWORD)numCellCols; ++j)
		{
			indices[k]     =   i   * numVertCols + j;
			indices[k + 1] =   i   * numVertCols + j + 1;
			indices[k + 2] = (i+1) * numVertCols + j;
					
			indices[k + 3] = (i+1) * numVertCols + j;
			indices[k + 4] =   i   * numVertCols + j + 1;
			indices[k + 5] = (i+1) * numVertCols + j + 1;

			// next quad
			k += 6;
		}
	}
}

 static std::string utf16ToUTF8( const std::wstring &s )
{
    const int size = ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL );

    std::vector<char> buf( size );
    ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL );

    return std::string( &buf[0] );
}

static std::wstring utf8ToUTF16( const std::string &ws )
{
 const int size = ::MultiByteToWideChar( CP_UTF8, 0, ws.c_str(), -1, NULL, 0);

    std::vector<wchar_t> buf( size );
    ::MultiByteToWideChar( CP_UTF8, 0, ws.c_str(), -1, &buf[0], size);

    return std::wstring( &buf[0] );
}

void LoadXFile(
 const std::wstring& filename, 
 ID3DXMesh** meshOut,
 std::vector<Mtrl>& mtrls, 
 std::vector<IDirect3DTexture9*>& texs)
{
 // Step 1: Load the .x file from file into a system memory mesh
 std::wstring fileEnd = filename.c_str();
 std::wstring finalFileName = L"../Render/models/" + fileEnd;
 ID3DXMesh* meshSys      = 0;
 ID3DXBuffer* adjBuffer  = 0;
 ID3DXBuffer* mtrlBuffer = 0;
 DWORD numMtrls          = 0;

 HR(D3DXLoadMeshFromX(finalFileName.c_str(), D3DXMESH_SYSTEMMEM, g_d3dDevice,
  &adjBuffer, &mtrlBuffer, 0, &numMtrls, &meshSys));


 // Step 2: Find out if the mesh already has normal info?

 D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
 HR(meshSys->GetDeclaration(elems));
 
 bool hasNormals = false;
 D3DVERTEXELEMENT9 term = D3DDECL_END();
 for(int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
 {
  // Did we reach D3DDECL_END() {0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0}?
  if(elems[i].Stream == 0xff )
   break;

  if( elems[i].Type == D3DDECLTYPE_FLOAT3 &&
   elems[i].Usage == D3DDECLUSAGE_NORMAL &&
   elems[i].UsageIndex == 0 )
  {
   hasNormals = true;
   break;
  }
 }


 // Step 3: Change vertex format to VertexPNT.

 D3DVERTEXELEMENT9 elements[64];
 UINT numElements = 0;
 VertexPNT::Decl->GetDeclaration(elements, &numElements);

 ID3DXMesh* temp = 0;
 HR(meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, 
  elements, g_d3dDevice, &temp));
 ReleaseCOM(meshSys);
 meshSys = temp;


 // Step 4: If the mesh did not have normals, generate them.

 if( hasNormals == false)
  HR(D3DXComputeNormals(meshSys, 0));


 // Step 5: Optimize the mesh.

 HR(meshSys->Optimize(D3DXMESH_MANAGED | 
  D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, 
  (DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0, meshOut));
 ReleaseCOM(meshSys); // Done w/ system mesh.
 ReleaseCOM(adjBuffer); // Done with buffer.

 // Step 6: Extract the materials and load the textures.

 if( mtrlBuffer != 0 && numMtrls != 0 )
 {
  D3DXMATERIAL* d3dxmtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
  for(DWORD i = 0; i < numMtrls; ++i)
  {
   // Save the ith material.  Note that the MatD3D property does not have an ambient
   // value set when its loaded, so just set it to the diffuse value.
   Mtrl m;
   m.ambient   = d3dxmtrls[i].MatD3D.Diffuse;
   m.diffuse   = d3dxmtrls[i].MatD3D.Diffuse;
   m.spec      = d3dxmtrls[i].MatD3D.Specular;
   m.specPower = d3dxmtrls[i].MatD3D.Power;
   mtrls.push_back( m );

   d3dxmtrls[i].pTextureFilename = d3dxmtrls[i].pTextureFilename;
   // Check if the ith material has an associative texture
   if( d3dxmtrls[i].pTextureFilename != 0 )
   {
    // Yes, load the texture for the ith subset
    IDirect3DTexture9* tex = 0;
    std::wstring TextureFileName = utf8ToUTF16(std::string("../Render/models/")) + utf8ToUTF16(d3dxmtrls[i].pTextureFilename);
    //wchar_t* texFN = (wchar_t*)d3dxmtrls[i].pTextureFilename;
    HR(D3DXCreateTextureFromFile(g_d3dDevice, TextureFileName.c_str(), &tex));

    // Save the loaded texture
    texs.push_back( tex );
   }
   else
   {
    // No texture for the ith subset
    texs.push_back( 0 );
   }
  }
 }
 ReleaseCOM(mtrlBuffer); // done w/ buffer
}


//-------------------------------------------------------------------------------
//===============================================================================
//---------------------------Load Data from XML----------------------------------
//===============================================================================

//Instantiates variables every time cResourceManager finds an audio asset

cResource* cRenderManager::load3DFromXML(TiXmlElement *Element)
{
	if(Element)
	{
		cRenderResource* Resource = new cRenderResource();
		
		for(TiXmlAttribute* ElementAttrib = Element->FirstAttribute(); ElementAttrib; ElementAttrib= ElementAttrib->Next())
		{
			//examine our audio resource object
			std::string AttribName = ElementAttrib->Name();
			std::string AttribValue = ElementAttrib->Value();

			if(AttribName=="UID")
			{
				Resource->m_ResourceID = atoi(AttribValue.c_str());
				
			}
			if(AttribName=="filename")
			{
				Resource->m_FileName = AttribValue;
			}
			if(AttribName=="scenescope")
			{
				Resource->m_Scope = atoi(AttribValue.c_str());
			}

		}
		return Resource;
	}
	return NULL;
}


void cRenderResource::load()
{
	ID3DXMesh* tempMesh;
	LoadXFile(StringToLPCWSTR(m_FileName), &tempMesh, mMtrl, mTex);
	mMesh = tempMesh;
	
}

void cRenderResource::unload()
{
	if (mMesh != NULL)
	{
		SAFE_DELETE(mMesh);
	}

}
LPCWSTR cRenderResource::StringToLPCWSTR(std::string s_Variable)
{
	std::wstring ws_Temp = std::wstring(s_Variable.begin(), s_Variable.end());
	return ws_Temp.c_str();
}