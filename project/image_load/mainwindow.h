#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


#include <QMouseEvent>
#include <QKeyEvent>
#include <QPen>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

protected:

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private slots:

    void on_loadButton_clicked();

    void on_zoomInButton_clicked();

    void on_zoomOutButton_clicked();

    void on_saveButton_clicked();

private:

    Ui::MainWindow *ui;

    QGraphicsScene *scene;

    QGraphicsPolygonItem *previewPolygon;

    QPolygonF currentPolygon;

    QVector<QPolygonF> annotations;

    QStringList labels =
        {
            "Tumor",
            "Organ",
            "Lesion"
        };

    double scaleFactor=1.0;

    bool drawing=false;
};

#endif