package org.mozilla.ResourceBlockerAndroid

import android.util.Log

class JNIResourceBlocker {

    external fun startResourceBlocker() : Int
    external fun useFullCPU(numOfThreads: Int) : Int
    external fun useFullMemory(numOfThreads: Int, aMBperSec: Int) : Int

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}