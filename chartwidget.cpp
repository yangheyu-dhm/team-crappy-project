#include "chartwidget.h"
#include "ui_chartwidget.h"
#include "chart.h"
chartwidget::chartwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chartwidget)
{
    ui->setupUi(this);

    QString sql = "SELECT type, SUM(amount) AS total_amount "
                  "FROM Payments "
                  "GROUP BY type";

    // 创建图表视图并添加到主窗口
    QChartView* chartView = createChartFromSql(sql, this, "缴费类型统计");
    if (chartView) {
        // 创建中央部件和布局
        QWidget* centralWidget = new QWidget(this);

        QVBoxLayout* layout = new QVBoxLayout(centralWidget);
        layout->addWidget(chartView);

        resize(800, 600);
    }
}

chartwidget::~chartwidget()
{
    delete ui;
}
