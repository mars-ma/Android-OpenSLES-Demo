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




extern "C" {

#include "speex.h"


JNIEXPORT jint JNICALL
Java_dev_mars_openslesdemo_NativeLib_encode(JNIEnv *env, jobject instance, jstring pcm_,jstring speex_) {
    const char *pcm_path = env->GetStringUTFChars(pcm_, 0);
    const char *speex_path = env->GetStringUTFChars(speex_, 0);
    // TODO
    /*帧的大小在这个例程中是一个固定的值,但它并不是必须这样*/
    int FRAME_SIZE = 160;

    const char *inFile;
    FILE *fin;
    FILE *fout;
    short in[FRAME_SIZE];
    float input[FRAME_SIZE];
    char cbits[200];
    int nbBytes;
    /*保存编码的状态*/
    void *state;
    /*保存字节因此他们可以被speex常规读写*/
    SpeexBits bits;
    int i, tmp;
    //新建一个新的编码状态在窄宽(narrowband)模式下
    state = speex_encoder_init(&speex_nb_mode);
    //设置质量为8(15kbps)
    tmp=8;
    speex_encoder_ctl(state, SPEEX_SET_QUALITY, &tmp);
    inFile = pcm_path;

    fin = fopen(inFile, "r");
    if(fin==NULL){
        LOG("%s open failed",inFile);
    }
    fout=fopen(speex_path,"wb");
    if(fout==NULL){
        LOG("%s open failed",speex_path);
    }
    //初始化结构使他们保存数据
    speex_bits_init(&bits);
    while (1)
    {
        //读入一帧16bits的声音
        fread(in, sizeof(short), FRAME_SIZE, fin);
        if (feof(fin))
            break;
        //把16bits的值转化为float,以便speex库可以在上面工作
        for (i=0;i<FRAME_SIZE;i++)
            input[i]=in[i];

        //清空这个结构体里所有的字节,以便我们可以编码一个新的帧
        speex_bits_reset(&bits);
        //对帧进行编码
        speex_encode(state, input, &bits);
        //把bits拷贝到一个利用写出的char型数组
        nbBytes = speex_bits_write(&bits, cbits, 200);
        //首先写出帧的大小,这是sampledec文件需要的一个值,但是你的应用程序中可能不一样
        fwrite(&nbBytes, sizeof(int), 1, fout);
        //写出压缩后的数组
        fwrite(cbits, 1, nbBytes, fout);
    }

    //释放编码器状态量
    speex_encoder_destroy(state);
    //释放bit_packing结构
    speex_bits_destroy(&bits);
    fclose(fin);
    fclose(fout);
    env->ReleaseStringUTFChars(pcm_, pcm_path);
    env->ReleaseStringUTFChars(speex_, speex_path);
    LOG("%s convert to %s success!",inFile,speex_path);
    return 0;
}

JNIEXPORT jint JNICALL
Java_dev_mars_openslesdemo_NativeLib_decode(JNIEnv *env, jobject instance, jstring speex_,
                                            jstring pcm_) {
    const char *speex = env->GetStringUTFChars(speex_, 0);
    const char *pcm = env->GetStringUTFChars(pcm_, 0);
    // TODO
    /*帧的大小在这个例程中是一个固定的值,但它并不是必须这样*/
    int FRAME_SIZE = 160;

    const char *inFile;
    FILE *fin;
    FILE *fout;
    short out[FRAME_SIZE];
    float output[FRAME_SIZE];
    char cbits[200];
    int nbBytes;
    /*保存编码的状态*/
    void *state;
    /*保存字节因此他们可以被speex常规读写*/
    SpeexBits bits;
    int i, tmp;
    //新建一个新的编码状态在窄宽(narrowband)模式下
    state = speex_encoder_init(&speex_nb_mode);
    //设置质量为8(15kbps)
    tmp=8;
    speex_encoder_ctl(state, SPEEX_SET_QUALITY, &tmp);
    inFile = speex;

    fin = fopen(inFile, "r");
    if(fin==NULL){
        LOG("%s open failed",inFile);
        return -1;
    }
    fout=fopen(pcm,"wb");
    if(fout==NULL){
        LOG("%s open failed",pcm);
        return -1;
    }
    //初始化结构使他们保存数据
    speex_bits_init(&bits);

    while (1)
    {

        int size =0;
        fread(&size, sizeof(int),1,fin);
        fread(cbits, 1, size,fin);
        LOG("得到长度%d的一帧数据" ,size);
        if (feof(fin)){
            LOG("文件解析完毕");
            break;
        }

        speex_bits_reset(&bits);
        //把读入的char数组拷贝到bits
        speex_bits_read_from(&bits,cbits,200);
        //将bits中的数据解码到output
        speex_decode(state,&bits,output);

        //把16bits的float转换short,以便pcm播放
        for (i=0;i<FRAME_SIZE;i++)
            out[i]=output[i];

        fwrite(out, sizeof(short), FRAME_SIZE, fout);
    }

    //释放编码器状态量
    speex_encoder_destroy(state);
    //释放bit_packing结构
    speex_bits_destroy(&bits);
    fclose(fin);
    fclose(fout);

    env->ReleaseStringUTFChars(speex_, speex);
    env->ReleaseStringUTFChars(pcm_, pcm);
    LOG("%s convert to %s success!",inFile,pcm);
}

JNIEXPORT void JNICALL
Java_dev_mars_openslesdemo_NativeLib_startRecording(JNIEnv *env, jobject instance,jint sampleRate,jint periodTime,jint channels,jstring audioPath) {
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
        LOG(" samples*sizeof(uint16_t) : %d", samples*sizeof(uint16_t));
        LOG(" sizeof(buffer) : %d", sizeof(buffer));
        //为了防止缓冲数组未写满，所以用samples*sizeof(uint16_t),samples表示缓冲数组中有效写入的字节
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
Java_dev_mars_openslesdemo_NativeLib_stopRecording(JNIEnv *env, jobject instance) {
    g_loop_exit = 1;
}

JNIEXPORT void JNICALL
Java_dev_mars_openslesdemo_NativeLib_playRecording(JNIEnv *env, jobject instance,jint sampleRate,jint periodTime,jint channels,jstring audioPath) {
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
Java_dev_mars_openslesdemo_NativeLib_stopPlaying(JNIEnv *env, jobject instance) {
    g_loop_exit = 1;
    return ;
}

}