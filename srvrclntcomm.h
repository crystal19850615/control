#ifndef SRVRCLNTCOMM_H
#define SRVRCLNTCOMM_H

#include <QObject>
#include <QTime>
#include <QMutex>
#include <QSqlQuery>
#include <qdebug.h>
#include <fcntl.h>
#include <unistd.h>

//#include "crc.h"
#include "serial.h"
#include "modbus-rtu.h"
#include "crc.h"
#include "dbmanager.h"

class SrvClntComm : public QObject
{
    Q_OBJECT

public:
    SrvClntComm(const QString &portName = "/dev/ttyUSB0");
    ~SrvClntComm();
    bool m_isFailure = false;
    int m_rtAS;
    uint8_t val1, val2;
    uint16_t val3, val4;

public slots:
    void updateFromClnt();
    int send_customized_command(uint8_t *pbReq, int iSizeOfReq);
    int receive_customized_result(const char *lpPortName, char *lpReadBuf, size_t iNumToRead);
    int openModbus(const char *lpPortName, int iBaud = 38400, char cParity = 'N', int iDataBit = 8, int iStopBit = 1);
    void closeModbus();

private:
    modbus_t *m_mb;
    QString m_portName;
    const QString m_dbPath = "confData";
    const QString m_strUpdateData = "update confNew set val = ? where name = \"10V-L\"";
    QMutex mutex;
    unsigned long m_count = 0, m_count_failed = 0, m_count_all = 0;
    DbManager *m_dbm;
};

#endif // SRVRCLNTCOMM_H
