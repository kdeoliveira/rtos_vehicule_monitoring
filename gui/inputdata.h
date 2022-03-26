#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <QObject>

#include <qqml.h>
#include "reader.h"

class InputData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float speed READ speed WRITE setSpeed NOTIFY speedChanged)
public:
    explicit InputData(QObject *parent = nullptr);
    ~InputData(){
        delete m_thread;
    }

    float speed(){
        return this->m_speed;
    }

    void init(const char*);


    void setSpeed(const float& speed){
        if(this->m_speed == speed){
            return;
        }

        m_speed= speed;
        emit speedChanged(m_speed);
    }

signals:
    void speedChanged(float);




private:
    float m_speed;
    Reader* m_thread;


public slots:
    void readPipe(const float&);


};

#endif // INPUTDATA_H
