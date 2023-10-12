/*
 * This file is part of the https://github.com/QQxiaoming/quardCRT.git
 * project.
 *
 * Copyright (C) 2021 Quard <2014500726@smail.xtu.edu.cn>
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
#include <QApplication>
#include <QCommandLineParser>
#include <QTranslator>
#include <QLibraryInfo>
#include <QStyleFactory>
#include <QRegularExpression>

#include "qfonticon.h"

#include "mainwindow.h"

QString VERSION = APP_VERSION;
QString GIT_TAG =
#include <git_tag.inc>
;

class CommandLineParser
{
private:
    CommandLineParser(void){
        static CommandLineParser::GC gc;
        parser.setApplicationDescription(
            "QuardCRT Tools, "
            "The following parameters can be configured to start the application:");
        parser.addHelpOption();
        parser.addVersionOption();
        foreach(QString opt,commandLineMap.keys()) {
            parser.addOption(
                commandLineMap.value(opt,QCommandLineOption("defaultValue")));
        }
    }
    static CommandLineParser* self;
    QCommandLineParser parser;
    bool processApp = false;
    QMap<QString, QCommandLineOption> commandLineMap = {
        {"dark_theme",
            QCommandLineOption(
                {"d","dark_theme"},
                "GUI dark theme",
                "dark-theme",
                "auto"
            )
        },
        {"language",
            QCommandLineOption(
                {"l","language"},
                "application language",
                "language",
                "auto"
            )
        },
        {"miniui",
            QCommandLineOption(
                {"m","miniui"},
                "miniui mode",
                "miniui",
                "false"
            )
        },
        {"start_dir",
            QCommandLineOption(
                {"s","start_dir"},
                "start with directory",
                "start_dir",
                ""
            )
        },
    };

public:
    void process(const QCoreApplication &app) {
        parser.process(app);
        processApp = true;
    }
    QString getOpt(const QString &optKey) const {
        if(processApp) {
            foreach(QString opt,commandLineMap.keys()) {
                if(opt == optKey){
                    QCommandLineOption dstOpt =
                        commandLineMap.value(opt,QCommandLineOption("defaultValue"));
                    if(parser.isSet(dstOpt)) {
                        return parser.value(dstOpt);
                    } else {
                        if(dstOpt.defaultValues().size() > 0)
                            return dstOpt.defaultValues().at(0);
                        else 
                            return "";
                    }
                }
            }
        }
        return "";
    }
    bool isSetOpt(const QString &optKey) const {
        if(processApp) {
            foreach(QString opt,commandLineMap.keys()) {
                if(opt == optKey){
                    QCommandLineOption dstOpt =
                        commandLineMap.value(opt,QCommandLineOption("defaultValue"));
                    return parser.isSet(dstOpt);
                }
            }
        }
        return false;
    }
    static CommandLineParser *instance() {
        if(!self) {
            self = new CommandLineParser();
        }
        return self;
    }

private:
    class GC
    {
    public:
        ~GC() {
            if (self != nullptr) {
                delete self;
                self = nullptr;
            }
        }
    };
};

CommandLineParser* CommandLineParser::self = nullptr;
#define  AppComLineParser   CommandLineParser::instance()
static QTranslator qtTranslator;
static QTranslator qtbaseTranslator;
static QTranslator appTranslator;

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QApplication application(argc, argv);

    QApplication::setApplicationName("QuardCRT");
    QApplication::setOrganizationName("Copyright (c) 2023 Quard(QiaoQiming)");
    QApplication::setOrganizationDomain("https://github.com/QQxiaoming/quardCRT");
    QApplication::setApplicationVersion(VERSION+" "+GIT_TAG);

    AppComLineParser->process(application);
    QString dark_theme = AppComLineParser->getOpt("dark_theme");
    QString app_lang = AppComLineParser->getOpt("language");

    QLocale locale;
    QLocale::Language lang = locale.language();
    if(app_lang == "zh_CN") lang = QLocale::Chinese;
    if(app_lang == "ja_JP") lang = QLocale::Japanese;
    if(app_lang == "en_US") lang = QLocale::English;
    MainWindow::setAppLangeuage(lang);

#if 0 // Now we always use the dark theme, Because the dark theme is more beautiful
    int text_hsv_value = QPalette().color(QPalette::WindowText).value();
    int bg_hsv_value = QPalette().color(QPalette::Window).value();
    bool isDarkTheme = text_hsv_value > bg_hsv_value?true:false;
#else
    bool isDarkTheme = true;
#endif
    if(dark_theme == "true") isDarkTheme = true;
    if(dark_theme == "false") isDarkTheme = false;
    QString themeName;
    if(isDarkTheme) {
        themeName = ":/qdarkstyle/dark/darkstyle.qss";
    } else {
        themeName = ":/qdarkstyle/light/lightstyle.qss";
    }
    QFile ftheme(themeName);
    if (!ftheme.exists())   {
        qDebug() << "Unable to set stylesheet, file not found!";
    } else {
        ftheme.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&ftheme);
        qApp->setStyleSheet(ts.readAll());
    }

    bool isMiniUI = AppComLineParser->getOpt("miniui") == "true"?true:false;
    QString dir = AppComLineParser->getOpt("start_dir");

    QFontIcon::addFont(":/icons/icons/fontawesome-webfont.ttf");
    QFontIcon::instance()->setColor(isDarkTheme?Qt::white:Qt::black);
    //QApplication::setStyle(QStyleFactory::create("Fusion"));

    MainWindow window(dir,isMiniUI?MainWindow::MINIUI_MODE:MainWindow::STDUI_MODE,lang,isDarkTheme);
    window.show();

    return application.exec();
}
