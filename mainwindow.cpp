#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "quiwidget.h"

#define MAX_SET_TEMPERATURE    30
#define MIN_SET_TEMPERATURE    17
#define NO_WIND                0
#define LOW_WIND               1
#define MID_WIND               2
#define HIGH_WIND              3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cur_temperature = 28.0;
    set_temperature = 26.0;
    is_on           = false;
    wind            = 0;

    ui->cur_temperature_label->setText(QString::number(cur_temperature));
    ui->set_temperature_label->setText("-");
    ui->state_label->setText(tr("关机"));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop())); //与定时器槽函数相连接
    timer->start(1000);

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_turn_up_pushButton_clicked()
{
    if (is_on && (set_temperature < MAX_SET_TEMPERATURE))
    {
        set_temperature++;
        send_request(0, roomID, 1, set_temperature, last_wind);

        ui->set_temperature_label->setText(QString::number(set_temperature));
    }
}


void MainWindow::on_turn_down_pushButton_clicked()
{
    if (is_on && (set_temperature > MIN_SET_TEMPERATURE))
    {
        set_temperature--;

        send_request(0, roomID, 1, set_temperature, last_wind);

        ui->set_temperature_label->setText(QString::number(set_temperature));
    }
}


void MainWindow::on_power_pushButton_clicked()
{
    is_on = !is_on;

    if (is_on)
    {
        QString newID = ui->lineEditRoomID->text();
        if (newID.isEmpty())
        {
            QUIWidget::showMessageBoxError("房间号不能为空！");
            is_on = false;
            return;
        }
        roomID = newID;

        QString newIP = ui->lineEditServerIP->text();
        if (!QUIWidget::isIP(newIP))
        {
            QUIWidget::showMessageBoxError("IP不合法！");
            is_on = false;
            return;
        }
        serverIP = newIP;

        bool    newPortOK = false;
        quint16 newPort   = ui->lineEditServerPort->text().toInt(&newPortOK);
        if (!newPortOK || (newPort == 0))
        {
            QUIWidget::showMessageBoxError("端口不合法！");
            is_on = false;
            return;
        }
        serverPort = newPort;

        tcpSocket->connectToHost(serverIP, serverPort);

        set_temperature = 26;
        last_wind       = LOW_WIND;
        wind            = LOW_WIND;
        is_working      = false;
        //需要一个请求服务的函数 返回给is_serving
        send_request(0, roomID, 1, set_temperature, wind);

        //显示设置
        ui->set_temperature_label->setText(QString::number(set_temperature));
        if (is_serving)
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
        send_request(0, roomID, 0, -1, -1);
        is_working = false;
        is_serving = false;
        ui->set_temperature_label->setText("-");
        ui->state_label->setText(tr("关机"));
    }
}


void MainWindow::refrigerate()
{
    switch (wind)
    {
    case HIGH_WIND:
        cur_temperature -= 0.2;
        ui->state_label->setText(tr("强风服务中"));
        break;

    case MID_WIND:
        cur_temperature -= 0.1;
        ui->state_label->setText(tr("中风服务中"));
        break;

    case LOW_WIND:
        cur_temperature -= 0.05;
        ui->state_label->setText(tr("低风服务中"));
        break;

    default:
        qDebug() << "refrigerate err";
        break;
    }
}


void MainWindow::heat()
{
    switch (wind)
    {
    case HIGH_WIND:
        cur_temperature += 0.2;
        ui->state_label->setText(tr("强风服务中"));
        break;

    case MID_WIND:
        cur_temperature += 0.1;
        ui->state_label->setText(tr("中风服务中"));
        break;

    case LOW_WIND:
        cur_temperature += 0.05;
        ui->state_label->setText(tr("低风服务中"));
        break;

    default:
        qDebug() << "refrigerate err";
        break;
    }
}


