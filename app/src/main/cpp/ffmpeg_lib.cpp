#include <jni.h>
extern "C" {
#include "libavcodec/avcodec.h"
JNIEXPORT jstring JNICALL
Java_dev_mars_openslesdemo_FFMpegNative_getConfiguration(JNIEnv *env, jobject instance) {
    return env->NewStringUTF(avcodec_configuration());
}
}
