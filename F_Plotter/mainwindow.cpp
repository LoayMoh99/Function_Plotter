#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //add&configure the graph initially
    ui->plot->addGraph();
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("f(x)");

    //set QLineEdits to be Round Corners:
    ui->fx-> setStyleSheet("QLineEdit { background-color: rgb(255, 255, 255); border: 2px solid gray;""border-radius: 5px;}");
    ui->min_x-> setStyleSheet("QLineEdit { background-color: rgb(255, 255, 255); border: 2px solid gray;""border-radius: 5px;}");
    ui->max_x-> setStyleSheet("QLineEdit { background-color: rgb(255, 255, 255); border: 2px solid gray;""border-radius: 5px;}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getOrderEqu(QString equ,int &order)
{
    //int order =0;
    QString newEqu = ""; //newEqu without any spaces:
    for (int i=0;i<equ.size();++i) {
        if(equ[i].isSpace()){
            continue;
        }
        else{
            newEqu+=equ[i];
        }

        if(equ[i] == '^'){
            if(i+1<equ.size() && equ[i+1].isDigit()){
                QString chr = equ[i+1].toLower();
                int ord = chr.toInt();
                if(ord>order)
                    order=ord;
            }
            else{
                //Wrong Input : Can't get the order of the Function!!
                msgBox.critical(0,"Error","Wrong Input : Can't get the order of the Function!!");
            }
        }
        else if(equ[i]=='x' || equ[i]=='X'){ //for 1st order equations..
            if(order==0)
                order=1;
        }
    }

    return newEqu;
}

double MainWindow::func_x(double x)
{
    double fx=1;
    for(int i =0;i<operations.size();++i){
        if(operations[i]=="+" || operations[i] =="-"){
            if(i+1>=operations.size()){
                //Wrong Input : More +/- operations!!
                msgBox.critical(0,"Error","Wrong Input : More (+ or -) operations!!");
            }
            i++;
            double fx2 = 1;
            int ord = operations[i].toInt();
            bool negPower = false;
            if(ord<0) {
                negPower = true;
                ord = -1*ord;
            }
            for (int i=0;i<ord;++i) {
                if(negPower) {
                    if(x==0){
                        //Error : Divide by zero
                        msgBox.critical(0,"Division by Zero!!","x input range contains zero and there is division!!");
                    }
                    else
                        fx2*=1/x;
                }
                else fx2*=x;
            }
            fx2 *= equOrders[ord];

            if(operations[i-1]=="+"){
                fx+=fx2;
            }
            else { // "-"
                fx-=fx2;
            }
        }
        else {//number of specific order
            int ord = operations[i].toInt();
            bool negPower = false;
            if(ord<0) {
                negPower = true;
                ord = -1*ord;
            }

            for (int i=0;i<ord;++i) {
                if(negPower) {
                    if(x==0){
                        //Error : Divide by zero
                        msgBox.critical(0,"Division by Zero!!","x input range contains zero and there is division!!");
                    }
                    else
                        fx*=1/x;
                }
                else fx*=x;
            }
            fx *= equOrders[ord];
        }
    }
    return fx;
}

bool MainWindow::evaluteEquation()
{
    QString equStr = ui->fx->text();
    int order = 0;
    equStr=getOrderEqu(equStr,order);//return newEqu without spaces
    bool isPrevNum = false;

    equOrders.resize(order+1);//indecies of equation's orders ; initially=0

    double currIndex =0;
    for (int i=0;i<equStr.size();++i) {
        if(equStr[i].isSpace()){
            continue;
        }
        else if(equStr[i].isLetter() && equStr[i]!='x' && equStr[i]!='X'){
            msgBox.critical(0,"Error","Wrong Input : There is letters other than x!!");
            return false;
        }
        else if(equStr[i].isNumber()){
            if(isPrevNum){
                currIndex*=10;
            }
            QString chr = equStr[i].toLower();
            currIndex+=chr.toDouble();
            isPrevNum=true;
        }
        else if(equStr[i] == '*' || equStr[i] == '/' ){
            isPrevNum=false;
            if(i+1<equStr.size()){
                if(equStr[i+1].isDigit()){
                    QString chr = equStr[i+1].toLower();
                    if(equStr[i]=='*')
                        currIndex*=chr.toDouble();
                    else
                        currIndex/=chr.toDouble();

                    i++;
                }
                else if(equStr[i+1] == 'x' || equStr[i+1] == 'X'){
                    i++;
                    if(currIndex==0) currIndex=1;
                    if(i+1<equStr.size() && equStr[i+1] == '^'){
                       i++;
                       QString chr = equStr[i+1].toLower();
                       if(equOrders[chr.toInt()]>0){
                           QString lastOper = operations[operations.size()-1];
                           operations.removeLast();
                           if(lastOper=="+")
                                equOrders[chr.toInt()] += currIndex;
                           else
                               equOrders[chr.toInt()] -= currIndex;
                       }
                       else{
                           if(equStr[i-2] == '*')
                               operations.append(chr);
                           else
                               operations.append('-'+chr);
                           equOrders[chr.toInt()] = currIndex;
                       }


                       currIndex=0;
                    }
                    else { //order =1
                        if(equOrders[1]>0){
                            QString lastOper = operations[operations.size()-1];
                            operations.removeLast();
                            if(lastOper=="+")
                                 equOrders[1] += currIndex;
                            else
                                equOrders[1] -= currIndex;
                        }
                        else{
                            if(equStr[i-1] == '*')
                                operations.append("1");
                            else
                                operations.append("-1");
                            equOrders[1] = currIndex;
                        }
                        currIndex=0;
                    }
                }
                else {
                    //Wrong Input : Unvalid input after (*) multiplication!!
                    msgBox.critical(0,"Error","Wrong Input : Unvalid input after (* or /)!!");
                    return false;
                }
            }
            else{
                //Wrong Input : Unvalid input after (*) multiplication!!
                msgBox.critical(0,"Error","Wrong Input : Unvalid input after (* or /)!!");
                return false;
            }
        }
        else if(equStr[i] == 'x' || equStr[i] == 'X'){
            isPrevNum=false;
            if(currIndex==0) currIndex=1;
            if(i+1<equStr.size() && equStr[i+1].isNumber()){
                msgBox.critical(0,"Error","Wrong Input : number after x directly!!");
                return false;
            }

            if(i+1<equStr.size() && equStr[i+1] == '^'){
               i++;
               QString chr = equStr[i+1].toLower();
               if(equOrders[chr.toInt()]>0){
                   QString lastOper = operations[operations.size()-1];
                   operations.removeLast();
                   if(lastOper=="+")
                        equOrders[chr.toInt()] += currIndex;
                   else
                       equOrders[chr.toInt()] -= currIndex;
               }
               else{
                   operations.append(chr);
                   equOrders[chr.toInt()] = currIndex;
               }
               currIndex=0;
               i++;
            }
            else { //order =1
                if(equOrders[1]>0){
                    QString lastOper = operations[operations.size()-1];
                    operations.removeLast();
                    if(lastOper=="+")
                         equOrders[1] += currIndex;
                    else
                        equOrders[1] -= currIndex;
                }
                else{
                    operations.append("1");
                    equOrders[1] = currIndex;
                }
                currIndex=0;

            }
        }
        else if(equStr[i]=='+'||equStr[i]=='-'){
            isPrevNum=false;
            if(i+1<equStr.size()&& (equStr[i+1].isNumber() || equStr[i+1] == 'x' || equStr[i+1] == 'X')){
                if(currIndex!=0){
                    operations.append("0");
                    equOrders[0] = currIndex;
                    currIndex=0;
                }
                if(equStr[i]=='+'){
                    cout << "we do +" <<endl;
                    operations.append("+");
                }
                else {
                    cout << "we do -" <<endl;
                    operations.append("-");
                }
            }
            else{
                msgBox.critical(0,"Error","Wrong Input : Unvalid input after (+ or -)!!");
                return false;
            }
        }
    }
    if(currIndex!=0){
        operations.append("0");
        equOrders[0] = currIndex;
    }
    cout << "order = " <<to_string(order) <<endl;
    for(int i =0;i<order+1;++i){
        cout << "index of order("<<to_string(i) << ") = " <<to_string(equOrders[i]) <<endl;
    }
    return true;
}

bool MainWindow::validateInput()
{
    if(ui->min_x->text().isEmpty()||ui->min_x->text().isEmpty() ||ui->max_x->text().isEmpty()){
        //Empty fields
        msgBox.critical(0,"Error","There is Empty fields!!");
        return false;
    }
    if(ui->min_x->text().toDouble()>=ui->max_x->text().toDouble()){
        //Min Value of x can't be larger or equal to Max Value of x
        msgBox.critical(0,"Error","Min Value of x can't be larger or equal to Max Value of x!!");
        return false;
    }
    return evaluteEquation();
}

void MainWindow::setRanges(double min_y, double max_y)
{
    ui->plot->xAxis->setRange(ui->min_x->text().toDouble(),ui->max_x->text().toDouble());
    ui->plot->yAxis->setRange(min_y,max_y);
}

void MainWindow::addData()
{
    double min_y =9999 , max_y=-9999 ,fx = 0, x =ui->min_x->text().toDouble();
    double step = (ui->max_x->text().toDouble()-ui->min_x->text().toDouble())/100;
    cout << "operations size = " <<to_string(operations.size()) <<endl;
    for(int i = 0;i<101;i++){
        qv_x.append(x+step*i);
        fx=func_x(qv_x[i]);
        qv_y.append(fx);

        //set min y and max y:
        if(fx<min_y)min_y=fx;
        if(fx>max_y)max_y=fx;
    }

    setRanges(min_y,max_y);
}

void MainWindow::clearData()
{
    qv_x.clear();
    qv_y.clear();
    operations.clear();
    equOrders.clear();
}

void MainWindow::plot()
{
    ui->plot->graph(0)->setData(qv_x,qv_y);
    ui->plot->replot();
    ui->plot->update();
}


void MainWindow::on_btn_draw_clicked()
{
    clearData();
    if(validateInput()){
        addData();
        plot();
    }
}


void MainWindow::on_btn_clear_clicked()
{
    clearData();
    plot();
}

