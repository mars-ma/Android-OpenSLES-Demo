package dev.mars.openslesdemo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;

public class MainActivity extends AppCompatActivity {
    String[] pers = new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.RECORD_AUDIO};
    ExecutorService executor = Executors.newSingleThreadExecutor();
    // Used to load the 'native-lib' library on application startup.
    NativeBridge nativeBridge = new NativeBridge();



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }


    public void startRecord(View view) {
        if(!nativeBridge.isRecording()) {
            if (hasPermission()) {
                launchRecordThread();
            } else {
                requestPermissions();
            }
        }else{
            Toast.makeText(MainActivity.this,"Already in recording state!",Toast.LENGTH_SHORT).show();
        }
    }



    public void stopRecord(View view) {
        if(nativeBridge.isRecording()) {
            nativeBridge.stopRecord();
        }else{
            Toast.makeText(MainActivity.this,"Not in recording state!",Toast.LENGTH_SHORT).show();
        }
    }

    private void requestPermissions(){
        if(isLollipop()) {
            requestPermissions(pers, 0);
        }
    }

    private void launchRecordThread(){
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                nativeBridge.setIsRecording(true);
                nativeBridge.startRecord();
            }
        };
        executor.execute(runnable);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if(requestCode==0){
            if(hasPermission()){
                launchRecordThread();
            }else{
                Toast.makeText(MainActivity.this,"Unable to get permissions",Toast.LENGTH_SHORT).show();
            }
        }
    }

    private boolean hasPermission() {
        return hasPermission(pers);
    }


    private boolean hasPermission(String[] pers) {
        if(isLollipop()){
            for(String per:pers){
                if(checkSelfPermission(per)!= PackageManager.PERMISSION_GRANTED){
                    return false;
                }
            }
        }
        return true;
    }

    private boolean isLollipop(){
        return Build.VERSION.SDK_INT>Build.VERSION_CODES.LOLLIPOP_MR1;
    }


    public void startPlay(View view) {

        if(nativeBridge.isPlaying()){
            Toast.makeText(MainActivity.this,"Is playing!",Toast.LENGTH_SHORT).show();
        }else{

            Runnable runnable = new Runnable() {
                @Override
                public void run() {
                    nativeBridge.setIsPlaying(true);
                    nativeBridge.playRecord();
                }
            };
            executor.execute(runnable);

        }
    }

    public void stopPlay(View view) {
        if(nativeBridge.isPlaying()){
            nativeBridge.stopPlay();
        }else{
            Toast.makeText(MainActivity.this,"Not play!",Toast.LENGTH_SHORT).show();
        }
    }
}
