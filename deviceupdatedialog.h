#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <utility>

struct twoedit {
    int flag;
    QString st1;
    QString st2;
};

twoedit showme1(QWidget *parent) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle("更新设备状态");
    msgBox.setMinimumSize(400, 250);

    QLabel *idLabel = new QLabel("设备ID:", &msgBox);
    QLineEdit *idEdit = new QLineEdit(&msgBox);
    idEdit->setPlaceholderText("输入设备编号...");
    idEdit->setFocus();

    QLabel *remarksLabel = new QLabel("新的状态:", &msgBox);
    QLineEdit *remarksEdit = new QLineEdit(&msgBox);
    remarksEdit->setPlaceholderText("输入设备状态...");

    QPushButton *approveButton = msgBox.addButton("确认", QMessageBox::AcceptRole);
    approveButton->setObjectName("approveButton");

    QPushButton *rejectButton = msgBox.addButton("取消", QMessageBox::ActionRole);
    rejectButton->setObjectName("rejectButton");

    QGridLayout *layout = qobject_cast<QGridLayout*>(msgBox.layout());
    if (layout) {
        layout->addWidget(idLabel, 0, 0);
        layout->addWidget(idEdit, 0, 1, 1, 2); // 跨2列

        layout->addWidget(remarksLabel, 1, 0);
        layout->addWidget(remarksEdit, 1, 1, 1, 2); // 跨2列

        layout->addWidget(approveButton, 2, 0);
        layout->addWidget(rejectButton, 2, 1);
    }

    msgBox.setStyleSheet(R"(
        QMessageBox {
            background: #f8f9fa;
            border-radius: 8px;
            padding: 20px;
            font-family: "Microsoft YaHei", sans-serif;
        }
        QLabel {
            color: #495057;
            font-weight: 500;
            font-size: 14px;
            padding: 5px 0;
        }
        QLineEdit {
            border: 1px solid #ced4da;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 14px;
            margin-bottom: 15px;
            background: #ffffff;
        }
        QLineEdit:focus {
            border-color: #4dabf7;
            box-shadow: 0 0 0 0.2rem rgba(77, 171, 247, 0.25);
        }
        QPushButton {
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: 500;
            min-width: 80px;
            transition: all 0.2s;
        }
        QPushButton:hover {
            transform: translateY(-1px);
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        QPushButton#approveButton {
            background: darkblue;
            color: white;
        }
        QPushButton#approveButton:hover {
            background: lightblue;
        }
        QPushButton#rejectButton {
            background: lightblue;
            color: black;
        }
        QPushButton#rejectButton:hover {
            background: darkblue;
        }
    )");

    msgBox.exec();

    twoedit result;
    result.st1 = idEdit->text().trimmed();
    result.st2 = remarksEdit->text().trimmed();

    QAbstractButton *clickedButton = msgBox.clickedButton();

    if (clickedButton == approveButton) {
        result.flag = 1;
    } else if (clickedButton == rejectButton) {
        result.flag = 0;
    }

    return result;
}
