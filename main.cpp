#include <QApplication>
#include "ui_caller.h"
#include "ui_counters.h"
#include "ui_coefficients.h"
#include "callermainwindow.h"
#include "Counters.h"
#include "Coefficients.h"
#include "QScrollBar"
#include  <locale.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    CallerMainWindow *window = new CallerMainWindow(nullptr);
    Counters *countersWin = new Counters(nullptr);
    Coefficients * coefWin = new Coefficients(nullptr);

    Ui::MainWindow caller;
    Ui::Form_1 counters;
    Ui::Form_2 coefficients;

    caller.setupUi(window);
    counters.setupUi(countersWin);
    coefficients.setupUi(coefWin);

    window->textBrowser = caller.textBrowser;
    window->textBrowser_2 = caller.textBrowser_2;
    window->textBrowser_3 = caller.textBrowser_3;
    window->lineEdit = caller.lineEdit;
    window->lineEdit_5 = caller.lineEdit_5;
    window->lineEdit_6 = caller.lineEdit_6;
    window->lineEdit_7 = caller.lineEdit_7;
    window->lineEdit_8 = caller.lineEdit_8;
    window->lineEdit_9 = caller.lineEdit_9;
    window->lineEdit_10 = caller.lineEdit_10;
    window->checkBox_3 = caller.checkBox_3;
    window->checkBox_4 = caller.checkBox_4;
    window->checkBox_6 = caller.checkBox_6;
    window->checkBox_7 = caller.checkBox_7;
    window->pushButton_3 = caller.pushButton_3;

    caller.lineEdit->setText(QString::number(window->measTime));
    caller.lineEdit_7->setText(QString::number(window->edgePoint));
    caller.lineEdit_9->setText(QString::number(window->avWindow));
    caller.lineEdit_10->setText(QString::number(window->leftTime));

    caller.checkBox->setChecked(1);
    caller.checkBox_3->setChecked(0);
    caller.checkBox_4->setChecked(0);
    caller.checkBox_6->setChecked(1);
    caller.lineEdit_5->setText(QString::number(window->integrationTime));
    caller.lineEdit_6->setText(QString::number(window->totalCounts));
    caller.lineEdit_8->setText(QString::number(window->readDelay));
    caller.lineEdit_6->setReadOnly(1);
    caller.lineEdit_7->setReadOnly(1);
    caller.lineEdit_10->setReadOnly(1);

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
          {caller.comboBox->addItem(serialPortInfo.portName());}
    window->resize(480, 640);
    window->show();
    return QApplication::exec();
}
