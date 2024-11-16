#include "opengraphfile.h"
#include "ui_opengraphfile.h"

#include "fstream"
#include "iostream"

#define N1g  1U
#define N2g  3U
#define P2g  5U
#define T3g  7U
#define T4g  9U
#define Rg  11U
#define dKyg  13U

#define ALg  15U
#define Gtg  16U
#define Rudg  17U

OpenGraphFile::OpenGraphFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenGraphFile)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon("E:\\soft_QT\\ModelAI25\\images\\display.png"));
    this->setWindowTitle("Графики");

    CreateGraphic();
    connect(ui->ButShow, &QPushButton :: clicked, this,  &OpenGraphFile::ShowGraphicParametr);
}

OpenGraphFile::~OpenGraphFile()
{
    delete ui;
}
void OpenGraphFile::CopyArray(std::vector<float> &ArrayPoint)
{
    arrayPlot.clear();
    copy(ArrayPoint.begin(), ArrayPoint.end(), std::back_inserter(arrayPlot));
}
void OpenGraphFile::ShowGraphicParametr()
{
    QString str = ui->ComboBoxParametrEng->currentText();

    if(str == "Частота вращения ротора КНД")
    {
        UpDateSeries(arrayPlot, N1g, RangeMinMax);
        CreateAxis(RangeMinMax, str);
    }
    else if(str == "Частота вращения ротора КВД")
    {
        UpDateSeries(arrayPlot, N2g, RangeMinMax);
        CreateAxis(RangeMinMax, str);
    }
    else if(str == "Давление за КВД")
    {
        UpDateSeries(arrayPlot, P2g, RangeMinMax);
        CreateAxis(RangeMinMax, str);
    }
    else if(str == "Температура газа за КС")
    {
        UpDateSeries(arrayPlot, T3g, RangeMinMax);
        CreateAxis(RangeMinMax, str);
    }
    else if(str == "Температура газа за ТНД")
    {
        UpDateSeries(arrayPlot, T4g, RangeMinMax);
        CreateAxis(RangeMinMax, str);
    }
    else if(str == "Тяга")
    {
        UpDateSeries(arrayPlot, Rg, RangeMinMax);
        CreateAxis(RangeMinMax, str);
    }
    else if(str == "Газодинамическая устойчивость")
    {
        UpDateSeries(arrayPlot, dKyg, RangeMinMax);
        CreateAxis(RangeMinMax, str);
    }
}
void OpenGraphFile::UpDateSeries(std::vector<float> &ArrayPoint, int NumberSeries, float range[])
{
    if(!(StaticGraph->series().isEmpty()))
    {
        SeriesStaticGraphParam->clear();
        SeriesStaticGraphParamZad->clear();
    }
    int j=0;
    int maxj=0, minj=0;
    int maxtime=0, mintime=0;
    int NumbersOfParametrs = 18; //число записываемых в файл параметров МЕНЯЕТСЯ ВРУЧНУЮ
    minj = ArrayPoint[NumberSeries];
    mintime = ArrayPoint[0];
    for(uint i=0; i<(ArrayPoint.size()/NumbersOfParametrs); i++, j+=NumbersOfParametrs)
    {
        if(minj>ArrayPoint[j+NumberSeries])
        {
            minj = ArrayPoint[j+NumberSeries];
        }
        if(maxj<ArrayPoint[j+NumberSeries])
        {
            maxj = ArrayPoint[j+NumberSeries];
        }

        if(mintime>ArrayPoint[j])
        {
            mintime = ArrayPoint[j];
        }
        if(maxtime<ArrayPoint[j])
        {
            maxtime = ArrayPoint[j];
        }
        SeriesStaticGraphParam->append(ArrayPoint[j], ArrayPoint[j+NumberSeries]);
        SeriesStaticGraphParamZad->append(ArrayPoint[j], ArrayPoint[j+NumberSeries+1]);
    }
    range[1] = maxj;
    range[0] = minj;
    range[3] = maxtime;
    range[2] = mintime;
}
void OpenGraphFile::CreateAxis(float *setRange, QString TitlText)
{
    RangeMinOX = setRange[0]-setRange[0]*0.1;
    RangeMaxOX = setRange[1]+setRange[1]*0.1;
    AxisY->setRange(setRange[0]-setRange[0]*0.1+deltaY, setRange[1]+setRange[1]*0.1+deltaY);
    AxisX->setRange(setRange[2]+deltaX, setRange[2]+20.0f+deltaX);
    AxisY->setTitleText(TitlText);
}
void OpenGraphFile::CreateGraphic()
{
    AxisX->setLabelFormat("%0.01f");
    AxisX->setTitleText("Время, с");
    AxisX->setTickCount(21);
    StaticGraph->addAxis(AxisX, Qt::AlignBottom);
    AxisX->setGridLinePen(QPen(Qt::black,0.5, Qt::DashDotLine));

    AxisY->setLabelFormat("%.1f");
    AxisY->setTickCount(11);
    StaticGraph->addAxis(AxisY, Qt::AlignLeft);
    AxisY->setGridLinePen(QPen(Qt::black,0.5, Qt::DashDotLine));

    WidgetStaticGraph->setChart(StaticGraph);


    StaticGraph->addSeries(SeriesStaticGraphParamZad);
    StaticGraph->addSeries(SeriesStaticGraphParam);
    SeriesStaticGraphParamZad->attachAxis(AxisX);
    SeriesStaticGraphParam->attachAxis(AxisX);
    SeriesStaticGraphParamZad->attachAxis(AxisY);
    SeriesStaticGraphParam->attachAxis(AxisY);
    SeriesStaticGraphParamZad->setName("Заданные значения параметра");
    SeriesStaticGraphParam->setName("Действительные значения параметра");
    SeriesStaticGraphParam->setPen(QPen(Qt::blue, 4));
    SeriesStaticGraphParamZad->setPen(QPen(Qt::red, 4));
    ui->verticalLayout->addWidget(WidgetStaticGraph);
}

