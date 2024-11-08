#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


class paintScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit paintScene(QObject *parent = NULL);
    ~paintScene();

signals:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // PAINTSCENE_H
