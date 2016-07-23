
//
//  EspeakTTSWorker.h
//  TextReading
//
//  Created by Ringeder_Team on 30/4/16.
//                           Hamdy Fouad ,Ahmed Attia ,Ahmed Hamdy, Sara Hassan
//


#ifndef __TextReading__EspeakTTSWorker__
#define __TextReading__EspeakTTSWorker__

#include <QAudioOutput>
#include <QBuffer>
#include <QRunnable>
#include <QThreadPool>
#include <QEventLoop>
#include <string.h>
#include <vector>
//#include <flite/flite.h>
//#include </usr/local/include/espeak/speak_lib.h>
#include <espeak/speak_lib.h>

using namespace std ;
//int samplerate; // determined by espeak, will be in Hertz (Hz)
const int buflength = 200; // passed to espeak, in milliseconds (ms)
int SynthCallback(short *wav, int numsamples, espeak_EVENT *events);

/*
std::vector<short> sounddata;
int counter =0;
short* waves;

int SynthCallback(short *wav, int numsamples, espeak_EVENT *events) {
  if (wav == NULL)
      return 1; // NULL means done.


  // process your samples here, let's just gather them

  sounddata.insert(sounddata.end(), wav, wav + numsamples);
  counter =numsamples;
  waves=wav;
  return 0; // 0 continues synthesis, 1 aborts
}
*/

class EspeakTTSWorker : public QObject, public QRunnable {
  Q_OBJECT

//    QAudioOutput*    m_audioOutput;
  QAudioDeviceInfo* info;
  QAudioFormat     m_format;
  QBuffer          b;
  QEventLoop       el;

public slots:
  void finishedPlaying(QAudio::State state);

public:
 EspeakTTSWorker() {
      info = new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());
  }
  ~EspeakTTSWorker() {
      delete info;
  }
/*   void setVoice( )
                {

                espeak_ERROR Test = espeak_SetVoiceByName("ar");
                if(Test==EE_OK) qDebug() <<"arabic voice seleccted ";
                else if (Test==EE_BUFFER_FULL)
               qDebug() <<"the command can not be buffered";
                else qDebug()<<"error selecting voice ";
                }*/
void setText(const std::string& t);
  


  void run();
};


#endif
/* defined(__TextReading__EspeakTTSWorker__) */
