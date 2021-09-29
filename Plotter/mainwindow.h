#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void setRanges(double min_y, double max_y);
    void addData();
    void clearData();
    void plot();

    //buttons clicked functions
    void on_btn_draw_clicked();
    void on_btn_clear_clicked();

private:
    Ui::MainWindow *ui;
    QVector<double> qv_x,qv_y; //vectors storing the x and y points
};
#endif // MAINWINDOW_H