void MainWindow::loop()
{
    if (is_on && !tcpSocket->isOpen())
    {
        ui->state_label->setText(tr("连接失败"));
        return;
    }


//    qDebug() << "wind=" << wind << endl
//             << "last_wind=" << last_wind << endl
//             << "is_on=" << is_on << endl;
    if (is_on)//空调电源开
    {
        send_request_common(1, roomID, cur_temperature);

        if (is_serving)//服务端管理客户端
        {
            switch (wind)
            {
            case HIGH_WIND:
            case MID_WIND:
            case LOW_WIND:
                if (cur_temperature > set_temperature)
                {
                    //制冷
                    refrigerate();
                }
                else
                {
                    //制热
                    heat();
                }
                break;

            case NO_WIND:
                natural_temp();
                ui->state_label->setText(tr("无风"));
                break;

            default:
                ui->state_label->setText(tr("wind=") + wind);
                break;
            }
            ui->cur_temperature_label->setText(QString::number(cur_temperature));
            ui->set_temperature_label->setText(QString::number(set_temperature));
            ui->cost_label->setText(QString::number(cost));
        }
        else
        {
            ui->state_label->setText("等待服务中");
        }
    }
    else
    {
        ui->state_label->setText(tr("关机"));
        ui->cost_label->setText("-");
    }

    /*
     * if ((cur_temperature != set_temperature) || ((cur_temperature == set_temperature) && is_working))
     * {
     *  if (is_serving) //如果有资源
     *  {
     *      if (!is_working && (fabs(cur_temperature - set_temperature) > 0.99))
     *      {
     *          //qDebug() << "start working" << endl;
     *          is_working = true;
     *          send_request(0, roomID, 1, set_temperature, last_wind);
     *      }
     *
     *      if ((cur_temperature > set_temperature) &&
     *          (((cur_temperature - set_temperature < 1) && is_working) ||
     *           (cur_temperature - set_temperature >= 1))) //制冷
     *      {
     *          //qDebug() << "zhi leng" << endl;
     *
     *          wind = last_wind;
     *
     *          if (is_working && (((cur_temperature - set_temperature < 0.11) && (last_wind == LOW_WIND)) ||
     *                             ((cur_temperature - set_temperature < 0.21) && (last_wind == HIGH_WIND))))
     *          {
     *              //qDebug() << "stop working" << endl;
     *              is_working = false;
     *              send_request(0, roomID, 1, set_temperature, 0);
     *          }
     *          if (last_wind == 1) //低档
     *          {
     *              cur_temperature -= 0.1;
     *          }
     *          else if (last_wind == 2) //高档
     *          {
     *              if (cur_temperature - set_temperature >= 0.2)
     *              {
     *                  cur_temperature -= 0.2;
     *              }
     *              else
     *              {
     *                  cur_temperature = set_temperature;
     *              }
     *          }
     *          else
     *          {
     *              qDebug() << "refrigerating error" << endl;
     *          }
     *      }
     *      else if ((cur_temperature < set_temperature) &&
     *               (((cur_temperature - set_temperature > -1) && is_working) ||
     *                (cur_temperature - set_temperature <= -1)))//制热
     *      {
     *          //qDebug() << "gao dang" << endl;
     *
     *          wind = last_wind;
     *
     *          if (is_working && (((cur_temperature - set_temperature > -0.11) && (last_wind == LOW_WIND)) ||
     *                             ((cur_temperature - set_temperature > -0.21) && (last_wind == HIGH_WIND))))
     *          {
     *              is_working = false;
     *              send_request(0, roomID, 1, set_temperature, 0);
     *          }
     *
     *          if (last_wind == 1) //低档
     *          {
     *              cur_temperature += 0.1;
     *          }
     *          else if (last_wind == 2) //高档
     *          {
     *              if (set_temperature - cur_temperature >= 0.2)
     *              {
     *                  cur_temperature += 0.2;
     *              }
     *              else
     *              {
     *                  cur_temperature = set_temperature;
     *              }
     *          }
     *          else
     *          {
     *              qDebug() << "heating error" << endl;
     *          }
     *      }
     *      else
     *      {
     *          wind = NO_WIND;
     *          natural_temp();
     *      }
     *      if (wind == LOW_WIND)
     *      {
     *          ui->state_label->setText("低风服务中");
     *      }
     *      else if (wind == HIGH_WIND)
     *      {
     *          ui->state_label->setText("强风服务中");
     *      }
     *      else
     *      {
     *          ui->state_label->setText("无风");
     *      }
     *  }
     *  else if (wind == 0)
     *  {
     *      natural_temp();
     *      if (!is_working && (cur_temperature - set_temperature >= 0.89))
     *      {
     *          qDebug() << "start working" << endl;
     *          is_working = true;
     *          send_request(0, roomID, 1, set_temperature, last_wind);
     *      }
     *  }
     *  else
     *  {
     *      ui->state_label->setText("无风");
     *  }
     * }
     * else //空调不工作
     * {
     *  send_request_common(1, roomID, cur_temperature);
     *  natural_temp();
     * }
     * //        qDebug() << "is_working=" << is_working << endl
     * //                 << "is_serving=" << is_serving << endl;
     * }
     * else //空调电源关，按照自然温度函数改变
     * {
     * natural_temp();
     * }*/
//   ui->cost_label->setText(QString::number(cost));
//   ui->cur_temperature_label->setText(QString::number(cur_temperature));
}


