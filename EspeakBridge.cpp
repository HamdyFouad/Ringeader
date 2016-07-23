//
//  EspeakBrdige.cpp
//  TextReading
//
//  Created by Ringeader team
//                       Hamdy Fouad ,Ahmed Attia ,Ahmed Hamdy, Sara Hassan
#include "EspeakBridge.h"
#include <iostream>
#include "EspeakTTSWorker.h"
std::string Ntext;

//cst_voice *v;
//espeak_VOICE* v;

void EspeakBridge::init() {
  espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0 );
    espeak_SetVoiceByName("ar");
   espeak_SetSynthCallback(&SynthCallback);

     threadPool.setMaxThreadCount(1);
}


void EspeakBridge::process(const std::string& text) {
    EspeakTTSWorker* work = new EspeakTTSWorker;

    Ntext=text;
    work->setAutoDelete(true);
    work->setText(Ntext);
    size_t size = Ntext.length();
    char txt[size] ;
    strncpy(txt, Ntext.c_str(), sizeof(txt));
    txt[sizeof(txt) - 1] = '\0';
    espeak_Synth(txt, size + 1, 0, POS_CHARACTER, 0, (espeakCHARS_AUTO | espeakENDPAUSE), NULL, NULL);
    espeak_Synchronize();

    threadPool.start(work);
}

void EspeakBridge::close()
 {
espeak_ERROR Test =espeak_Terminate();
if(Test==EE_OK) cout<<"Termination Done ";
else cout<<"Termination Failed ";

       threadPool.waitForDone();
}
