/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *cur_temperature_label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *set_temperature_label;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_1;
    QLabel *state_label;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QLabel *cost_label;
    QPushButton *power_pushButton;
    QVBoxLayout *verticalLayout;
    QPushButton *turn_up_pushButton;
    QPushButton *turn_down_pushButton;
    QPushButton *low_pushButton;
    QPushButton *high_pushButton;
    QPushButton *pushButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(339, 283);
        MainWindow->setMinimumSize(QSize(0, 0));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_3 = new QHBoxLayout(centralWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        cur_temperature_label = new QLabel(centralWidget);
        cur_temperature_label->setObjectName(QStringLiteral("cur_temperature_label"));

        gridLayout->addWidget(cur_temperature_label, 0, 1, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        set_temperature_label = new QLabel(centralWidget);
        set_temperature_label->setObjectName(QStringLiteral("set_temperature_label"));

        gridLayout->addWidget(set_temperature_label, 1, 1, 1, 1);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 2, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_1 = new QLabel(centralWidget);
        label_1->setObjectName(QStringLiteral("label_1"));

        horizontalLayout_2->addWidget(label_1);

        state_label = new QLabel(centralWidget);
        state_label->setObjectName(QStringLiteral("state_label"));

        horizontalLayout_2->addWidget(state_label);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);

        cost_label = new QLabel(centralWidget);
        cost_label->setObjectName(QStringLiteral("cost_label"));

        horizontalLayout->addWidget(cost_label);


        verticalLayout_2->addLayout(horizontalLayout);

        power_pushButton = new QPushButton(centralWidget);
        power_pushButton->setObjectName(QStringLiteral("power_pushButton"));

        verticalLayout_2->addWidget(power_pushButton);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        turn_up_pushButton = new QPushButton(centralWidget);
        turn_up_pushButton->setObjectName(QStringLiteral("turn_up_pushButton"));

        verticalLayout->addWidget(turn_up_pushButton);

        turn_down_pushButton = new QPushButton(centralWidget);
        turn_down_pushButton->setObjectName(QStringLiteral("turn_down_pushButton"));

        verticalLayout->addWidget(turn_down_pushButton);

        low_pushButton = new QPushButton(centralWidget);
        low_pushButton->setObjectName(QStringLiteral("low_pushButton"));

        verticalLayout->addWidget(low_pushButton);

        high_pushButton = new QPushButton(centralWidget);
        high_pushButton->setObjectName(QStringLiteral("high_pushButton"));

        verticalLayout->addWidget(high_pushButton);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);


        horizontalLayout_3->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\345\275\223\345\211\215\346\270\251\345\272\246\357\274\232", Q_NULLPTR));
        cur_temperature_label->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "\342\204\203", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256\346\270\251\345\272\246\357\274\232", Q_NULLPTR));
        set_temperature_label->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "\342\204\203", Q_NULLPTR));
        label_1->setText(QApplication::translate("MainWindow", "\345\275\223\345\211\215\347\212\266\346\200\201\357\274\232", Q_NULLPTR));
        state_label->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "\346\234\254\346\254\241\346\266\210\350\264\271", Q_NULLPTR));
        cost_label->setText(QString());
        power_pushButton->setText(QApplication::translate("MainWindow", "\345\274\200\345\205\263", Q_NULLPTR));
        turn_up_pushButton->setText(QApplication::translate("MainWindow", "\342\206\221", Q_NULLPTR));
        turn_down_pushButton->setText(QApplication::translate("MainWindow", "\342\206\223", Q_NULLPTR));
        low_pushButton->setText(QApplication::translate("MainWindow", "\344\275\216\346\241\243\351\243\216\351\200\237", Q_NULLPTR));
        high_pushButton->setText(QApplication::translate("MainWindow", "\351\253\230\346\241\243\351\243\216\351\200\237", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "\346\234\215\345\212\241\346\250\241\346\213\237", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
