#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QDebug>
#include <QImage>
#include <QThread>
#include <QMessageBox>
#include <QFileDialog>


#define VIDEO_W 430
#define VIDEO_H 320

int numInfracoes = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tbCaminhoArquivo->setText("/Users/guilherme/GUILHERME/MESTRADO/UDESC/PMI/Carros/Dia/dia_3.mpg");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    if(ui->tbCaminhoArquivo->text().isEmpty()){
        QMessageBox::about(this, tr("Ops"),
                     tr("Informe o caminho do arquivo de video"));

        return;
    }

    ip = new ImageProcessor;
    ip->filePath = new QString(ui->tbCaminhoArquivo->text());
    ip->SalvaVideo = true;

    connect(ip,SIGNAL(captureFrame(QImage)),this,SLOT(frameCaptured(QImage)));
    connect(ip,SIGNAL(captureOriginalFrame(QImage)),this,SLOT(frameOriginalCaptured(QImage)));
    connect(ip,SIGNAL(infracao(QImage)),this,SLOT(novaInfracao(QImage)));
    connect(ip,SIGNAL(frameCount(int)),this,SLOT(frameCount(int)));

    cThread = new QThread;
    ip->DoSetup(*cThread);
    ip->moveToThread(cThread);
    cThread->start();

}

void MainWindow::frameCount(int count){
    ui->frameCountLabel->setNum(count);
}

void MainWindow::frameCaptured(QImage img){
    ui->lblVideoProcessado->setPixmap(QPixmap::fromImage(img).scaled(VIDEO_W,VIDEO_H, Qt::KeepAspectRatio));
}

void MainWindow::frameOriginalCaptured(QImage img){
    ui->lblVideoOriginal->setPixmap(QPixmap::fromImage(img).scaled(VIDEO_W,VIDEO_H, Qt::KeepAspectRatio));
}

void MainWindow::novaInfracao(QImage){
    numInfracoes++;
    ui->numInfracoesLabel->setNum(numInfracoes);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Video"), "/", tr("Image Files (*.avi *.mpg *.wmv *.mp4 *.ts)"));
    ui->tbCaminhoArquivo->setText(file);

}

void MainWindow::on_pushButton_2_clicked()
{
    if(ip)
        ip->Stop = true;

}
