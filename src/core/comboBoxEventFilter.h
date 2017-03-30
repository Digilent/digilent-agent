#ifndef COMBOBOXEVENTFILTER_H
#define COMBOBOXEVENTFILTER_H

#include <QObject>

class ComboBoxEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit ComboBoxEventFilter(QObject *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void mouseClick();

public slots:
};

#endif // COMBOBOXEVENTFILTER_H
