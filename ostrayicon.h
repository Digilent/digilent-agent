#ifndef OSTRAYICON_H
#define OSTRAYICON_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class OsTrayIcon : public QSystemTrayIcon
{
public:    
    OsTrayIcon(QObject *parent);
};

#endif // OSTRAYICON_H
