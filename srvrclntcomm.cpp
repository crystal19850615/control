#include "srvrclntcomm.h"

SrvClntComm::SrvClntComm(const QString &portName)
{
    int modBusOpenRes, rirRes, iCount = 0;
    m_portName = portName;
    QTime time;
    QString strTime;
    uint16_t tab_reg[22];
    uint8_t tab_reg_8_le[40];
    uint16_t crcVal;

    m_isFailure = false;
    modBusOpenRes = openModbus(m_portName.toStdString().c_str());
    if (modBusOpenRes == -1)
    {
        qDebug() << "openModbus failed...";
    }
    strTime = time.currentTime().toString("hh:mm:ss");
    qDebug() << "start time: " << strTime;
    while(iCount < 3)
    {
        rirRes = modbus_read_input_registers(m_mb, 0, 22, tab_reg);
        for (int i = 0; i < 40; i+=2)
        {
            tab_reg_8_le[i + 1] = ((uint8_t *)tab_reg)[i];
            tab_reg_8_le[i] = ((uint8_t *)tab_reg)[i+1];
        }
        if (rirRes > 0)
        {
            /*crcVal = crc16(tab_reg_8_le, 40);
            qDebug() << "crcVal: " << crcVal;
            if (crcVal == tab_reg[20])
            {
                QSqlQuery *queryRes;
                QString strUpdate;
                m_dbm = new DbManager(m_dbPath, "updateInit");
                QString strASLVal;
                strASLVal = QString::number(tab_reg[0]);
                strUpdate = "update confNew set val = " + strASLVal + " where name = \"10V-L\"";
                qDebug() << strUpdate;
                queryRes = m_dbm->query(strUpdate);
                queryRes->clear();
                delete queryRes;

                strASLVal = QString::number(tab_reg[1]);
                strUpdate = "update confNew set val = " + strASLVal + " where name = \"10V-H\"";
                qDebug() << strUpdate;
                queryRes = m_dbm->query(strUpdate);
                queryRes->clear();
                delete queryRes;

                m_dbm->close();
                delete m_dbm;
                break;
            }*/
        }
        iCount++;
    }
    if (iCount >= 3)
    {
        m_isFailure = true;
    }
}

SrvClntComm::~SrvClntComm()
{
    QTime time;
    QString strTime;

    strTime = time.currentTime().toString("hh:mm:ss");
    qDebug() << "end time: " << strTime;

    qDebug() << "count of event: " << m_count;
    qDebug() << "value of m_count_failed: " << m_count_failed;
    qDebug() << "value of m_count_all: " << m_count_all;
    closeModbus();
}

int SrvClntComm::openModbus(const char *lpPortName, int iBaud, char cParity, int iDataBit, int iStopBit)
{
    int connRes;
    struct timeval timeout;
    struct timeval byte_timeout;

    m_mb = modbus_new_rtu(lpPortName, iBaud, cParity, iDataBit, iStopBit);

    if(m_mb == NULL)
    {
        return -1;
    }
    //modbus_get_response_timeout(m_mb, &timeout);
    timeout.tv_sec = 0;
    timeout.tv_usec = 40000;
    byte_timeout.tv_sec = 0;
    byte_timeout.tv_usec = 40000;
    modbus_set_response_timeout(m_mb, &timeout);
    modbus_set_byte_timeout(m_mb, &byte_timeout);
    //modbus_set_debug(mb, 1);
    modbus_set_slave(m_mb, 1);
    connRes = modbus_connect(m_mb);
    //modbus_get_response_timeout(m_mb, &timeout);
    //qDebug() << "modbus_strerror: " << modbus_strerror(errno);

    return connRes;
}

void SrvClntComm::closeModbus()
{
    modbus_close(m_mb);
    modbus_free(m_mb);
}

int SrvClntComm::send_customized_command(uint8_t *pbReq, int iSizeOfReq)
{
    int iRes;
    iRes = modbus_send_raw_request(m_mb, pbReq, iSizeOfReq);
    if(iRes == -1)
    {
        return -1;
    }
    return iRes;
}

int SrvClntComm::receive_customized_result(const char *lpPortName, char *lpReadBuf, size_t iNumToRead)
{
    int iRes;
    int iNumRead;
    int fd;
    int iFuncRes = 0;

    fd = open (lpPortName, O_RDONLY | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        qDebug() << strerror(errno);
        qDebug() << "open failed...";
        return -1;
    }

    do
    {
        iRes = set_interface_attribs(fd, B38400, 0);
        if(iRes == -1)
        {
            qDebug() << "set_interface_attribs failed: " << iRes;
            iFuncRes = -1;
            break;
        }
        set_blocking (fd, 0);

        iNumRead = read(fd, lpReadBuf, iNumToRead);
        if(iNumRead == -1)
        {
            qDebug() << "read failed: " << iNumRead;
            ::close(fd);
            iFuncRes = -1;
            break;
        }
        iFuncRes = iNumRead;
    }
    while(false);

    return iFuncRes;
}

void SrvClntComm::updateFromClnt()
{
    uint8_t cReadBuf[MODBUS_RTU_MAX_ADU_LENGTH];
    int recvRes, sendRes, flushRes;
    uint8_t raw_req[] = { 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    //qDebug() << val1;
    m_count_all++;

    if (mutex.tryLock())
    {
        m_count++;
        //qDebug() << "count of event: " << m_count;

        flushRes = modbus_flush(m_mb);
        if (flushRes == -1)
        {
            //qDebug() << "flushRes: " << flushRes;
            //qDebug() << "modbus_flush modbus_strerror(errno): " << modbus_strerror(errno);
        }
        sendRes = send_customized_command(raw_req, sizeof(raw_req));
        if (sendRes == -1)
        {
            m_count_failed++;
            //qDebug() << "sendRes: " << sendRes;
            //qDebug() << "send_customized_command modbus_strerror(errno): " << modbus_strerror(errno);
        }
        else
        {
            recvRes = modbus_receive_raw_data(m_mb, cReadBuf);
            if (recvRes == -1)
            {
                m_count_failed++;
                //qDebug() << "recv failed m_count with m_count_failed: " << m_count << ", " << m_count_failed;
                //qDebug() << "modbus_receive_raw_data modbus_strerror(errno): " << modbus_strerror(errno);
            }
            else
            {
                m_rtAS = (cReadBuf[5] << 8) + cReadBuf[6];
            }
        }
        mutex.unlock();
    }
}
