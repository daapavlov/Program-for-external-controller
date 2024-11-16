#ifndef DIALOGCOMPORT_H
#define DIALOGCOMPORT_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class DialogCOMport;
}

class DialogCOMport : public QWidget
{
    Q_OBJECT

public:
    explicit DialogCOMport(QWidget *parent = nullptr);
    ~DialogCOMport();

    bool FlagWriteReadComPort = false, FlagSettingPort = false, StatusWriteStringSettingComPort = false;
    bool StatusReadyForExchange = false; //статус, который выставляется в true, если микроконтроллер подал сигнал о принятии проверочных 8 байт
    bool ClickFlag = true;

    QByteArray arr;
    char arrc[6];

    QString StrLabComParametr1 = "Порт не подключен", StrLabComParametr2, StrLabComParametr3;

    QSerialPort *SerialPort = new QSerialPort();

    void WriteComPort(char data[], int dataSize);
    void WriteComPort(QByteArray &data);
    void ReadComPort(char data[], int dataSize);
    void ReadComPort(QByteArray &data);
    void FormToTX(int np, char TxBuffer[], float paramOut[]);
    void FormFromRX(int np,char RxBufferFS[],float paramIn[]);
    void SettingPortName();
    void ButClickOnOff();

signals:
    void SignalsButOnOffPort();

private:
    struct Settings {
            QString name;
            qint32 baudRate;
            QString stringBaudRate;
            QSerialPort::DataBits dataBits;
            QString stringDataBits;
            QSerialPort::Parity parity;
            QString stringParity;
            QSerialPort::StopBits stopBits;
            QString stringStopBits;
            QSerialPort::FlowControl flowControl;
            QString stringFlowControl;
            bool localEchoEnabled;
        };

    /****Преобразование в байты****/
    union
        {
         uint8_t sbyte[4];
         uint32_t lval;
        }tbi;

    union
        {
         unsigned char sbyte[4];
         float fval;
        }tbfmod;
    union
        {
         uint8_t sbyte[4];
         float fval;
        }tbf;

    Ui::DialogCOMport *ui;
    void SettingComPort();
    QTimer *timerPort = new QTimer(this);


    Settings updateSettings;


    void SettingMenuComPort();
    void TimerPortEnable();

    void Read();

};

#endif // DIALOGCOMPORT_H
