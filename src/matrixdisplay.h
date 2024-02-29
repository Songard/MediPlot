#ifndef MATRIXDISPLAY_H
#define MATRIXDISPLAY_H

#include <QWidget>
#include <ui_chartdisplay.h>
#include <ui_matrixAndBarDisplay.h>
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
#include <QMessageBox>
#include "common.h"
class MatrixDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit MatrixDisplay(QStandardItemModel* model, QModelIndexList* selectedColumns, QWidget *parent = nullptr);
    Ui::chartDisplay *ui;
    Ui::MatrixColorBar *ui_paint;
    QStringList topicList;
    Eigen::MatrixXf covMatrix;
    Eigen::MatrixXf relMatrix;
private:
    const QStandardItemModel* model;
    const QModelIndexList* selectedColumns;
    QWidget* chartView;
    bool isCov = 1;
    int Hue = -1;

private slots:
    void paintMatrix(int);
    void matrixChanged(int);
    void styleChanged(bool);
signals:

};


#endif // MATRIXDISPLAY_H
