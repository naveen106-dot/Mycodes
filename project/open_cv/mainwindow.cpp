#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QPixmap>

bool cameraStarted = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout,
            this, &MainWindow::updateFrame);
}

MainWindow::~MainWindow()
{
    cap.release();

    writer.release();

    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    cap.open(0);

    cameraStarted = true;

    writer.open("output.avi",
                cv::VideoWriter::fourcc('M','J','P','G'),
                10,
                cv::Size(640,480));




    timer->start(30);
}

void MainWindow::updateFrame()
{
    cap >> frame;

    if(frame.empty())
        return;

    writer.write(frame);

    cv::cvtColor(frame,
                 frame,
                 cv::COLOR_BGR2RGB);

    QImage img(frame.data,
               frame.cols,
               frame.rows,
               frame.step,
               QImage::Format_RGB888);

    ui->videoLabel->setPixmap(
        QPixmap::fromImage(img));
}

void MainWindow::on_captureButton_clicked()
{

    //to check that the camere is on or off


    if(!cap.isOpened())
    {
        return;
    }


    if(frame.empty())
    {
        return;
    }
   //........................................................
    std::string name =
        "img_" +
        std::to_string(img_count++) +
        ".png";

    cv::imwrite(name, frame);
}