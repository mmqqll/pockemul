//TODO Contextual menu on breakpoints
#include <QDebug>
#include <QScrollBar>
#include <QFileDialog>
#include <QSettings>

#include "dialogdasm.h"
#include "ui_dialogdasm.h"
#include "pcxxxx.h"
#include "Debug.h"
#include "bineditor/bineditor.h"
#include "ui/cregssc61860widget.h"
#include "cpu.h"
#include "cregcpu.h"

#define CODE_LINE 0
#define LABEL_LINE 1
#define SEPARATOR_LINE 2

DialogDasm::DialogDasm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDasm)
{
    ui->setupUi(this);

    pPC = (CpcXXXX*)parent;
    Index = 0;
    MaxAdr = 0;
    NextMaxAdr = 0;

    imemHexEditor = new BINEditor::BinEditor(ui->imemframe);
    connect(imemHexEditor,SIGNAL(update(int,uchar)),this,SLOT(Update(int,uchar)));
//    memHexEditor = new BINEditor::BinEditor(ui->memframe);

    connect(ui->tbStart,SIGNAL(clicked()),this,SLOT(start()));
//    connect(ui->pbStop,SIGNAL(clicked()),this,SLOT(stop()));
    connect(ui->tbStep,SIGNAL(clicked()),this,SLOT(step()));
    connect(ui->tbStepOver,SIGNAL(clicked()),this,SLOT(stepOver()));
    connect(pPC,SIGNAL(RefreshDasm()),this,SLOT(RefreshDasm()));
    connect(ui->pbDasm,SIGNAL(clicked()),this,SLOT(ManualDasm()));

    connect(ui->tbAddBrkPt,SIGNAL(clicked()),this,SLOT(addBreakPoint()));
    connect(ui->lwBreakPts,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(breakPointChanged(QListWidgetItem*)));

    connect(ui->tbAddTraceRange,SIGNAL(clicked()),this,SLOT(addTraceRange()));
    connect(ui->lwTraceRange,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(traceRangeChanged(QListWidgetItem*)));

    connect(ui->pbAddSymbol,SIGNAL(clicked()),this,SLOT(addSymbolFile()));
    connect(ui->pbRemoveSymbol,SIGNAL(clicked()),this,SLOT(removeSymbolFile()));
    connect(ui->lwSymbolFiles, SIGNAL(itemChanged(QListWidgetItem *)),this, SLOT(loadSymbolMap()));

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    ui->codelistWidget->setFont(font);

    if (pPC->pCPU->regwidget) {
        regwidget = pPC->pCPU->regwidget;
        regwidget->setParent(ui->regframe);

    }
    this->show();
    this->resize(545,490);
    regwidget->show();

    imem=true;

    load();

}

DialogDasm::~DialogDasm()
{
    saveAll();
    delete ui;
}

void DialogDasm::Update(int adr,uchar val) {
    qWarning()<<"Update["<<adr<<"]="<<val;
    if (imem) {
        pPC->pCPU->imem[adr]=val;
    }
    else {
        pPC->mem[adr] = val;
    }
}

void DialogDasm::selectRow(int index) {

    QListWidgetItem *item = ui->codelistWidget->currentItem();
    if (item) {
        ui->codelistWidget->currentItem()->setBackground(Qt::white);
        ui->codelistWidget->currentItem()->setForeground(Qt::black);
    }
    ui->codelistWidget->setCurrentRow(index);
    //ui->codelistWidget->currentItem()->setSelected(true);
    ui->codelistWidget->currentItem()->setBackground(Qt::blue);
    ui->codelistWidget->currentItem()->setForeground(Qt::red);
    //ui->codelistWidget->currentItem()->font().setBold(true);
//    font.setBold(true);
//    ui->codelistWidget->currentItem()->setFont(font);
}

bool DialogDasm::IsAdrInList(qint32 adr)
{

    int NbLig;

    NbLig = ui->codelistWidget->count();

    for (int i=0;i<NbLig;i++)
    {
        QListWidgetItem *item = ui->codelistWidget->item(i);
        if ( (item->data(Qt::UserRole+1) == CODE_LINE) &&
             (adr == item->data(Qt::UserRole)) )
        {
            selectRow(i);

            return(true);
        }
    }

    return(false);

}

