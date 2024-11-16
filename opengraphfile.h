#ifndef OPENGRAPHFILE_H
#define OPENGRAPHFILE_H

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QtCharts>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QTimer>
#include <QValueAxis>
#include <QFileDialog>

using namespace QtCharts;

namespace Ui {
class OpenGraphFile;
}

class OpenGraphFile : public QWidget
{
    Q_OBJECT

public:
    explicit OpenGraphFile(QWidget *parent = nullptr);
    ~OpenGraphFile();

    bool OpenFileRead(std::vector<float> &array);
    void CreateGraphic();
    void DeleteGraphic();
    void UpDateSeries(std::vector<float> &ArrayPoint, int NumberSeries, float range[]);
    void CreateAxis(float *setRange, QString TitlText);
    void ShowGraphicParametr();
    void CopyArray(std::vector<float> &ArrayPoint);

    QLineSeries *SeriesStaticGraphParam = new QLineSeries();
    QLineSeries *SeriesStaticGraphParamZad = new QLineSeries();
    QChart *StaticGraph = new QChart();
    QValueAxis *AxisY = new QValueAxis();
    bool StatusOpenFile;
private:
    Ui::OpenGraphFile *ui;

    FILE *FileReadBin = new FILE();
    QChartView *WidgetStaticGraph = new QChartView();
    QValueAxis *AxisX = new QValueAxis();


    std::vector<float> arrayPlot;
    QString NameOpenFile;
    std::string FileRead;

    void keyPressEvent(QKeyEvent *event);
    int iu;
    float deltaX=0, deltaY=0;
    float RangeMinMax[4];
    float RangeMinOX=0, RangeMaxOX=0;
};

#endif // OPENGRAPHFILE_H
