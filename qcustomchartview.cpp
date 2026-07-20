#include "qcustomchartview.h"
#include <QtCharts/QValueAxis>

QCustomChartView::QCustomChartView(QWidget *parent) : QChartView(parent), m_isPanning(false) {
    // 1. Çizgilerin piksel piksel tırtıklı görünmesini engeller
    setRenderHint(QPainter::Antialiasing);

    // 2. Sol tıkla kutu çizip sadece o bölgeye yakınlaştırma özelliği
    setRubberBand(QChartView::RectangleRubberBand);

    // 2. Grafiği (Chart) TEK SEFERLİK oluştur ve ayarla
    QChart *myChart = new QChart();

    // --- TEMA VE RENK AYARLARI ---
    myChart->setTheme(QChart::ChartThemeDark);
    QColor darkGray(45, 45, 45);

    myChart->setBackgroundBrush(QBrush(darkGray)); // Dış çerçeve
    myChart->setPlotAreaBackgroundVisible(true);
    myChart->setPlotAreaBackgroundBrush(QBrush(darkGray)); // İç çizim alanı

    // Lejant (açıklama) ayarları
    myChart->legend()->setVisible(true);
    myChart->legend()->setAlignment(Qt::AlignBottom);

    // 3. Hazırlanan grafiği View (Görünüm) içine yerleştir
    setChart(myChart);

    // View'ın kendi arka planını da gri yap
    setBackgroundBrush(QBrush(darkGray));
}

void QCustomChartView::wheelEvent(QWheelEvent *event) {
    if (chart()) {
        // Fare tekerleği dönüş yönüne göre yakınlaştır veya uzaklaştır
        // 1.1 demek %10 oranında yumuşak zoom demektir. (Daha da yavaş isterseniz 1.05 yapabilirsiniz)
        qreal zoomFactor = 1.05;
        if (event->angleDelta().y() > 0) {
            chart()->zoom(zoomFactor);
        } else {
            chart()->zoom(1.0 / zoomFactor);
        }
        event->accept();
    }
}

void QCustomChartView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        // Sağ tıka basılı tutulduğunda kaydırma (Pan) modunu başlat
        m_isPanning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor); // İmleci yumruk şekline çevir
        event->accept();
    } else {
        QChartView::mousePressEvent(event);
    }
}

void QCustomChartView::mouseMoveEvent(QMouseEvent *event) {
    if (m_isPanning && chart()) {
        // Fareyi hareket ettirdikçe grafiği de peşinden sürükle
        QPoint delta = event->pos() - m_lastMousePos;
        chart()->scroll(-delta.x(), delta.y());
        m_lastMousePos = event->pos();
        event->accept();
    } else {
        QChartView::mouseMoveEvent(event);
    }
}

void QCustomChartView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        // Sağ tık bırakıldığında kaydırma modundan çık
        m_isPanning = false;
        setCursor(Qt::CrossCursor); // İmleci eski haline (artı şekline) döndür
        event->accept();
    } else {
        QChartView::mouseReleaseEvent(event);
    }
}

void QCustomChartView::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && chart()) {

        // 1. Zoom geçmişini sıfırla
        chart()->zoomReset();

        // 2. Eğer sınırlar ilk başta kaydedildiyse, döngüye girmeden anında uygula (O(1) Karmaşıklığı)
        if (m_hasBounds) {
            QValueAxis *axisX = qobject_cast<QValueAxis*>(chart()->axes(Qt::Horizontal).value(0));
            if (axisX) axisX->setRange(m_minX, m_maxX);

            QValueAxis *axisY = qobject_cast<QValueAxis*>(chart()->axes(Qt::Vertical).value(0));
            if (axisY) axisY->setRange(m_minY, m_maxY);
        }

        event->accept();
    } else {
        QChartView::mouseDoubleClickEvent(event);
    }
}

void QCustomChartView::setOriginalBounds(qreal minX, qreal maxX, qreal minY, qreal maxY) {
    m_minX = minX;
    m_maxX = maxX;
    m_minY = minY;
    m_maxY = maxY;
    m_hasBounds = true; // Sınırlar kaydedildi bayrağını kaldır
}