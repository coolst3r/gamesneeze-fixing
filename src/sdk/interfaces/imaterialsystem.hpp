#pragma once

class KeyValues;

class IMaterialVar {
public:
    void setTint(float r, float g, float b) {
		typedef void (* Fn)(void *, float, float, float);

		return getVirtualFunc<Fn>(this, 12)(this, r, g, b);
    }
};

class IMaterial {
public:
	virtual const char *getName();
	virtual const char *getTextureGroupName();

	IMaterialVar *findVar(const char *name, bool *found, bool complain = true) {
		typedef IMaterialVar *(* Fn)(void *, const char *, bool *, bool);

		return getVirtualFunc<Fn>(this, 11)(this, name, found, complain);
	}

	IMaterialVar *findVar(const char *name) {
		auto found = false;
		auto material = findVar(name, &found);

		return found ? material : nullptr;
	}

	void setColor(ImColor& color) {
		typedef void (* SetAlphaFn)(void *, float);
		typedef void (* SetColorFn)(void *, float, float, float);

		getVirtualFunc<SetAlphaFn>(this, 27)(this, color.Value.w);
		getVirtualFunc<SetColorFn>(this, 28)(this, color.Value.x, color.Value.y, color.Value.z);

	    if (auto var = findVar("$envmaptint"); var != nullptr) {
	        var->setTint(color.Value.x, color.Value.y, color.Value.z);
	    }
	}

private:
	void setFlag(int flag, bool enabled) {
		typedef void (* Fn)(void *, int, bool);

		return getVirtualFunc<Fn>(this, 29)(this, flag, enabled);
	}

public:
	void ignoreZ(bool enabled) {
		setFlag(1 << 15 /* MATERIAL_VAR_IGNOREZ */, enabled);
	}
	
	void wireframe(bool enabled) {
		setFlag(1 << 28 /* MATERIAL_VAR_WIREFRAME */, enabled);
	}

	ImColor color() {
		typedef float (* AlphaFn)(void *);
		typedef void (* ColorFn)(void *, float&, float&, float&);

		auto color = ImColor{0.0f, 0.0f, 0.0f, getVirtualFunc<AlphaFn>(this, 44)(this)};

		getVirtualFunc<ColorFn>(this, 45)(this, color.Value.x, color.Value.z, color.Value.y);

		return color;
	}
};

class IMaterialSystem {
public:
	IMaterial *createMaterial(const char *pMaterialName, KeyValues *pVMTKeyValues) {
		typedef IMaterial *(* Fn)(void *, const char *, KeyValues *);

		return getVirtualFunc<Fn>(this, 83)(this, pMaterialName, pVMTKeyValues);
	}

	IMaterial *findMaterial(
		char const *pMaterialName,
		const char *pTextureGroupName = nullptr,
		bool complain = true,
		const char *pComplainPrefix = 0
	) {
		typedef IMaterial *(* Fn)(void *, char const *, const char *, bool, const char *);

		return getVirtualFunc<Fn>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	unsigned short FirstMaterial() {
		typedef unsigned short (*Fn)( void* );
		return getVirtualFunc<Fn>( this, 86 )( this );
	}

	unsigned short NextMaterial( unsigned short h ) {
		typedef unsigned short (*Fn)( void*, unsigned short );
		return getVirtualFunc<Fn>( this, 87 )( this, h );
	}

	unsigned short InvalidMaterial() {
		typedef unsigned short (*Fn)( void* );
		return getVirtualFunc<Fn>( this, 88 )( this );
	}

	IMaterial* GetMaterial( unsigned short h ) {
		typedef IMaterial* (*Fn)( void*, unsigned short );
		return getVirtualFunc<Fn>( this, 89 )( this, h );
	}
};
