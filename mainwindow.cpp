#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "quiwidget.h"

#include <QtCore>

#define MAX_SET_TEMPERATURE    30
#define MIN_SET_TEMPERATURE    17

#define NO_WIND                0
#define LOW_WIND               1
#define MID_WIND               2
#define HIGH_WIND              3

#define REQ_TYPE_CODE          0
#define COM_TYPE_CODE          1

#define SWITCH_ON              1
#define SWITCH_OFF             0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currentTemperature = 28.0;
    settingTemperature = 26.0;
    outsideTemperature = 28.0;

    isOn = false;
    wind = 0;

    ui->cur_temperature_label->setText(QString::number(currentTemperature));
    ui->set_temperature_label->setText("-");
    ui->state_label->setText(tr("关机"));

    loopTimer = new QTimer(this);
    connect(loopTimer, SIGNAL(timeout()), this, SLOT(loop()));
    loopTimer->start(1000);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_turn_up_pushButton_clicked()
{
    if (isOn && (settingTemperature < MAX_SET_TEMPERATURE))
    {
        settingTemperature++;

        sendRequestMsg(REQ_TYPE_CODE, roomID, SWITCH_ON, settingTemperature, lastWind);

        ui->set_temperature_label->setText(QString::number(settingTemperature));
    }
}


void MainWindow::on_turn_down_pushButton_clicked()
{
    if (isOn && (settingTemperature > MIN_SET_TEMPERATURE))
    {
        settingTemperature--;

        sendRequestMsg(REQ_TYPE_CODE, roomID, SWITCH_ON, settingTemperature, lastWind);

        ui->set_temperature_label->setText(QString::number(settingTemperature));
    }
}


void MainWindow::on_power_pushButton_clicked(bool checked)
{
    if (checked)
    {
        isOn = true;
        //初始化部分参数
        QString newID = ui->lineEditRoomID->text();
        if (newID.isEmpty())
        {
            QUIWidget::showMessageBoxError("房间号不能为空！");
            isOn = false;
            return;
        }
        roomID = newID;

        QString newIP = ui->lineEditServerIP->text();
        if (!QUIWidget::isIP(newIP))
        {
            QUIWidget::showMessageBoxError("IP不合法！");
            isOn = false;
            return;
        }
        serverIP = newIP;

        bool    newPortOK = false;
        quint16 newPort   = quint16(ui->lineEditServerPort->text().toInt(&newPortOK));
        if (!newPortOK || (newPort == 0))
        {
            QUIWidget::showMessageBoxError("端口不合法！");
            isOn = false;
            return;
        }
        serverPort = newPort;

        socket->connectToHost(serverIP, serverPort);

        if (!socket->isOpen())
        {
            socket->connectToHost(serverIP, serverPort);
        }

        settingTemperature = 26;
        lastWind           = LOW_WIND;
        wind      = LOW_WIND;
        isWorking = false;
        //需要一个请求服务的函数 返回给is_surving
        sendRequestMsg(REQ_TYPE_CODE, roomID, SWITCH_ON, settingTemperature, wind);

        //显示设置
        ui->set_temperature_label->setText(QString::number(settingTemperature));
        if (isServing)
        {
            ui->state_label->setText(tr("低风服务中"));
        }
        else
        {
            ui->state_label->setText(tr("等待服务"));
        }
    }
    else
    {
        isOn = false;
        sendRequestMsg(REQ_TYPE_CODE, roomID, SWITCH_OFF, -1.0, -1);
        isWorking = false;
        isServing = false;
        ui->set_temperature_label->setText("N/A");
        ui->state_label->setText(tr("关机"));
    }
}


void MainWindow::coolDown()
{
    switch (wind)
    {
    case HIGH_WIND:
        ui->state_label->setText(tr("强风服务中"));
        currentTemperature -= 0.2;
        break;

    case MID_WIND:
        ui->state_label->setText(tr("中风服务中"));
        currentTemperature -= 0.1;
        break;

    case LOW_WIND:
        ui->state_label->setText(tr("低风服务中"));
        currentTemperature -= 0.05;        break;

    default:
        qDebug() << DATETIME << "coolDown: unknown wind =" << wind;
        break;
    }
    if(currentTemperature<settingTemperature)
        currentTemperature=settingTemperature;
}


void MainWindow::heatUp()
{
    switch (wind)
    {
    case HIGH_WIND:
        currentTemperature += 0.2;
        ui->state_label->setText(tr("强风服务中"));
        break;

    case MID_WIND:
        currentTemperature += 0.1;
        ui->state_label->setText(tr("中风服务中"));
        break;

    case LOW_WIND:
        currentTemperature += 0.05;
        ui->state_label->setText(tr("低风服务中"));
        break;

    default:
        qDebug() << DATETIME << "heatUp: unknown wind =" << wind;
        break;
    }
    if(currentTemperature>settingTemperature)
        currentTemperature=settingTemperature;
}


