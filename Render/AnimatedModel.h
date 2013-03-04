#ifndef ANIMATEDMODEL_H_
#define ANIMATEDMODEL_H_
#pragma once

#include "Model.h"
#include "AllocMeshHierarchy.h"
//#include "d3dUtil.h"
#include "Vertex.h"



struct FrameEx : public D3DXFRAME
{
	D3DXMATRIX toRoot;
};

class AnimatedModel : public Model
{
public:
	AnimatedModel(const std::wstring& modelFileName);
	AnimatedModel(const std::wstring& modelFileName, const std::wstring& textureFileName);
	~AnimatedModel(void);

	UINT numVertices();
	UINT numTriangles();
	UINT numBones();
	const D3DXMATRIX* getFinalXFormArray();

	void update(float deltaTime);
	void draw(D3DXMATRIX wVPM);
	void setAnimTrack(UINT num);

protected:
	D3DXFRAME* findNodeWithMesh(D3DXFRAME* frame);
	bool hasNormals(ID3DXMesh* mesh);
	void buildSkinnedMesh(ID3DXMesh* mesh);
	void buildToRootXFormPtrArray();
	void buildToRootXForms(FrameEx* frame, D3DXMATRIX& parentsToRoot);

	// We do not implement the required functionality to do deep copies,
	// so restrict copying.
	//SkinnedMesh(const SkinnedMesh& rhs);
	//SkinnedMesh& operator=(const SkinnedMesh& rhs);

protected:
	//ID3DXMesh*     mSkinnedMesh;
	D3DXFRAME*     mRoot;
	DWORD          mMaxVertInfluences;
	DWORD          mNumBones;
	ID3DXSkinInfo* mSkinInfo;
	ID3DXAnimationController* mAnimCtrl;  
	ID3DXAnimationSet* pAnimSet;
	
	std::vector<D3DXMATRIX>  mFinalXForms;
	std::vector<D3DXMATRIX*> mToRootXFormPtrs;
	

	static const int MAX_NUM_BONES_SUPPORTED = 35; 

	Mtrl		m_WhiteMtrl;
};


#endif