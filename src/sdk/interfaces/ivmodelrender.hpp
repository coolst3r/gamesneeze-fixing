#pragma once
#include "../classes/vector.h"
#include "imaterialsystem.hpp"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/ivmodelrender.h

struct DrawModelState_t {
	void*			m_pStudioHdr;
	void*			m_pStudioHWData;
	void*		m_pRenderable;
	const Matrix3x4		*m_pModelToWorld;
	void*		m_decals;
	int						m_drawFlags;
	int						m_lod;
};

struct model_t {
	char name[255];
};

struct ModelRenderInfo_t {
	Vector origin;
	QAngle angles; 
	char _padding[0x4];
	void** pRenderable;
	const model_t* pModel;
	const Matrix3x4* pModelToWorld;
	const Matrix3x4* pLightingOffset;
	const Vector* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	void* instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

class IVModelRender {
public:
    void ForcedMaterialOverride(IMaterial* mat) {
		typedef void (*Fn)(void*, IMaterial*, int, int);
		return getVirtualFunc<Fn>(this, 1)(this, mat, 0, 0);
	}

	void DrawModelExecute(void* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, Matrix3x4 *pCustomBoneToWorld = NULL) {
		typedef void (*Fn)(void*, void* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, Matrix3x4 *pCustomBoneToWorld);
		return getVirtualFunc<Fn>(this, 21)(this, ctx, state, pInfo, pCustomBoneToWorld);
	}
};
