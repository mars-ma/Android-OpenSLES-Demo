#include <jni.h>
#include <string>

#include <stdio.h>
#include <jni.h>
#include <android/log.h>
#include "opensl_io.h"

#define LOG(...) __android_log_print(ANDROID_LOG_DEBUG,"dev_mars",__VA_ARGS__)

#define SAMPLERATE 44100
#define CHANNELS 1
#define PERIOD_TIME 20 //ms
#define FRAME_SIZE SAMPLERATE*PERIOD_TIME/1000
#define BUFFER_SIZE FRAME_SIZE*CHANNELS
#define TEST_CAPTURE_FILE_PATH "/sdcard/audio.pcm"

static volatile int g_loop_exit = 0;

extern "C" {

JNIEXPORT void JNICALL
Java_dev_mars_openslesdemo_NativeBridge_startRecord(JNIEnv *env, jobject instance) {
    jclass native_bridge_class = env->GetObjectClass(instance);
    jmethodID method_id_setIsRecording = env->GetMethodID(native_bridge_class,"setIsRecording","(Z)V");

    FILE * fp = fopen(TEST_CAPTURE_FILE_PATH, "wb"); //创建文件
    if( fp == NULL ) {
        LOG("cannot open file (%s)\n", TEST_CAPTURE_FILE_PATH);
        env->CallVoidMethod(instance,method_id_setIsRecording, false);
        return ;
    }else{
        LOG("open file %s",TEST_CAPTURE_FILE_PATH);
    }

    OPENSL_STREAM* stream = android_OpenAudioDevice(SAMPLERATE, CHANNELS, CHANNELS, FRAME_SIZE); //创建OPENSL流对象
    if (stream == NULL) {
        fclose(fp);
        LOG("failed to open audio device ! \n");
        env->CallVoidMethod(instance,method_id_setIsRecording, false);
        return ;
    }


    LOG("IN RECORDING STATE");


    int samples;
    short buffer[BUFFER_SIZE];
    g_loop_exit = 0;
    while (!g_loop_exit) {
        samples = android_AudioIn(stream, buffer, BUFFER_SIZE);
        if (samples < 0) {
            LOG("android_AudioIn failed !\n");
            break;
        }
        if (fwrite((unsigned char *)buffer, samples*sizeof(short), 1, fp) != 1) {
            LOG("failed to save captured data !\n ");
            break;
        }
        LOG("capture %d samples !\n", samples);
    }

    android_CloseAudioDevice(stream);
    fclose(fp);
    env->CallVoidMethod(instance,method_id_setIsRecording, false);
    LOG("native startRecord completed !");

}

JNIEXPORT void JNICALL
Java_dev_mars_openslesdemo_NativeBridge_stopRecord(JNIEnv *env, jobject instance) {

    g_loop_exit = 1;
}

JNIEXPORT void JNICALL
Java_dev_mars_openslesdemo_NativeBridge_playRecord(JNIEnv *env, jobject instance) {
    jclass native_bridge_class = env->GetObjectClass(instance);
    jmethodID method_id_setIsPlaying = env->GetMethodID(native_bridge_class,"setIsPlaying","(Z)V");


    FILE * fp = fopen(TEST_CAPTURE_FILE_PATH, "rb");
    if( fp == NULL ) {
        LOG("cannot open file (%s) !\n",TEST_CAPTURE_FILE_PATH);
        env->CallVoidMethod(instance,method_id_setIsPlaying, false);
        return ;
    }else{
        LOG("open file %s",TEST_CAPTURE_FILE_PATH);
    }

    OPENSL_STREAM* stream = android_OpenAudioDevice(SAMPLERATE, CHANNELS, CHANNELS, FRAME_SIZE);
    if (stream == NULL) {
        fclose(fp);
        LOG("failed to open audio device ! \n");
        env->CallVoidMethod(instance,method_id_setIsPlaying, false);
        return ;
    }




    int samples;
    short buffer[BUFFER_SIZE];
    g_loop_exit = 0;
    while (!g_loop_exit && !feof(fp)) {
        if (fread((unsigned char *)buffer, BUFFER_SIZE*2, 1, fp) != 1) {
            LOG("failed to read data \n ");
            break;
        }
        samples = android_AudioOut(stream, buffer, BUFFER_SIZE);
        if (samples < 0) {
            LOG("android_AudioOut failed !\n");
        }
        LOG("playback %d samples !\n", samples);
    }

    android_CloseAudioDevice(stream);
    fclose(fp);
    env->CallVoidMethod(instance,method_id_setIsPlaying, false);
    LOG("native playRecord completed !");

    return ;
}

JNIEXPORT void JNICALL
Java_dev_mars_openslesdemo_NativeBridge_stopPlay(JNIEnv *env, jobject instance) {

    g_loop_exit = 1;
    return ;

}

}