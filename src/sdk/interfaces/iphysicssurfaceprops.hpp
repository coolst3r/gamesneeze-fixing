#pragma once

struct SurfacePhysicsParams {
    float friction;
    float elasticity;
    float density;
    float thickness;
    float dampening;
};

struct SurfaceGameProps {
    float flPenetrationModifier;
    float flDamageModifier;
    unsigned short material;
};

struct SurfaceData {
    SurfacePhysicsParams physics;
    std::byte _pad[68];
    SurfaceGameProps game;
};

class IPhysicsSurfaceProps {
public:
    virtual ~IPhysicsSurfaceProps(void) {}

    // parses a text file containing surface prop keys
    virtual int parseSurfaceData(const char *pFilename, const char *pTextfile) = 0;

    // current number of entries in the database
    virtual int surfacePropCount(void) const = 0;

    virtual int surfaceIndex(const char *pSurfacePropName) const = 0;

    virtual void physicsProperties(int surfaceDataIndex, float *density, float *thickness, float *friction, float *elasticity) const = 0;

    virtual SurfaceData *surfaceData(int surfaceDataIndex) = 0;

    virtual const char *string(unsigned short stringTableIndex) const = 0;

    virtual const char *propName(int surfaceDataIndex) const = 0;

    // sets the global index table for world materials
    virtual void setWorldMaterialIndexTable(int *pMapArray, int mapSize) = 0;

    // NOTE: Same as getPhysicsProperties, but maybe more convenient
    virtual void getPhysicsParameters(int surfaceDataIndex, SurfacePhysicsParams *pParamsOut) const = 0;
};
