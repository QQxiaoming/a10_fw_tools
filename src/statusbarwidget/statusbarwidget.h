/*
 * This file is part of the https://github.com/QQxiaoming/quardCRT.git
 * project.
 *
 * Copyright (C) 2024 Quard <2014500726@smail.xtu.edu.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QEvent>
#include <QToolButton>
#include <QPainter>
#include <QStyle>
#include <QContextMenuEvent>
#include <QStyleOptionButton>

#include "sessionswindow.h"

class StatusBarToolButton : public QToolButton
{
    Q_OBJECT

public:
    explicit StatusBarToolButton(QWidget *parent = nullptr) 
        : QToolButton(parent) {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        setPopupMode(QToolButton::InstantPopup);
        setAutoRaise(true);
        setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    }
    ~StatusBarToolButton() {
    }
    void setText(const QString &text) {
        QToolButton::setText(text);
        QFontMetrics metrics = fontMetrics();
        int fontsize = metrics.horizontalAdvance(" ",1);
        if (!icon().isNull()) {
            int iconSize = metrics.height();
            if(text.isEmpty()) {
                setFixedWidth(iconSize);
            } else {
                setFixedWidth(metrics.horizontalAdvance(text)+fontsize+iconSize);
            }
        } else {
            setFixedWidth(metrics.horizontalAdvance(text)+fontsize);
        }
    }
    void setIcon(const QIcon &icon) {
        QToolButton::setIcon(icon);
        QFontMetrics metrics = fontMetrics();
        int fontsize = metrics.horizontalAdvance(" ",1);
        if (!icon.isNull()) {
            int iconSize = metrics.height();
            if(text().isEmpty()) {
                setFixedWidth(iconSize);
            } else {
                setFixedWidth(metrics.horizontalAdvance(text())+fontsize+iconSize);
            }
        } else {
            setFixedWidth(metrics.horizontalAdvance(text())+fontsize);
        }
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);

        QStyleOptionButton option;
        option.initFrom(this);
        style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

        QRect textRect = rect();
        if (!icon().isNull()) {
            QSize size = this->size();
            QFontMetrics fontMetrics = painter.fontMetrics();
            int fontsize = fontMetrics.horizontalAdvance(" ",1);
            int iconSize = fontMetrics.height();
            if(text().isEmpty()) {
                int iconX = (size.width() - iconSize) / 2;
                int iconY = (size.height() - iconSize) / 2;
                painter.drawPixmap(iconX, iconY, icon().pixmap(iconSize, iconSize));
            } else {
                int iconX = (size.width() - iconSize - fontsize/2 - fontMetrics.horizontalAdvance(text())) / 2;
                int iconY = (size.height() - iconSize) / 2;
                painter.drawPixmap(iconX, iconY, icon().pixmap(iconSize, iconSize));
                textRect.setLeft(iconX + iconSize + fontsize/2);
                painter.drawText(textRect, Qt::AlignVCenter, text());
            }
        } else {
            painter.drawText(textRect, Qt::AlignCenter, text());
        }
    }
};

namespace Ui {
class StatusBarWidget;
}

class StatusBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusBarWidget(QWidget *parent = nullptr);
    ~StatusBarWidget();
    void setCursorPosition(int64_t x, int64_t y);
    void setType(const QString &type);
    void setTransInfo(bool enable, int64_t tx = -1, int64_t rx = -1);
    void setSpeedInfo(bool enable, qreal tx = -1.0, qreal rx = -1.0);
    void setEndOfLine(bool enable, SessionsWindow::EndOfLineSeq type = SessionsWindow::AUTO);
    void setSSHEncryption(bool enable, QString type = QString());
    void setLogs(bool enable, bool isLogs = false);
    void setNotifiction(bool enable);
    void retranslateUi(void);
    void setFont(QFont &font);

signals:
    void cursorInfoTriggered(void);
    void typeTriggered(void);
    void transTxTriggered(void);
    void transRxTriggered(void);
    void speedTxTriggered(void);
    void speedRxTriggered(void);
    void endOfLineTriggered(void);
    void sshEncryptionTriggered(void);
    void logsTriggered(void);   
    void notifictionTriggered(void);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    
private:
    Ui::StatusBarWidget *ui;
    StatusBarToolButton *statusBarCursorInfo;
    StatusBarToolButton *statusBarType;
    StatusBarToolButton *statusBarTransTx;
    StatusBarToolButton *statusBarTransRx;
    StatusBarToolButton *statusBarSpeedTx;
    StatusBarToolButton *statusBarSpeedRx;
    StatusBarToolButton *statusBarEndOfLine;
    StatusBarToolButton *statusBarSSHEncryption;
    StatusBarToolButton *statusBarLogs;
    StatusBarToolButton *statusBarNotifiction;
    bool m_logs_show = false;
    bool m_logs = false;
    bool m_notifiction = false;
};

#endif // STATUSBARWIDGET_H
