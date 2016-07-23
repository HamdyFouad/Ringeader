//
//  EspeakBrdige.h
//  TextReading
//
//  Created by Ringeader team
//                       Hamdy Fouad, Ahmed Attia, Ahmed Hamdy, Sara Hassan
//
//


#ifndef __TextReading__EspeakBrdige__
#define __TextReading__EspeakBrdige__



#include <iostream>
#include <QObject>
#include <QThreadPool>
//#include <espeak/speak_lib.h>


class EspeakBridge : public QObject {
    Q_OBJECT


    QThreadPool threadPool;

public:
    void init();
    void process(const std::string& text);
    void close();


};


#endif /* defined(__TextReading__EspeakBrdige__) */
