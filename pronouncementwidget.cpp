#include "PronouncementWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <QTextBrowser>
#include <QLabel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>
#include <QApplication>
#include <QStyle>
#include <QScrollBar>
#include <QPushButton>

PronouncementWidget::PronouncementWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

PronouncementWidget::~PronouncementWidget()
{
}

void PronouncementWidget::setDatabase()
{
    loadPronouncements();
}

void PronouncementWidget::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(5, 5, 5, 5);

    QLabel *titleLabel = new QLabel("公告列表", leftPanel);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14pt; color: #336699; }");
    leftLayout->addWidget(titleLabel);

    m_titleList = new QListWidget(leftPanel);
    m_titleList->setStyleSheet(
        "QListWidget { background-color: #f0f0f0; border: 1px solid #cccccc; }"
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #dddddd; }"
        "QListWidget::item:selected { background-color: #e0e0ff; color: #0000ff; }"
    );
    connect(m_titleList, &QListWidget::itemClicked,
            this, &PronouncementWidget::onAnnouncementSelected);
    leftLayout->addWidget(m_titleList);

    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(15, 15, 15, 15);

    m_dateLabel = new QLabel(rightPanel);
    m_dateLabel->setStyleSheet("QLabel { color: #888888; font-size: 10pt; }");
    rightLayout->addWidget(m_dateLabel);

    m_contentBrowser = new QTextBrowser(rightPanel);
    m_contentBrowser->setStyleSheet(
        "QTextBrowser {"
        "  background-color: #ffffff;"
        "  border: 1px solid #cccccc;"
        "  border-radius: 5px;"
        "  padding: 15px;"
        "  font-size: 12pt;"
        "}"
    );
    rightLayout->addWidget(m_contentBrowser);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    splitter->setHandleWidth(3);
    splitter->setStyleSheet(
        "QSplitter::handle { background-color: #d0d0d0; }"
    );

    mainLayout->addWidget(splitter);
    setLayout(mainLayout);

    m_contentBrowser->setHtml("<center><h3 style='color:#888888;'>请从左侧选择公告</h3></center>");
    this->setDatabase();
    this->resize(1000, 600);
    this->setWindowTitle("公告");
    QPushButton *refreshBtn = new QPushButton(QIcon(":/images/refresh.png"), "刷新", this);
    connect(refreshBtn, &QPushButton::clicked, this, &PronouncementWidget::refreshAnnouncements);

    leftLayout->addWidget(refreshBtn);
}

void PronouncementWidget::loadPronouncements()
{

    m_titleList->clear();
    QString allAnnouncementsHtml;

    QSqlQuery query("SELECT id, title, content, date FROM pronouncement ORDER BY date DESC");
    if (!query.exec()) {
        qCritical() << "Failed to fetch pronouncements:" << query.lastError().text();
        return;
    }

    allAnnouncementsHtml = "<div style='font-family: \"Microsoft YaHei\", sans-serif;'>";

    while (query.next()) {
        int id = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString content = query.value("content").toString();
        QDateTime date = query.value("date").toDateTime();

        QListWidgetItem *item = new QListWidgetItem(title, m_titleList);
        item->setData(Qt::UserRole, id);
        item->setData(Qt::UserRole + 1, date);

        if (date.date() == QDate::currentDate()) {
            item->setBackground(QColor(255, 255, 200));
            item->setForeground(Qt::darkRed);
        }

        allAnnouncementsHtml += QString(
            "<h2 style='color: #336699; margin-bottom: 5px; border-bottom: 1px solid #eeeeee; padding-bottom: 5px;'>%1</h2>"
            "<p style='color: #888888; font-size: 12px; margin-top: 0;'>发布日期: %2</p>"
            "<div style='line-height: 1.6; margin-bottom: 30px;'>%3</div>"
        ).arg(title)
         .arg(date.toString("yyyy-MM-dd hh:mm"))
         .arg(content);
    }

    allAnnouncementsHtml += "</div>";

    m_contentBrowser->setHtml(allAnnouncementsHtml);

    m_dateLabel->setText(QString("共 %1 条公告").arg(m_titleList->count()));
}

void PronouncementWidget::onAnnouncementSelected(QListWidgetItem *item)
{
    if (!item) return;

    int id = item->data(Qt::UserRole).toInt();
    QString anchor = QString("announcement_%1").arg(id);
    m_contentBrowser->scrollToAnchor(anchor);
}

void PronouncementWidget::refreshAnnouncements()
{
    m_contentBrowser->setHtml("<center><h3 style='color:#888888;'>正在加载公告...</h3></center>");
    qApp->processEvents();

    int scrollPosition = m_contentBrowser->verticalScrollBar()->value();

    m_titleList->clear();

    loadPronouncements();

    m_contentBrowser->verticalScrollBar()->setValue(scrollPosition);

    QDateTime currentTime = QDateTime::currentDateTime();
    m_dateLabel->setText(QString("最后刷新: %1 | 共 %2 条公告")
                        .arg(currentTime.toString("hh:mm:ss"))
                        .arg(m_titleList->count()));
}
