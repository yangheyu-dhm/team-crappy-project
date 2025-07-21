#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

// 使用QtCharts命名空间
QT_CHARTS_USE_NAMESPACE

QChartView* createChartFromSql(const QString& sqlQuery, QWidget* parent = nullptr, const QString& title = "数据统计")
{
    // 验证SQL查询是否为空
    if (sqlQuery.isEmpty()) {
        if (parent) {
            QMessageBox::warning(parent, "参数错误", "SQL查询语句不能为空");
        }
        return nullptr;
    }

    // 执行SQL查询
    QSqlQuery query(sqlQuery);
    if (!query.exec()) {
        if (parent) {
            QMessageBox::critical(parent, "数据库错误",
                                 QString("查询失败: %1").arg(query.lastError().text()));
        }
        return nullptr;
    }

    // 检查结果集是否有数据
    if (!query.first()) {
        if (parent) {
            QMessageBox::information(parent, "信息", "查询结果为空，没有数据可显示");
        }
        return nullptr;
    }



    // 收集数据
    QMap<QString, int> chartData;
    query.seek(-1); // 重置查询位置到开头
    while (query.next()) {
        QString category = query.value(0).toString();
        bool ok;
        int value = query.value(1).toInt(&ok);
        if (ok) {
            chartData.insert(category, value);
        } else {
            if (parent) {
                QMessageBox::warning(parent, "数据警告",
                                     QString("第 %1 行的数值无法转换为整数，已忽略").arg(query.at() + 1));
            }
        }
    }

    // 验证是否有有效数据
    if (chartData.isEmpty()) {
        if (parent) {
            QMessageBox::information(parent, "信息", "没有有效数据可显示");
        }
        return nullptr;
    }

    // 创建柱状图系列
    QBarSeries *series = new QBarSeries();

    // 创建柱状图数据集
    QBarSet *set = new QBarSet("数据");

    // 添加数据到数据集
    QStringList categories;
    for (auto it = chartData.begin(); it != chartData.end(); ++it) {
        *set << it.value();
        categories << it.key();
    }

    series->append(set);

    // 创建图表
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(title);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 设置主题和其他样式属性
    chart->setTheme(QChart::ChartThemeBlueNcs); // 确保在创建QChartView之前设置主题

    // 创建X轴和Y轴
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("数值");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    axisY->setRange(0, 1000);

    // 确保所有图表配置完成后再创建QChartView
    QChartView *chartView = new QChartView(chart, parent);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 400);

    return chartView;
}

bool updateChartWithSql(QChartView* chartView, const QString& sqlQuery, const QString& title = "数据统计")
{
    if (!chartView || !chartView->chart()) {
        qDebug() << "Error: Invalid chart view";
        return false;
    }

    QChart* chart = chartView->chart();

    // 执行SQL查询
    QSqlQuery query(sqlQuery);
    if (!query.exec()) {
        QMessageBox::critical(chartView->parentWidget(), "数据库错误",
                             QString("查询失败: %1").arg(query.lastError().text()));
        return false;
    }

    // 检查结果集是否有数据
    if (!query.first()) {
        QMessageBox::information(chartView->parentWidget(), "信息", "查询结果为空，没有数据可显示");
        return false;
    }

    // 收集数据
    QMap<QString, int> chartData;
    query.seek(-1); // 重置查询位置到开头
    while (query.next()) {
        QString category = query.value(0).toString();
        bool ok;
        int value = query.value(1).toInt(&ok);
        if (ok) {
            chartData.insert(category, value);
        } else {
            QMessageBox::warning(chartView->parentWidget(), "数据警告",
                                QString("第 %1 行的数值无法转换为整数，已忽略").arg(query.at() + 1));
        }
    }

    // 验证是否有有效数据
    if (chartData.isEmpty()) {
        QMessageBox::information(chartView->parentWidget(), "信息", "没有有效数据可显示");
        return false;
    }

    // 清空现有系列
    chart->removeAllSeries();

    // 创建新的柱状图系列
    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("数据");

    // 添加数据到数据集
    QStringList categories;
    for (auto it = chartData.begin(); it != chartData.end(); ++it) {
        *set << it.value();
        categories << it.key();
    }

    series->append(set);

    // 更新图表
    chart->addSeries(series);
    chart->setTitle(title);

    // 更新X轴（分类轴）
    QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis*>(chart->axisX());
    if (!axisX) {
        // 如果没有X轴，创建一个
        axisX = new QBarCategoryAxis();
        chart->addAxis(axisX, Qt::AlignBottom);
    } else {
        // 清空现有分类
        axisX->clear();
    }
    axisX->append(categories);
    series->attachAxis(axisX);

    // 更新Y轴（数值轴）
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axisY());
    if (!axisY) {
        // 如果没有Y轴，创建一个
        axisY = new QValueAxis();
        axisY->setTitleText("数值");
        chart->addAxis(axisY, Qt::AlignLeft);
    }
    series->attachAxis(axisY);

    // 自动计算Y轴范围，留出10%的空间
    int maxValue = 0;
    for (int value : chartData) {
        maxValue = qMax(maxValue, value);
    }
    axisY->setRange(0, 1000); // 确保最小值为1，避免全零数据导致的问题

    return true;
}
