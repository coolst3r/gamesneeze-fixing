#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "../sdk.hpp"
#include "interfaces.hpp"
#include "../classes/entity.hpp"

struct Plane {
	Vector normal;
	float dist;
	unsigned char type;
	unsigned char signbits;
	unsigned char pad[2];
};

struct Surface {
	const char *name;
	short surfaceProps;
	unsigned short flags;
};

struct Trace {
	Vector startpos;
	Vector endpos;
	Plane plane;

	float fraction;

	int contents;
	unsigned int dispFlags;

	bool allsolid;
	bool startsolid;

	float fractionleftsolid;

	Surface surface;

	HitGroups hitgroup;
	short physicsbone;

	unsigned short worldSurfaceIndex;
	Player *m_pEntityHit;
	int hitbox;
};

class IHandleEntity {
public:
	virtual ~IHandleEntity() {  };
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(Player *pEntity, int contentsMask) = 0;

	virtual int GetTraceType() const = 0;
};

class TraceFilter : public ITraceFilter {
public:
	bool ShouldHitEntity(Player* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pSkip);
	}

	virtual int GetTraceType() const {
		return 0;
	}

	void *pSkip;
};

struct Ray;

class IEngineTrace {
public:
	int getPointContents(const Vector &vecAbsPos, int contentsMask = 0xFFFFFFFF, IHandleEntity **ppEntity = nullptr) {
		typedef int (* Fn)(void *, const Vector &, int contentsMask, IHandleEntity **);

		return getVirtualFunc<Fn>(this, 0)(this, vecAbsPos, contentsMask, ppEntity);
	}

	void ClipRayToEntity(Ray &ray, unsigned int fMask, IHandleEntity *pTraceFilter, Trace *pTrace) {
		typedef void (* Fn)(void *, Ray&, unsigned int, IHandleEntity *, Trace *);
		return getVirtualFunc<Fn>(this, 3)(this, ray, fMask, pTraceFilter, pTrace);
	}

	void TraceRay(Ray &ray, unsigned int fMask, ITraceFilter *pTraceFilter, Trace *pTrace) {
		typedef void (* Fn)(void *, Ray&, unsigned int, ITraceFilter *, Trace *);
		return getVirtualFunc<Fn>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}

	inline void clipToEntity(Ray *ray, unsigned int mask, IHandleEntity *filter, Trace *trace) {
		typedef void (* Fn)(void *, Ray *, unsigned int, IHandleEntity *, Trace *);

		return getVirtualFunc<Fn>(this, 3)(this, ray, mask, filter, trace);
	}

	inline void trace(Ray *ray, unsigned int mask, ITraceFilter *filter, Trace *trace) {
		typedef void (* Fn)(void *, Ray *, unsigned int, ITraceFilter *, Trace *);

		return getVirtualFunc<Fn>(this, 5)(this, ray, mask, filter, trace);
	}
};

namespace Interfaces {
    extern IEngineTrace *trace;
}

struct Ray {
	VectorAligned m_Start;
	VectorAligned m_Delta;
	VectorAligned m_StartOffset;
	VectorAligned m_Extents;

	const Matrix3x4 *m_pWorldAxisTransform;

	bool m_IsRay;
	bool m_IsSwept;

	Ray(Vector vecStart, Vector vecEnd) : m_pWorldAxisTransform(nullptr) {
		m_Delta = vecEnd - vecStart;
		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;
		m_pWorldAxisTransform = nullptr;
		m_IsRay = true;
		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;
		m_Start = vecStart;
	}

	Ray(Vector const &start, Vector const &end, Vector const &mins, Vector const &maxs) : m_pWorldAxisTransform(nullptr) {
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
	
	inline void filteredTrace(
		unsigned int mask,
		TraceFilter *filter,
		Trace *trace
	) {
	    Interfaces::trace->trace(this, mask, filter, trace);
	}

	inline void ignoredTrace(
	    unsigned int mask,
	    Player *ignore,
	    Trace *trace
	) {
		TraceFilter filter;
	
		filter.pSkip = ignore;

	    filteredTrace(mask, &filter, trace);
	}
};
