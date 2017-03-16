package dev.mars.openslesdemo;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Created by mars_ma on 2017/3/16.
 */

public class AudioUtils {
    private ExecutorService executor = Executors.newSingleThreadExecutor();
    private NativeLib nativeBridge;

    public AudioUtils(){
        nativeBridge = new NativeLib();
    }

    public boolean recordAndPlayPCM(final int sampleRate, final int period, final int channels){
        if(!nativeBridge.isRecordingAndPlaying()) {
            executor.execute(new Runnable() {
                @Override
                public void run() {
                    nativeBridge.recordAndPlayPCM(sampleRate, period, channels);
                }
            });
            return true;
        }else{
            return false;
        }
    }

    public boolean stopRecordAndPlay(){
        if(!nativeBridge.isRecordingAndPlaying()) {
            return false;
        }else{
            nativeBridge.stopRecordingAndPlaying();
            return true;
        }
    }
}
