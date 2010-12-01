#include <QMenu>
#include <QClipboard>
#include <QFileDialog>

#include "cprinter.h"
#include "paperwidget.h"

Cprinter::Cprinter(CPObject *parent):CPObject(parent)
{
	PaperColor = QColor(255,255,255);
	paperWidget = 0;
	paper_X = 0;
	paper_Y = 0;
}

Cprinter::~Cprinter()
{
	delete paperWidget;
}

void Cprinter::resizeEvent ( QResizeEvent * ) {
    float ratio = (float)this->width()/this->Pc_DX ;

    QRect rect = this->paperWidget->baseRect;
    this->paperWidget->setGeometry( rect.x()*ratio,
                                    rect.y()*ratio,
                                    rect.width()*ratio,
                                    rect.height()*ratio);
}


void Cprinter::setPaperPos(QPoint pos)
{
	paper_X = pos.x();
	paper_Y = pos.y();
}

QPoint Cprinter::PaperPos()
{
	return(QPoint(paper_X,paper_Y));
}
