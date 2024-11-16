#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("E:\\soft_QT\\ModelAI25\\images\\icons8-64.png"));

    ui->WidgetPlotParametr->addWidget(GraphicEng->WidgetPlotEngParam);
    ui->ButClearListessage->setIcon(QIcon("E:\\soft_QT\\ModelAI25\\images\\clear.png"));

    ui->groupBox_3->setCheckable(true);


    TimerGraphic->start(20);

    connect(TimerGraphic, &QTimer::timeout, this, &MainWindow::TimerGraphEnable);
//    connect(TimerGraphic, &QTimer::timeout, this, &MainWindow::GraphicSeriesCreate);
    connect(timer, &QTimer::timeout, this, &MainWindow::TimerEnable);
    connect(ui->action, &QAction::triggered, this, &MainWindow::OpenGraphWindow);
//    connect(ui->action, &QAction::triggered, this, &MainWindow::OpenGrapgWindowASeparateThread);
    connect(ui->action_COM, &QAction::triggered, this, &MainWindow::OpenDialogComPort);
    connect(ui->ButWriteOnOff, &QPushButton :: clicked, this,  &MainWindow::WriteToFileOnOff);
    connect(ui->ButClearListessage, &QPushButton::clicked, this,  &MainWindow::ClearListMessage);
    connect(ui->ButExchange, &QPushButton::clicked, this,  &MainWindow::PricessingExchangeButton);
    connect(ComPort, &DialogCOMport::SignalsButOnOffPort, this, &MainWindow::StatusPort);
    connect(ui->ClearGraphic, &QPushButton::clicked, this, &MainWindow::ClearGraphic);

//    EngineAI25->init(ArrayParamEngineIn, ArrayParamEngineOut);
}
void MainWindow::ClearListMessage()
{
    ui->ListMessage->clear();
}
void MainWindow::ClearGraphic()
{
    GraphicEng->SeriesPlotEngParam->clear();
    GraphicEng->SeriesPlotEngParamZad->clear();
    ClearArrayParamRealTime();

}
void MainWindow::SerialPortReceive()
{

    ComPort->ReadComPort(ReceivedDataController, SizeRx);
    if(ReceivedDataController[0]=='%' && ReceivedDataController[1]=='#')
    {
        ComPort->FormFromRX((90-2)/4, ReceivedDataController, ReceiveParametrsController);

        if(FlagWriteMessage == 0)//Не дает повторяться сообщению в строке сообщений
        {
            MessageStringWrite("Подключено");
            FlagWriteMessage = 1;
        }

        FillingArraysParametrsFromController();
        TimeOfTimer = ReceiveParametrsController[0];
        GraphicSeriesCreate();
        ReceivedDataController[0]=0;
        ErrorConection=0;
    }
    CheckingPanelSimulation(TransmitedParametrsController, TransmittedDataController);
}
void MainWindow::PricessingExchangeButton()
{
    if(FlagWriteReadOnOff == 1)
    {
        if(ComPort->SerialPort->isOpen())
        {
            timer->start(20);
            CheckingPanelSimulation(TransmitedParametrsController, TransmittedDataController);
            MessageStringWrite("Попытка подключения");
            connect(ComPort->SerialPort, &QSerialPort::readyRead, this, &MainWindow::SerialPortReceive);
            ui->ButExchange->setText("Закончить обмен");
            FlagWriteReadOnOff = 2;
        }
        else
        {
            MessageStringWrite("Подключите порт");
        }
    }
    else if(FlagWriteReadOnOff == 2)
    {
        for(int i=0;i<3;i++)
        {
            TestWordOut[i]='-';
        }
        ComPort->ReadComPort(TestWordOut, 3);
        timer->stop();
        disconnect(ComPort->SerialPort, &QSerialPort::readyRead, this, &MainWindow::SerialPortReceive);
        ui->ButExchange->setText("Начать обмен");
        FlagWriteReadOnOff = 1;
        FlagWriteMessage = 0;
        ComPort->ButClickOnOff();
        MessageStringWrite("Обмен закончен. Порт отключен");
    }

}
void MainWindow::CheckingPanelSimulation(float Array[], char ArrayTx[])
{
    char Arrfloat[3];
    char Arrfloat2[14];
    if(ui->groupBox_3->isChecked())
    {
        Arrfloat[0] = 'M';
        Arrfloat[1] = 'O';
        Arrfloat[2] = 'N';
        Array[0] = ui->spinBoxRUD->value();
        Array[1] = static_cast<float>(ui->doubleSpinBoxT0->value());
        Array[2] = static_cast<float>(ui->doubleSpinBoxP0->value());
    }
    else
    {
        Arrfloat[0] = 'M';
        Arrfloat[1] = 'O';
        Arrfloat[2] = 'F';
        Array[0] = 0;
        Array[1] = 0;
        Array[2] = 0;
    }

    ComPort->FormToTX(3, Arrfloat2, Array);
    ArrayTx[0]=Arrfloat2[0];
    ArrayTx[1]=Arrfloat2[1];
    for(int i=2;i<17;i++)
    {
        ArrayTx[i+3]=Arrfloat2[i];
    }
    for(int i=0; i<3;i++)
    {
        ArrayTx[i+2]=Arrfloat[i];
    }

    ComPort->SerialPort->write(ArrayTx, 17);
}
void MainWindow::TimerEnable()
{
//    CheckingPanelSimulation(TransmitedParametrsController, TransmittedDataController);
    if(ReceivedDataController[0]=='%' && ReceivedDataController[1]=='#')
    {

    }
    else
    {
        ErrorConection++;
        if(ErrorConection>500)
        {
            MessageStringWrite(tr("Нет ответа от контроллера в течении %1 сек").arg(static_cast<int>(ErrorConection*0.2)));
            FlagWriteReadOnOff = 2;
            PricessingExchangeButton();
            ErrorConection=0;
        }
    }
}
void MainWindow::FillingArraysParametrsFromController()
{
    ArrayTime.push_back(ReceiveParametrsController[0]);
    ArrayN1.push_back(ReceiveParametrsController[1]);
    ArrayN1zad.push_back(ReceiveParametrsController[2]);
    ArrayN2.push_back(ReceiveParametrsController[3]);
    ArrayN2zad.push_back(ReceiveParametrsController[4]);
    ArrayP2.push_back(ReceiveParametrsController[5]);
    ArrayP2zad.push_back(ReceiveParametrsController[6]);
    ArrayT4.push_back(ReceiveParametrsController[7]);
    ArrayT4zad.push_back(ReceiveParametrsController[8]);
    ArrayT3.push_back(ReceiveParametrsController[9]);
    ArrayT3zad.push_back(ReceiveParametrsController[10]);
    ArrayR.push_back(ReceiveParametrsController[11]);
    ArrayRzad.push_back(ReceiveParametrsController[12]);
    ArraydKy.push_back(ReceiveParametrsController[13]);
    ArraydKyzad.push_back(ReceiveParametrsController[14]);
    ArrayAL.push_back(ReceiveParametrsController[15]);
    ArrayALzad.push_back(ReceiveParametrsController[16]);
    ArrayGt.push_back(ReceiveParametrsController[17]);
    ArrayKOT3.push_back(ReceiveParametrsController[18]);
    ArrayKOT5.push_back(ReceiveParametrsController[19]);
    ArrayRUD.push_back(ReceiveParametrsController[20]);

}

