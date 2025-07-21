#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <utility>

inline std::pair<QString, QString> parkingdialog(QWidget *parent) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle("审核车位");

    QLabel *titleLabel = new QLabel("请输入审核的车位ID:", &msgBox);
    QLineEdit *lineEdit = new QLineEdit(&msgBox);
    lineEdit->setPlaceholderText("输入车位ID...");
    lineEdit->setFocus();

    QPushButton *okButton = msgBox.addButton("通过", QMessageBox::AcceptRole);
    okButton->setObjectName("okButton");

    QPushButton *rejectButton = msgBox.addButton("拒绝", QMessageBox::ActionRole);
    rejectButton->setObjectName("rejectButton");

    QPushButton *cancelButton = msgBox.addButton("取消", QMessageBox::RejectRole);
    cancelButton->setObjectName("cancelButton");

    QGridLayout *layout = qobject_cast<QGridLayout*>(msgBox.layout());
    if (layout) {
        layout->addWidget(titleLabel, 0, 0, 1, 3);
        layout->addWidget(lineEdit, 1, 0, 1, 3);
        layout->addWidget(okButton, 2, 0);
        layout->addWidget(rejectButton, 2, 1);
        layout->addWidget(cancelButton, 2, 2);
    }

    msgBox.setStyleSheet(R"(
        QMessageBox {
            background: white;
            border-radius: 8px;
            padding: 15px;
            font-size: 14px;
            min-width: 300px;
        }
        QLabel {
            color: #333;
            font-weight: bold;
            margin-bottom: 10px;
        }
        QLineEdit {
            border: 1px solid #ccc;
            border-radius: 4px;
            padding: 8px;
            min-height: 25px;
            margin-bottom: 15px;
        }
        QLineEdit:focus {
            border-color: #1e88e5;
        }
        QPushButton {
            background: #f5f5f5;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 6px 15px;
            min-width: 70px;
        }
        QPushButton:hover {
            background: #e3f2fd;
        }
        QPushButton#okButton {
            background: darkblue;
            color: white;
        }
        QPushButton#rejectButton {
            background: lightblue;
            color: darkblue;
        }
        QPushButton#cancelButton {
            background: #f44336;
            color: white;
        }
    )");

    msgBox.exec();

    QAbstractButton *clickedButton = msgBox.clickedButton();
    QString input = lineEdit->text().trimmed();

    if (clickedButton == okButton) {
        return std::make_pair("approved", input);
    } else if (clickedButton == rejectButton) {
        return std::make_pair("rejected", input);
    } else {
        return std::make_pair("", "");
    }
}
