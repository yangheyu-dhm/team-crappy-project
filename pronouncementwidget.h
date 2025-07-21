#ifndef PRONOUNCEMENTWIDGET_H
#define PRONOUNCEMENTWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include "database.h"

class QListWidget;
class QListWidgetItem;
class QTextBrowser;
class QLabel;

class PronouncementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PronouncementWidget(QWidget *parent = nullptr);
    ~PronouncementWidget();

    void setDatabase();

    void loadPronouncements();

private slots:
    void onAnnouncementSelected(QListWidgetItem *item);


public slots:
    void refreshAnnouncements();

private:
    void setupUI();

    QListWidget *m_titleList;
    QTextBrowser *m_contentBrowser;
    QLabel *m_dateLabel;
};

#endif // PRONOUNCEMENTWIDGET_H