void DialogDasm::ManualDasm() {
    quint32 _adr = ui->leDasmAdr->text().toUInt(0,16);
    int _nb = ui->leNbLine->text().toInt(0,16);
    ui->codelistWidget->clear();
    Index		= 0;

    for (int j=0;j<_nb;j++)
    {
        qWarning()<<"adr="<<_adr;
        pPC->pCPU->pDEBUG->DisAsm_1(_adr);
        //MaxAdr		= pPC->pCPU->pDEBUG->DasmAdr;

        int adr = pPC->pCPU->pDEBUG->DasmAdr;
        if (pPC->pCPU->pDEBUG->symbolMap.contains(_adr)) {
            QListWidgetItem *item = new QListWidgetItem(pPC->pCPU->pDEBUG->symbolMap[_adr]->toLbl()+":");
            item->setData(Qt::UserRole,QVariant(_adr));
            item->setData(Qt::UserRole+1,LABEL_LINE);
            ui->codelistWidget->addItem(item);
        }
        QListWidgetItem *item = new QListWidgetItem(pPC->pCPU->pDEBUG->Buffer);
        item->setData(Qt::UserRole,_adr);
        item->setData(Qt::UserRole+1,CODE_LINE);
        ui->codelistWidget->addItem(item);
        Index++;
        _adr	= pPC->pCPU->pDEBUG->NextDasmAdr;
    }


}

void DialogDasm::RefreshDasm()
{

 if ( pPC->pCPU->pDEBUG->debugged)
    {
        QString	text;

        if (! IsAdrInList(pPC->pCPU->pDEBUG->DasmAdr))
        {
            if (MaxAdr > pPC->pCPU->pDEBUG->DasmAdr)
            {
                // effacer tout et recommencer au debut
                ui->codelistWidget->clear();

                Index		= 0;
                MaxAdr		= pPC->pCPU->pDEBUG->DasmAdr;
                NextMaxAdr	= pPC->pCPU->pDEBUG->NextDasmAdr;
            }
            else
            {
                if (pPC->pCPU->pDEBUG->DasmAdr > NextMaxAdr)
                {
                    // Insert a separator
                    text = QString("%1:---------------------").arg(pPC->pCPU->pDEBUG->DasmAdr-1,5,16,QChar('0'));
                    QListWidgetItem *item = new QListWidgetItem(text);
                    int adr = pPC->pCPU->pDEBUG->DasmAdr;
                    item->setData(Qt::UserRole,QVariant(adr));
                    item->setData(Qt::UserRole+1,SEPARATOR_LINE);
                    ui->codelistWidget->addItem(item);
                    selectRow(ui->codelistWidget->count()-1);

                    NextMaxAdr = pPC->pCPU->pDEBUG->NextDasmAdr;
                    Index++;
                }
                MaxAdr		= pPC->pCPU->pDEBUG->DasmAdr;
                NextMaxAdr	= pPC->pCPU->pDEBUG->NextDasmAdr;
            }

            int adr = pPC->pCPU->pDEBUG->DasmAdr;
            if (pPC->pCPU->pDEBUG->symbolMap.contains(adr)) {
                QListWidgetItem *item = new QListWidgetItem(pPC->pCPU->pDEBUG->symbolMap[adr]->toLbl()+":");
                item->setData(Qt::UserRole,QVariant(adr));
                item->setData(Qt::UserRole+1,LABEL_LINE);
                ui->codelistWidget->addItem(item);
            }
            QListWidgetItem *item = new QListWidgetItem(pPC->pCPU->pDEBUG->Buffer);
            item->setData(Qt::UserRole,adr);
            item->setData(Qt::UserRole+1,CODE_LINE);
            ui->codelistWidget->addItem(item);
            selectRow(ui->codelistWidget->count()-1);
            Index++;
            NextMaxAdr	= pPC->pCPU->pDEBUG->NextDasmAdr;
// full until 15 lines
            for (int j=Index;j<30;j++)
            {
                int _adr = pPC->pCPU->pDEBUG->NextDasmAdr;
                pPC->pCPU->pDEBUG->DisAsm_1(_adr);
                MaxAdr		= pPC->pCPU->pDEBUG->DasmAdr;
                NextMaxAdr	= pPC->pCPU->pDEBUG->NextDasmAdr;

                if (pPC->pCPU->pDEBUG->symbolMap.contains(_adr)) {
                    QListWidgetItem *item = new QListWidgetItem(pPC->pCPU->pDEBUG->symbolMap[_adr]->toLbl()+":");
                    item->setData(Qt::UserRole,QVariant(_adr));
                    item->setData(Qt::UserRole+1,LABEL_LINE);
                    ui->codelistWidget->addItem(item);
                }

                QListWidgetItem *item = new QListWidgetItem(pPC->pCPU->pDEBUG->Buffer);
                item->setData(Qt::UserRole,_adr);
                item->setData(Qt::UserRole+1,CODE_LINE);
                ui->codelistWidget->addItem(item);
                Index++;
            }
        }

    }
    if (regwidget) regwidget->refresh();
    loadImem();
    loadMem();

}

