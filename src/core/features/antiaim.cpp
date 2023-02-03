void Features::AntiAim::createMove(CUserCmd* cmd) {
    if (!Interfaces::engine->IsInGame()) {
        return;
    }
 
    if (!Globals::localPlayer || Globals::localPlayer->health() <= 0 || Globals::localPlayer->moveType() == 9) {
        // Player is dead or on a ladder, don't use anti-aim.
        return;
    }
 
    Weapon *weapon = (Weapon *) Interfaces::entityList->GetClientEntity((uintptr_t) Globals::localPlayer->activeWeapon() & 0xFFF);
    if(!weapon) {
        return;
    }
 
    if (weapon->itemIndex() == ItemIndex::WEAPON_DECOY || weapon->itemIndex() == ItemIndex::WEAPON_HEGRENADE || weapon->itemIndex() == ItemIndex::WEAPON_FLASHBANG || weapon->itemIndex() == ItemIndex::WEAPON_SMOKEGRENADE || weapon->itemIndex() == ItemIndex::WEAPON_MOLOTOV || weapon->itemIndex() == ItemIndex::WEAPON_INCGRENADE) {
        // Player is using a throwable, don't use anti-aim.
        return;
    }
 
    if ((cmd->buttons & (1 << 0)) || (cmd->buttons & (1 << 5))) {
        // Player is shooting, don't use anti-aim.
        return;
    }
 
    cmd->viewangles.x = CONFIGINT("Rage>AntiAim>Pitch");
 
    int real = 0;
    int fake = 0;
 
    int jitterDelay = CONFIGINT("Rage>AntiAim>Jitter>Jitter Delay");
    int jitterAmount = CONFIGINT("Rage>AntiAim>Jitter>Jitter Amount");
    int fakeJitterDelay = CONFIGINT("Rage>AntiAim>Fake Jitter>Jitter Delay");
    int fakeJitterAmount = CONFIGINT("Rage>AntiAim>Fake Jitter>Jitter Amount");
    int realJitterDelay = CONFIGINT("Rage>AntiAim>Real Jitter>Jitter Delay");
    int realJitterAmount = CONFIGINT("Rage>AntiAim>Real Jitter>Jitter Amount");
    int lbyDelay = CONFIGINT("Rage>AntiAim>LBY>LBY Delay");
    int lbyAmount = CONFIGINT("Rage>AntiAim>LBY>LBY Amount");
    int spinAmount = CONFIGINT("Rage>AntiAim>Spin>Spin Amount");
    int spinDelay = CONFIGINT("Rage>AntiAim>Spin>Spin Delay");
 
    switch (CONFIGINT("Rage>AntiAim>Type")) {
        case 1: { // Static
            real = cmd->viewangles.y + CONFIGINT("Rage>AntiAim>Offset");
            fake = CONFIGINT("Rage>AntiAim>Static>Desync");
            break;
        }
      case 2: { // Jitter
            int jitterAmt = (((cmd->tick_count % jitterDelay) < jitterDelay / 2) ? 1 : -1) * (jitterAmount - (jitterAmount/2));
            real = cmd->viewangles.y + CONFIGINT("Rage>AntiAim>Offset") + jitterAmt;
            fake = CONFIGINT("Rage>AntiAim>Jitter>Desync");
            break;
        }
        case 3: { // Fake Jitter
            int jitterAmt = (((cmd->tick_count % fakeJitterDelay) < fakeJitterDelay / 2) ? 1 : -1) * (fakeJitterAmount - (fakeJitterAmount/2));
            real = cmd->viewangles.y + CONFIGINT("Rage>AntiAim>Offset");
            fake = jitterAmt;
            break;
        }
        case 4: { // Real Jitter
            int jitterAmt = (((cmd->tick_count % realJitterDelay) < realJitterDelay / 2) ? 1 : -1) * (realJitterAmount - (realJitterAmount/2));
            real = cmd->viewangles.y + CONFIGINT("Rage>AntiAim>Offset") + jitterAmt;
            fake = CONFIGINT("Rage>AntiAim>Real Jitter>Desync");
            break;
        }
        case 5: { // LBY
            if (updatingLby()) {
                real = cmd->viewangles.y + CONFIGINT("Rage>AntiAim>Offset") + lbyAmount;
            } else {
                real = cmd->viewangles.y + CONFIGINT("Rage>AntiAim>Offset");
            }
            fake = CONFIGINT("Rage>AntiAim>LBY>Desync");
            break;
        }
        case 6: { // Spin
            int spinAmt = (((cmd->tick_count % spinDelay) < spinDelay / 2) ? 1 : -1) * (spinAmount - (spinAmount/2));
            real = cmd->viewangles.y + CONFIGINT("Rage>AntiAim>Offset") + spinAmt;
            fake = CONFIGINT("Rage>AntiAim>Spin>Desync");
            break;
        }
    }
 
    if (CONFIGINT("Rage>AntiAim>Fake>Method") == 1) {
        cmd->viewangles.y = real;
        cmd->viewangles.y += 180;
    } else {
        cmd->viewangles.y = fake;
    }
}
