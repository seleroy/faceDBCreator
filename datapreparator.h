#ifndef DATAPREPARATOR_H
#define DATAPREPARATOR_H

#include <QStringBuilder>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QElapsedTimer>
#include "opencv2/opencv.hpp"

class DataPreparator
{
public:
    DataPreparator();
    QString processDBImages();
private:
    int fileNumber;
    int folderLabel;
    int nbFilesSkipped;

    cv::Mat convertColor(cv::Mat);
    cv::Mat detectFace(cv::Mat);
    void processSingleImage(QString imagePath,
                            QString pathToWrite);
};

#endif // DATAPREPARATOR_H
