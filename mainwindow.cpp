#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btnProcess->setEnabled(false);
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit->setPlaceholderText("Selecione a pasta com os voos");
    process = new QProcess();
    toolPath = qApp->applicationDirPath();
    toolPath = QDir::toNativeSeparators(toolPath + "/");
    qDebug()<<"Path: "<<toolPath;
}

//******************************************************************************

MainWindow::~MainWindow()
{
    process->kill();
    delete ui;
}


//******************************************************************************

void MainWindow::on_btnDirectory_clicked()
{
    folderPath = QFileDialog::getExistingDirectory(this, tr("Selecione a pasta dos voos."));

    if(!folderPath.isEmpty())
    {
        ui->status->setText("Status:");
        ui->btnProcess->setEnabled(true);
        folderPath = QDir::toNativeSeparators(folderPath + "/");

        ui->lineEdit->setText(folderPath);

        qDebug()<<"folderPath: "<<folderPath;

        tmpFolders = QDir(folderPath).entryList(QDir::Dirs);

        if(tmpFolders.size()<=2){
            ui->btnProcess->setEnabled(false);
            ui->lineEdit->clear();
            ui->lineEdit->setPlaceholderText("Selecione a pasta com os voos");
            QMessageBox::critical(this, tr("Aviso"), tr("A pasta selecionada deve conter as subpastas com as fotos!"));
        }
        else if(tmpFolders.size()==3){
            ui->btnProcess->setEnabled(false);
            ui->lineEdit->clear();
            ui->lineEdit->setPlaceholderText("Selecione a pasta com os voos");
            QMessageBox::critical(this, tr("Aviso"), tr("A pasta selecionada deve conter mais de 1 subpasta com as fotos!"));
        }
        else{
             ui->btnProcess->setEnabled(true);
        }
    }
    else{
        QMessageBox::critical(this, tr("Aviso"), tr("Nenhuma pasta selecionada!"));
        ui->btnProcess->setEnabled(false);
    }
}


//******************************************************************************
void MainWindow::on_btnProcess_clicked()
{
    QString cmd;
    int folderCount = 0;
    QString timeInfo;
    QStringList fileList;
    QString folder;
    QString outputStatus = "";

    QStringList filter;
    filter << QLatin1String("*.png");
    filter << QLatin1String("*.jpeg");
    filter << QLatin1String("*.jpg");

    if(tmpFolders.size()>=3 || folderPath!= ""){

       for(int i=3; i<tmpFolders.size(); i++){

            folderCount += 1;
            folder = tmpFolders.at(i);

            qDebug()<<"Folder :"<<folderPath+folder;
            qDebug()<<"FolderCount :"<<folderCount;

            QDir dir(folderPath+folder);
            dir.setNameFilters(filter);
            filelistinfo = dir.entryInfoList();

            QString sufix = filelistinfo.at(filelistinfo.size()/2).suffix();
            qDebug()<<"filelistinfo"<<sufix;


            QString imageFile = folderPath+folder+"\\*."+sufix;

            QString folderCountStr = QString::number(folderCount);

            if(folderCount<10){
                timeInfo = "0000:00:0"+folderCountStr+" 00:00:00"; // fazer pelo dia
            }
            else{
                timeInfo = "0000:00:"+folderCountStr+" 00:00:00"; // fazer pelo dia
            }

            qDebug()<<"imageFile: "<< imageFile;
            qDebug()<<"timeInfo: "<<timeInfo;

            cmd = QStringLiteral("\"%1exiftool\" -v0 -overwrite_original -alldates+=\"%2\" %3").arg(toolPath).arg(timeInfo).arg(imageFile);

            if(runCmd(cmd,folder)){
                 qDebug()<<"Falha no cmd";
                 outputStatus += finalStatus;
            }
            else{
                qDebug()<<"Executou o cmd";
                outputStatus += finalStatus;
                qDebug()<<outputStatus;
            }
         }
         ui->status->setText("Status final:\n"+outputStatus);
    }else{
        ui->status->setText("Status:\nErro ao processar pasta!");
    }

}

//******************************************************************************
int MainWindow::runCmd(QString cmd, QString dirNum)
{
    if(!cmd.isNull()){
        process->start(cmd);
    }
    else{
        return -1;
    }

    QString outputStdOut;
    QStringList splitData;
    int count = 0;
    int i = 0;

    while(!process->waitForFinished(5)) {
            QCoreApplication::processEvents();
            outputStdOut = process->readAllStandardOutput();

            if(outputStdOut != ""){

                splitData = outputStdOut.split('/');
                outputStdOut = splitData.at(splitData.size()-1);
                splitData.clear();
                splitData = outputStdOut.split(' ');

                if(splitData.size()>3){
                    qDebug()<<"splitData :"<<splitData;

                    while(splitData.at(i)==""){
                        i++;
                    };

                   qDebug()<<"splitData :"<<splitData;
                   qDebug()<<"i: "<< i;

                    outputStdOut = splitData.at(i) + "  imagens atualizadas";
                }

                count++;
//                qDebug()<<"outputStdOut :"<<outputStdOut;
                ui->status->setText("Status: \nPasta "+dirNum+": "+outputStdOut);
                finalStatus = "Pasta "+dirNum+": "+outputStdOut+"\n";
            }
    //        QCoreApplication::processEvents();

    }
        qDebug()<<"count: "<<count;
//    QCoreApplication::processEvents();

    return process->exitCode();
}








