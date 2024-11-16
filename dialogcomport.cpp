#include "dialogcomport.h"
#include "ui_dialogcomport.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

DialogCOMport::DialogCOMport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogCOMport)
{
    ui->setupUi(this);
    timerPort->start(100);
    SettingMenuComPort();
    this->setWindowIcon(QIcon("E:\\soft_QT\\ModelAI25\\images\\settings.png"));
    this->setWindowTitle("Настройки порта");
    ui->ButOnOffPort->setIcon(QIcon("E:\\soft_QT\\ModelAI25\\images\\disconnect.png"));
    ui->LabComName->setText("Порт неактивен");

    connect(ui->ButOnOffPort, &QPushButton::clicked, this, &DialogCOMport::ButClickOnOff);
    connect(ui->ButOnOffPort, &QPushButton::clicked, this, &DialogCOMport::SignalsButOnOffPort);
}

DialogCOMport::~DialogCOMport()
{
    delete ui;
}
void DialogCOMport::SettingComPort()
{
    SerialPort->setPortName(updateSettings.name = ui->serialPortInfoListBox->currentText());

    SerialPort->setBaudRate(updateSettings.baudRate = static_cast<QSerialPort::BaudRate>(ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt()));
    updateSettings.stringBaudRate = QString::number(updateSettings.baudRate);
    SerialPort->setDataBits(updateSettings.dataBits = static_cast<QSerialPort::DataBits>(ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt()));
    updateSettings.stringDataBits = ui->dataBitsBox->currentText();
    SerialPort->setParity(updateSettings.parity = static_cast<QSerialPort::Parity>(ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt()));
    updateSettings.stringParity = ui->parityBox->currentText();
    SerialPort->setStopBits(updateSettings.stopBits = static_cast<QSerialPort::StopBits>(ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt()));
    updateSettings.stringStopBits = ui->stopBitsBox->currentText();
    SerialPort->setFlowControl(updateSettings.flowControl = static_cast<QSerialPort::FlowControl>(ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt()));
    updateSettings.stringFlowControl = ui->flowControlBox->currentText();


}
void DialogCOMport::TimerPortEnable()
{

}
void DialogCOMport::ButClickOnOff()
{

    if(!ClickFlag)
    {
        ui->ButOnOffPort->setIcon(QIcon("E:\\soft_QT\\ModelAI25\\images\\disconnect.png"));
        SerialPort->close();
        ui->LabComName->setText("Порт неактивен");
        StrLabComParametr1 = "Порт не подключен";

        ui->baudRateBox->setEnabled(true);
        ui->dataBitsBox->setEnabled(true);
        ui->flowControlBox->setEnabled(true);
        ui->parityBox->setEnabled(true);
        ui->stopBitsBox->setEnabled(true);
        ui->serialPortInfoListBox->setEnabled(true);
        ClickFlag = true; //флаг кнопки connect/disconnect
        FlagSettingPort = false; //флаг посылается на кнопку проверки соединенния в функцию CheckingConnectionDevice()
        StatusReadyForExchange = false;
    }
    else
    {
        SettingComPort();
        if(SerialPort->open(QIODevice::ReadWrite))
        {
//            connect(SerialPort, &QSerialPort::readyRead, this, &DialogCOMport::Read);
            ui->ButOnOffPort->setIcon(QIcon("E:\\soft_QT\\ModelAI25\\images\\connect.png"));
            ui->LabComName->setText(tr("Подключен %1").arg(updateSettings.name));
            StrLabComParametr1 = tr("Подключен %1 с настройками: %2, %3, %4, %5, %6").arg(updateSettings.name).arg(updateSettings.stringBaudRate).arg(updateSettings.stringDataBits).arg(updateSettings.stringParity).arg(updateSettings.stringStopBits).arg(updateSettings.stringFlowControl);
            ui->baudRateBox->setEnabled(false);
            ui->dataBitsBox->setEnabled(false);
            ui->flowControlBox->setEnabled(false);
            ui->parityBox->setEnabled(false);
            ui->stopBitsBox->setEnabled(false);
            ui->serialPortInfoListBox->setEnabled(false);
            ClickFlag = false; //флаг кнопки connect/disconnect
            FlagSettingPort = true; //флаг посылается на кнопку проверки соединенния в функцию CheckingConnectionDevice()
        }
        else
        {
//            disconnect(SerialPort, &QSerialPort::readyRead, this, &DialogCOMport::Read);
            if(updateSettings.name == "Custom")
            {
                QMessageBox::about(this,"Ошибка", tr("Не выбран порт"));
            }
            else
            {//"E:\\soft_QT\\ModelAI25\\images\\24.png"
                QMessageBox::about(this,"Ошибка", tr("Не удалось открыть %1").arg(updateSettings.name));
            }
        }
    }
}

void DialogCOMport::WriteComPort(char data[], int dataSize)
{
    SerialPort->write(data, dataSize);
}
void DialogCOMport::WriteComPort(QByteArray &data)
{
    SerialPort->write(data);
}
void DialogCOMport::ReadComPort(QByteArray &data)
{
    data = SerialPort->readAll();
}
void DialogCOMport::ReadComPort(char data[], int dataSize)
{
    SerialPort->read(data, dataSize);
}
void DialogCOMport::SettingPortName()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }

    ui->serialPortInfoListBox->addItem(tr("Custom"));
}

void DialogCOMport::SettingMenuComPort()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));
    ui->baudRateBox->setCurrentIndex(3);

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}
void DialogCOMport::FormFromRX(int np, char RxBufferFS[], float paramIn[])
 {
    for(int kt=0;kt<np;kt++)
    {
        tbf.sbyte[0]=RxBufferFS[0+kt*4+2];
        tbf.sbyte[1]=RxBufferFS[1+kt*4+2];
        tbf.sbyte[2]=RxBufferFS[2+kt*4+2];
        tbf.sbyte[3]=RxBufferFS[3+kt*4+2];
        paramIn[kt]=tbf.fval;
    }
}
void DialogCOMport::FormToTX(int np, char TxBuffer[], float paramOut[])
{
   int kt;
   TxBuffer[0]='%';
   TxBuffer[1]='#';
   for(kt=0;kt<np;kt++)
   {
       tbf.fval=paramOut[kt];
       TxBuffer[0+kt*4+2]=tbf.sbyte[0];
       TxBuffer[1+kt*4+2]=tbf.sbyte[1];
       TxBuffer[2+kt*4+2]=tbf.sbyte[2];
       TxBuffer[3+kt*4+2]=tbf.sbyte[3];
   }

}
