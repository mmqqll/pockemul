#ifndef CPB1000_H
#define CPB1000_H

#include "pcxxxx.h"

class CHD44352;

class Cpb1000 : public CpcXXXX
{
    Q_OBJECT

    Q_PROPERTY(int angle READ angle WRITE setAngle)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom)
public:
    Cpb1000(CPObject *parent = 0);
    virtual ~Cpb1000();

    virtual bool init();
    virtual bool run();
    virtual bool Chk_Adr(DWORD *d, DWORD data);
    virtual bool Chk_Adr_R(DWORD *d, DWORD *data);
    virtual UINT8 in(UINT8 Port);
    virtual UINT8 out(UINT8 Port, UINT8 Value);
    virtual void TurnOFF();
    virtual void TurnON();
    virtual void Reset();
    virtual bool LoadConfig(QXmlStreamReader *xmlIn);
    virtual bool SaveConfig(QXmlStreamWriter *xmlOut);

    virtual bool Set_Connector(void);
    virtual bool Get_Connector(void);

    virtual bool UpdateFinalImage(void);
    virtual void paintEvent(QPaintEvent *event);

    CHD44352 *pHD44352;
    virtual UINT16 getKey();
    virtual void setKey(UINT8 data);
    UINT8 m_kb_matrix;
    virtual WORD Get_16rPC(DWORD adr);
    virtual UINT8 readPort(void);
    virtual void writePort(UINT8);
    virtual UINT8 lcdDataRead();
    virtual void lcdDataWrite(UINT8 data);
    virtual void lcdControlWrite(UINT8 data);
    virtual void MemBank(DWORD *d);


    bool flipping;
    QImage *back;

    BYTE pdi;
    bool prev_P2;


//    void keyPressEvent(QKeyEvent *event);
//    void keyReleaseEvent(QKeyEvent *event);

//    bool shift;
    //    bool fct;
    virtual void TurnCLOSE(void);
    void setAngle(int value);
    int angle() const { return m_angle; }
    int m_angle;

    void setZoom(qreal value);
    qreal zoom() const { return m_zoom; }
    qreal m_zoom;

    qint64 lcd_on_timer_rate;
    qint64 adrBus;
    bool writeIO;

public slots:
    void endAnimation(void);
};

#endif // CPB1000_H
