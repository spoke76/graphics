#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paintScene* scene = new paintScene(this);
    pmi= scene -> addPixmap(QPixmap());
    ui->graphicsView  ->setScene(scene);
    axisPen = QPen(QColor::fromRgb(0, 0, 0));
    axisPen.setWidth(3);
    girdPen = QPen(QColor::fromRgb(0, 175, 0));
    girdPen.setWidth(2);
    linePen = QPen(QColor::fromRgb(255, 0, 0));
    linePen.setWidth(4);
    h=ui->graphicsView->height();
    w=ui->graphicsView->width();
    Zero.rx()=w/2;
    Zero.ry()=h/2;
    ScX=w/100;
    ScY=w/100;
    ScaleX=1.5;
    ScaleY=1.5;

    connect(scene, &paintScene::mousePressEvent , this, &MainWindow::point);
}

void MainWindow::point(){
    QPoint POINT = QMainWindow::mapFromGlobal(QCursor::pos());
    POINT = POINT - ui->graphicsView->geometry().topLeft();
    addPoint(POINT);
    Render();
}

MainWindow::~MainWindow()
{
    delete pts;
    delete ui;
}


void MainWindow::addPoint(QPoint pt){
    if (len<n){
        pt-=Zero;
        pts[curPoint].rx=pt.rx()/ScX;
        pts[curPoint].ry=pt.ry()/-ScY;
        curPoint++;
        len++;
        ui->cbx->addItem(QString::number(curPoint) + " в.: " + QString::number(pts[curPoint-1].rx, 'f', 3)+" ; "+QString::number(pts[curPoint-1].ry, 'f', 3));
        ui->cbx->setCurrentIndex(curPoint-1);
    }
    else{
        QMessageBox msgb;
        msgb.setText("К сожалению програма не поддерживает больше чем " + QString::number(n) + " точек");
    }
}

void clear(QPixmap& image, const QColor& color = QColor::fromRgb(255, 255, 255)){
    QPainter p(&image);
    p.fillRect(image.rect(), color);
}

void MainWindow::drawAxis(QPixmap &image, const QPen &pn){
    QPainter p(&image);
    p.setPen(pn);
    p.drawLine(Zero.rx(), 0, Zero.rx(), h);
    p.drawLine(0, Zero.ry(), w, Zero.ry());

}

void MainWindow::drawGird(QPixmap &image, const QPen &pn){
    QPainter p(&image);

    for(int i=0; i<=w; i+=50){
        QPoint d;
        d.rx()=i;
        d.ry()=0;
        QPoint u;
        u.rx()=i;
        u.ry()=h;
        p.drawLine(d, u);
        u.ry()-=30;
        p.drawText(u, QString::number((i-Zero.x())/ScX, 'h', 2));
    }
    for(int i=0; i<=w; i+=50){
        QPoint d;
        d.ry()=i;
        d.rx()=0;
        QPoint u;
        u.ry()=i;
        u.rx()=w;
        p.drawLine(d, u);
        d.rx()+=10;
        p.drawText(d, QString::number((i-Zero.y())/-ScY, 'h', 2));
    }
    p.setPen(linePen);
    QPoint aim;
    aim.ry()=h/2;
    aim.rx()=w/2;
    p.drawText(w/2+20,h/2-4, QString::number((w/2-Zero.rx())/ScX, 'h', 3));
    p.drawText(w/2+4,h/2-20, QString::number(-1*(h/2-Zero.ry())/ScY, 'h', 3));
    p.drawEllipse(aim, 5, 1);
    p.drawEllipse(aim, 1, 5);

}

