#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QTime>

QT_BEGIN_NAMESPACE

#define TRUE    1
#define FALSE   0

namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSerialPort *serialPort;
    QString     SendTextStr;
    QByteArray  SendTextByte;
    quint32     dataRxNumber = 0;      // 记录Rx数据量
    quint8      Serial_State = 0;      // 串口状态


private slots:
    void SendDataHex(int state);
    void serialPortReadyRead_Slot();
    void on_Button_Connect_Serial_clicked();
    void on_Button_Disconnect_Serial_clicked();
    void on_Button_Send_Message_clicked();
    void on_Button_Clear_Receive_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