void MainWindow::OpenDialogComPort()
{
    ComPort->SettingPortName();
    ComPort->show();
}
void MainWindow::OpenGrapgWindowASeparateThread()//удалить потом
{
    std::thread Thread1(&MainWindow::OpenGraphWindow, this);
    Thread1.join();
}
void MainWindow::OpenGraphWindow()
{
    if(!(OpenFileGraph->StaticGraph->series().isEmpty()))
    {
        OpenFileGraph->SeriesStaticGraphParam->clear();
        OpenFileGraph->SeriesStaticGraphParamZad->clear();
        ArrayGraph.clear();
    }
    if(OpenFileGraph->OpenFileRead(ArrayGraph))
    {
        OpenFileGraph->CopyArray(ArrayGraph);
        OpenFileGraph->show();
    }
    else
    {
        QMessageBox::about(this,"Ошибка", "Не удалось отобразить график, так как не удалось открыть файл");
    }

}
void MainWindow::WriteToFileOnOff()
{
    if(FlagWriteFile == false)
    {
        MessageStringWrite("Запись в файл начата");
        ui->ButWriteOnOff->setText("Выкл. запись");//
        FlagWriteFile = true;
    }
    else
    {
        if(!(ui->EnableSavingByDate->isChecked()))
        {
            MessageString = FileSaveClass->OpenAndWriteFile(ArrayWrTime, ArrayWrN1, ArrayWrN1zad, ArrayWrN2, ArrayWrN2zad, ArrayWrP2, ArrayWrP2zad, ArrayWrT3, ArrayWrT3zad, ArrayWrT4, ArrayWrT4zad,
                                                            ArrayWrR, ArrayWrRzad, ArrayWrdKy, ArrayWrdKyzad, ArrayWrAL, ArrayWrGt, ArrayWrRUD);
        }
        else
        {
            MessageString = FileSaveClass->OpenAndWriteFileByDate(ArrayWrTime, ArrayWrN1, ArrayWrN1zad, ArrayWrN2, ArrayWrN2zad, ArrayWrP2, ArrayWrP2zad, ArrayWrT3, ArrayWrT3zad, ArrayWrT4, ArrayWrT4zad,
                                                                  ArrayWrR, ArrayWrRzad, ArrayWrdKy, ArrayWrdKyzad, ArrayWrAL, ArrayWrGt, ArrayWrRUD);
        }
        ClearWriteArray();
        MessageStringWrite(MessageString);
        ui->ButWriteOnOff->setText("Вкл. запись");
        FlagWriteFile = false;
    }
}
void MainWindow::OpenSaveWindow()
{
    copy(ArrayTime.begin(), ArrayTime.end(), std::back_inserter(FileSaveClass->ArrayTime));
    copy(ArrayN1.begin(), ArrayN1.end(), std::back_inserter(FileSaveClass->ArrayN1));
    copy(ArrayN1zad.begin(), ArrayN1zad.end(), std::back_inserter(FileSaveClass->ArrayN1zad));
    copy(ArrayN2.begin(), ArrayN2.end(), std::back_inserter(FileSaveClass->ArrayN2));
    copy(ArrayN2zad.begin(), ArrayN2zad.end(), std::back_inserter(FileSaveClass->ArrayN2zad));
    copy(ArrayT3.begin(), ArrayT3.end(), std::back_inserter(FileSaveClass->ArrayT3));
    copy(ArrayT3zad.begin(), ArrayT3zad.end(), std::back_inserter(FileSaveClass->ArrayT3zad));
    copy(ArrayT4.begin(), ArrayT4.end(), std::back_inserter(FileSaveClass->ArrayT4));
    copy(ArrayT4zad.begin(), ArrayT4zad.end(), std::back_inserter(FileSaveClass->ArrayT4zad));
    copy(ArrayP2.begin(), ArrayP2.end(), std::back_inserter(FileSaveClass->ArrayP2));
    copy(ArrayP2zad.begin(), ArrayP2zad.end(), std::back_inserter(FileSaveClass->ArrayP2zad));

    copy(ArrayR.begin(), ArrayR.end(), std::back_inserter(FileSaveClass->ArrayR));
    copy(ArrayRzad.begin(), ArrayRzad.end(), std::back_inserter(FileSaveClass->ArrayRzad));
    copy(ArraydKy.begin(), ArraydKy.end(), std::back_inserter(FileSaveClass->ArraydKy));
    copy(ArraydKyzad.begin(), ArraydKyzad.end(), std::back_inserter(FileSaveClass->ArraydKyzad));
    copy(ArrayAL.begin(), ArrayAL.end(), std::back_inserter(FileSaveClass->ArrayAL));
    copy(ArrayALzad.begin(), ArrayALzad.end(), std::back_inserter(FileSaveClass->ArrayALzad));
    copy(ArrayKOT3.begin(), ArrayKOT3.end(), std::back_inserter(FileSaveClass->ArrayKOT3));
    copy(ArrayKOT5.begin(), ArrayKOT5.end(), std::back_inserter(FileSaveClass->ArrayKOT5));
    copy(ArrayRUD.begin(), ArrayRUD.end(), std::back_inserter(FileSaveClass->ArrayRUD));
    copy(ArrayGt.begin(), ArrayGt.end(), std::back_inserter(FileSaveClass->ArrayGt));

    FileSaveClass->show();
}
void MainWindow::GraphicSeriesCreate()
{
    if(ArrayTime.size()>1)
    {
        switch (ui->BoxSetParametrToPlot->currentIndex())
        {
        case 0:
            CreateSeriesParametra(1, ArrayTime, ArrayN1, ArrayN1zad, TimeOfTimer);
            break;
        case 1:
            CreateSeriesParametra(2, ArrayTime, ArrayN2, ArrayN2zad, TimeOfTimer);
            break;
        case 2:
            CreateSeriesParametra(3, ArrayTime, ArrayT3, ArrayT3zad, TimeOfTimer);
            break;
        case 3:
            CreateSeriesParametra(4, ArrayTime, ArrayT4, ArrayT4zad, TimeOfTimer);
            break;
        case 4:
            CreateSeriesParametra(5, ArrayTime, ArrayP2, ArrayP2zad, TimeOfTimer);
        break;
        default:
            MessageStringWrite("Параметр не доступен");
            break;
        }
    }
}
void MainWindow::CreateSeriesParametra(int StatusRadioButtonFun, std::vector<float>ArrayArgumentOX, std::vector<float>ArrayArgumentOY, std::vector<float>ArrayArgumentOYzad, float time)
{
    GraphicEng->CreateRangeAxis(ArrayArgumentOY);
    GraphicEng->AxisY->setTitleText(ui->BoxSetParametrToPlot->currentText());
    if(StatusRadioButton == StatusRadioButtonFun)
    {
        GraphicEng->CreateSeriesParametr(GraphicEng->SeriesPlotEngParam, ArrayArgumentOX, ArrayArgumentOY);
        GraphicEng->CreateSeriesParametr(GraphicEng->SeriesPlotEngParamZad, ArrayArgumentOX, ArrayArgumentOYzad);
    }
    else
    {
        GraphicEng->SeriesPlotEngParam->clear();
        GraphicEng->SeriesPlotEngParamZad->clear();
        GraphicEng->CreateSeriesParametr(GraphicEng->SeriesPlotEngParam, ArrayArgumentOX, ArrayArgumentOY);
        GraphicEng->CreateSeriesParametr(GraphicEng->SeriesPlotEngParamZad, ArrayArgumentOX, ArrayArgumentOYzad);
        StatusRadioButton = StatusRadioButtonFun;
    }
    time=0;
}
void MainWindow::TimerGraphEnable()
{

    if(ui->groupBox_3->isChecked())
    {
        if(!flagSimulation)
        {
            MessageStringWrite("Моделирование включено");
            flagSimulation = true;
        }

        if(FlagWriteFile==true && ArrayTime.size()>0)
        {
            if(ArrayTime.back()>=0.0)
            {
                WriteArrayToArray();
            }
        }
    }
    else
    {
        if(flagSimulation)
        {
            MessageStringWrite("Моделирование выключено");
            flagSimulation = false;
        }
    }
}