bool OpenGraphFile::OpenFileRead(std::vector<float> &array)
{
    NameOpenFile = QFileDialog::getOpenFileName(this, "Открыть","E:\\soft_QT\\ModelAI25\\Graphic","*.txt;;All files(*.*)");
    std::ifstream fin;
    fin.open(NameOpenFile.toUtf8().data());
    if(fin.is_open())
    {
        array.clear();
        std::copy(std::istream_iterator<float>(fin),std::istream_iterator<float>(), std::back_inserter(array));
        fin.close();
        this->close();
        return true;
    }
    else
    {
        QMessageBox::about(this,"Ошибка", "Не удалось открыть файл");
        this->close();
        return false;
    }
}
void OpenGraphFile::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Plus:
        StaticGraph->zoomIn();
        break;
    case Qt::Key_Minus:
        StaticGraph->zoomOut();
        break;
    case Qt::Key_6:
        if(event->modifiers() & Qt::ControlModifier)
        {
            deltaX+=1.0f;
            AxisX->setRange(RangeMinMax[2]+deltaX, RangeMinMax[2]+20.f+deltaX);
        }
        else if(event->modifiers() & Qt::AltModifier)
        {
            deltaX+=0.1f;
            AxisX->setRange(RangeMinMax[2]+deltaX, RangeMinMax[2]+20.f+deltaX);
        }
        else
        {
            deltaX+=5.0f;
            AxisX->setRange(RangeMinMax[2]+deltaX, RangeMinMax[2]+20.f+deltaX);
        }
        break;
    case Qt::Key_4:
        if(event->modifiers() & Qt::ControlModifier)
        {
            if(deltaX==0)
            {
                AxisX->setRange(RangeMinMax[2], RangeMinMax[2]+20.0f);
            }
            else
            {
                deltaX-=1.0f;
                AxisX->setRange(RangeMinMax[2]+deltaX, RangeMinMax[2]+20.f+deltaX);
            }
        }
        else if(event->modifiers() & Qt::AltModifier)
        {
            if(deltaX==0)
            {
                AxisX->setRange(RangeMinMax[2], RangeMinMax[2]+20.0f);
            }
            else
            {
                deltaX-=0.1f;
                AxisX->setRange(RangeMinMax[2]+deltaX, RangeMinMax[2]+20.f+deltaX);
            }
        }
        else
        {
            if(deltaX==0)
            {
                AxisX->setRange(RangeMinMax[2], RangeMinMax[2]+20.0f);
            }
            else
            {
                deltaX-=5.0f;
                AxisX->setRange(RangeMinMax[2]+deltaX, RangeMinMax[2]+20.f+deltaX);
            }
        }
        break;
    case Qt::Key_8:
        if(event->modifiers() & Qt::ControlModifier)
        {
            StaticGraph->scroll(0.0, 0.5);
        }
        else if(event->modifiers() & Qt::AltModifier)
        {
            StaticGraph->scroll(0.0, 100.0);
        }
        else
        {
            StaticGraph->scroll(0.0, 10.0);
        }

        break;
    case Qt::Key_2:
        if(event->modifiers() & Qt::ControlModifier)
        {
            StaticGraph->scroll(0.0, -0.5);
        }
        else if(event->modifiers() & Qt::AltModifier)
        {
            StaticGraph->scroll(0.0, -100.0);
        }
        else
        {
            StaticGraph->scroll(0.0, -10.0);
        }
        break;

    }

}
