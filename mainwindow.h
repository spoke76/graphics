#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QPen>
#include "paintscene.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QGraphicsPixmapItem* pmi;
    QPen linePen, axisPen, girdPen;

public:
    MainWindow(QWidget *parent = NULL);
    ~MainWindow();
    void point();
    double h, w;
    void addPoint(QPoint);
    void drawPgn(QPixmap& image, const QPen& pn);
    void drawAxis(QPixmap& image, const QPen& pn);
    void drawGird(QPixmap& image, const QPen& pn);
    void Render();
    enum state{
        enable,
        disable
    };
    state ST=enable;


private slots:
    void on_pbShow_clicked();

    void on_pbClear_clicked();

    void on_pbRotateLeft_clicked();

    void on_pbRotateRight_clicked();

    void on_pbScXPlus_clicked();

    void on_pbScYPlus_clicked();

    void on_pbScXMinus_clicked();

    void on_pbScYMinus_clicked();

    void on_pbUp_clicked();

    void on_pbLeft_clicked();

    void on_pbRight_clicked();

    void on_pbDown_clicked();

    void on_pbHome_clicked();

    void on_pbRes_clicked();

    void on_cbx_currentIndexChanged(int index);

    void on_pbShow_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pb_Offset_clicked();

    void on_pb_Comp_clicked();

    void on_pb_Rote_clicked();

    void on_pb_DeleteVert_clicked();

private:
    int n=50, len=0;
    int curPoint=0;
    QPoint Zero, ptC;
    struct s_point
    {
        double rx;
        double ry;
    };
    enum offsetMode{
        offset_vert,
        offset_center
    };
    offsetMode offsetmod=offset_center;
    enum compMode{
        comp_vert,
        comp_center
    };
    compMode compmod=comp_center;
    enum roteMode{
       rote_vert,
       rote_center
    };
    roteMode rotemod=rote_center;
    double ScX,ScY,X,Y, ScaleX, ScaleY;
    Ui::MainWindow *ui;
    paintScene *scene;
    s_point* pts=new s_point [n];
};
#endif // MAINWINDOW_H
