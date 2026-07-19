#include "SignalInterface.h"
#include "ui_SignalInterface.h"
#include "DataImportDialog.h"
#include <QFileDialog> // İşletim sisteminin dosya seçme penceresi için gerekli
#include <QString>     // Metin işlemleri için
#include <qcustomplot.h>

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
    // Veri yoksa çökmeyi engellemek için güvenlik kontrolü
    if(refY.isEmpty() || actY.isEmpty()) return;

    // X Ekseni (Zaman/İndeks) verisi oluşturuyoruz (0, 1, 2, 3... şeklinde)
    QVector<double> xData(refY.size());
    for (int i = 0; i < refY.size(); ++i) {
        xData[i] = i*samplingTime;
    }

    // QCustomPlot ayarları
    QCustomPlot *plot = ui->signalGraphWidget;
    plot->clearGraphs();

    // --- ARKA PLAN VE EKSEN RENKLERİ ---
    plot->setBackground(QBrush(Qt::black));

    QPen whitePen(Qt::white, 1);
    plot->xAxis->setBasePen(whitePen);    plot->yAxis->setBasePen(whitePen);
    plot->xAxis->setTickPen(whitePen);    plot->yAxis->setTickPen(whitePen);
    plot->xAxis->setSubTickPen(whitePen); plot->yAxis->setSubTickPen(whitePen);
    plot->xAxis->setTickLabelColor(Qt::white); plot->yAxis->setTickLabelColor(Qt::white);
    plot->xAxis->setLabelColor(Qt::white);     plot->yAxis->setLabelColor(Qt::white);

    // --- REFERANS SİNYALİ ÇİZİMİ (Açık Mavi) ---
    plot->addGraph();
    plot->graph(0)->setData(xData, refY);
    plot->graph(0)->setPen(QPen(QColor(100, 200, 255), 2));
    plot->graph(0)->setName("Referans");

    // --- GERÇEK SİNYAL ÇİZİMİ (Açık Yeşil) ---
    plot->addGraph();
    plot->graph(1)->setData(xData, actY);
    plot->graph(1)->setPen(QPen(QColor(150, 255, 150), 2));
    plot->graph(1)->setName("Gerçek");

    // Etkileşimler ve Ölçeklendirme
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->graph(0)->rescaleAxes();
    plot->graph(1)->rescaleAxes(true);

    plot->replot();
}