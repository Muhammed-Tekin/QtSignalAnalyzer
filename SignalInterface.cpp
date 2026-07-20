#include "SignalInterface.h"
#include "ui_SignalInterface.h"
#include "DataImportDialog.h"
#include <QFileDialog> // İşletim sisteminin dosya seçme penceresi için gerekli
#include <QString>     // Metin işlemleri için
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtMath>
#include "qcustomchartview.h"
#include <QChart>

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

        QVector<double> refY = dialog.getReferenceData();
        QVector<double> actY = dialog.getActualData();
        double sTime = dialog.getSamplingTime();
        drawSignals(refY, actY, sTime);
    }
}


void SignalInterface::drawSignals(const QVector<double> &refY, const QVector<double> &actY, double samplingTime)
{
    // 1. Zaten var olan chart nesnesini al
    QChart *chart = ui->signalGraphWidget->chart();

    // 2. Ekranı Temizle (Eski çizgileri ve eksenleri tamamen uçur)
    chart->removeAllSeries();
    for (auto axis : chart->axes()) {
        chart->removeAxis(axis);
    }

    // 3. Referans Sinyalini Oluştur ve Ayarla
    QLineSeries *seriesRef = new QLineSeries();
    seriesRef->setName("Reference Signal");
    QPen refPen(QColor(0x00, 0xE5, 0xFF));
    refPen.setWidth(2);
    refPen.setStyle(Qt::DashLine);
    seriesRef->setPen(refPen);

    // 4. Gerçekleşen Sinyali Oluştur ve Ayarla
    QLineSeries *seriesAct = new QLineSeries();
    seriesAct->setName("Actual Signal");
    QPen actPen(QColor(0xFF, 0x9D, 0x00));
    actPen.setWidth(2);
    seriesAct->setPen(actPen);

    int dataSize = qMin(refY.size(), actY.size());

    // 5. Verileri Çizgilere Doldur (Önce veri yükleniyor)
    for (int i = 0; i < dataSize; ++i) {
        double timeX = i * samplingTime;
        seriesRef->append(timeX, refY[i]);
        seriesAct->append(timeX, actY[i]);
    }

    // 6. İçi Dolu Çizgileri Grafiğe Ekle
    chart->addSeries(seriesRef);
    chart->addSeries(seriesAct);

    // 7. Eksenleri Veriye Göre Otomatik Oluştur (Qt zaten sınırları burada buldu)
    chart->createDefaultAxes();

    // 8. Eksen İsimlerini Ayarla ve Sınırları Kaydet
    QValueAxis *axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).value(0));
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).value(0));

    if (axisX && axisY) {
        axisX->setTitleText("Time");
        axisX->setLabelFormat("%.2f");
        axisY->setTitleText("Amplitude");

        // YENİ: Arayüzdeki widget'ımızı kendi sınıfımıza dönüştürüp, Qt'nin bulduğu limitleri içine kaydediyoruz
        QCustomChartView *customView = qobject_cast<QCustomChartView*>(ui->signalGraphWidget);
        if (customView) {
            customView->setOriginalBounds(axisX->min(), axisX->max(), axisY->min(), axisY->max());
        }
    }
}
