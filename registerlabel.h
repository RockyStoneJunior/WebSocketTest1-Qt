#ifndef REGISTERLABEL_H
#define REGISTERLABEL_H

#pragma execution_character_set("utf-8")

#include <QLabel>
#include <QWidget>
#include <QObject>
#include <QMouseEvent>
#include <QPoint>

class RegisterLabel : public QLabel
{
    Q_OBJECT

public:
    RegisterLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);
    RegisterLabel(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~RegisterLabel();

protected:
    void mousePressEvent(QMouseEvent *ev);

signals:
    void clicked(QPoint pos);
};

#endif // REGISTERLABEL_H
