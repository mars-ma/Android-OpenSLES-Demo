package dev.mars.openslesdemo;

import java.io.UnsupportedEncodingException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by mars_ma on 2017/3/8.
 */

public class OpenSLPlayer {
    private ExecutorService executor = Executors.newSingleThreadExecutor();
    private NativeLib nativeBridge;
    public OpenSLPlayer(){
        nativeBridge= new NativeLib();
    }

    public boolean startToPlay(final int sampleRate, final int period, final int channels, final String path){
        if(nativeBridge.isPlaying())
            return false;
        else{
            executor.execute(new Runnable() {
                @Override
                public void run() {
                    try {
                        nativeBridge.playRecording(sampleRate,period,channels,new String(path.getBytes(),"UTF-8"));
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                }
            });
            return true;
        }
    }

    public boolean stopPlaying(){
        if(nativeBridge.isPlaying()){
            nativeBridge.stopPlaying();
            return true;
        }else{
            return false;
        }
    }
}
