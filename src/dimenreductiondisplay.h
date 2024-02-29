#ifndef DIMENREDUCTIONDISPLAY_H
#define DIMENREDUCTIONDISPLAY_H

#include <QWidget>
#include <ui_chartdisplay.h>
#include <ui_scatter2D_options.h>
#include <ui_dimen_option.h>
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
#include <QToolTip>
#include <Qt3DWindow>
#include <vector>
#include "MyChartViews.h"
class DimenReductionDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit DimenReductionDisplay(QStandardItemModel* model, QModelIndexList* selectedColumns, QWidget *parent = nullptr, int classlist = 1);
    void paintImage();
    void paint3DImage();
    Ui::chartDisplay *ui;
    Ui::DimenOption* ui_options;
private:
    std::vector<float> col1, col2;
    Eigen::MatrixXf lowDimenData;
    const QStandardItemModel* model;
    const QModelIndexList* selectedColumns;
    ChartView2D* chartView;
    QCheckBox* drawLineCheckbox;
    int classList =1;
    std::vector<QScatterSeries> graph;
    std::vector<QScatter3DSeries> graph3d;
    int degree = 3;//降维次数
private slots:
    void degreeChanged(int);
    void drawLineChanged(bool);
signals:

};

#endif // DIMENREDUCTIONDISPLAY_H
