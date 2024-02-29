#ifndef HISTOGRAMCHARTDISPLAY_H
#define HISTOGRAMCHARTDISPLAY_H

#include <QWidget>
#include <ui_chartdisplay.h>
#include <ui_histogram_paint.h>
#include <QMap>
#include <QCheckBox>
#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QBarSet>
#include <vector>
#include <QModelIndexList>
#include <QStandardItemModel>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <QSplineSeries>

class HistogramChartDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramChartDisplay(std::tuple<float,float> avgVar, QStandardItemModel* model, QModelIndexList* selectedColumns, QWidget *parent = nullptr);
    void paintImage();
    Ui::chartDisplay *ui;
    Ui::histogram_paint *ui_paint;
private:
    std::vector<float> datas;
    const QStandardItemModel* model;
    const QModelIndexList* selectedColumns;
    QChartView* chartView;
    QCheckBox* drawLineCheckbox;
    int barCount = 7;//bar最多的数量
    float avg = 0;
    float var = 0;
    bool drawLine = false;
    inline float density(float mean){return exp(-(avg - mean) * (avg - mean) / (2 * sqrt(var))) / sqrt(2 * M_PI )/var;}
private slots:
    void barCountChanged(int);
    void drawLineChanged(bool);
signals:

};


#endif // HISTOGRAMCHARTDISPLAY_H
