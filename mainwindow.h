#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <model_engine.h>
#include <plotparametreng.h>
#include "savefileclass.h"
#include <opengraphfile.h>
#include <dialogcomport.h>

#include <QTimer>
#include <vector>
#include <iostream>
#include <QMessageBox>
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    float ArrayParamEngineIn[12] = {1.0332,288.15,244.0/3600.0,-15.0,0.1,
                                0.1,3650.2,12509.1,1.9993,666.419};
    float ArrayParamEngineOut[12];

    float GT = 0.05f, ErrorGT, GTReg;
    int j3=2;

    Model_Engine *EngineAI25 = new Model_Engine();
    PlotParametrEng *GraphicEng = new PlotParametrEng();
    SaveFileClass *FileSaveClass = new SaveFileClass();
    OpenGraphFile *OpenFileGraph = new OpenGraphFile();
    DialogCOMport *ComPort = new DialogCOMport();

    QTimer *timer = new QTimer(this);
    QTimer *TimerGraphic = new QTimer(this);



    float TimeOfTimer = 0;
    int StatusRadioButton = 0;

    std::vector<float> ArrayN1, ArrayN1zad, ArrayN2, ArrayN2zad, ArrayTime;
    std::vector<float> ArrayT3, ArrayT3zad, ArrayT4, ArrayT4zad, ArrayP2, ArrayP2zad;
    std::vector<float> ArrayR, ArrayRzad, ArraydKy, ArraydKyzad, ArrayGt, ArrayAL, ArrayALzad, ArrayKOT3, ArrayKOT5, ArrayRUD, ArrayT0, ArrayP0;

    std::vector<float> ArrayWrN1, ArrayWrN1zad, ArrayWrN2, ArrayWrN2zad, ArrayWrTime;
    std::vector<float> ArrayWrT3, ArrayWrT3zad, ArrayWrT4, ArrayWrT4zad, ArrayWrP2, ArrayWrP2zad;
    std::vector<float> ArrayWrR, ArrayWrRzad, ArrayWrdKy, ArrayWrdKyzad, ArrayWrGt, ArrayWrAL, ArrayWrALzad, ArrayWrKOT3, ArrayWrKOT5, ArrayWrRUD, ArrayWrT0, ArrayWrP0;

    std::vector<float> ArrayGraph;
    bool StatusWriteButton = false; //флаг старта записис в файл
    bool FlagWriteFile = false; //флаг старта записис в файл
    bool flagSimulation = false;
    bool FlagVereficationPassed = false;
    int FlagWriteReadOnOff = 1;
    int FlagWriteMessage = 0;
    QString MessageString;
    uint16_t ErrorConection=0;

    /***/
    QByteArray TestWordIn;
    QByteArray ReceibeByteArray;
    char ReceiveChar[3];
    qint64 gt;
    char TestWordOut[3] = {'%', '%', '%'};
    static const int SizeRx = 94;
    char ReceivedDataController[SizeRx], TransmittedDataController[17];

    float ReceiveParametrsController[23];
    /*Параметры, принимаемые с контроллера [0]-time, [1]-n1, [2]-n1z, [3]-n2, [4]-n2z, [5]-p2, [6]-p2z,
        [7]-t3, [8]-t3z, [9]-t4, [10]-t4z, [11]-R, [12]-Rz, [13]-dKy, [14]-Gt, [15]-al, [16]-kot3, [17]-kot5, [18]-rud, [19]-t0, [20]-p0*/

    float TransmitedParametrsController[3];
    /*Параметры, передаваемые контроллеру [0]-rud, [1]-t0, [2]-p0 */


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
        uint8_t sdata[4];

    /***/

    void TimerEnable();
    void TimerGraphEnable();
    void Modeling();

    void GraphicSeriesCreate();
    void WriteToFileOnOff();
    void WriteArrayToArray();
    void ClearWriteArray();
    void ClearArrayParamRealTime();
    void ClearGraphic();
    void OpenSaveWindow();
    void OpenGraphWindow();
    void OpenGrapgWindowASeparateThread();
    void OpenDialogComPort();
    void FillingArraysParametrsFromController();
    void ClearListMessage();
    void MessageStringWrite(QString message);
    void StatusPort();
    void PricessingExchangeButton();
    void SerialPortReceive();
    void CheckingPanelSimulation(float Array[], char ArrayTx[]);
    void CreateSeriesParametra(int StatusRadioButtonFun, std::vector<float>ArrayArgumentOX,
                               std::vector<float>ArrayArgumentOY, std::vector<float>ArrayArgumentOYzad, float time);

    void FormToTX(int np, char TxBuffer[],float paramOut[]);
    void FormToRX(int np,char RxBuffer[],float paramIn[]);

};
#endif // MAINWINDOW_H
