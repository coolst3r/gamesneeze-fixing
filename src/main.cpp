#include "core/menu/config.hpp"
#include "includes.hpp"
#include <unistd.h>

/* initialise everything in */
void MainThread() {
    /* if serverbrowser is not open then wait, (serverbrowser is last to be loaded) */
    while (!dlopen("./bin/linux64/serverbrowser_client.so", RTLD_NOLOAD | RTLD_NOW))
        usleep(500*1000);
    try {
        Log::log(LOG, "Initialising...");

        Config::init();
        
        /* Initialise interfaces */
        if (!Interfaces::init()) {
            Log::log(ERR, "Failed to initialise interfaces!");
        }

        /* Initialise netvars/offsets */
        if (!Netvar::init()) {
            Log::log(ERR, "Failed to initialise netvars!");
        }

        /* Initialise hooks */
        if (!Hooks::init()) {
            Log::log(ERR, "Failed to initialise hooks!");
        }

        Features::Notifications::addNotification(ImColor(30, 255, 30), "[gs] Successfully injected!");
        Log::log(LOG, "Initialised!");
    } catch(const std::exception& e) {
        Log::log(ERR, e.what());
    }
}

/* Called when injected */
int __attribute__((constructor)) Main() {
	std::thread mainThread(MainThread);
    
	mainThread.detach();
    return 0;
}
