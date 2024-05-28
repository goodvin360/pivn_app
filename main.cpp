#include <QApplication>
#include "ui_caller.h"
#include "callermainwindow.h"
#include "QScrollBar"
#include  <locale.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    CallerMainWindow *window = new CallerMainWindow(nullptr);
    Ui::MainWindow caller;
    caller.setupUi(window);
    window->textBrowser = caller.textBrowser;
    window->textBrowser_2 = caller.textBrowser_2;
    window->textBrowser_3 = caller.textBrowser_3;
    window->lineEdit = caller.lineEdit;
    window->lineEdit_2 = caller.lineEdit_2;
    window->lineEdit_3 = caller.lineEdit_3;
    window->lineEdit_4 = caller.lineEdit_4;
    window->lineEdit_5 = caller.lineEdit_5;
    window->lineEdit_6 = caller.lineEdit_6;
    window->checkBox_6 = caller.checkBox_6;
    window->checkBox_7 = caller.checkBox_7;
    caller.lineEdit->setText(QString::number(window->measTime));
    caller.checkBox->setChecked(1);
    caller.checkBox_2->setChecked(1);
    caller.checkBox_6->setChecked(1);
    caller.lineEdit_2->setText(QString::number(window->coeff_a));
    caller.lineEdit_3->setText(QString::number(window->coeff_b));
    caller.lineEdit_4->setText(QString::number(window->distance));
    caller.lineEdit_5->setText(QString::number(window->integrationTime));
    caller.lineEdit_6->setText(QString::number(window->totalCounts));
    caller.lineEdit_6->setReadOnly(1);
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
          {caller.comboBox->addItem(serialPortInfo.portName());}
    window->pushButton_3 = caller.pushButton_3;
    window->resize(480, 640);
    window->show();
    return QApplication::exec();
}
