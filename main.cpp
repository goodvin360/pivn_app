#include <QApplication>
#include "ui_caller.h"
#include "callermainwindow.h"
#include "QScrollBar"
#include  <locale.h>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    CallerMainWindow window(nullptr);
    Ui::MainWindow caller;
    caller.setupUi(&window);
    window.textBrowser = caller.textBrowser;
    window.textBrowser_2 = caller.textBrowser_2;
    window.lineEdit = caller.lineEdit;
    window.lineEdit_2 = caller.lineEdit_2;
    caller.lineEdit->setText(QString::number(window.measTime));
    caller.lineEdit_2->setText(QString::number(window.coefficient));
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
          {caller.comboBox->addItem(serialPortInfo.portName());}
    window.pushButton_3 = caller.pushButton_3;

    window.resize(480, 640);
    window.show();
    return QApplication::exec();
}
