#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QString>
#include <string>
#include <QLayout>
#include <QFile>
#include <QList>
#include <QStringList>
#include <QTableWidget>
#include <QTextStream>
#include <QStandardItemModel>
#include <QMessageBox>
#include "ui_page.h"
#include "histogramChartdisplay.h"
#include "common.h"
#include "Eigen\Dense"


class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QString& filepath, QWidget *parent = nullptr);
    ~MainPage();
    Ui::MainPage *ui;

private slots:
    void tableSelectionChanged();
    void computeAverageAndSquare();
    void drawHistogram();
    void drawMatrix();
    void drawScatter2D();
    void drawLowDimenChart();
    void drawClusterSChart();
    void cluster();
private:
    QFile* file;//对应的源数据文件
    QStandardItemModel* model;//表格模型
    int selectedColNumber = 0;//选中的列数
    QModelIndexList selectedColumns;//所有选中的列
    std::tuple<float,float> avgVar;
    int classCount = 2;
    int ClusterMethod = 1;
    std::vector<int> classList;
    int centreCount = 3, maxIter = 1000;


    inline float itemToFloat();//将非数字类型的数据转换为数字类型的数据




signals:

};


#endif // MAINPAGE_H
