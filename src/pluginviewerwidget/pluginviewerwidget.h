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
#ifndef PLUGINVIEWERWIDGET_H
#define PLUGINVIEWERWIDGET_H

#include <QWidget>
#include <QMap>

#include "pluginviewerhomewidget.h"

namespace Ui {
class PluginViewerWidget;
}

class PluginViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PluginViewerWidget(QWidget *parent = nullptr);
    ~PluginViewerWidget();
    bool addPlugin(QWidget *pluginWidget, const QString &pluginName);
    void setPluginVisible(const QString &pluginName, bool visible);
    void retranslateUi(void);

private:
    Ui::PluginViewerWidget *ui;
    PluginViewerHomeWidget *m_homeWidget;
    QMap<QString, QWidget*> m_plugins;
};

#endif // PLUGINVIEWERWIDGET_H
