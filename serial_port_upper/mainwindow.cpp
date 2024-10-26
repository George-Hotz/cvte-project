#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList serialNamePort;

    serialPort = new QSerialPort(this);

    // 绑定HEX发送chexkBox信号与对应的槽函数
    connect(ui->checkBox_send_hex,SIGNAL(stateChanged(int)),this,SLOT(SendDataHex(int)));

    foreach(const QSerialPortInfo &info , QSerialPortInfo::availablePorts()){
        serialNamePort<<info.portName();
    }
    QFont font("Microsoft YaHei",10,QFont::Normal);        //微软雅黑。字体大小16，Normal：正常，Bold 粗体，Black：黑体，Light：高亮
    setWindowTitle(" 串口助手 V1.0 ");
    setFixedSize(781,406);                                 // 固定窗口的大小
    ui->comboBox_uart->addItems(serialNamePort);           // 引入当前串口
    ui->Button_Disconnect_Serial->setEnabled(false);       // 断开按键关使能
    ui->textEdit_recv->setFont(font);
    ui->textEdit_send->setFont(font);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::serialPortReadyRead_Slot()
{
    QString stringdata;

    ui->textEdit_recv->setTextColor(QColor(Qt::black));

    // 读取串口接收的数据
    QByteArray RecvBuff = serialPort->readAll();

    //16进制接受-选框
    if(ui->checkBox_recv_hex->isChecked()){
        stringdata = RecvBuff.toHex(' ').trimmed().toUpper();    /*hex显示*/
    }else{
       stringdata = QString(RecvBuff);                           /*ascii显示*/
    }

    ui->textEdit_recv->insertPlainText(stringdata);
    ui->textEdit_recv->moveCursor(QTextCursor::End);  // 自动滚屏到最后一行 有BUG,不可用光标点击文本框
    dataRxNumber += RecvBuff.length();
    ui->label_Rx_Num->setText(QString::number(dataRxNumber));
    RecvBuff.clear();
}


void MainWindow::on_Button_Connect_Serial_clicked()
{
    serialPort->setPortName(ui->comboBox_uart->currentText());
    serialPort->setBaudRate(ui->comboBox_baud->currentText().toInt());      //设置波特率
    serialPort->setDataBits(QSerialPort::Data8);                            //设置数据位数  默认8位
    serialPort->setParity(QSerialPort::NoParity);                           //设置奇偶校验  默认无奇偶
    serialPort->setStopBits(QSerialPort::OneStop);                          //设置停止位    默认无停止
    serialPort->setFlowControl(QSerialPort::NoFlowControl);                 //设置流控制    默认无

    connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_Slot()));

    if(serialPort->open(QIODevice::ReadWrite)){                             //打开串口成功
        ui->Button_Connect_Serial->setEnabled(false);
        ui->Button_Disconnect_Serial->setEnabled(true);
        Serial_State = TRUE;
        QMessageBox::warning(this,tr("提示"),tr("串口连接成功"));
    }else{
        QMessageBox::warning(this,tr("错误"),tr("串口连接失败"));
    }
}


void MainWindow::on_Button_Disconnect_Serial_clicked()
{
    serialPort->close();
    Serial_State = FALSE;
    ui->Button_Connect_Serial->setEnabled(true);     // 连接串口按键使能
    ui->Button_Disconnect_Serial->setEnabled(false);   // 断开按键关使能
}


void MainWindow::on_Button_Send_Message_clicked()
{
    if (Serial_State == TRUE){
        //获取发送框字符
        SendTextStr = ui->textEdit_send->document()->toPlainText();
        SendTextByte = SendTextStr.toUtf8();
        if (SendTextByte.isEmpty() != true){
            if (ui->checkBox_send_hex->isChecked()){
                SendTextByte = SendTextByte.fromHex(SendTextByte);
                serialPort->write(SendTextByte); // 发送hex数据
                QString strdata = SendTextByte.toHex(' ').trimmed().toUpper();

                strdata = strdata.append("\r\n");

                ui->textEdit_recv->setTextColor(QColor(Qt::black));
                ui->textEdit_recv->insertPlainText(strdata);
            }else{
                // 发送ascii数据
                serialPort->write(SendTextByte);
                QString strdata = QString(SendTextByte);

                strdata = strdata.append("\r\n");

                ui->textEdit_recv->setTextColor(QColor(Qt::black));
                ui->textEdit_recv->insertPlainText(strdata);
            }
            //移动光标到末尾
            ui->textEdit_recv->moveCursor(QTextCursor::End);
        }else{
            QMessageBox::warning(this, "警告", "您需要在发送编辑框中输入要发送的数据");
        }

    }else{
        QMessageBox::information(this, "警告", "串口未打开");
    }
}


void MainWindow::on_Button_Clear_Receive_clicked()
{
    ui->textEdit_recv->clear();
    dataRxNumber = 0 ;
    ui->label_Rx_Num->setText(QString::number(dataRxNumber));
}

void MainWindow::SendDataHex(int state)
{
    //获取发送框字符
    SendTextStr = ui->textEdit_send->document()->toPlainText();
    SendTextByte = SendTextStr.toUtf8();
    if (SendTextStr.isEmpty()){
        return;
    }
    //asccii与hex转换
    if (state == Qt::Checked){
        //转换成16进制数并转换为大写
        SendTextByte = SendTextByte.toHex(' ').toUpper();
        ui->textEdit_send->document()->setPlainText(SendTextByte);
    }else{
        //从QByteArray转换为QString
        SendTextStr = SendTextByte.fromHex(SendTextByte);
        ui->textEdit_send->document()->setPlainText(SendTextStr);
    }
}
