#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>

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

    void updateFrame();

    void on_startButton_clicked();

    void on_captureButton_clicked();

private:

    Ui::MainWindow *ui;

    QTimer *timer;

    cv::VideoCapture cap;

    cv::VideoWriter writer;

    cv::Mat frame;

    int img_count = 0;
};

#endif