#pragma once
#include <cstdint>
//#include "../classes/entity.hpp"

class IClientEntityList {
public:
    // NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().   
	uintptr_t GetClientEntity(int entnum) {
		typedef uintptr_t (* Fn)(void *, int);
		return getVirtualFunc<Fn>(this, 3)(this, entnum);
	}

	void* GetClientEntityFromHandle(void *handle) {
		typedef void *(* Fn)(void *, void *);
		return getVirtualFunc<Fn>(this, 4)(this, handle);
	}

	int GetHighestEntityIndex() {
		typedef int (* Fn)(void *);
		return getVirtualFunc<Fn>(this, 6)(this);
	}

	// NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().   
	Entity *entity(int index) {
		typedef Entity *(* Fn)(void *, int);
		return getVirtualFunc<Fn>(this, 3)(this, index);
	}

	Player *player(int index) {
		return reinterpret_cast<Player *>(this->entity(index));
	}

	Weapon *weapon(int index) {
		return reinterpret_cast<Weapon *>(this->entity(index));
	}

	Weapon *activeWeapon(Player *player) {
		return this->weapon(reinterpret_cast<uintptr_t>(player->activeWeapon()) & 0xFFF);
	}

	Entity *entityFromHandle(int handle) {
		typedef Entity *(* Fn)(void *, int);
		return getVirtualFunc<Fn>(this, 4)(this, handle);
	}

	int highestEntityIndex() {
		typedef int (* Fn)(void *);
		return getVirtualFunc<Fn>(this, 6)(this);
	}
};
