#include "mainpage.h"
#include <vector>
#include <iostream>
#include "common.h"
using namespace std;
void MainPage::computeAverageAndSquare(){
    selectedColumns = ui->dataTable->selectionModel()->selectedColumns();// 获取选中的列数量
    selectedColNumber = selectedColumns.count();
    if(selectedColNumber <= 0){
        QMessageBox::information(this,"Warning","Please Select at least One Column!");
        return;
    }
    //转换为vector
    vector<float> colVector;
    int count = model->rowCount();
    for(int i = 1 ; i < count ; i++){
        colVector.push_back(model->item(i, selectedColumns.first().column())->data(Qt::DisplayRole).toFloat());
    }
    avgVar = getAvgVar(colVector);
    ui->averageDisplay->setText(QString::number(get<0>(avgVar)));
    ui->SquareDisplay->setText(QString::number(get<1>(avgVar)));
}