void MainWindow:: WriteArrayToArray()
{
        ArrayWrTime.push_back(ArrayTime.back());
        ArrayWrN1.push_back(ArrayN1.back());
        ArrayWrN1zad.push_back(ArrayN1zad.back());
        ArrayWrN2.push_back(ArrayN2.back());
        ArrayWrN2zad.push_back(ArrayN2zad.back());
        ArrayWrT3.push_back(ArrayT3.back());
        ArrayWrT3zad.push_back(ArrayT3zad.back());
        ArrayWrT4.push_back(ArrayT4.back());
        ArrayWrT4zad.push_back(ArrayT4zad.back());
        ArrayWrP2.push_back(ArrayP2.back());
        ArrayWrP2zad.push_back(ArrayP2zad.back());
        ArrayWrR.push_back(ArrayR.back());
        ArrayWrRzad.push_back(ArrayRzad.back());
        ArrayWrdKy.push_back(ArraydKy.back());
        ArrayWrdKyzad.push_back(ArraydKyzad.back());
        ArrayWrAL.push_back(ArrayAL.back());
        ArrayWrALzad.push_back(ArrayALzad.back());
        ArrayWrGt.push_back(ArrayGt.back());
        ArrayWrRUD.push_back(ArrayRUD.back());
        ArrayWrKOT3.push_back(ArrayKOT3.back());
        ArrayWrKOT5.push_back(ArrayKOT5.back());
}
void MainWindow::ClearWriteArray()
{
    ArrayWrN1.clear();
    ArrayWrN1zad.clear();
    ArrayWrN2.clear();
    ArrayWrN2zad.clear();
    ArrayWrT3.clear();
    ArrayWrT3zad.clear();
    ArrayWrT4.clear();
    ArrayWrT4zad.clear();
    ArrayWrP2.clear();
    ArrayWrP2zad.clear();
    ArrayWrTime.clear();
    ArrayWrR.clear();
    ArrayWrRzad.clear();
    ArrayWrAL.clear();
    ArrayWrALzad.clear();
    ArrayWrKOT3.clear();
    ArrayWrKOT5.clear();
    ArrayWrdKy.clear();
    ArrayWrdKyzad.clear();
    ArrayWrRUD.clear();
    ArrayWrGt.clear();
}
void MainWindow::ClearArrayParamRealTime()
{
    ArrayN1.clear();
    ArrayN1zad.clear();
    ArrayN2.clear();
    ArrayN2zad.clear();
    ArrayT3.clear();
    ArrayT3zad.clear();
    ArrayT4.clear();
    ArrayT4zad.clear();
    ArrayP2.clear();
    ArrayP2zad.clear();
    ArrayTime.clear();
    ArrayR.clear();
    ArrayRzad.clear();
    ArrayAL.clear();
    ArrayALzad.clear();
    ArrayKOT3.clear();
    ArrayKOT5.clear();
    ArraydKy.clear();
    ArraydKyzad.clear();
    ArrayRUD.clear();
    ArrayGt.clear();
}
void MainWindow::Modeling()
{
    for (int iter=0; iter<10; iter++)
    {
//        EngineAI25->eng(ArrayParamEngineIn, ArrayParamEngineOut);
    }
}
void MainWindow::StatusPort()
{
   MessageStringWrite(ComPort->StrLabComParametr1);
}
void MainWindow::MessageStringWrite(QString message)
{
    ui->ListMessage->addItem(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->ListMessage->addItem(message);
    ui->ListMessage->scrollToBottom();
}
MainWindow::~MainWindow()
{
    delete ui;
}

