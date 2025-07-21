#ifndef SHOWCUSTOMINPUTDIALOG_H
#define SHOWCUSTOMINPUTDIALOG_H

#endif // SHOWCUSTOMINPUTDIALOG_H
#include <utility>

inline std::pair<bool, QString> showCustomInputDialog(QWidget *parent) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle("修改数据");

    QLabel *titleLabel = new QLabel("请输入修改内容:", &msgBox);
    QLineEdit *lineEdit = new QLineEdit(&msgBox);
    lineEdit->setPlaceholderText("输入新内容...");
    lineEdit->setFocus();

    QPushButton *okButton = msgBox.addButton("确定", QMessageBox::AcceptRole);
    okButton->setObjectName("okButton");

    QPushButton *cancelButton = msgBox.addButton("取消", QMessageBox::RejectRole);
    cancelButton->setObjectName("cancelButton");

    QGridLayout *layout = qobject_cast<QGridLayout*>(msgBox.layout());
    if (layout) {
        layout->addWidget(titleLabel, 0, 0, 1, 2);
        layout->addWidget(lineEdit, 1, 0, 1, 2);
        layout->addWidget(okButton, 2, 0);
        layout->addWidget(cancelButton, 2, 1);
    }

    msgBox.setStyleSheet(R"(
        QMessageBox {
            background: white;
            border-radius: 8px;
            padding: 15px;
            font-size: 14px;
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
        QPushButton#cancelButton {
            background: lightblue;
            color: blue;
        }
    )");

    return (msgBox.exec() == QMessageBox::AcceptRole)
        ? std::make_pair(true, lineEdit->text())
        : std::make_pair(false, QString());
}


