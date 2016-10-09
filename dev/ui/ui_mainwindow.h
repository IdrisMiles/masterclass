/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QScrollBar *verticalScrollBar;
    QLabel *label;
    QSlider *horizontalSlider;
    QLabel *label_3;
    QLabel *label_2;
    QSlider *horizontalSlider_2;
    QSlider *horizontalSlider_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QPushButton *s_btnRender;
    QSpacerItem *horizontalSpacer;
    QLabel *s_labelRenderView;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer_2;
    QLabel *s_labelQuickView;
    QPushButton *s_btnProcess;
    QPushButton *s_btnLoad;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(777, 531);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalScrollBar = new QScrollBar(groupBox);
        verticalScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
        verticalScrollBar->setOrientation(Qt::Vertical);

        gridLayout_3->addWidget(verticalScrollBar, 0, 2, 3, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        horizontalSlider = new QSlider(groupBox);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout_3->addWidget(horizontalSlider, 0, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_3->addWidget(label_3, 2, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        horizontalSlider_2 = new QSlider(groupBox);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        gridLayout_3->addWidget(horizontalSlider_2, 1, 1, 1, 1);

        horizontalSlider_3 = new QSlider(groupBox);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setOrientation(Qt::Horizontal);

        gridLayout_3->addWidget(horizontalSlider_3, 2, 1, 1, 1);


        gridLayout_2->addWidget(groupBox, 1, 0, 1, 2);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        s_btnRender = new QPushButton(groupBox_2);
        s_btnRender->setObjectName(QStringLiteral("s_btnRender"));

        gridLayout_4->addWidget(s_btnRender, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 1, 1, 1, 1);

        s_labelRenderView = new QLabel(groupBox_2);
        s_labelRenderView->setObjectName(QStringLiteral("s_labelRenderView"));
        s_labelRenderView->setStyleSheet(QStringLiteral("Background-color: #000;"));

        gridLayout_4->addWidget(s_labelRenderView, 0, 0, 1, 2);


        gridLayout_2->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_5 = new QGridLayout(groupBox_3);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        s_labelQuickView = new QLabel(groupBox_3);
        s_labelQuickView->setObjectName(QStringLiteral("s_labelQuickView"));
        s_labelQuickView->setStyleSheet(QStringLiteral("Background-color: #000;"));

        gridLayout_5->addWidget(s_labelQuickView, 0, 0, 1, 3);

        s_btnProcess = new QPushButton(groupBox_3);
        s_btnProcess->setObjectName(QStringLiteral("s_btnProcess"));

        gridLayout_5->addWidget(s_btnProcess, 1, 0, 1, 1);

        s_btnLoad = new QPushButton(groupBox_3);
        s_btnLoad->setObjectName(QStringLiteral("s_btnLoad"));

        gridLayout_5->addWidget(s_btnLoad, 1, 1, 1, 1);


        gridLayout_2->addWidget(groupBox_3, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 777, 19));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Video Input", 0));
        label->setText(QApplication::translate("MainWindow", "Video 1", 0));
        label_3->setText(QApplication::translate("MainWindow", "Video 3", 0));
        label_2->setText(QApplication::translate("MainWindow", "Video 2", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Final Render", 0));
        s_btnRender->setText(QApplication::translate("MainWindow", "Render", 0));
        s_labelRenderView->setText(QString());
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Quick View", 0));
        s_labelQuickView->setText(QString());
        s_btnProcess->setText(QApplication::translate("MainWindow", "Process", 0));
        s_btnLoad->setText(QApplication::translate("MainWindow", "Load", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
