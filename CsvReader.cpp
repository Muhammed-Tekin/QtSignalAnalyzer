#include "CsvReader.h"
#include <QFile>
#include <QTextStream>

CsvReader::CsvReader() {}

QList<QStringList> CsvReader::readCsv(const QString &filePath)
{
    QList<QStringList> data;
    QFile file(filePath);

    // Dosyayı sadece okuma (ReadOnly) ve metin (Text) modunda açıyoruz
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return data; // Açılamazsa boş liste döner
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        // CSV'deki veriler genelde virgül ile ayrılır.
        // Excel'den alınan bazı Türkçe CSV'lerde noktalı virgül (;) olabilir.
        // Gerekirse buradaki ',' kısmını ';' yapabilirsin.
        QStringList row = line.split(';');
        data.append(row);
    }

    file.close();
    return data;
}