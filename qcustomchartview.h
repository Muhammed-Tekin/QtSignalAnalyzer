#ifndef QCUSTOMCHARTVIEW_H
#define QCUSTOMCHARTVIEW_H

#include <QChartView>
#include <QChart>
#include <QMouseEvent>
#include <QWheelEvent>


    class QCustomChartView : public QChartView {
    Q_OBJECT
public:
    explicit QCustomChartView(QWidget *parent = nullptr);
    void setOriginalBounds(qreal minX, qreal maxX, qreal minY, qreal maxY);

protected:
    // Fare ve klavye etkileşimlerini yakalayan fonksiyonlar
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    bool m_isPanning;
    QPoint m_lastMousePos;

    bool m_hasBounds = false;
    qreal m_minX, m_maxX, m_minY, m_maxY;
};

#endif // QCUSTOMCHARTVIEW_H