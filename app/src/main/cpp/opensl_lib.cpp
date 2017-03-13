#include <jni.h>
#include <string>

#include <stdio.h>
#include <jni.h>
#include "log.h"
#include "opensl_io.h"



/*#define SAMPLERATE 44100
#define CHANNELS 1
#define PERIOD_TIME 20 //ms
#define FRAME_SIZE SAMPLERATE*PERIOD_TIME/1000
#define BUFFER_SIZE FRAME_SIZE*CHANNELS
#define TEST_CAPTURE_FILE_PATH "/sdcard/audio.pcm"*/

/*static int SAMPLERATE;
static int CHANNELS;
static int PERIOD_TIME;
static int FRAME_SIZE;
static int BUFFER_SIZE;*/

static volatile int g_loop_exit = 0;

struct OpenSLEngine{

};



extern "C" {


JNIEXPORT void JNICALL
Java_dev_mars_openslesdemo_OpenSLNative_startRecording(JNIEnv *env, jobject instance,jint sampleRate,jint periodTime,jint channels,jstring audioPath) {
    const char * audio_path = env->GetStringUTFChars(audioPath,NULL);
    jclass native_bridge_class = env->GetObjectClass(instance);
    //此方法用于设置录音状况的同步标记
    jmethodID method_id_setIsRecording = env->GetMethodID(native_bridge_class,"setIsRecording","(Z)V");

    //以只写方式打开或新建一个二进制文件，只允许写数据。
    FILE * fp = fopen(audio_path, "wb"); //创建文件
    if( fp == NULL ) {
        LOG("cannot open file (%s)\n", audio_path);
        //设置状态录音状态为:空闲
        env->CallVoidMethod(instance,method_id_setIsRecording, false);
        return ;
    }else{
        LOG("open file %s",audio_path);
    }

    //参数依次为采样率、频道数量、录入频道数量、播放频道数量，每帧的大学，模式
    uint32_t FRAME_SIZE = sampleRate*periodTime/1000;
    OPENSL_STREAM* stream = android_OpenAudioDevice(sampleRate, channels, channels,FRAME_SIZE ,RECORD_MODE);
    if (stream == NULL) {
        fclose(fp);
        LOG("failed to open audio device ! \n");
        env->CallVoidMethod(instance,method_id_setIsRecording, false);
        return ;
    }

    LOG("IN RECORDING STATE");
    env->CallVoidMethod(instance,method_id_setIsRecording, true);
    uint32_t samples;
    //缓冲数组,单位usigned short,16bit
    uint32_t BUFFER_SIZE = FRAME_SIZE*channels;
    uint16_t buffer[BUFFER_SIZE];
    g_loop_exit = 0;
    while (!g_loop_exit) {
        samples = android_AudioIn(stream, buffer, BUFFER_SIZE);
        if (samples < 0) {
            LOG("android_AudioIn failed !\n");
            break;
        }
        if (fwrite(buffer, samples*sizeof(uint16_t), 1, fp) != 1) {
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
Java_dev_mars_openslesdemo_OpenSLNative_stopRecording(JNIEnv *env, jobject instance) {
    g_loop_exit = 1;
}

JNIEXPORT void JNICALL
Java_dev_mars_openslesdemo_OpenSLNative_playRecording(JNIEnv *env, jobject instance,jint sampleRate,jint periodTime,jint channels,jstring audioPath) {
    const char * audio_path = env->GetStringUTFChars(audioPath,NULL);
    jclass native_bridge_class = env->GetObjectClass(instance);
    jmethodID method_id_setIsPlaying = env->GetMethodID(native_bridge_class,"setIsPlaying","(Z)V");

    FILE * fp = fopen(audio_path, "rb");
    if( fp == NULL ) {
        LOG("cannot open file (%s) !\n",audio_path);
        env->CallVoidMethod(instance,method_id_setIsPlaying, false);
        return ;
    }else{
        LOG("open file %s",audio_path);
    }

    uint32_t FRAME_SIZE = sampleRate*periodTime/1000;
    OPENSL_STREAM* stream = android_OpenAudioDevice(sampleRate, channels, channels, FRAME_SIZE,PLAY_MODE);
    if (stream == NULL) {
        fclose(fp);
        LOG("failed to open audio device ! \n");
        env->CallVoidMethod(instance,method_id_setIsPlaying, false);
        return ;
    }
    LOG("In playing state");
    env->CallVoidMethod(instance,method_id_setIsPlaying, true);
    int samples;
    int BUFFER_SIZE = FRAME_SIZE*channels;
    uint16_t buffer[BUFFER_SIZE];
    g_loop_exit = 0;
    while (!g_loop_exit && !feof(fp)) {
        if (fread(buffer, BUFFER_SIZE*sizeof(uint16_t), 1, fp) != 1) {
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
Java_dev_mars_openslesdemo_OpenSLNative_stopPlaying(JNIEnv *env, jobject instance) {
    g_loop_exit = 1;
    return ;
}

}