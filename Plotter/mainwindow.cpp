#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //add&configure the graph initially
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDot);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("f(x)");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRanges(double min_y, double max_y)
{
    ui->plot->xAxis->setRange(ui->min_x->text().toDouble(),ui->max_x->text().toDouble());
    ui->plot->yAxis->setRange(min_y,max_y);
    ui->plot->graph(0)->rescaleAxes();
}

void MainWindow::addData()
{
    double min_y =0 , max_y=0;
    for(int i = ui->min_x->text().toDouble();i<=ui->max_x->text().toDouble();i+=ui->step_x->text().toDouble()){
        qv_x.append(i);
        //qv_y.append(fx(i));
        qv_y.append(i);//as if f(x)=x
    }

    setRanges(min_y,max_y);
}

void MainWindow::clearData()
{
    qv_x.clear();
    qv_y.clear();
}

void MainWindow::plot()
{
    ui->plot->graph(0)->setData(qv_x,qv_y);
    ui->plot->replot();
    ui->plot->update();
}


void MainWindow::on_btn_draw_clicked()
{
    addData();
    plot();
}


void MainWindow::on_btn_clear_clicked()
{
    clearData();
    plot();
}

