/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.mozilla.ResourceBlockerAndroid

import android.os.Bundle
import android.view.*
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.fragment.app.Fragment

/**
 * Fragment used to show how to navigate to another destination
 */
class HomeFragment : Fragment() {
    override fun onCreateView(
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {
        setHasOptionsMenu(true)
        return inflater.inflate(R.layout.home_fragment, container, false)
    }

    val JNIResourceBlocker = JNIResourceBlocker()
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)


        view.findViewById<Button>(R.id.initResourceBlocker)?.setOnClickListener {
            JNIResourceBlocker.startResourceBlocker()
        }

        view.findViewById<Button>(R.id.startFullCpuLoad)?.setOnClickListener {
            val numOfThreads = view.findViewById<EditText>(R.id.numOfThreads).text.toString()
            if(numOfThreads == ""){
                JNIResourceBlocker.useFullCPU(5)
            }else{
                JNIResourceBlocker.useFullCPU(numOfThreads.toInt())
            }
        }

        view.findViewById<Button>(R.id.useFullMemory)?.setOnClickListener {
            val numOfThreads = view.findViewById<EditText>(R.id.threadsForMemory).text.toString()
            val aMBperSec = view.findViewById<EditText>(R.id.memoryUsed).text.toString()
            if(numOfThreads == ""){
                JNIResourceBlocker.useFullMemory(5, aMBperSec.toInt())
            }else{
                JNIResourceBlocker.useFullMemory(numOfThreads.toInt(), aMBperSec.toInt())
            }
        }
    }
}
