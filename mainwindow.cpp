#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings.beginGroup("pathes");
    ui->nllbInferencePath->setText(settings.value("nllbInferencePath", "").toString());
    ui->modelPath->setText(settings.value("modelPath", "").toString());
    settings.endGroup();
    settings.beginGroup("languages");
    ui->sourceLanguage->setCurrentIndex(settings.value("sourceLanguage", 0).toInt());
    ui->targetLanguage->setCurrentIndex(settings.value("targetLanguage", 0).toInt());
    settings.endGroup();
    settings.beginGroup("lastRun");
    ui->source->setPlainText(settings.value("sourceText", "").toString());
    ui->target->setPlainText(settings.value("targetText", "").toString());
    settings.endGroup();
}

MainWindow::~MainWindow()
{
    settings.beginGroup("pathes");
    settings.setValue("nllbInferencePath", ui->nllbInferencePath->text());
    settings.setValue("modelPath", ui->modelPath->text());
    settings.endGroup();
    settings.beginGroup("languages");
    settings.setValue("targetLanguage", ui->targetLanguage->currentIndex());
    settings.setValue("sourceLanguage", ui->sourceLanguage->currentIndex());
    settings.endGroup();
    settings.beginGroup("lastRun");
    settings.setValue("sourceText", ui->source->toPlainText());
    settings.setValue("targetText", ui->target->toPlainText());
    settings.endGroup();

    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString formatedSource = ui->source->toPlainText().toUtf8().toBase64(); // because of quotes and other symbols

    //QString comand = "bash "+ui->nllbInferencePath->text()+" "+ui->modelPath->text()+" "+languages[ui->sourceLanguage->currentText()]+" "+languages[ui->targetLanguage->currentText()]+" <<< '"+formatedSource+"'";
    QString comand = "bash "+ui->nllbInferencePath->text()+" "+ui->modelPath->text()+" "+languages[ui->sourceLanguage->currentText()]+" "+languages[ui->targetLanguage->currentText()]+" <<< `echo '"+formatedSource+"' | base64 --decode`";
    ui->target->setPlainText("sh -c "+comand+"\n");
    ui->target->verticalScrollBar()->setValue(ui->target->verticalScrollBar()->maximum());

    c_output =  new QProcess(this);
    connect(c_output, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
    connect(c_output, SIGNAL(readyReadStandardError()), this, SLOT(processOutput()));
    c_output->start("sh", QStringList() << "-c" << comand);
}

void MainWindow::processOutput(){
    ui->target->setPlainText(ui->target->toPlainText()+c_output->readAllStandardOutput());
    ui->target->setPlainText(ui->target->toPlainText()+c_output->readAllStandardError());
    ui->target->verticalScrollBar()->setValue(ui->target->verticalScrollBar()->maximum());
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->modelPath->setText(QFileDialog::getOpenFileName(this, "Open nllb model", QString(), "Checkpoint file (*.pt)"));
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->nllbInferencePath->setText(QFileDialog::getOpenFileName(this, "Open nllb-inference script file", QString(), "Script file (translate.sh)"));
}

