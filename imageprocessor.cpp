#include "imageprocessor.h"
#include <QDebug>
#include "opencv2/opencv.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/video/background_segm.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <vector>

ImageProcessor::ImageProcessor(QObject *parent) :
    QObject(parent)
{
}


void ImageProcessor::DoSetup(QThread &cThread){
    connect(&cThread, SIGNAL(started()), this, SLOT(DoWork()));
    Stop = false;
    util = new Util;
}


void ImageProcessor::DoWork(){
    qDebug() << "Iniciando thread";
    VideoCapture cap(filePath->toStdString());
    Mat foreground,image,frameAnt;

    VideoWriter videoWriter;
    if(SalvaVideo){
        videoWriter.open("/Users/guilherme/GUILHERME/MESTRADO/UDESC/PMI/QT/tmp/resultado.mpg",CV_FOURCC('P','I','M','1'),30,Size(640,480),true);

    }

    int fps = (int)cap.get(CV_CAP_PROP_FPS);
    qDebug() << "FPS: " << fps;

    qDebug() << "Carregando imagem de mascara";

    Mat mask;
    mask = imread("/Users/guilherme/GUILHERME/MESTRADO/UDESC/PMI/Artigo_Transito/areainteresse3.png", CV_LOAD_IMAGE_COLOR);

    if(!mask.data){
        qDebug() << "Erro ao ler mascara";
        return;
    }else{
        qDebug() << "Mascara ";
        qDebug() << "width: " << mask.size().width;
        qDebug() << "height: " << mask.size().height;
    }


    threshold(mask,mask,1,255,THRESH_BINARY);
    mask.inv();

    BackgroundSubtractorMOG2 mog;

    int frmCount = 1;

    while(cap.grab())
    {
        QMutex mutex;
        mutex.lock();
        if(Stop){
            if(SalvaVideo){
                videoWriter.~VideoWriter();
            }

            break;
        }
        mutex.unlock();

        Mat frame;
        cap >> frame;
        if(frame.empty()){
            break;
        }

        emit(frameCount(frmCount));
        frmCount++;


        image = frame.clone();
        //image = image - mask;
        frame =  frame - mask;
        frameAnt = image;

        mog(frame,foreground,-1);


        threshold(foreground,foreground,64,255,THRESH_BINARY);
        medianBlur(foreground,foreground,3);
        ////erode(foreground,foreground,Mat());
        ////dilate(foreground,foreground,Mat());


        element = getStructuringElement(CV_SHAPE_ELLIPSE,Size(15,15));
        morphologyEx(foreground,foreground,MORPH_DILATE,element);

        element = getStructuringElement(CV_SHAPE_ELLIPSE,Size(10,10));
        morphologyEx(foreground,foreground,MORPH_ERODE,element);

        vector<vector<Point> > contours;
        Mat foregroundTmp = foreground.clone();
        findContours(foregroundTmp, contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        Mat result(foregroundTmp.size(),CV_8U,cv::Scalar(0));
        ////drawContours(result, contours, -1,Scalar(255),2);

        foreach(vector<Point> pontos, contours){
            Rect r0= boundingRect(Mat(pontos));

            if(r0.area() > 2600 && r0.area() < 5000){
                Mat imageRectangle(foregroundTmp.size(),CV_8U,cv::Scalar(0));
                rectangle(imageRectangle,r0,Scalar(255),CV_FILLED);
                char strArea[10] ;
                sprintf(strArea, "%d ",r0.area() ) ;
                putText( result, strArea, pontos[0], CV_FONT_VECTOR0, 0.6,Scalar(255),2 );

                result = result + imageRectangle;

                if(SalvaVideo){
                    element = getStructuringElement(CV_SHAPE_CROSS,Size(4,4));
                    morphologyEx(imageRectangle,imageRectangle,MORPH_GRADIENT,element);
                    cvtColor(imageRectangle, imageRectangle, CV_GRAY2RGB);
                    videoWriter.write(image + imageRectangle);
                }

                //cvtColor(imageRectangleClone, imageRectangleClone, CV_GRAY2RGB);
                QImage imagemInfracao = util->IplImage2QImage(new IplImage(image));
                infracao(imagemInfracao);


            }

        }


        cvtColor(result, result, CV_GRAY2RGB);
        result = image + result;


        cvtColor(foreground, foreground, CV_GRAY2RGB);
        foreground = image + foreground;

        QImage originalFrame = util->IplImage2QImage(new IplImage(image));
        emit captureOriginalFrame(originalFrame);

        QImage qFrame = util->IplImage2QImage(new IplImage(result));
        emit captureFrame(qFrame);

        //sleep(1);


    }

    if(SalvaVideo)
        videoWriter.~VideoWriter();
}
