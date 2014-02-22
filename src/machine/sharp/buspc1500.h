#ifndef BUSPC1500_H
#define BUSPC1500_H


#include <QtCore>
#include <QMetaType>
#include "bus.h"



class CbusPc1500:public Cbus {

public:
    CbusPc1500();

    virtual quint64 toUInt64() const;
    virtual void fromUInt64(quint64 val);

    virtual void setAddr(quint32 val);

    bool isME1() const { return me1; }
    void setME1(bool val) { me1 = val; }

    bool isPU() const { return pu; }
    void setPU(bool val) { pu = val; }

    bool isPV() const { return pv; }
    void setPV(bool val) { pv = val; }

    bool isINHIBIT() const { return inhibit; }
    void setINHIBIT(bool val) { inhibit = val; }

    virtual QString toLog() const;

private:
    bool me1;
    bool pu,pv;
    bool inhibit;
};



#endif // BUSPC1500_H
