#include "registerlabel.h"

RegisterLabel::RegisterLabel(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent, f)
{

}

RegisterLabel::RegisterLabel(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text, parent, f)
{

}

RegisterLabel::~RegisterLabel()
{

}

void RegisterLabel::mousePressEvent(QMouseEvent *ev)
{
    emit clicked(ev->pos());
}
