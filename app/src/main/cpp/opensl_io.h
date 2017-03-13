/* opensl_io.c:
   Android OpenSL input/output module header
   Copyright (c) 2012, Victor Lazzarini All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OPENSL_IO
#define OPENSL_IO

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <pthread.h>
#include <stdlib.h>

#define RECORD_MODE 0
#define PLAY_MODE 1

typedef struct threadLock_ {
    pthread_mutex_t m;
    pthread_cond_t  c;
    unsigned char   s;
} threadLock;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct opensl_stream {
  
    // engine interfaces
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    // output mix interfaces
    SLObjectItf outputMixObject;

    // buffer queue player interfaces
    SLObjectItf bqPlayerObject;
    SLPlayItf bqPlayerPlay;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
    SLEffectSendItf bqPlayerEffectSend;

    // recorder interfaces
    SLObjectItf recorderObject;
    SLRecordItf recorderRecord;
    SLAndroidSimpleBufferQueueItf recorderBufferQueue;

    // buffer indexes
    uint32_t currentInputIndex;
    uint32_t currentOutputIndex;

    // current buffer half (0, 1)
    uint8_t currentOutputBuffer;
    uint8_t currentInputBuffer;
  
    // buffers
    uint16_t *outputBuffer[2];
    uint16_t *inputBuffer[2];

    // size of buffers
    uint32_t outBufSamples; //播放缓冲数组的大小
    uint32_t inBufSamples;  //录入缓冲数组的大小

    // locks
    void*  inlock;  //录入同步锁
    void*  outlock; //播放同步锁

    double time;
    uint32_t inchannels; //输入的声道数量
    uint32_t outchannels; //输出的声道数量
    uint32_t sampleRate; //采样率

} OPENSL_STREAM;

/*
  Open the audio device with a given sampling rate (sr), input and output channels and IO buffer size
  in frames. Returns a handle to the OpenSL stream
*/
OPENSL_STREAM* android_OpenAudioDevice(uint32_t sr, uint32_t inchannels, uint32_t outchannels, uint32_t bufferframes , uint32_t mode);

/*
  Close the audio device
*/
void android_CloseAudioDevice(OPENSL_STREAM *p);

/*
  Read a buffer from the OpenSL stream *p, of size samples. Returns the number of samples read.
*/
uint32_t android_AudioIn(OPENSL_STREAM *p, uint16_t *buffer,uint32_t size);

/*
  Write a buffer to the OpenSL stream *p, of size samples. Returns the number of samples written.
*/
uint32_t android_AudioOut(OPENSL_STREAM *p, uint16_t *buffer,uint32_t size);

/*
  Get the current IO block time in seconds
*/
double android_GetTimestamp(OPENSL_STREAM *p);
  
#ifdef __cplusplus
};
#endif

#endif // #ifndef OPENSL_IO
