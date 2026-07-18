#ifndef CSVREADER_H
#define CSVREADER_H
#include <QString>
#include <QList>
#include <QStringList>

class CsvReader
{
public:
    CsvReader();

    static QList<QStringList> readCsv(const QString &filePath);
};

#endif // CSVREADER_H
