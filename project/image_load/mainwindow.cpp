#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    previewPolygon =
        scene->addPolygon(
            QPolygonF(),
            QPen(Qt::green,2));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadButton_clicked()
{
    QString file =
        QFileDialog::getOpenFileName(
            this,
            "Open Image",
            "",
            "Images (*.png *.jpg)");

    if(file.isEmpty())
        return;

    QPixmap img(file);

    scene->clear();

    scene->addPixmap(img);

    previewPolygon =
        scene->addPolygon(
            QPolygonF(),
            QPen(Qt::green,2));

    ui->graphicsView->fitInView(
        scene->itemsBoundingRect(),
        Qt::KeepAspectRatio);
}

void MainWindow::on_zoomInButton_clicked()
{
    ui->graphicsView->scale(1.2,1.2);
}

void MainWindow::on_zoomOutButton_clicked()
{
    ui->graphicsView->scale(0.8,0.8);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPointF pt =
        ui->graphicsView
            ->mapToScene(event->pos());

    if(event->button()==Qt::LeftButton)
    {
        currentPolygon<<pt;

        drawing=true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(!drawing)
        return;

    QPolygonF temp=currentPolygon;

    temp<<ui->graphicsView
                ->mapToScene(event->pos());

    previewPolygon->setPolygon(temp);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *)
{
    if(!drawing)
        return;

    drawing=false;

    scene->addPolygon(
        currentPolygon,
        QPen(Qt::red,2));

    bool ok;

    QString label =
        QInputDialog::getItem(
            this,
            "Label",
            "Select structure:",
            labels,
            0,
            false,
            &ok);

    if(ok)
    {
        QPointF center =
            currentPolygon
                .boundingRect()
                .center();

        scene->addText(label)
            ->setPos(center);
    }

    annotations.append(currentPolygon);

    currentPolygon.clear();

    previewPolygon->setPolygon(QPolygonF());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape)
    {
        drawing=false;

        currentPolygon.clear();

        previewPolygon->setPolygon(QPolygonF());
    }
}

void MainWindow::on_saveButton_clicked()
{
    QJsonArray arr;

    for(auto poly:annotations)
    {
        QJsonArray pts;

        for(auto p:poly)
        {
            QJsonArray xy;

            xy.append(p.x());

            xy.append(p.y());

            pts.append(xy);
        }

        QJsonObject obj;

        obj["points"]=pts;

        arr.append(obj);
    }

    QJsonDocument doc(arr);

    QFile file("annotations.json");

    file.open(QIODevice::WriteOnly);

    file.write(doc.toJson());
}