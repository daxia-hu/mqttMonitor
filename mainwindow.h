#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMqttClient>
#include <QImage>
#include <QImageReader>
#include <QBuffer>
#include <QMessageBox>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_msgRecv(const QByteArray &message, const QMqttTopicName &topic);
    void on_timerOut();
    void on_lineEdit_3_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QMqttClient *m_client;
    QTimer timer;
    bool sus;
    int errorCnt;
    bool init;
};
#endif // MAINWINDOW_H
