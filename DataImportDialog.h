#ifndef DATAIMPORTDIALOG_H
#define DATAIMPORTDIALOG_H

#include <QDialog>
#include <QString>

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
private slots:
    void on_assignRefButton_clicked();

    void on_assignActualButton_clicked();

    void on_okButton_clicked();

private:
    Ui::DataImportDialog *ui;

    // Seçilen sütunların indekslerini hafızada tutacağımız değişkenler (-1: henüz seçilmedi demek)
    int refColumnIndex = -1;
    int actColumnIndex = -1;

    // Renkleri temizlemek ve atamak için işimizi kolaylaştıracak yardımcı bir fonksiyon
    void colorColumn(int colIndex, const QColor &color);
};

#endif // DATAIMPORTDIALOG_H
