#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("监控平台");
    m_client = new QMqttClient(this);
    connect(&this->timer, &QTimer::timeout, this, &MainWindow::on_timerOut);
    connect(m_client, &QMqttClient::messageReceived, this, &MainWindow::on_msgRecv);
    sus = false;
    errorCnt = 0;
    init = false;
    ui->pushButton_2->setEnabled(false);
    ui->lineEdit_3->setValidator(new QIntValidator(ui->lineEdit_3));
}
void MainWindow::on_timerOut()
{
    if(!init)
    {
        QString topicStr = "monitor/topic"+ui->lineEdit_3->text();
        m_client->subscribe(topicStr,0);
        init = true;
        return;
    }
    if(!sus)
    {
        m_client->unsubscribe(ui->lineEdit_3->text());
        sus = true;
        timer.stop();
        timer.setInterval(50);
        timer.start();
    }
    else
    {
        QMqttSubscription* subscription = m_client->subscribe(ui->lineEdit_3->text(),0);
        qDebug()<<subscription->state();
        if(3 == subscription->state())
        {
            if(++errorCnt > 5)
            {
                sus = false;
            }
            return;
        }
        errorCnt = 0;
        sus = false;
        timer.stop();
        timer.setInterval(8000);
        timer.start();
    }
}
void MainWindow::on_msgRecv(const QByteArray &message, const QMqttTopicName &topic)
{
    QBuffer buffer((QByteArray *)&message);
    buffer.open(QIODevice::ReadOnly);
    QImageReader reader(&buffer,"JPG");
    QImage img = reader.read();
    if(!img.isNull())
    {
        QPixmap pix = QPixmap::fromImage(img);
        ui->label->setPixmap(pix.scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
}

void MainWindow::on_pushButton_2_clicked()
{
    if("进入房间" == ui->pushButton_2->text())
    {
        m_client->setHostname("mq.tongxinmao.com");
        m_client->setPort(18830);
        m_client->setCleanSession(true);
        m_client->connectToHost();
        init = false;
        ui->pushButton_2->setText("退出房间");
        ui->lineEdit_3->setEnabled(false);
        timer.setInterval(1000);
        timer.start();
    }
    else
    {
        ui->label->clear();
        m_client->disconnectFromHost();
        timer.stop();
        ui->pushButton_2->setText("进入房间");
        ui->lineEdit_3->setEnabled(true);
    }
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    if(ui->lineEdit_3->text() == "")
    {
        ui->pushButton_2->setEnabled(false);
    }else
    {

        ui->pushButton_2->setEnabled(true);
    }
}
