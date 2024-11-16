#include "plotparametreng.h"

PlotParametrEng::PlotParametrEng(QWidget *parent) : QWidget(parent)
{
    CreateGraphic();
}
void PlotParametrEng::CreateGraphic()
{

    PlotEngParam->addSeries(SeriesPlotEngParamZad);
    PlotEngParam->addSeries(SeriesPlotEngParam);

    AxisX->setLabelFormat("%.1f");
    AxisX->setTickCount(11);
    AxisX->setTitleText("Время, с");
    PlotEngParam->addAxis(AxisX, Qt::AlignBottom);
    SeriesPlotEngParam->attachAxis(AxisX);
    SeriesPlotEngParamZad->attachAxis(AxisX);

    SeriesPlotEngParamZad->setName("Заданные значения параметра");
    SeriesPlotEngParam->setName("Действительные значения параметра");
    SeriesPlotEngParam->setPen(QPen(Qt::blue, 2));
    SeriesPlotEngParamZad->setPen(QPen(Qt::red, 2));

    AxisY->setLabelFormat("%.1f");
    AxisY->setTickCount(11);
    PlotEngParam->addAxis(AxisY, Qt::AlignLeft);
    SeriesPlotEngParam->attachAxis(AxisY);
    SeriesPlotEngParamZad->attachAxis(AxisY);


    WidgetPlotEngParam->setDragMode(QGraphicsView::ScrollHandDrag);
    WidgetPlotEngParam->setMouseTracking(true);
    WidgetPlotEngParam->setChart(PlotEngParam);
}
void PlotParametrEng::CreateRangeAxis(std::vector<float>ArrayArgument)
{
    PlotEngParam->axisY()->setRange(ArrayArgument.back()-static_cast<int>(ArrayArgument.back()*0.8),ArrayArgument.back()+static_cast<int>(ArrayArgument.back()*0.8));
}

void PlotParametrEng::CreateSeriesParametr(QLineSeries *series, std::vector<float>& ArrayArgumentOX, std::vector<float>& ArrayArgumentOY)
{

    series->append(ArrayArgumentOX.back(), ArrayArgumentOY.back());

    if(static_cast<float>(ArrayArgumentOX.back())>10.0f)
    {
        series->remove(ArrayArgumentOX.front(), ArrayArgumentOY.front());
    }
    if(fmod(ArrayArgumentOX.back(), 120.0f) == 0)
    {
        ArrayArgumentOX.clear();
        ArrayArgumentOY.clear();
    }
    if(ArrayArgumentOX.back()<10.0f)
    {
       PlotEngParam->axisX()->setRange(0.0f,10.0);
    }
    else
    {
       PlotEngParam->axisX()->setRange(ArrayArgumentOX.back()-deltatime,ArrayArgumentOX.back());
    }

}

void PlotParametrEng::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Plus:
        PlotEngParam->zoomIn();
        break;
    case Qt::Key_Minus:
        PlotEngParam->zoomOut();
        break;
    case Qt::Key_6:
        PlotEngParam->scroll(10.0,0.0);
        break;
    case Qt::Key_4:
        PlotEngParam->scroll(-10.0,0.0);
        break;
    case Qt::Key_8:
        PlotEngParam->scroll(0.0,10.0);
        break;
    case Qt::Key_2:
        PlotEngParam->scroll(0.0,-10.0);
        break;
    }
}
