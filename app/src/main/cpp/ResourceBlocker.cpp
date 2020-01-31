#include "ResourceBlocker.h"
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <android/log.h>
#include <random>
#include <sstream>
#include <chrono>
#ifdef WIN32
#include <Windows.h>
#endif

#include <android/log.h>

#define  LOG_TAG    "someTag"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

using namespace std;
using namespace std::chrono;

bool kTerminateCPU = false;
bool kTerminateMemory = false;

std::vector<thread> kCPUThreads;
std::vector<thread> kMemoryThreads;


double foundTotal;


void ResourceBlocker::useFullCPU()
    {

        std::ostringstream ss;
        ss << std::this_thread::get_id();
        std::string idstr = ss.str();

      __android_log_print(ANDROID_LOG_ERROR, "Starting CPU Load on thread", "%s", idstr.c_str());
      const uint32_t kCyclesBeforeCheck = 100000;
      while (!kTerminateCPU) {
        default_random_engine generator;
        uniform_int_distribution<int> distribution(0, 50000);
        double groundNumber = double(distribution(generator));
        double total = groundNumber;
        for (uint32_t i = 0; i < kCyclesBeforeCheck; i++) {
          total *= groundNumber;
        }
        foundTotal = total;
      }

    }

void ResourceBlocker::useMemory(uint32_t aMBPerSec)
    {

        std::ostringstream ss;
        ss << std::this_thread::get_id();
        std::string idstr = ss.str();

        __android_log_print(ANDROID_LOG_ERROR, "Starting CPU Load on thread", "%s", idstr.c_str());
      // Divide by 2 since we do read and writes.
      uint32_t mbPer10MS = aMBPerSec / 200;

      void* block1 = malloc(mbPer10MS * 1000000);
      void* block2 = malloc(mbPer10MS * 1000000);

      uint32_t insufficientBandwidth = 0;
      while (!kTerminateMemory) {
        milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        memcpy(block1, block2, mbPer10MS * 1000000);
        milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        milliseconds elapsed = end - start;
        if (elapsed.count() < 10) {
          this_thread::sleep_for(milliseconds(10) - elapsed);
        } else {
          if (++insufficientBandwidth > 100) {
            cout << "Insufficient bandwidth..\n";
            insufficientBandwidth = 0;
          }
        }
      }

      free(block1);
      free(block2);
    }

void ResourceBlocker::reconfigureMemory(uint32_t pressure, uint32_t threadCount)
    {
      kTerminateMemory = true;
      for (thread& thread : kMemoryThreads) {
        thread.join();
      }
      kMemoryThreads.clear();
      kTerminateMemory = false;

      for (uint32_t i = 0; i < threadCount; i++) {
        kMemoryThreads.push_back(thread([this] {this->useMemory(2000);}));
    #ifdef WIN32
        DWORD_PTR threadAffinity = (DWORD_PTR)(1 << i);
        // Set thread affinity for these to minimize cache polution caused accross logical processors.
        ::SetThreadAffinityMask(kMemoryThreads.back().native_handle(), threadAffinity);
    #endif
      }
    }

void ResourceBlocker::reconfigureCPU(uint32_t numCores)
    {
    #ifdef WIN32
      SYSTEM_INFO sysInfo;
      ::GetSystemInfo(&sysInfo);
      numCores = sysInfo.dwNumberOfProcessors;
    #else
      numCores = 5;
    #endif
      uint32_t threadCount = 5;
      kTerminateCPU = true;
      for (thread& thread : kCPUThreads) {
        thread.join();
      }
      kCPUThreads.clear();
      kTerminateCPU = false;

      for (uint32_t i = 0; i < threadCount; i++) {
        kCPUThreads.push_back(thread([this] {this->useFullCPU();}));
      }
    }

void ResourceBlocker::stopAndClean()
    {
      kTerminateCPU = true;
      kTerminateMemory = true;
      for (thread& thread : kCPUThreads) {
        thread.join();
      }
      for (thread& thread : kMemoryThreads) {
        thread.join();
      }
    }


static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void ResourceBlocker::initStats(){
    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
           &lastTotalSys, &lastTotalIdle);
    fclose(file);
}

double ResourceBlocker::getCurrentValue() {
    double percent;
    FILE *file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
           &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle) {
        //Overflow detection. Just skip this value.
        percent = -1.0;
    } else {
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
                (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}