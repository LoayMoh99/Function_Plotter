#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox> //used for error messages showing
#define DOUBLE_MAX_VALUE 1.79769e+308
#define DOUBLE_MIN_VALUE 2.22507e-308
//used for debugging only::
#include <string>
#include <iostream>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //helper functions
    QString  getOrderEqu(QString equ,int &order);//to get the order of equation
    double func_x(double x);//to calculate the fx value
    bool evaluteEquation();//to evalute equation -> convert string to func_x
    bool validateInput();//to validate equation and user inputs
    void setRanges(double min_y, double max_y);//set axes range
    void addData();//add data to be drawn in plot graph
    void clearData();//clear all data from plot graph
    void plot();//plot the data itself in the graph

    //buttons clicked functions (listner functions)
    void on_btn_draw_clicked();
    void on_btn_clear_clicked();

private:
    Ui::MainWindow *ui;
    QVector<double> qv_x,qv_y; //vectors storing the x and y points
    QVector<QString> operations; //holds order with operations inorder of execution
    QVector<double> equOrders; //holds the indecies of the equation orders
    QMessageBox msgBox; //used to show error messages
};
#endif // MAINWINDOW_H
