//
// Created by Marc Leclair on 1/30/20.
//

#ifndef ANDROID_NAVIGATION_RESOURCEBLOCKER_H
#define ANDROID_NAVIGATION_RESOURCEBLOCKER_H


#include <cstdint>

class ResourceBlocker {
    public:
        void useFullCPU();
        void useMemory(uint32_t aMBPerSec);
        void reconfigureMemory(uint32_t pressure, uint32_t threadCount);
        void reconfigureCPU(uint32_t numCores);
        void stopAndClean();
        void initStats();
        double getCurrentValue();
};



#endif //ANDROID_NAVIGATION_RESOURCEBLOCKER_H
