#include "comboBoxEventFilter.h"
#include <QEvent>
#include <QDebug>

ComboBoxEventFilter::ComboBoxEventFilter(QObject *parent) : QObject(parent)
{

}

bool ComboBoxEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == 2) {
        qDebug("Mouse Click");
        emit mouseClick();
    }
    return false;
}
