//  EspeakTTSWorker.cpp
//  TextReading
//
//   Created by Ringeader team
//                       Hamdy Fouad ,Ahmed Attia ,Ahmed Hamdy, Sara Hassan
//


#include "EspeakTTSWorker.h"
#include <QDebug>
#include <iostream>
int samplerate; // determined by espeak, will be in Hertz (Hz)
//extern int counter;
//extern short* waves;
std::string      text;

std::vector<short> sounddata;
int counter =0;
short* waves;

int SynthCallback(short *wav, int numsamples, espeak_EVENT *events) {
  if (wav == NULL)
      return 1; // NULL means done.


  /* process your samples here, let's just gather them */
  sounddata.insert(sounddata.end(), wav, wav + numsamples);
  counter =numsamples;
  waves=wav;
  return 0; // 0 continues synthesis, 1 aborts
}


void EspeakTTSWorker::finishedPlaying(QAudio::State state)
{
    switch (state) {
        case QAudio::StoppedState:
            qDebug("Stopped state");
            break;


        case QAudio::IdleState:
            qDebug("Idle state");
            break;


        case QAudio::ActiveState:
            qDebug("Active state");
            break;


        case QAudio::SuspendedState:
            qDebug("Suspended state");
            break;


        default:
            qDebug("some other thing happened");
    }
}


void EspeakTTSWorker::setText(const std::string& t)
    {
      text = t;
   }

void EspeakTTSWorker::run() {

  //  int freq = cst_wave_sample_rate(w);
    int freq = espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, buflength, NULL, 0);
//  espeak_EVENT_TYPE freq   = espeak_EVENT_TYPE-> espeakEVENT_SAMPLERATE = 8       // internal use, set sample rate


  //  int numchannels = cst_wave_num_channels(w);    // can not find alternative in espeak
    int numchannels = 1;                                                 //mono setting
  //int samplesize_bytes = sizeof(typeof(*(w->samples)));  // search in struct . sample (short * sample )
   int samplesize_bytes = sizeof(typeof(sounddata));
   int samplesize = samplesize_bytes * 8;           //size in bits


    //short* buf = (short*)(cst_wave_samples(w));      // sounddata
  //  short* buf = (short*)(sounddata);
    short* buf = waves;

   // int numsamples = cst_wave_num_samples(w);
   qDebug() <<"Number of Samples  :"<<counter<<endl;
// numsamples: is the number of entries in wav. SynthCallback(short *wav, int numsamples, espeak_EVENT *events);


//Qt Functions need to change the passed parameter to be from Espeak
    m_format.setFrequency(freq);          //done
    m_format.setChannels(numchannels);
    m_format.setSampleSize(samplesize); //bits per sample




//not changed
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);






    if (!info->isFormatSupported(m_format)) {
        std::cerr << "Default format not supported - trying to use nearest";
        m_format = info->nearestFormat(m_format);
    }


    QAudioOutput m_audioOutput(m_format, 0);
//    connect(m_audioOutput,SIGNAL(stateChanged(QAudio::State)),this,SLOT(finishedPlaying(QAudio::State)));


    int sizeinbytes = counter * samplesize_bytes;
    b.open(QIODevice::ReadWrite);   // b for Qbuffer
    b.write((char*)buf, sizeinbytes);
    b.seek(0);
    m_audioOutput.start(&b);
    //hold until sound is done
    QEventLoop loop;
    QObject::connect(&m_audioOutput, SIGNAL(stateChanged(QAudio::State)), &loop, SLOT(quit()));
    do {
        loop.exec();
    } while(m_audioOutput.state() == QAudio::ActiveState);
}
