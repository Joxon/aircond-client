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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_power_pushButton_clicked();

    void on_turn_up_pushButton_clicked();

    void on_turn_down_pushButton_clicked();

    void on_low_pushButton_clicked();

    void on_high_pushButton_clicked();

    void loop();

    void natural_temp();

    void on_pushButton_clicked();

    void send_request(int type, QString roomID, int Switch, double temperature, int wind);
    void send_request_common(int type, QString roomID, double temperature);

    void readMessage();
    void displayError(QAbstractSocket::SocketError);

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;
    QByteArray message;
};

#endif // MAINWINDOW_H