void MainWindow::loop()
{
    if (isOn && !socket->isOpen())
    {
        ui->state_label->setText(tr("连接失败"));
        return;
    }

    //qDebug() << "wind=" << wind << endl;
    //qDebug() << "last_wind=" << last_wind << endl;
    //qDebug() << "is_on" << is_on << endl;
    if (isOn)//空调电源开
    {
        sendCommonMsg(1, roomID, currentTemperature);

        if (isServing)//服务端管理客户端
        {
            switch (wind)
            {
            case HIGH_WIND:
            case MID_WIND:
            case LOW_WIND:
                if (currentTemperature > settingTemperature)
                {
                    //制冷
                    coolDown();
                }
                else if (currentTemperature < settingTemperature)
                {
                    //制热
                    heatUp();
                }
                break;

            case NO_WIND:
                naturalTemp();
                ui->state_label->setText(tr("无风"));
                break;

            default:
                ui->state_label->setText(tr("未知风速") + (QString::number(wind)));
                break;
            }
            ui->cur_temperature_label->setText(QString::number(currentTemperature));
            ui->set_temperature_label->setText(QString::number(settingTemperature));
            ui->cost_label->setText(QString::number(cost));
        }
        else
        {
            ui->state_label->setText("等待服务中");
        }
    }
    else
    {
        naturalTemp();
        ui->cur_temperature_label->setText(QString::number(currentTemperature));
        ui->state_label->setText(tr("关机"));
        ui->cost_label->setText("-");
    }
//   ui->cost_label->setText(QString::number(cost));
//   ui->cur_temperature_label->setText(QString::number(cur_temperature));
}


void MainWindow::naturalTemp()
{
    if (currentTemperature - outsideTemperature > 0.05)
    {
        currentTemperature -= 0.05;
    }
    else if (currentTemperature - outsideTemperature < -0.05)
    {
        currentTemperature += 0.05;
    }
    else
    {
        currentTemperature = outsideTemperature;
    }
}


void MainWindow::on_low_pushButton_clicked()
{
    if (isOn)
    {
        wind     = LOW_WIND;
        lastWind = LOW_WIND;

        sendRequestMsg(REQ_TYPE_CODE, roomID, SWITCH_ON, settingTemperature, lastWind);

        ui->state_label->setText(tr("低风服务中"));
    }
}


void MainWindow::on_high_pushButton_clicked()
{
    if (isOn)
    {
        wind     = HIGH_WIND;
        lastWind = HIGH_WIND;

        sendRequestMsg(REQ_TYPE_CODE, roomID, SWITCH_ON, settingTemperature, lastWind);

        ui->state_label->setText(tr("强风服务中"));
    }
}


void MainWindow::on_mid_pushButton_clicked()
{
    if (isOn)
    {
        wind     = MID_WIND;
        lastWind = MID_WIND;

        sendRequestMsg(REQ_TYPE_CODE, roomID, SWITCH_ON, settingTemperature, lastWind);

        ui->state_label->setText(tr("中风服务中"));
    }
}


void MainWindow::readMessage()
{
    QJsonParseError err;
    QByteArray      bytes = socket->readAll();
    QJsonDocument   doc   = QJsonDocument::fromJson(bytes, &err);

    if (err.error == QJsonParseError::NoError)
    {
        if (doc.isObject())
        {
            QJsonObject obj = doc.object();
            if (obj.contains("type"))
            {
                QJsonValue type_value = obj.take("type");
                if (type_value.isDouble())
                {
                    int type = type_value.toInt();
                    qDebug() << "type:" << type;
                }
            }
            if (obj.contains("switch"))
            {
                QJsonValue switch_value = obj.take("switch");
                if (switch_value.isDouble())
                {
                    int Switch = switch_value.toInt();
                    isServing = Switch;
                    qDebug() << "switch:" << Switch;
                }
            }
            if (obj.contains("wind"))
            {
                QJsonValue wind_value = obj.take("wind");
                if (wind_value.isDouble())
                {
                    wind = wind_value.toInt();
                    qDebug() << "wind:" << wind;
                }
            }
            if (obj.contains("temperature"))
            {
                QJsonValue temperature_value = obj.take("temperature");
                if (temperature_value.isDouble())
                {
                    double temperature = temperature_value.toDouble();
                    qDebug() << "temperature:" << temperature;
                    if (!qFuzzyCompare(temperature, settingTemperature))//temperature != set_temperature
                    {
                        sendRequestMsg(REQ_TYPE_CODE, roomID, SWITCH_ON, settingTemperature, lastWind);
                    }
                }
            }
            if (obj.contains("cost"))
            {
                QJsonValue cost_value = obj.take("cost");
                if (cost_value.isDouble())
                {
                    cost = cost_value.toDouble();
                    ui->cost_label->setText(QString::number(cost));
                    qDebug() << "cost:" << cost;
                }
            }
            if (obj.contains("isServed"))
            {
                QJsonValue isServing_value = obj.take("isServed");
                if (isServing_value.isDouble())
                {
                    isServing = isServing_value.toInt();
                    qDebug() << "isServed:" << isServing;
                }
            }
        }
    }
}


void MainWindow::sendRequestMsg(int type, QString roomID, int Switch, double temperature, int wind)
{
    qDebug() << DATETIME << "send_request:"
             << "cur_temperature" << currentTemperature
             << "type" << type
             << "room" << roomID
             << "switch" << Switch
             << "temperture" << temperature
             << "wind" << wind;

    QJsonObject   json;
    QJsonDocument document;
    QByteArray    bytes;

    json.insert("type", type);
    json.insert("room", roomID);
    json.insert("switch", Switch);
    json.insert("temperature", temperature);
    json.insert("wind", wind);

    document.setObject(json);
    bytes = document.toJson(QJsonDocument::Compact);

    socket->flush();
    socket->write(bytes);
}


void MainWindow::sendCommonMsg(int type, QString roomID, double temperature)
{
    qDebug() << DATETIME << "send_request_commom:"
             << "type =" << type
             << "room =" << roomID
             << "temperture =" << temperature;

    QJsonObject   json;
    QJsonDocument document;
    QByteArray    bytes;

    json.insert("type", type);
    json.insert("room", roomID);
    json.insert("temperature", temperature);

    document.setObject(json);
    bytes = document.toJson(QJsonDocument::Compact);

    socket->flush();
    socket->write(bytes);
}