void MainWindow::drawPgn(QPixmap& image, const QPen& pn){
    QPainter p(&image);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(linePen);
    int i=0;
    for(i; i< len-1; i++){
        s_point last, cur;

        last.rx=pts[i].rx;
        cur.rx=pts[i+1].rx;
        last.ry=pts[i].ry;
        cur.ry=pts[i+1].ry;
        last.rx*=ScX;
        last.ry*=-ScY;
        last.rx+=Zero.rx();
        last.ry+=Zero.ry();
        cur.rx*=ScX;
        cur.ry*=-ScY;
        cur.ry+=Zero.ry();
        cur.rx+=Zero.rx();
        QPoint c, l;
        c.rx()=cur.rx;
        l.rx()=last.rx;
        c.ry()=cur.ry;
        l.ry()=last.ry;
        p.drawLine(c, l);

    }
    s_point last, cur;

    last.rx=pts[i].rx;
    cur.rx=pts[0].rx;
    last.ry=pts[i].ry;
    cur.ry=pts[0].ry;
    last.rx*=ScX;
    last.ry*=-ScY;
    last.rx+=Zero.rx();
    last.ry+=Zero.ry();
    cur.rx*=ScX;
    cur.ry*=-ScY;
    cur.ry+=Zero.ry();
    cur.rx+=Zero.rx();
    QPoint c, l;
    c.rx()=cur.rx;
    l.rx()=last.rx;
    c.ry()=cur.ry;
    l.ry()=last.ry;
    p.drawLine(c, l);

}

void MainWindow::Render(){
    QPixmap image(ui->graphicsView->rect().size());
    clear(image);
    pmi->setPixmap(image);
    drawAxis(image, axisPen);
    pmi->setPixmap(image);
    drawGird(image,girdPen);
    pmi->setPixmap(image);
    if (ST==enable){
        for(int i=0; i<len; i++){
            ui->cbx->setItemText(i, QString::number(i+1) + " в.: " + QString::number(pts[i].rx, 'f', 3)+" ; "+QString::number(pts[i].ry, 'f', 3));
        }
        ptC.rx()=pts[ui->cbx->currentIndex()].rx*ScX;
        ptC.ry()=pts[ui->cbx->currentIndex()].ry*-ScY;
        ptC+=Zero;
        drawPgn(image,girdPen);
        pmi->setPixmap(image);
        QPainter p(&image);
        p.setPen(girdPen);
        p.drawEllipse(ptC, 5, 5);
        pmi->setPixmap(image);
    }
}


void MainWindow::on_pbShow_clicked()
{
    ST=enable;
    Render();
}


void MainWindow::on_pbClear_clicked()
{
    ST=disable;
    Render();
}


void MainWindow::on_pbRotateLeft_clicked()
{
    double a=3.14/12;
    if (offsetmod==offset_vert){
    int CUR = ui->cbx->currentIndex();
    for(int i=0; i<len; i++){
        double rx = pts[i].rx;
        double ry = pts[i].ry;
        pts[i].rx=(rx-pts[CUR].rx)*cos(a)-sin(a)*(ry-pts[CUR].ry)+pts[CUR].rx;
        pts[i].ry=(rx-pts[CUR].rx)*sin(a)+cos(a)*(ry-pts[CUR].ry)+pts[CUR].ry;
    }
    }
    else{
        for(int i=0; i<len; i++){
        double rx = pts[i].rx;
        double ry = pts[i].ry;
        pts[i].rx=(rx)*cos(a)-sin(a)*(ry);
        pts[i].ry=(rx)*sin(a)+cos(a)*(ry);
        }
    }
    Render();

}


void MainWindow::on_pbRotateRight_clicked()
{
    double a=-3.14/12;
    if (offsetmod==offset_vert){
        int CUR = ui->cbx->currentIndex();
        for(int i=0; i<len; i++){
            double rx = pts[i].rx;
            double ry = pts[i].ry;
            pts[i].rx=(rx-pts[CUR].rx)*cos(a)-sin(a)*(ry-pts[CUR].ry)+pts[CUR].rx;
            pts[i].ry=(rx-pts[CUR].rx)*sin(a)+cos(a)*(ry-pts[CUR].ry)+pts[CUR].ry;
        }
    }
    else{
        for(int i=0; i<len; i++){
            double rx = pts[i].rx;
            double ry = pts[i].ry;
            pts[i].rx=(rx)*cos(a)-sin(a)*(ry);
            pts[i].ry=(rx)*sin(a)+cos(a)*(ry);
        }
    }
    Render();

}


void MainWindow::on_pbScXPlus_clicked()
{
    if(compmod == comp_vert){
        int CUR = ui->cbx->currentIndex();
        for(int i=0; i<len; i++){
            double rx = pts[i].rx;
            pts[i].rx=(rx-pts[CUR].rx)*ScaleX+pts[CUR].rx;
        }
    }
    else{
        for(int i=0; i<len; i++){
            double rx = pts[i].rx;
            pts[i].rx=(rx)*ScaleX;
    }
    }
    Render();
}


