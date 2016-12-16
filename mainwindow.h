#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <QPair>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPair<int, QGraphicsItem*> draw(int i, int j, float x, float y, QGraphicsItem *parent = nullptr);
    QGraphicsItem *display(QString str, float x, float y);
    QString getNextLine(QTextStream & ts);
    QVector<QString> lines;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    bool flag;
};

#endif // MAINWINDOW_H
