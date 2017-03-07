package dev.mars.openslesdemo;

import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Created by ma.xuanwei on 2017/3/7.
 */

public class NativeBridge {

    private AtomicBoolean isRecording = new AtomicBoolean(false);
    private AtomicBoolean isPlaying = new AtomicBoolean(false);
    static {
        System.loadLibrary("native-lib");
    }

    public void setIsRecording(boolean v){
        isRecording.set(v);
        LOG.DEBUG("setIsRecording "+v);
    }

    public boolean isRecording(){
        return isRecording.get();
    }

    public void setIsPlaying(boolean b){
        isPlaying.set(b);
    }

    public boolean isPlaying(){
        return isPlaying.get();
    }

    public native void startRecord();
    public native void stopRecord();
    public native void playRecord();
    public native void stopPlay();
}
