#ifndef PLOTPARAMETRENG_H
#define PLOTPARAMETRENG_H

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QtCharts>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QTimer>
#include <QValueAxis>
#include <math.h>

using namespace QtCharts;

class PlotParametrEng : public QWidget
{
public:
    explicit PlotParametrEng(QWidget *parent = nullptr);

    QChart *PlotEngParam = new QChart();
    QChartView *WidgetPlotEngParam = new QChartView();
    QValueAxis *AxisX = new QValueAxis();
    QValueAxis *AxisY = new QValueAxis();
    QLineSeries *SeriesPlotEngParam = new QLineSeries();
    QLineSeries *SeriesPlotEngParamZad = new QLineSeries();

    void CreateGraphic();
    void CreateSeriesParametr(QLineSeries *series, std::vector<float>ArrayArgumentOX, std::vector<float>ArrayArgumentOY, float time);
    void CreateSeriesParametr(QLineSeries *series, std::vector<float>& ArrayArgumentOX, std::vector<float>& ArrayArgumentOY);
    void CreateGraphicSeries(QLineSeries *series, std::vector<float> &array1, std::vector<float> &array2, float time);
    void CreateRangeAxis(std::vector<float>ArrayArgument);

    float deltatime = 10.0f, deltaY = 0.0f, deltaX = 0.0f;

private:
    void keyPressEvent(QKeyEvent *event);
};

#endif // PLOTPARAMETRENG_H
