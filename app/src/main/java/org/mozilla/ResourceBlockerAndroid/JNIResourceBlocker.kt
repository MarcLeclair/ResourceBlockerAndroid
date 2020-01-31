package org.mozilla.ResourceBlockerAndroid

import android.util.Log

class JNIResourceBlocker {

    external fun startResourceBlocker() : Int
    external fun useFullCPU() : Int
    external fun useFullMemory() : Int

    fun createResourceBlocker() {
         val success = startResourceBlocker()
        if(success == 0 ){
            Log.i("success", "got resource Blocker")
        }else{
            Log.i("error", "no resource blocker")
        }
    }

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}