void DialogDasm::loadImem()
{
    int _curPos = imemHexEditor->cursorPosition();
    int _scrollPos = imemHexEditor->verticalScrollBar()->value();
    if (pPC) {
        if (imem)
            imemHexEditor->setData(pPC->pCPU->getimem());
        else
            imemHexEditor->setData(pPC->getmem());
    }

    imemHexEditor->setReadOnly(false);
    imemHexEditor->setCursorPosition(_curPos,BINEditor::BinEditor::MoveAnchor);
    imemHexEditor->verticalScrollBar()->setValue(_scrollPos);

    update();
}

void DialogDasm::loadMem()
{
//    if (pPC) {
//        memHexEditor->setData(pPC->getmem());
//    }
//    update();
}

void DialogDasm::resizeEvent( QResizeEvent * event )
{
    imemHexEditor->resize( ui->imemframe->size());
//    memHexEditor->resize(ui->memframe->size());
    if (regwidget) regwidget->resize(ui->regframe->size());
}

void DialogDasm::start()
{
    pPC->DasmFlag = false;
    pPC->DasmLastAdr = 0xFFFFFFFF;
    ui->codelistWidget->clear();
    Index = 0;
    if (pPC->BreakSubLevel == pPC->pCPU->CallSubLevel)
        pPC->BreakSubLevel = -1;

}

void DialogDasm::stop()
{
//    pPC->DasmFlag = false;
//    pPC->pCPU->halt = 1;
    pPC->DasmStep = true;
}

void DialogDasm::step()
{
    pPC->BreakSubLevel = 99999;
    pPC->DasmStep = true;
    pPC->DasmFlag = false;
}

void DialogDasm::stepOver()
{
    pPC->BreakSubLevel = pPC->pCPU->CallSubLevel;
    pPC->DasmStep = true;
    pPC->DasmFlag = false;
}


void DialogDasm::addBreakPoint()
{
    pPC->BreakPoints[ui->breakPointLineEdit->text().toLongLong(0,16)] = Qt::Checked;

    QListWidgetItem *_item = new QListWidgetItem(ui->breakPointLineEdit->text());
    _item->setCheckState(Qt::Checked);
    ui->lwBreakPts->addItem(_item);
}

void DialogDasm::breakPointChanged(QListWidgetItem *item)
{
    pPC->BreakPoints[item->text().toLongLong(0,16)] = item->checkState();

}

void DialogDasm::addTraceRange()
{
    // Check log
    pPC->pCPU->logsw = true;
    pPC->pCPU->Check_Log();

    QPair<UINT32,UINT32> _pair;
    _pair.first = ui->leTraceFrom->text().toUInt(0,16);
    _pair.second = ui->leTraceTo->text().toUInt(0,16);
    pPC->TraceRange[_pair] = Qt::Checked;
    QListWidgetItem *_item = new QListWidgetItem(ui->leTraceFrom->text()+"-"+ui->leTraceTo->text());
    _item->setCheckState(Qt::Checked);
    ui->lwTraceRange->addItem(_item);
}

void DialogDasm::traceRangeChanged(QListWidgetItem *item)
{
    QPair<UINT32,UINT32> _pair;
    _pair.first = ui->leTraceFrom->text().toUInt(0,16);
    _pair.second = ui->leTraceTo->text().toUInt(0,16);
    pPC->TraceRange[_pair] = item->checkState();

}

