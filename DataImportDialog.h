#ifndef DATAIMPORTDIALOG_H
#define DATAIMPORTDIALOG_H

#include <QDialog>
#include <QString>
#include <QVector>

namespace Ui {
class DataImportDialog;
}

class DataImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataImportDialog(const QString &filePath, QWidget *parent = nullptr);
    ~DataImportDialog();

    void fillDataTableWidget(const QList<QStringList> &csvData);
    // Ana pencerenin (SignalInterface) kullanıcının neyi seçtiğini öğrenmesi için:
    int getReferenceColumn() const;
    int getActualColumn() const;
    bool isStepResponse() const; // Hangi veri türünün (Step/Frekans) seçildiğini öğrenmek için

    QVector<double> getReferenceData() const;
    QVector<double> getActualData() const;

    double getSamplingTime() const;

private slots:
    void on_assignRefButton_clicked();

    void on_assignActualButton_clicked();

    void on_okButton_clicked();

private:
    Ui::DataImportDialog *ui;

    // Seçilen sütunların indekslerini hafızada tutacağımız değişkenler (-1: henüz seçilmedi demek)
    int refColumnIndex = -1;
    int actColumnIndex = -1;

    QVector<double> referenceData;
    QVector<double> actualData;

    // Renkleri temizlemek ve atamak için işimizi kolaylaştıracak yardımcı bir fonksiyon
    void colorColumn(int colIndex, const QColor &color);
    void extractDataFromTable();
};

#endif // DATAIMPORTDIALOG_H
