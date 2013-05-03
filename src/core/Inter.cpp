#include <QTimer>
#include <QPainter>

#include "common.h"
#include "Inter.h"
#include "pcxxxx.h"
#include "init.h"
#include "Log.h"
#include "mainwindowpockemul.h"

extern MainWindowPockemul *mainwindow;


void Ctimer::SetCPUspeed(float t)
{
    //set CPU speed by %
    CPUSpeed = t ;
    state = currentState();
}

qint64 Ctimer::currentState(void) {
    return (qint64) (( mainwindow->rawclk * (CPUSpeed *(pPC->getfrequency() / 1000L)) )/1000000L);
}

bool Ctimer::init(void)
{
    AddLog(LOG_MASTER,QObject::tr("Timer init..."));
    deltaStep = 0;
	SetCPUspeed(1);

	AddLog(LOG_MASTER,"Success");
    return(true);
}

bool Ctimer::exit(void)
{
    return(true);
}

bool Ctimer::CheckSpeed(void)
{
    qint64 r = currentState();//qint64) ( mainwindow->rawclk * (CPUSpeed *(pPC->getfrequency() / 1000L)) );
    return (state < r);
}

void Ctimer::check(void)
{

}
bool Ctimer::resetTimer(int id) {
    if ( (id <10) && (id >=0)) {
        timerSate[id] = this->state;
        return true;
    }
    return false;
}

qint64 Ctimer::stElapsedId(int id) {
    return (state - timerSate[id]);
}

int Ctimer::msElapsedId(int id) {

    qint64 r = (state - timerSate[id]) * 1000L / (CPUSpeed *pPC->getfrequency());
    return r;
}
qint64 Ctimer::usElapsedId(int id) {

    qint64 r = (state - timerSate[id]) * 1000000L / (CPUSpeed *pPC->getfrequency());
    return r;
}
qint64 Ctimer::nsElapsedId(int id) {

    qint64 r = (state - timerSate[id]) * 1000000000L / (CPUSpeed *pPC->getfrequency());
    return r;
}
int Ctimer::msElapsed(qint64 stateRef) {

    qint64 r = (state - stateRef) * 1000L / (CPUSpeed *pPC->getfrequency());
    return r;
}
qint64 Ctimer::usElapsed(qint64 stateRef) {

    qint64 r = (state - stateRef) * 1000000L / (CPUSpeed *pPC->getfrequency());
    return r;
}
qint64 Ctimer::nsElapsed(qint64 stateRef) {

    qint64 r = (state - stateRef) * 1000000000L / (CPUSpeed *pPC->getfrequency());
    return r;
}
// return TP index
int Ctimer::initTP(int frequency)
{
    // Search the first free TP
    int index = -1;

    for ( int i = 0 ; (index==-1)&&(i< 10) ; i++ )
    {
        if (frequency_tp[i] == 0)
        {
            index = i;
        }
    }
    if (index != -1) {
        previous_state_tp[index] = state;
        frequency_tp[index] = frequency;
    }
    return index;
}

void Ctimer::deleteTP(int index)
{
    previous_state_tp[index] = 0;
    frequency_tp[index] = 0;
}

int Ctimer::getFreqTP(int index)
{
    return frequency_tp[index];
}
void Ctimer::setFreqTP(int index,int freq)
{
    previous_state_tp[index] = state;
    frequency_tp[index] = freq;
}
void Ctimer::resetTP(int index)
{
    tp[index] = false;
    previous_state_tp[index] = 0;
}

bool	Ctimer::GetTP(int index)
{
    if ( (index < 0) || (index >= 10)) return false;

// generate Timer Pulse signal
#define TP_STATE(index)		(pPC->getfrequency() / frequency_tp[index])
    qint64 delta_state;

    if (previous_state_tp[index] == 0) previous_state_tp[index] = pPC->pTIMER->state;

    // Process a while because we could have miss some ticks :-) . I don't know if it is relevant
    // to take this into account .
    while ( (delta_state = (state - previous_state_tp[index])) >= (TP_STATE(index)/2) )
    {
        tp[index] ^= 1;
        previous_state_tp[index] += (TP_STATE(index) / 2);
    };

    return(tp[index]);
}