void DialogDasm::addSymbolFile()
{
    // open openfilebox
    QString fn = QFileDialog::getOpenFileName(
            this,
            tr("Choose a Symbole file"),
            ".");
    if (fn.isEmpty()) return;


    QListWidgetItem *_item = new QListWidgetItem(QFileInfo(fn).fileName());
    _item->setToolTip(fn);
    _item->setCheckState(Qt::Checked);
    ui->lwSymbolFiles->addItem(_item);
    loadSymbolMap();
}

void DialogDasm::removeSymbolFile()
{
    ui->lwSymbolFiles->takeItem(ui->lwSymbolFiles->currentRow());
    loadSymbolMap();
}

extern QString workDir;

void DialogDasm::loadSymbolMap()
{

    qWarning()<<"loadSymbolMap";

    pPC->pCPU->pDEBUG->symbolMap.clear();

    if (ui->lwSymbolFiles->count()==0) return;

    for (int i=0;i<ui->lwSymbolFiles->count();i++) {
        if (ui->lwSymbolFiles->item(i)->checkState()==Qt::Checked) {
            qWarning()<<"loading file:"<<ui->lwSymbolFiles->item(i)->toolTip();
            QFile file(ui->lwSymbolFiles->item(i)->toolTip());
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray line = file.readLine();
                if (line.contains(".SYMBOLS:")) {
                    // LHASM Symbols file
                    while (!file.atEnd()) {
                        QByteArray line = file.readLine();
                        quint32 adr;
                        char lbl[80];
                        sscanf(line,"\t%x\t%s",&adr,lbl);
                        pPC->pCPU->pDEBUG->symbolMap[adr] = new Csymbol(QString(lbl));
                        qWarning()<<"adr:"<<adr<<pPC->pCPU->pDEBUG->symbolMap[adr]->toLbl();
                    }
                }
            }
        }
    }
    qWarning()<<pPC->pCPU->pDEBUG->symbolMap;
}

void DialogDasm::saveAll() {
    qWarning()<<"saveall";
    QSettings settings(workDir+"config.ini",QSettings::IniFormat);

    QString s;
    QXmlStreamWriter *xmlOut = new QXmlStreamWriter(&s);
    xmlOut->setAutoFormatting(true);

    xmlOut->writeStartElement("dasm");
        xmlOut->writeAttribute("version", "1.0");
        xmlOut->writeStartElement("symbols");
            for (int i=0; i < ui->lwSymbolFiles->count(); i++) {
                QString _fn = ui->lwSymbolFiles->item(i)->text();
                QString _fullName = ui->lwSymbolFiles->item(i)->toolTip();
                QString _checked = (ui->lwSymbolFiles->item(i)->checkState()==Qt::Checked ? "true":"false");
                xmlOut->writeStartElement("file");
                xmlOut->writeAttribute("filename", _fn);
                xmlOut->writeAttribute("fullname", _fullName);
                xmlOut->writeAttribute("checked", _checked);
                xmlOut->writeEndElement();
            }
        xmlOut->writeEndElement();  // symbol
    xmlOut->writeEndElement();  // dasm

    settings.setValue("dasm", s);
}

void DialogDasm::load() {
    qWarning()<<"LOAD";
    QSettings settings(workDir+"config.ini",QSettings::IniFormat);
    QString xmlData = settings.value("dasm").toString();
    qWarning()<<xmlData;
    if (xmlData.isEmpty()) return;

    QXmlStreamReader *xml = new QXmlStreamReader(xmlData);

    if (xml->readNextStartElement() && (xml->name() == "dasm")) {
        if (xml->readNextStartElement() &&
                (xml->name() == "symbols")) {
            while (xml->readNextStartElement()) {
                QString eltname = xml->name().toString();
//                            AddLog(LOG_TEMP,eltname);
                if (eltname == "file") {
                    QString _fn = xml->attributes().value("filename").toString();
                    QString _fullname = xml->attributes().value("fullname").toString();
                    Qt::CheckState _checkstate = xml->attributes().value("checked").toString()=="true"?Qt::Checked : Qt::Unchecked;
                    QListWidgetItem *_item = new QListWidgetItem(_fn);
                    _item->setToolTip(_fullname);
                    _item->setCheckState(_checkstate);
                    ui->lwSymbolFiles->addItem(_item);
                }
                xml->skipCurrentElement();
            }
        }
    }
}
