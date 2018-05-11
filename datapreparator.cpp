#include "datapreparator.h"

#define DATA_ROOT "C:/dev"
#define DATA_PATH "/data_sample"
#define CROPPED_PATH "/cropped/s"

DataPreparator::DataPreparator()
{

}

cv::Mat DataPreparator::convertColor(cv::Mat colorMat) {
    using namespace cv;
    Mat greyMat;
    cvtColor(colorMat, greyMat, CV_BGR2GRAY);
    return greyMat;
}

cv::Mat DataPreparator::detectFace(cv::Mat mat) {
    using namespace cv;
    std::vector<Rect> faces;
    Mat greyMat = convertColor(mat);

    if (!greyMat.empty()){
        CascadeClassifier cascade;
        if(!cascade.load("C:/dev/opencv/data/haarcascades/haarcascade_frontalface_alt.xml")){
            qDebug() << "Error with file";
            throw std::runtime_error("Could not open file haarcascade_frontalface_alt");
        }
        cascade.detectMultiScale(greyMat, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(20, 20));
    }
    if (faces.empty()){
        //qDebug() << "empty face";
        Mat m;
        return m;
    }

    /* drawing the detected face on the original image
    int x = faces[0].x;
    int h_temp = faces[0].height;    // storing original height
    int y = faces[0].y - h_temp*0.3; // y is reduced by 0.3*h
    int w = faces[0].width;
    int h = h_temp*1.6;

    rectangle(mat,Point (x,y),Point(x + w,y +h),Scalar(255,0,255),1,4,0);
    */
    return greyMat(faces[0]);

}


void DataPreparator::processSingleImage(QString imageSourcePath,
                                        QString pathToWrite){
    using namespace cv;
    fileNumber++;

    //First step is checking if the cropped file already exists on the disk
    //If the file exists, we still process and skip it only if the face is not empty
    QString folderPath = pathToWrite % QString::number(folderLabel);
    QString imageDestPath= folderPath % "/" % QString::number(fileNumber) % ".jpg";
    QFileInfo imageInfo(imageDestPath);

    Mat image = imread(imageSourcePath.toStdString());

    //qDebug() << "Reading image" << imagePath;
    if (image.empty()){
        throw std::runtime_error("Error while reading image");
    }
    Mat face = detectFace(image);

    if(face.empty()){
        nbFilesSkipped++;
        fileNumber--;
    } else {

        if(imageInfo.exists()){
            //If image exists, it is still considered processed though not written to disk
            return;
        }

        //Create folder if needed
        if(!QDir(folderPath).exists()){
            QDir().mkdir(folderPath);
        }

        //qDebug() << "Writing to" << imagePath;
        imwrite(imageDestPath.toStdString(),face);

    }
}


QString DataPreparator::processDBImages(){
    nbFilesSkipped = 0;
    int nbFilesProcessed = 0;
    folderLabel=1;
    using namespace cv;
    QString origPath=DATA_ROOT;
    QDir croppedDir(origPath % CROPPED_PATH);
    QDir origDir(origPath % DATA_PATH);

    QElapsedTimer elapsedTimer;
    elapsedTimer.start();



    QFileInfoList dirList =origDir.entryInfoList(QStringList() << "*.jpg" << "*.png",
                           QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                           QDir::Name);


    qDebug() << "Browsing " << origDir.absolutePath();

    //Loop subdirs in MainFolder
    for (int i=0; i<dirList.count();++i){

        qDebug() << "In " << dirList[i].absoluteFilePath();

        //Check that this is a subdir
        if(dirList[i].isDir()){
            fileNumber=0;

             QDir sd(dirList[i].absoluteFilePath());
             QFileInfoList imagesList =sd.entryInfoList(QStringList() << "*.jpg" << "*.png",
                                    QDir::NoDotAndDotDot | QDir::Files ,
                                    QDir::Name);

             if (imagesList.count() ==0) {
                 continue; //skipping to avoid creating an empty folder number
             }

             //Loop on subfolders
             for (int j=0;j<imagesList.count();j++) {
                 try {
                     processSingleImage(imagesList[j].absoluteFilePath(),
                                    croppedDir.absolutePath());
                 }
                 catch(const std::runtime_error& error){
                     qDebug() << error.what();
                 }

             }
             nbFilesProcessed+=fileNumber;
             folderLabel++;
        }
    }

    qint64 e = elapsedTimer.elapsed();

    QString status=QString::number(nbFilesProcessed);
    status += " files processed in ";
    status += QString::number(e);
    status += " milliseconds\n";
    status += QString::number(nbFilesSkipped);
    status += " files with no detected face have been skipped.";
    return status;
}
