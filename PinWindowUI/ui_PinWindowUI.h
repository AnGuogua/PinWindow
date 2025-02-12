/********************************************************************************
** Form generated from reading UI file 'PinWindowUI.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PINWINDOWUI_H
#define UI_PINWINDOWUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PinWindowUIClass
{
public:
    QWidget *centralWidget;
    QLabel *label_2;
    QLineEdit *pID;
    QLineEdit *style;
    QTextEdit *log;
    QLabel *label_9;
    QLabel *label_5;
    QLabel *label_3;
    QComboBox *comboBox;
    QFrame *line;
    QLabel *label;
    QLabel *label_6;
    QLineEdit *HWND;
    QLineEdit *title;
    QLabel *label_7;
    QPlainTextEdit *plainTextEdit;
    QLabel *label_8;
    QLineEdit *wclass;
    QPushButton *pushButton;
    QLineEdit *pName;
    QLineEdit *rect;
    QLabel *label_4;
    QTableWidget *tableWidget;
    QLabel *label_10;
    QComboBox *comboBox_2;
    QLineEdit *zbid;
    QLabel *label_11;
    QPushButton *refresh;
    QPushButton *pushButton_2;

    void setupUi(QMainWindow *PinWindowUIClass)
    {
        if (PinWindowUIClass->objectName().isEmpty())
            PinWindowUIClass->setObjectName("PinWindowUIClass");
        PinWindowUIClass->resize(762, 445);
        PinWindowUIClass->setMinimumSize(QSize(762, 445));
        PinWindowUIClass->setMaximumSize(QSize(762, 445));
        centralWidget = new QWidget(PinWindowUIClass);
        centralWidget->setObjectName("centralWidget");
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(200, 30, 54, 16));
        pID = new QLineEdit(centralWidget);
        pID->setObjectName("pID");
        pID->setGeometry(QRect(70, 60, 113, 20));
        pID->setReadOnly(true);
        style = new QLineEdit(centralWidget);
        style->setObjectName("style");
        style->setGeometry(QRect(250, 90, 161, 20));
        style->setReadOnly(true);
        log = new QTextEdit(centralWidget);
        log->setObjectName("log");
        log->setGeometry(QRect(430, 300, 321, 131));
        log->setLineWrapMode(QTextEdit::LineWrapMode::WidgetWidth);
        log->setReadOnly(true);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 120, 54, 16));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(430, 10, 54, 20));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 60, 54, 16));
        comboBox = new QComboBox(centralWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(480, 10, 261, 22));
        line = new QFrame(centralWidget);
        line->setObjectName("line");
        line->setGeometry(QRect(410, 10, 20, 431));
        line->setFrameShape(QFrame::Shape::VLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);
        label = new QLabel(centralWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 30, 54, 16));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(430, 280, 54, 16));
        HWND = new QLineEdit(centralWidget);
        HWND->setObjectName("HWND");
        HWND->setGeometry(QRect(70, 30, 113, 20));
        HWND->setReadOnly(true);
        title = new QLineEdit(centralWidget);
        title->setObjectName("title");
        title->setGeometry(QRect(250, 30, 161, 20));
        title->setReadOnly(true);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 90, 54, 16));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setGeometry(QRect(430, 80, 321, 171));
        plainTextEdit->setReadOnly(true);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(200, 90, 54, 16));
        wclass = new QLineEdit(centralWidget);
        wclass->setObjectName("wclass");
        wclass->setGeometry(QRect(70, 90, 113, 20));
        wclass->setReadOnly(true);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(670, 260, 75, 24));
        pName = new QLineEdit(centralWidget);
        pName->setObjectName("pName");
        pName->setGeometry(QRect(250, 60, 161, 20));
        pName->setReadOnly(true);
        rect = new QLineEdit(centralWidget);
        rect->setObjectName("rect");
        rect->setGeometry(QRect(70, 120, 171, 20));
        rect->setReadOnly(true);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(200, 60, 54, 16));
        tableWidget = new QTableWidget(centralWidget);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(10, 190, 401, 241));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(430, 40, 54, 20));
        comboBox_2 = new QComboBox(centralWidget);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(480, 40, 261, 22));
        zbid = new QLineEdit(centralWidget);
        zbid->setObjectName("zbid");
        zbid->setGeometry(QRect(70, 150, 171, 20));
        zbid->setReadOnly(true);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(20, 150, 54, 16));
        refresh = new QPushButton(centralWidget);
        refresh->setObjectName("refresh");
        refresh->setGeometry(QRect(354, 160, 51, 24));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(590, 260, 75, 24));
        PinWindowUIClass->setCentralWidget(centralWidget);

        retranslateUi(PinWindowUIClass);

        QMetaObject::connectSlotsByName(PinWindowUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *PinWindowUIClass)
    {
        PinWindowUIClass->setWindowTitle(QCoreApplication::translate("PinWindowUIClass", "PinWindowUI", nullptr));
        label_2->setText(QCoreApplication::translate("PinWindowUIClass", "\346\240\207\351\242\230\357\274\232", nullptr));
        label_9->setText(QCoreApplication::translate("PinWindowUIClass", "\345\244\247\345\260\217\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("PinWindowUIClass", "\347\252\227\345\217\243\346\256\265\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("PinWindowUIClass", "\350\277\233\347\250\213ID\357\274\232", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("PinWindowUIClass", "ZBID_DESKTOP", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("PinWindowUIClass", "ZBID_IMMERSIVE_BACKGROUND", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("PinWindowUIClass", "ZBID_IMMERSIVE_INACTIVEMOBODY", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("PinWindowUIClass", "ZBID_IMMERSIVE_NOTIFICATION", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("PinWindowUIClass", "ZBID_IMMERSIVE_APPCHROME", nullptr));
        comboBox->setItemText(5, QCoreApplication::translate("PinWindowUIClass", "ZBID_IMMERSIVE_MOGO", nullptr));
        comboBox->setItemText(6, QCoreApplication::translate("PinWindowUIClass", "ZBID_IMMERSIVE_EDGY", nullptr));
        comboBox->setItemText(7, QCoreApplication::translate("PinWindowUIClass", "ZBID_SYSTEM_TOOLS", nullptr));
        comboBox->setItemText(8, QCoreApplication::translate("PinWindowUIClass", "ZBID_LOCK", nullptr));
        comboBox->setItemText(9, QCoreApplication::translate("PinWindowUIClass", "ZBID_ABOVELOCK_UX", nullptr));
        comboBox->setItemText(10, QCoreApplication::translate("PinWindowUIClass", "ZBID_IMMERSIVE_IHM", nullptr));
        comboBox->setItemText(11, QCoreApplication::translate("PinWindowUIClass", "ZBID_GENUINE_WINDOWS", nullptr));
        comboBox->setItemText(12, QCoreApplication::translate("PinWindowUIClass", "ZBID_UIACCESS", nullptr));

        label->setText(QCoreApplication::translate("PinWindowUIClass", "\345\217\245\346\237\204\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("PinWindowUIClass", "\346\227\245\345\277\227", nullptr));
        label_7->setText(QCoreApplication::translate("PinWindowUIClass", "\347\261\273\357\274\232", nullptr));
        plainTextEdit->setPlainText(QCoreApplication::translate("PinWindowUIClass", "ZBID_DESKTOP\357\274\232\346\231\256\351\200\232\347\250\213\345\272\217\346\211\200\345\234\250\347\252\227\345\217\243\346\256\265\n"
"ZBID_IMMERSIVE_MOGO\357\274\232\345\274\200\345\247\213\345\261\217\345\271\225\345\222\214\344\273\273\345\212\241\346\240\217\n"
"ZBID_IMMERSIVE_NOTIFICATIONS\357\274\232\346\223\215\344\275\234\344\270\255\345\277\203\345\222\214\351\200\232\347\237\245\n"
"ZBID_SYSTEM_TOOLS\357\274\232Alt+Tab\350\247\206\345\233\276\357\274\214\344\273\273\345\212\241\347\256\241\347\220\206\345\231\250\n"
"ZBID_ABOVELOCK_UX\357\274\232\345\215\263\346\227\266\346\222\255\346\224\276\n"
"ZBID_IMMERSIVE_SEARCH\357\274\232Cortana\345\222\214\346\220\234\347\264\242\n"
"ZBID_IMMERSIVE_INACTIVEMOBODY\357\274\232UWP CompactOverlay\n"
"ZBID_IMMERSIVE_APPCHROME\357\274\232\344\273\273\345\212\241\350\247\206\345\233\276\n"
"ZBID_GENUINE_WINDOWS\357\274\232\342\200\234\346\277\200\346\264\273Windows\342\200\235", nullptr));
        label_8->setText(QCoreApplication::translate("PinWindowUIClass", "\346\240\267\345\274\217\357\274\232", nullptr));
        pushButton->setText(QCoreApplication::translate("PinWindowUIClass", "\350\256\276\347\275\256\347\252\227\345\217\243\346\256\265", nullptr));
        label_4->setText(QCoreApplication::translate("PinWindowUIClass", "\350\277\233\347\250\213\345\220\215\357\274\232", nullptr));
        label_10->setText(QCoreApplication::translate("PinWindowUIClass", "Z\345\272\217\346\256\265\357\274\232", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("PinWindowUIClass", "HWND_BOTTOM", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("PinWindowUIClass", "HWND_TOP", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("PinWindowUIClass", "HWND_NOTOPMOST", nullptr));
        comboBox_2->setItemText(3, QCoreApplication::translate("PinWindowUIClass", "HWND_TOPMOST", nullptr));

        zbid->setText(QString());
        label_11->setText(QCoreApplication::translate("PinWindowUIClass", "\347\252\227\345\217\243\346\256\265\357\274\232", nullptr));
        refresh->setText(QCoreApplication::translate("PinWindowUIClass", "\345\210\267\346\226\260", nullptr));
        pushButton_2->setText(QCoreApplication::translate("PinWindowUIClass", "\345\205\263\344\272\216", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PinWindowUIClass: public Ui_PinWindowUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PINWINDOWUI_H
