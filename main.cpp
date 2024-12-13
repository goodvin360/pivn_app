#include <QApplication>
#include "ui_caller.h"
#include "ui_counters.h"
#include "ui_coefficients.h"
#include "ui_processing.h"
#include "ui_readFile.h"
#include "ui_plotter.h"
#include "callermainwindow.h"
#include "Counters.h"
#include "Coefficients.h"
#include "Processing.h"
#include "ReadFromFile.h"
#include "PlotterOptions.h"
#include "QScrollBar"
#include  <locale.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    CallerMainWindow *window = new CallerMainWindow(nullptr);
    Counters *countersWin = new Counters(nullptr);
    Coefficients * coefWin = new Coefficients(nullptr);
    Processing * procWin = new Processing(nullptr);
    ReadFromFile * readFromFileWin = new ReadFromFile(nullptr);
    PlotterOptions * plotterOptionsWin = new PlotterOptions(nullptr);

    Ui::MainWindow caller;
    Ui::Form_1 counters;
    Ui::Form_2 coefficients;
    Ui::Form_3 processing;
    Ui::Form_RF readFile;
    Ui::Form_Plot plotter;

    caller.setupUi(window);
    counters.setupUi(countersWin);
    coefficients.setupUi(coefWin);
    processing.setupUi(procWin);
    readFile.setupUi(readFromFileWin);
    plotter.setupUi(plotterOptionsWin);

    window->textBrowser = caller.textBrowser;
    window->textBrowser_2 = caller.textBrowser_2;
    window->textBrowser_3 = caller.textBrowser_3;
    window->textBrowser_4 = caller.textBrowser_4;
    window->textBrowser_5 = caller.textBrowser_5;

    window->lineEdit = caller.lineEdit;
    window->lineEdit_2 = caller.lineEdit_2;
    window->lineEdit_3 = caller.lineEdit_3;
    window->lineEdit_4 = caller.lineEdit_4;
    window->lineEdit_5 = caller.lineEdit_5;
    window->lineEdit_6 = caller.lineEdit_6;
    window->lineEdit_7 = caller.lineEdit_7;
    window->lineEdit_8 = caller.lineEdit_8;
    window->lineEdit_9 = caller.lineEdit_9;
    window->lineEdit_10 = caller.lineEdit_10;
    window->lineEdit_11 = caller.lineEdit_11;
    window->lineEdit_12 = caller.lineEdit_12;
    window->lineEdit_13 = caller.lineEdit_13;
    window->lineEdit_14 = caller.lineEdit_14;

    window->checkBox_2 = caller.checkBox_2;
    window->checkBox_3 = caller.checkBox_3;
    window->checkBox_4 = caller.checkBox_4;
    window->checkBox_6 = caller.checkBox_6;
    window->checkBox_7 = caller.checkBox_7;
    window->checkBox_9 = caller.checkBox_9;

    window->pushButton = caller.pushButton;
    window->pushButton_2 = caller.pushButton_2;
    window->pushButton_3 = caller.pushButton_3;
    window->pushButton_4 = caller.pushButton_4;
    window->pushButton_5 = caller.pushButton_5;
    window->pushButton_7 = caller.pushButton_7;
    window->pushButton_8 = caller.pushButton_8;
    window->pushButton_10 = caller.pushButton_10;
    window->pushButton_11 = caller.pushButton_11;

    caller.lineEdit->setText(QString::number(window->measTime));
    caller.lineEdit_2->setText(QString::number(0));
    caller.lineEdit_3->setText(QString::number(0));
    caller.lineEdit_4->setText(QString::number(0));
    caller.lineEdit_7->setText(QString::number(window->edgePoint));
    caller.lineEdit_9->setText(QString::number(window->avWindow));
    caller.lineEdit_10->setText(QString::number(window->leftTime));
    caller.lineEdit_11->setText(QString::number(0));
    caller.lineEdit_12->setText(QString::number(0));
    caller.lineEdit_13->setText(QString::number(0));
    caller.lineEdit_14->setText(QString::number(0));

    caller.checkBox->setChecked(1);
    caller.checkBox_3->setChecked(0);
    caller.checkBox_4->setChecked(0);
    caller.checkBox_6->setChecked(1);

    caller.pushButton->setEnabled(0);
    caller.pushButton_8->setEnabled(0);
    caller.pushButton_10->setEnabled(0);
    caller.pushButton_11->setEnabled(0);

    caller.lineEdit_5->setText(QString::number(window->integrationTime));
    caller.lineEdit_6->setText(QString::number(window->totalCounts));
    caller.lineEdit_8->setText(QString::number(window->readDelay));
    caller.lineEdit_2->setReadOnly(1);
    caller.lineEdit_3->setReadOnly(1);
    caller.lineEdit_4->setReadOnly(1);
    caller.lineEdit_6->setReadOnly(1);
    caller.lineEdit_7->setReadOnly(1);
    caller.lineEdit_10->setReadOnly(1);
    caller.lineEdit_11->setReadOnly(1);
    caller.lineEdit_12->setReadOnly(1);
    caller.lineEdit_13->setReadOnly(1);
    caller.lineEdit_14->setReadOnly(1);

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
          {caller.comboBox->addItem(serialPortInfo.portName());}
    window->resize(480, 640);
    window->show();
    return QApplication::exec();
}

void createAction() {

}
