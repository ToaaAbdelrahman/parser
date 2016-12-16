#include "logic.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    run();
    QFile file("parser_output.txt");
    if (!file.open(QIODevice::ReadOnly)) return;
    QTextStream ts(&file);

    while(!ts.atEnd()) {
        lines.push_back(getNextLine(ts));
    }

    draw(0, lines.size() - 1, 0, 0);
}

QPair<int, QGraphicsItem*> MainWindow::draw(int i, int j, float x, float y, QGraphicsItem* parent) {
    QGraphicsItem* io = nullptr;
    do {
        if (x <= 0)
            x = scene->itemsBoundingRect().right() + 30;
        float old = x;
        QString str = lines[i];
        if (str.startsWith("read ")) {
            auto o = display(str, x, y);
            if (io != nullptr) {
                scene->addLine(io->x() + io->boundingRect().width(), io->y() + (io->boundingRect().height()/2), o->x(), o->y() + (o->boundingRect().height() / 2), QPen(QColor(Qt::black)));
            }
            if (parent != nullptr) {
                scene->addLine(parent->x() + (parent->boundingRect().width() / 2), parent->y() + parent->boundingRect().height(), o->x() + (o->boundingRect().width() / 2), o->y(), QPen(QColor(Qt::black)));
                parent = nullptr;
            }
            io = o;
            x = scene->itemsBoundingRect().right() + 30;
        }
        if (str == "write") {
            auto pair = draw(i+1, i+1, x, y+2);
            x = scene->itemsBoundingRect().right() + 30;
            auto o = display(str, old + ((x-old)/4), y); //if
            scene->addLine(o->x() + (o->boundingRect().width() / 2), o->y() + o->boundingRect().height(), pair.second->x() + (pair.second->boundingRect().width() / 2), pair.second->y(), QPen(QColor(Qt::black)));
            x = scene->itemsBoundingRect().right() + 30;
            if (io != nullptr) {
                scene->addLine(io->x() + io->boundingRect().width(), io->y() + (io->boundingRect().height()/2), o->x(), o->y() + (o->boundingRect().height() / 2), QPen(QColor(Qt::black)));
            }
            if (parent != nullptr) {
                scene->addLine(parent->x() + (parent->boundingRect().width() / 2), parent->y() + parent->boundingRect().height(), o->x() + (o->boundingRect().width() / 2), o->y(), QPen(QColor(Qt::black)));
                parent = nullptr;
            }
            i = pair.first;
            io = o;
        }
        else if (str == "if") {
            auto pair1 = draw(i+1, i+1, x, y+2); //compare
            x = scene->itemsBoundingRect().right() + 30;
            int j2 = lines.indexOf("end_if", i);
            auto o = display(str, x - ((x-old)/2), y); //if
            x = scene->itemsBoundingRect().right() + 30;
            auto pair2 = draw(pair1.first, j2, x, y+2, o); // then
            x = scene->itemsBoundingRect().right() + 30;
            scene->addLine(o->x() + (o->boundingRect().width() / 2), o->y() + o->boundingRect().height(), pair1.second->x() + (pair1.second->boundingRect().width() / 2), pair1.second->y(), QPen(QColor(Qt::black)));
//            scene->addLine(o->x() + (o->boundingRect().width() / 2), o->y() + o->boundingRect().height(), pair2.second->x() + (pair2.second->boundingRect().width() / 2), pair2.second->y(), QPen(QColor(Qt::black)));
            if (io != nullptr) {
                scene->addLine(io->x() + io->boundingRect().width(), io->y() + (io->boundingRect().height()/2), o->x(), o->y() + (o->boundingRect().height() / 2), QPen(QColor(Qt::black)));
            }
            if (parent != nullptr) {
                scene->addLine(parent->x() + (parent->boundingRect().width() / 2), parent->y() + parent->boundingRect().height(), o->x() + (o->boundingRect().width() / 2), o->y(), QPen(QColor(Qt::black)));
                parent = nullptr;
            }
            i = pair2.first;
            io = o;
            continue;
        }
        else if (str.startsWith("number(") || str.startsWith("identifier(")) {
            io = display(str, x, y);
            x = scene->itemsBoundingRect().right() + 30;
        }
        else if (str.indexOf("_op(") != -1) {
            auto pair1 = draw(i+1, i+1, x, y+2);
            x = scene->itemsBoundingRect().right() + 30;
            auto o = display(str.right(3), x, y);
            x = scene->itemsBoundingRect().right() + 30;
            auto pair2 = draw(pair1.first, pair1.first, x, y+2);
            x = scene->itemsBoundingRect().right() + 30;
            scene->addLine(o->x() + (o->boundingRect().width() / 2), o->y() + o->boundingRect().height(), pair1.second->x() + (pair1.second->boundingRect().width() / 2), pair1.second->y(), QPen(QColor(Qt::black)));
            scene->addLine(o->x() + (o->boundingRect().width() / 2), o->y() + o->boundingRect().height(), pair2.second->x() + (pair2.second->boundingRect().width() / 2), pair2.second->y(), QPen(QColor(Qt::black)));
            if (io != nullptr) {
                scene->addLine(io->x() + io->boundingRect().width(), io->y() + (io->boundingRect().height()/2), o->x(), o->y() + (o->boundingRect().height() / 2), QPen(QColor(Qt::black)));
            }
            i = pair2.first;
            io = o;
            continue;
        }
        else if (str == "assign_stmt") {
            auto o = display(lines[i+1], x, y);
            auto pair = draw(i+2, i+2,  x, y+2);
            scene->addLine(o->x() + (o->boundingRect().width() / 2), o->y() + o->boundingRect().height(), pair.second->x() + (pair.second->boundingRect().width() / 2), pair.second->y(), QPen(QColor(Qt::black)));
            x = scene->itemsBoundingRect().right() + 30;
            if (io != nullptr) {
                scene->addLine(io->x() + io->boundingRect().width(), io->y() + (io->boundingRect().height()/2), o->x(), o->y() + (o->boundingRect().height() / 2), QPen(QColor(Qt::black)));
            }
            if (parent != nullptr) {
                scene->addLine(parent->x() + (parent->boundingRect().width() / 2), parent->y() + parent->boundingRect().height(), o->x() + (o->boundingRect().width() / 2), o->y(), QPen(QColor(Qt::black)));
                parent = nullptr;
            }
            i = pair.first;
            io = o;
            continue;
        }
        else if (str == "repeat") {
            int j2 = lines.indexOf("repeat_cond", i);
            auto o = display(str, x - ((x-old)/2), y); //repeat
            auto pair = draw(i+1, j2, x, y+2, o);
            x = scene->itemsBoundingRect().right() + 30;
            int j3 = lines.indexOf("end_repeat", j2);
            pair = draw(pair.first+1, pair.first+1, x, y+2);
            x = scene->itemsBoundingRect().right() + 30;
            scene->addLine(o->x() + (o->boundingRect().width() / 2), o->y() + o->boundingRect().height(), pair.second->x() + (pair.second->boundingRect().width() / 2), pair.second->y(), QPen(QColor(Qt::black)));
            if (io != nullptr) {
                scene->addLine(io->x() + io->boundingRect().width(), io->y() + (io->boundingRect().height()/2), o->x(), o->y() + (o->boundingRect().height() / 2), QPen(QColor(Qt::black)));
            }
            i = pair.first;
            io = o;
            continue;
        }
        i++;
    } while(i < j);
    return QPair<int, QGraphicsItem*>(i, io);
}

QGraphicsItem* MainWindow::display(QString str, float x, float y) {
    QGraphicsTextItem* io;
    io = new QGraphicsTextItem();
//    io->setFont(QFont("Monospace", 5));
    io->setPlainText(str);
    io->setPos(x, y * 25);
    scene->addItem(io);
    scene->addRect(io->pos().x(), io->pos().y(), io->boundingRect().width(), io->boundingRect().height(), QPen(QColor(Qt::black)));
    return io;
}

QString MainWindow::getNextLine(QTextStream &ts) {
    while(!ts.atEnd()) {
        QString line = ts.readLine();
        if (line.isEmpty()) continue;
        return line;
    }
    return "";
}

MainWindow::~MainWindow()
{
    delete ui;
}