void MainWindow::on_pbScYPlus_clicked()
{
    if(compmod == comp_vert){

        int CUR = ui->cbx->currentIndex();
        for(int i=0; i<len; i++){
            double ry = pts[i].ry;
            pts[i].ry=(ry-pts[CUR].ry)*ScaleY+pts[CUR].ry;
    }
    }
    else{
        for(int i=0; i<len; i++){
            double ry = pts[i].ry;
            pts[i].ry=(ry)*ScaleY;
        }
    }
    Render();
}


void MainWindow::on_pbScXMinus_clicked()
{

    if(compmod == comp_vert){

        int CUR = ui->cbx->currentIndex();
        for(int i=0; i<len; i++){
            double rx = pts[i].rx;
            pts[i].rx=(rx-pts[CUR].rx)/ScaleX+pts[CUR].rx;
        }
    }
    else{
        for(int i=0; i<len; i++){
            double rx = pts[i].rx;
            pts[i].rx=(rx)/ScaleX;
        }
    }
    Render();
}


void MainWindow::on_pbScYMinus_clicked()
{
    if(compmod == comp_vert){

        int CUR = ui->cbx->currentIndex();
        for(int i=0; i<len; i++){
            double ry = pts[i].ry;
            pts[i].ry=(ry-pts[CUR].ry)/ScaleY+pts[CUR].ry;
        }
    }
    else{
        for(int i=0; i<len; i++){
            double ry = pts[i].ry;
            pts[i].ry=(ry)/ScaleY;
        }
    }
    Render();
}


void MainWindow::on_pbUp_clicked()
{
    if(rotemod == rote_center)
        Zero.ry()+=10;
    else{
        for (int i=0; i<len; i++){
            pts[i].ry-=10/-ScY;
        }
    }
    Render();
}


void MainWindow::on_pbLeft_clicked()
{
    if(rotemod == rote_center)
        Zero.rx()+=10;
    else{
        for (int i=0; i<len; i++){
            pts[i].rx-=10/ScX;
        }
    }
    Render();
}


void MainWindow::on_pbRight_clicked()
{
    if(rotemod == rote_center)
        Zero.rx()-=10;
    else{
        for (int i=0; i<len; i++){
            pts[i].rx+=10/ScX;
        }
    }
    Render();
}


void MainWindow::on_pbDown_clicked()
{
    if(rotemod == rote_center)
        Zero.ry()-=10;
    else{
        for (int i=0; i<len; i++){
            pts[i].ry+=10/-ScY;
        }
    }
    Render();
}


void MainWindow::on_pbHome_clicked()
{
    Zero.rx()=w/2;
    Zero.ry()=h/2;
    Render();
}


void MainWindow::on_pbRes_clicked()
{
    delete [] pts;
    s_point* pts=new s_point [n];
    curPoint=0;
    len=0;
    ui->cbx->clear();
    on_pbShow_clicked();
}


void MainWindow::on_cbx_currentIndexChanged(int index)
{
    ptC.rx()=pts[ui->cbx->currentIndex()].rx*ScX;
    ptC.ry()=pts[ui->cbx->currentIndex()].ry*-ScY;
    ptC+=Zero;
    Render();
}









void MainWindow::on_pb_Offset_clicked()
{
    if (offsetmod==offset_center)
        offsetmod=offset_vert;
    else
       offsetmod=offset_center;

}

void MainWindow::on_pb_Comp_clicked()
{
    if (compmod==comp_center)
        compmod=comp_vert;
    else
        compmod=comp_center;
}

void MainWindow::on_pb_Rote_clicked()
{
    if (rotemod == rote_center){
        rotemod = rote_vert;
    }
    else
        rotemod = rote_center;
}

void MainWindow::on_pb_DeleteVert_clicked()
{
    if(ui->cbx->currentText()!=""){
       int i=ui->cbx->currentIndex();
       ui->cbx->removeItem(i);
       for (i; i<len; i++){
           pts[i].rx=pts[i+1].rx;
           pts[i].ry=pts[i+1].ry;
       }
       curPoint-=1;
       len-=1;
    }
       Render();
}
