package dev.mars.openslesdemo;

import java.io.UnsupportedEncodingException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by mars_ma on 2017/3/8.
 */

public class OpenSLRecorder {
    private ExecutorService executor = Executors.newSingleThreadExecutor();
    private NativeLib nativeBridge;
    public OpenSLRecorder(){
        nativeBridge= new NativeLib();
    }

    public boolean startToRecord(final int sampleRate, final int period, final int channels, final String path){
        if(nativeBridge.isRecording())
            return false;
        else{
            executor.execute(new Runnable() {
                @Override
                public void run() {
                    int bufferSize = sampleRate*period*channels/1000;
                    try {
                        nativeBridge.startRecording(sampleRate,period,channels,new String(path.getBytes(),"UTF-8"));
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                }
            });
            return true;
        }
    }

    public boolean stopRecording(){
        if(nativeBridge.isRecording()){
            nativeBridge.stopRecording();
            return true;
        }else{
            return false;
        }
    }
}
