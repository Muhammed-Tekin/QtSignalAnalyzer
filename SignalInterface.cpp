#include "SignalInterface.h"
#include "ui_SignalInterface.h"

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
