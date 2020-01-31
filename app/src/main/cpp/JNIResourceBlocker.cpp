#include <21.0.6113669/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/jni.h>
#include "ResourceBlocker.h"
#include "jni.h"
#define JNIIMPORT
#define JNIEXPORT  __attribute__ ((visibility ("default")))
#define JNICALL
#ifdef __cplusplus
extern "C" {
#endif

ResourceBlocker *r;
jint Java_org_mozilla_ResourceBlockerAndroid_JNIResourceBlocker_startResourceBlocker(JNIEnv *env,
                                                                                        jobject thiz) {
    r = new ResourceBlocker();

    return 0;
  }

jint Java_org_mozilla_ResourceBlockerAndroid_JNIResourceBlocker_useFullCPU(JNIEnv *env,
                                                                                        jobject thiz){
    r->reconfigureCPU(0);
    return 0;
    }
jint Java_org_mozilla_ResourceBlockerAndroid_JNIResourceBlocker_useFullMemory(JNIEnv *env,
                                                                                jobject thiz){
    r->reconfigureMemory(0,1);
    return 0;
}
}
