#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtCore>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

class QTcpSocket;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_power_pushButton_clicked();
    void on_turn_up_pushButton_clicked();
    void on_turn_down_pushButton_clicked();
    void on_low_pushButton_clicked();
    void on_high_pushButton_clicked();
    void on_mid_pushButton_clicked();

    void loop();
    void natural_temp();
    void refrigerate();
    void heat();

    void send_request(int type, QString roomID, int Switch, double temperature, int wind);
    void send_request_common(int type, QString roomID, double temperature);

    void readMessage();
    void displayError(QAbstractSocket::SocketError);

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;
    QByteArray message;

    double cur_temperature;             //当前室温
    double outside_temperature = 28.0;  //室外温度恒定
    double set_temperature;             //设定温度
    bool is_on;                         //空调开关
    int wind;                           //0是无风，1是低档位，2高档位
    int last_wind;                      //记录风速的最新设置
    bool is_serving = false;            //是否有服务资源
    bool is_working = true;             //是否正在工作
    double cost     = 0.0;              //本次消费
    QString roomID;
    QString serverIP;
    quint16 serverPort;
};

#endif // MAINWINDOW_H
