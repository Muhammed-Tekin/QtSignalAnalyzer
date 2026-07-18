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

private:
    Ui::SignalInterface *ui;
};
#endif // SIGNALINTERFACE_H
