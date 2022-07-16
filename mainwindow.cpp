#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QProcess c_output;
    c_output.setProcessChannelMode(QProcess::MergedChannels);
    c_output.setWorkingDirectory("/run/media/max/02c10692-598c-4d07-a7fa-44a11c161337/home/max/content//sentencepiece/build/src/");
    c_output.start("bash", QStringList() << "/run/media/max/02c10692-598c-4d07-a7fa-44a11c161337/home/max/content/NLLB-inference/translate.sh" << "/run/media/max/02c10692-598c-4d07-a7fa-44a11c161337/home/max/content/checkpoint.pt" << "jpn_Jpan rus_Cyrl" << "<<< '今日もキメてくネガティ'");
    if(!c_output.waitForStarted()){
        ui->target->setPlainText("Failed to tarnslate");
    }
    c_output.waitForFinished();

    c_output.write("...");
    c_output.closeWriteChannel();

    while(c_output.state() != QProcess::NotRunning){
        //ui->target->setPlainText(".");
        if(c_output.waitForReadyRead())
            ui->target->setPlainText(ui->target->toPlainText()+c_output.readAllStandardOutput());
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->modelPath->setText(QFileDialog::getOpenFileName(this, "Open nllb model", QString(), "Checkpoint file (*.pt)"));
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->nllbInferencePath->setText(QFileDialog::getOpenFileName(this, "Open nllb-inference script file", QString(), "Script file (translate.sh)"));
}

