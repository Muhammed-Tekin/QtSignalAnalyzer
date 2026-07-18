#include "SignalInterface.h"
#include "ui_SignalInterface.h"
#include "DataImportDialog.h"
#include <QFileDialog> // İşletim sisteminin dosya seçme penceresi için gerekli
#include <QString>     // Metin işlemleri için
SignalInterface::SignalInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SignalInterface)
{
    ui->setupUi(this);
}

SignalInterface::~SignalInterface()
{
    delete ui;
}

void SignalInterface::on_loadDataButton_clicked()
{
    // 1. QFileDialog ile kullanıcıdan bir dosya yolu istiyoruz.
    // getOpenFileName; (pencere sahibi, pencere başlığı, başlangıç klasörü, filtre) parametrelerini alır.
    QString secilenDosyaYolu = QFileDialog::getOpenFileName(this,
                                                            "Veri Dosyası Seç",
                                                            "",
                                                            "Veri Dosyaları (*.csv *.xlsx);;Tüm Dosyalar (*.*)");

    // 2. Kullanıcı dosya seçmekten vazgeçip "İptal"e veya çarpıya basarsa,
    // secilenDosyaYolu boş döner. Boşsa hiçbir şey yapmadan fonksiyondan çıkıyoruz (return).
    if (secilenDosyaYolu.isEmpty()) {
        return;
    }

    // 3. Dosya başarıyla seçildiyse (artık yolunu biliyoruz), bizim özel diyalog penceremizi açıyoruz.
    // NOT: Bir sonraki adımda bu 'secilenDosyaYolu' değişkenini CsvReader'a verip içini okutacağız!
    DataImportDialog dialog(secilenDosyaYolu, this);

    // 2. Diyaloğu ekranda modal (diğer pencereleri kilitleyen) olarak açıyoruz
    // exec() fonksiyonu, kullanıcı pencereyi kapatana kadar kodu burada bekletir.
    if (dialog.exec() == QDialog::Accepted) {

        // Eğer kullanıcı "Tamam" (okButton) tuşuna basarak çıkarsa bu bloğa girer.
        // TODO: Seçilen sütunları al, CsvReader'ı çalıştır, ana ekrana çiz.

    } else {
        // Kullanıcı "İptal"e (cancelButton) veya çarpı (X) tuşuna basarsa buraya düşer.
        // Hiçbir şey yapmamıza gerek yok, sessizce kapanır.
    }
}

