#include "dbmanager.h"
#include <qdebug.h>

DbManager::DbManager(const QString& path, const QString& connName)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", connName);
    m_db.setDatabaseName(path);

    if(!m_db.open())
    {
        qDebug() << "Error: connection with database fail...";
    }
}

DbManager::~DbManager()
{
    qDebug() << "DbManager::~DbManager is called...";
    close();
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

QSqlQuery* DbManager::query(const QString& strQuery)
{
    QSqlQuery *retQuery = new QSqlQuery(strQuery, m_db);
    retQuery->exec();

    return retQuery;
}

void DbManager::close()
{
    QString connName = m_db.connectionName();
    if(m_db.isOpen())
    {
        qDebug() << "Database: close connection.";
        m_db.close();
    }
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connName);
}
