#ifndef SIGNALINTERFACE_H
#define SIGNALINTERFACE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class SignalInterface;
}
QT_END_NAMESPACE

class SignalInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignalInterface(QWidget *parent = nullptr);
    ~SignalInterface() override;

private slots:
    void on_loadDataButton_clicked();

private:
    Ui::SignalInterface *ui;

    void drawSignals(const QVector<double> &refY, const QVector<double> &actY, double samplingTime);
};
#endif // SIGNALINTERFACE_H
