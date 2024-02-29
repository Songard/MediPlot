#include "mainpage.h"
#include "histogramChartdisplay.h"
#include "matrixdisplay.h"
#include "scattertwodisplay.h"
#include "dimenreductiondisplay.h"

using namespace std;
void MainPage::drawHistogram(){
    selectedColumns = ui->dataTable->selectionModel()->selectedColumns();// 获取选中的列数量
    selectedColNumber = selectedColumns.count();
    if(selectedColNumber <= 0){
        QMessageBox::information(this,"Warning","Please Select at least One Column!");
        return;
    }
    computeAverageAndSquare();
    HistogramChartDisplay* histo = new HistogramChartDisplay(avgVar,model, &selectedColumns, this);
    ui->ImageDisplay->addTab(histo, "histogram_col" + QString::number(1 + selectedColumns.first().column()));
    ui->ImageDisplay->setCurrentWidget(histo);
}

void MainPage::drawMatrix(){
    selectedColumns = ui->dataTable->selectionModel()->selectedColumns();// 获取选中的列数量
    selectedColNumber = selectedColumns.count();
    if(selectedColNumber <= 1){
        QMessageBox::information(this,"Warning","Please Select at least Two Columns!");
        return;
    }
    MatrixDisplay* matri = new MatrixDisplay(model, &selectedColumns, this);
    ui->ImageDisplay->addTab(matri, "Matrix of " + QString::number(selectedColNumber)+"columns");
    ui->ImageDisplay->setCurrentWidget(matri);

}

void MainPage::drawScatter2D(){
    selectedColumns = ui->dataTable->selectionModel()->selectedColumns();// 获取选中的列数量
    selectedColNumber = selectedColumns.count();
    if(selectedColNumber <= 1){
        QMessageBox::information(this,"Warning","Please Select at least Two Columns!");
        return;
    }
    computeAverageAndSquare();
    ScatterTwoDisplay* scatt = new ScatterTwoDisplay(avgVar,model, &selectedColumns, this);
    ui->ImageDisplay->addTab(scatt, "ScatterGram_col" + QString::number(1 + selectedColumns.first().column()) + "-" + QString::number(1 + selectedColumns.at(1).column()));
    ui->ImageDisplay->setCurrentWidget(scatt);
}

void MainPage::drawLowDimenChart(){
    selectedColumns = ui->dataTable->selectionModel()->selectedColumns();// 获取选中的列数量
    selectedColNumber = selectedColumns.count();
    if(selectedColNumber <= 1){
        QMessageBox::information(this,"Warning","Please Select at least Three Columns!");
        return;
    }
    DimenReductionDisplay* scatt = new DimenReductionDisplay(model, &selectedColumns, this);
    ui->ImageDisplay->addTab(scatt, "DimenReduced_from" + QString::number(selectedColNumber)+"columns");
    ui->ImageDisplay->setCurrentWidget(scatt);
    scatt->paint3DImage();
    DimenReductionDisplay* scatt1 = new DimenReductionDisplay(model, &selectedColumns, this);
    ui->ImageDisplay->addTab(scatt1, "DimenReduced_from" + QString::number(selectedColNumber)+"columns");
    ui->ImageDisplay->setCurrentWidget(scatt1);
    scatt1->paintImage();
}

void MainPage::drawClusterSChart(){
    selectedColumns = ui->dataTable->selectionModel()->selectedColumns();// 获取选中的列数量
    selectedColNumber = selectedColumns.count();
    if(selectedColNumber <= 1){
        QMessageBox::information(this,"Warning","Please Select at least Three Columns!");
        return;
    }
    DimenReductionDisplay* scatt = new DimenReductionDisplay(model, &selectedColumns, this, model->columnCount()-1);
    ui->ImageDisplay->addTab(scatt, "DimenReduced_from" + QString::number(selectedColNumber)+"columns");
    ui->ImageDisplay->setCurrentWidget(scatt);
    scatt->paintImage();

}
