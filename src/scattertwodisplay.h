#ifndef SCATTERTWODISPLAY_H
#define SCATTERTWODISPLAY_H

#include <QWidget>
#include <ui_chartdisplay.h>
#include <ui_scatter2D_options.h>
#include <QMap>
#include <QCheckBox>
#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include "common.h"
#include <vector>
#include <QModelIndexList>
#include <QStandardItemModel>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QPoint>
#include <MyChartViews.h>

class ScatterTwoDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ScatterTwoDisplay(std::tuple<float,float> avgVar, QStandardItemModel* model, QModelIndexList* selectedColumns, QWidget *parent = nullptr);
    void paintImage();
    Ui::chartDisplay *ui;
    Ui::scatter2D_options *ui_option;
private:
    std::vector<float> col1,col2;
    const QStandardItemModel* model;
    const QModelIndexList* selectedColumns;
    ChartView* chartView;
    QCheckBox* drawLineCheckbox;
    int degree = 2;//拟合次数
    bool drawLine = false;
private slots:
    void degreeChanged(int);
    void drawLineChanged(bool);
signals:

};

#endif // SCATTERTWODISPLAY_H
