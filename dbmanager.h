#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DbManager
{
public:
    DbManager(const QString& path, const QString& connName);
    ~DbManager();

    bool isOpen() const;
    QSqlQuery* query(const QString& strQuery);
    void close();

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
