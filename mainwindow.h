#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfoList>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int runCmd(QString cmd, QString dirNum);

private slots:
    void on_btnDirectory_clicked();
    void on_btnProcess_clicked();

private:
    Ui::MainWindow *ui;
    QString folderPath;
    QFileInfoList filelistinfo;
    QProcess *process;
    QString dateFilePath;
    QString toolPath;

    QString getNewDate(int folderCount);

};

#endif // MAINWINDOW_H
