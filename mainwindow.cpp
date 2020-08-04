#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
//#include <QMediaMetaData>
//#include <QExifImageHeader>
//#include <QImageWriter>
//#include <QMediaObject>
//#include <QMediaService>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->btnProcess->setEnabled(false);
    ui->lineEdit->setReadOnly(true);
    process = new QProcess();

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::runCmd(QString cmd, QString dirNum)
{
    process->start(cmd);

    QString outputStdOut;
    QStringList splitData;

    while(!process->waitForFinished(5)) {
        QCoreApplication::processEvents();
        outputStdOut = process->readAllStandardOutput();

        if(outputStdOut != ""){

            splitData = outputStdOut.split('/');
            outputStdOut = splitData.at(splitData.size()-1);
            qDebug()<<"outputStdOut :"<<outputStdOut;
            ui->status->setText("Status: \nDir "+dirNum+": "+outputStdOut);
        }

//        QCoreApplication::processEvents();
//        process->readAllStandardError();
    }

    QCoreApplication::processEvents();

    return process->exitCode();
}


void MainWindow::on_btnDirectory_clicked()
{
    toolPath = qApp->applicationDirPath();
    toolPath = QDir::toNativeSeparators(toolPath + "/");
    qDebug()<<"Path: "<<toolPath;

    folderPath = QFileDialog::getExistingDirectory(this, tr("Selecione a pasta dos voos."));

    if(!folderPath.isEmpty())
    {
//        QDir dir(folderPath);
//        QStringList filter;
//        filter << QLatin1String("*.png");
//        filter << QLatin1String("*.jpeg");
//        filter << QLatin1String("*.jpg");
//        dir.setNameFilters(filter);
//        filelistinfo = dir.entryInfoList();
        ui->btnProcess->setEnabled(true);
        ui->lineEdit->setReadOnly(false);
        ui->lineEdit->setText(folderPath);
        ui->lineEdit->setReadOnly(true);
        folderPath = QDir::toNativeSeparators(folderPath + "/");
        qDebug()<<"folderPath: "<<folderPath;

    }
    else{
        QMessageBox::critical(this, tr("Aviso"), tr("Nenhuma pasta selecionada!"));
    }

}

void MainWindow::on_btnProcess_clicked()
{
    qDebug()<<"Processando";


//    folderPath =  "C:\\Users\\kulie\\Documents\\HORUS\\HOME_OFFICE\\UPA_HOME_OFFICE\\Foxtek_sequencer\\build-Sequenciador_de_fotos-Desktop_Qt_5_12_0_MinGW_64_bit-Release\\teste_fotos\\3\\";

    QDir dir(folderPath);
    QString cmd;
    int folderCount = 1;
    QString timeInfo;
//    QString tempFolder;

    // Fazer a varredura das pastas
//    tempFolder = "";


    // Em cada pasta processar as fotos
    QStringList filter;
    filter << QLatin1String("*.png");
    filter << QLatin1String("*.jpeg");
    filter << QLatin1String("*.jpg");
    dir.setNameFilters(filter);
    filelistinfo = dir.entryInfoList();
    QStringList fileList;


    QString imageFile = folderPath+"*.jpg";
    QString folderCountStr = QString::number(folderCount);

    if(folderCount<10){
        timeInfo = "0000:00:0"+folderCountStr+" 00:00:00"; // fazer pelo dia
    }
    else{
        timeInfo = "0000:00:"+folderCountStr+" 00:00:00"; // fazer pelo dia
    }


    // adiciona timeInfo na data original
    cmd = QStringLiteral("\"%1exiftool\" -v0 -overwrite_original -alldates+=\"%2\" %3").arg(toolPath).arg(timeInfo).arg(imageFile);

    if(runCmd(cmd,folderCountStr)){
         qDebug()<<"Falha no cmd";
    }
    else{
        qDebug()<<"Executou o cmd";

    }
}







/*
    foreach (const QFileInfo &fileinfo, filelistinfo) {

//        QString imageFile = fileinfo.absoluteFilePath();

        QString img = fileinfo.fileName();
        QString imageFile = folderPath+img;

        if(folderCount<10){
            timeInfo = "0000:00:0"+QString::number(folderCount)+" 00:00:00"; // fazer pelo dia
        }
        else{
            timeInfo = "0000:00:"+QString::number(folderCount)+" 00:00:00"; // fazer pelo dia
        }

        qDebug()<<"\nImageFile :"<<img;
        qDebug()<<timeInfo;


        // adiciona timeInfo na data original
        cmd = QStringLiteral("\"%1exiftool\" -overwrite_original -alldates-=\"%2\" %3").arg(toolPath).arg(timeInfo).arg(imageFile);

//        cmd = QStringLiteral("%1exiftool  -alldates %2").arg(toolPath).arg(imageFile);

        if(runCmd(cmd)){
             qDebug()<<"Falha no cmd";
             ui->status->setText("Processando: falha no arquivo "+img);
        }
        else{
            qDebug()<<"Executou o cmd";
            ui->status->setText("Processando: "+img);
        }
    }
    */






