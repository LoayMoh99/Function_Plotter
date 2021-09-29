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
        //remove spaces:
        if(equ[i].isSpace()){
            continue;
        }
        else{
            newEqu+=equ[i];
        }
        //detect if there is power operator:
        if(equ[i] == '^'){
            if(i+1<equ.size() && equ[i+1].isDigit()){
                QString chr = equ[i+1].toLower();
                int ord = chr.toInt();
                //preserve largest order(power)
                if(ord>order)
                    order=ord;
            }
            else{
                //Wrong Input : Can't get the order of the Function!!
                msgBox.critical(0,"Error","Wrong Input : Can't get the order of the Function!!");
            }
        }
        else if(equ[i]=='x' || equ[i]=='X'){
            //for 1st order equations..
            if(order==0)
                order=1;
        }
    }

    return newEqu;//return equation free of spaces..
}

double MainWindow::func_x(double x)
{
    double fx=1;//to store accumulated value of fx

    for(int i =0;i<operations.size();++i){
        if(operations[i]=="+" || operations[i] =="-"){
            if(i+1>=operations.size()){
                //Wrong Input : More +/- operations!!
                msgBox.critical(0,"Error","Wrong Input : More (+ or -) operations!!");
            }
            i++;
            double fx2 = 1;
            int ord = operations[i].toInt();
            //to see if power of this order is negative of not (i.e. num/x^num)
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
            fx2 *= equOrders[ord]; //multiply by the index of each order

            //determine wheater to add or subtract fx2 to fx
            if(operations[i-1]=="+"){
                fx+=fx2;
            }
            else { // "-"
                fx-=fx2;
            }
        }
        else {
            //number of specific order
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

void MainWindow::checkOrderExistence(double currIndex,QString orderIndex )
{
    if(equOrders[orderIndex.toInt()]>0){
        QString lastOper = operations[operations.size()-1];
        operations.removeLast();
        if(lastOper=="+")
             equOrders[orderIndex.toInt()] += currIndex;
        else
            equOrders[orderIndex.toInt()] -= currIndex;
    }
    else{
        operations.append(orderIndex);
        equOrders[orderIndex.toInt()] = currIndex;
    }
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
        if(equStr[i].isLetter() && equStr[i]!='x' && equStr[i]!='X'){
            msgBox.critical(0,"Error","Wrong Input : There is letters other than x!!");
            return false;
        }
        else if(equStr[i].isNumber()){
            //if two numbers or more after each others:
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

                       i++;
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
               QString ord = equStr[i+1].toLower();
               checkOrderExistence(currIndex,ord);
               currIndex=0;
               i++;
            }
            else { //order =1
                checkOrderExistence(currIndex,"1");
                currIndex=0;

            }
        }
        else if(equStr[i]=='+'||equStr[i]=='-'){
            isPrevNum=false;
            if(i+1<equStr.size()&& (equStr[i+1].isNumber() || equStr[i+1] == 'x' || equStr[i+1] == 'X')){
                if(currIndex!=0){
                    checkOrderExistence(currIndex,"0");
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
        checkOrderExistence(currIndex,"0");
    }
    cout << "order = " <<to_string(order) <<endl;
    for(int i =0;i<order+1;++i){
        cout << "index of order("<<to_string(i) << ") = " <<to_string(equOrders[i]) <<endl;
    }
    return true;
}


//validate user inputs and equation::
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


//set the ranges of both axes and rescale them:
void MainWindow::setRanges(double min_y, double max_y)
{
    ui->plot->xAxis->setRange(ui->min_x->text().toDouble(),ui->max_x->text().toDouble());
    ui->plot->yAxis->setRange(min_y,max_y);
}


//update data vectors to be added to graph::
void MainWindow::addData()
{
    //to store the min and max value of Y:
    double min_y =DOUBLE_MAX_VALUE , max_y=DOUBLE_MIN_VALUE ;
    double fx = 0, x =ui->min_x->text().toDouble();
    double step = (ui->max_x->text().toDouble()-ui->min_x->text().toDouble())/100;
    for(int i = 0;i<101;i++){
        qv_x.append(x+step*i);
        fx=func_x(qv_x[i]);//calculate fx
        qv_y.append(fx);

        //set min y and max y:
        if(fx<min_y)min_y=fx;
        if(fx>max_y)max_y=fx;
    }
    //set y ranges for constants:
    if(min_y == max_y){
        min_y=min_y/2;
        max_y+=min_y;
    }

    setRanges(min_y,max_y);//set ranges of axes
}


//clear all the vectors:
void MainWindow::clearData()
{
    qv_x.clear();
    qv_y.clear();
    operations.clear();
    equOrders.clear();
}


//update the plot on graph by new data vectors
void MainWindow::plot()
{
    ui->plot->graph(0)->setData(qv_x,qv_y);
    ui->plot->replot();
    ui->plot->update();
}

//draw button listner::
void MainWindow::on_btn_draw_clicked()
{
    clearData();
    if(validateInput()){
        addData();
        plot();
    }
}

//clear button listner::
void MainWindow::on_btn_clear_clicked()
{
    clearData();
    plot();
}

