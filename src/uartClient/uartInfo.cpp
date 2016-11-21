#include "uartInfo.h"

UartInfo::UartInfo()
{

}

void UartInfo::refreshPortInfo() {
    this->ports = QSerialPortInfo::availablePorts();
}
