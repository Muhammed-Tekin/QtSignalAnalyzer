#include "DataImportDialog.h"
#include "ui_DataImportDialog.h"
#include "CsvReader.h"        // Okuyucumuzu dahil ediyoruz
#include <QTableWidgetItem>   // Tablo hücrelerine veri girmek için gerekli
#include <QMessageBox>

DataImportDialog::DataImportDialog(const QString &filePath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DataImportDialog)
{
    ui->setupUi(this);


    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    QList<QStringList> csvData = CsvReader::readCsv(filePath);
    fillDataTableWidget(csvData);

}


DataImportDialog::~DataImportDialog()
{
    delete ui;
}


void DataImportDialog::fillDataTableWidget(const QList<QStringList> &csvData)
{
    // 2. Eğer dosya boş değilse tabloyu yapılandırıyoruz
    if (!csvData.isEmpty()) {
        int rowCount = csvData.size();
        int columnCount = csvData[0].size(); // İlk satırdaki eleman sayısı sütun sayısını verir

        // Tablonun boyutlarını belirliyoruz (İlk satır başlık olacağı için satır sayısından 1 çıkarıyoruz)
        ui->dataTableWidget->setRowCount(rowCount - 1);
        ui->dataTableWidget->setColumnCount(columnCount);

        // İlk satırı tablonun en üstüne (Header) başlık olarak ekliyoruz
        ui->dataTableWidget->setHorizontalHeaderLabels(csvData[0]);

        // 3. Verileri hücre hücre tabloya yerleştiriyoruz (r=1'den başlıyoruz çünkü r=0 başlıktır)
        for (int r = 1; r < rowCount; ++r) {
            for (int c = 0; c < columnCount; ++c) {
                // Eğer bazı satırlarda eksik veri varsa uygulamanın çökmesini engellemek için güvenlik kontrolü
                QString cellText = (c < csvData[r].size()) ? csvData[r][c] : "";

                // Hücre nesnesini oluşturup tabloya yerleştiriyoruz
                QTableWidgetItem *item = new QTableWidgetItem(cellText);
                ui->dataTableWidget->setItem(r - 1, c, item); // r-1 çünkü tablo satır indeksi 0'dan başlar
            }
        }
    }
}


// --- Referans Sinyal Ata Butonu ---
void DataImportDialog::on_assignRefButton_clicked()
{
    int secilenSutun = ui->dataTableWidget->currentColumn();

    if (secilenSutun < 0 || secilenSutun == refColumnIndex){
        return;
    }
    else if(secilenSutun == actColumnIndex){
        actColumnIndex = -1;
    }

    // Varsa eski seçilmiş sütunun rengini beyaza (temiz) çevir
    if (refColumnIndex != -1) {
        colorColumn(refColumnIndex, QColor(Qt::white));
    }
    refColumnIndex = secilenSutun;
    // Yeni sütunu Açık Maviye boya (RGB değerleri: 200, 230, 255)
    colorColumn(refColumnIndex, QColor(200, 230, 255));
}


// --- Actual Sinyal Ata Butonu ---
void DataImportDialog::on_assignActualButton_clicked()
{
    int secilenSutun = ui->dataTableWidget->currentColumn();

    if (secilenSutun < 0 || secilenSutun == actColumnIndex){
        return;
    }
    else if(secilenSutun == refColumnIndex){
        refColumnIndex = -1;
    }

    // Varsa eskisinin rengini temizle
    if (actColumnIndex != -1) {
        colorColumn(actColumnIndex, QColor(Qt::white));
    }
    actColumnIndex = secilenSutun;
    // Yeni sütunu Açık Yeşile boya (RGB değerleri: 200, 255, 200)
    colorColumn(actColumnIndex, QColor(200, 255, 200));
}


// --- Yardımcı Renklendirme Fonksiyonu ---
void DataImportDialog::colorColumn(int colIndex, const QColor &color)
{
    if (colIndex < 0) return; // Geçersiz sütun numarasıysa hiçbir şey yapma

    // Tablodaki o sütunun tüm satırlarını tek tek dönüp arka plan rengini değiştiriyoruz
    for (int r = 0; r < ui->dataTableWidget->rowCount(); ++r) {
        QTableWidgetItem *item = ui->dataTableWidget->item(r, colIndex);
        if (item) {
            item->setBackground(color);
        }
    }
}


// --- Ana Ekranın Çekeceği Veriler İçin Okuyucu (Getter) Fonksiyonlar ---
int DataImportDialog::getReferenceColumn() const {
    return refColumnIndex;
}


int DataImportDialog::getActualColumn() const {
    return actColumnIndex;
}


bool DataImportDialog::isStepResponse() const {
    // ui->stepRadio objesinin adı (senin koyduğun isme göre stepRadio olduğunu varsayıyorum)
    // Eğer seçiliyse true, değilse false döner.
    return ui->stepRadio->isChecked();
}


void DataImportDialog::on_okButton_clicked()
{
    // Hem Referans hem de Gerçek sinyal atanmış mı diye kontrol et
    if (refColumnIndex == -1 || actColumnIndex == -1) {

        QMessageBox::warning(this,
                             "Eksik Atama",
                             "Lütfen hem Referans hem de Gerçek Sinyal için birer sütun atayınız!");
        return;
    }
    accept();
}

