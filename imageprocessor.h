#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>
#include "util.h"

using namespace cv;
class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessor(QObject *parent = 0);
    void DoSetup(QThread &cThread);
    QString *filePath;
    bool Stop;
    Util *util;
    bool SalvaVideo;

private:
    Mat frameAnt;
    Mat element;
    bool Interseccao(Mat img1, Mat img2);

signals:
    void captureOriginalFrame(QImage);
    void captureFrame(QImage);
    void infracao(QImage);
    void frameCount(int);

public slots:
    void DoWork();
    
};

#endif // IMAGEPROCESSOR_H