void MainWindow::natural_temp()
{
    if (cur_temperature - outside_temperature > 0.1)
    {
        cur_temperature -= 0.1;
    }
    else if (cur_temperature - outside_temperature < -0.1)
    {
        cur_temperature += 0.1;
    }
    else
    {
        cur_temperature = outside_temperature;
    }
}


void MainWindow::on_low_pushButton_clicked()
{
    if (is_on)
    {
        wind      = LOW_WIND;
        last_wind = LOW_WIND;
        send_request(0, roomID, 1, set_temperature, wind);

        ui->state_label->setText(tr("低风服务中"));
    }
}


void MainWindow::on_high_pushButton_clicked()
{
    if (is_on)
    {
        wind      = HIGH_WIND;
        last_wind = HIGH_WIND;

        send_request(0, roomID, 1, set_temperature, last_wind);

        ui->state_label->setText(tr("强风服务中"));
    }
}


void MainWindow::on_mid_pushButton_clicked()
{
    if (is_on)
    {
        wind      = MID_WIND;
        last_wind = MID_WIND;

        send_request(0, roomID, 1, set_temperature, last_wind);

        ui->state_label->setText(tr("中风服务中"));
    }
}


void MainWindow::readMessage()
{
    int             Switch;
    double          temperature;
    int             type;
    bool            isServing;
    QJsonParseError json_error;

    //double cost;
    message = tcpSocket->readAll();

    qDebug() << "readfromMessage";
    //qDebug() << "message" << message << endl;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(message, &json_error);
    if (json_error.error == QJsonParseError::NoError)
    {
        if (parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if (obj.contains("type"))
            {
                QJsonValue type_value = obj.take("type");
                if (type_value.isDouble())
                {
                    type = type_value.toInt();
                    qDebug() << "type:" << type;
                }
            }
            if (obj.contains("switch"))
            {
                QJsonValue switch_value = obj.take("switch");
                if (switch_value.isDouble())
                {
                    Switch     = switch_value.toInt();
                    is_serving = Switch;
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
                    temperature = temperature_value.toDouble();
                    qDebug() << "temperature:" << temperature;
                }
            }
            if (obj.contains("cost"))
            {
                QJsonValue cost_value = obj.take("cost");
                if (cost_value.isDouble())
                {
                    cost = cost_value.toDouble();
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

    qDebug() << "read_end" << endl;
    ui->cost_label->setText(QString::number(cost));
}


void MainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
//    ui->messageLabel->setText("连接失败");
//    ui->connectBtn->setEnabled(true);
//    killTimer(refresh);
}


void MainWindow::send_request(int type, QString roomID, int Switch, double temperature, int wind)
{
    QJsonObject   json;
    QJsonDocument document;
    QByteArray    byte_array;

    json.insert("type", type);
    json.insert("room", roomID);
    json.insert("switch", Switch);
    json.insert("temperature", temperature);
    json.insert("wind", wind);

    document.setObject(json);
    byte_array = document.toJson(QJsonDocument::Compact);

    tcpSocket->flush();
    tcpSocket->write(byte_array);
//    qDebug() << "send_request";
//    qDebug() << "cur_temperature" << cur_temperature;
//    qDebug() << "type" << type
//             << " room" << roomID
//             << " switch" << Switch
//             << " temperture" << temperature
//             << " wind" << wind;
}


void MainWindow::send_request_common(int type, QString roomID, double temperature)
{
    QJsonObject   json;
    QJsonDocument document;
    QByteArray    byte_array;

    json.insert("type", type);
    json.insert("room", roomID);
    json.insert("temperature", temperature);

    document.setObject(json);

    byte_array = document.toJson(QJsonDocument::Compact);
    tcpSocket->flush();
    tcpSocket->write(byte_array);
}
