/*
    Copyright 2013 Christian Surlykke

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/
#include <QMenu>
#include <QAction>
#include <QRegularExpression>
#include <QDebug>

#include "SearchBar.h"
#include "qfonticon.h"

SearchBar::SearchBar(QWidget *parent) : QWidget(parent)
{
    widget.setupUi(this);
    setAutoFillBackground(true); // make it always opaque, especially inside translucent windows

    connect(widget.closeButton, &QToolButton::clicked, this, &SearchBar::hide);
    connect(widget.searchTextEdit, &QLineEdit::textChanged, this, &SearchBar::searchCriteriaChanged);
    connect(widget.findPreviousButton, &QToolButton::clicked, this, &SearchBar::findPrevious);
    connect(widget.findNextButton, &QToolButton::clicked, this, &SearchBar::findNext);

    connect(this, &SearchBar::searchCriteriaChanged, this, &SearchBar::clearBackgroundColor);

    QMenu *optionsMenu = new QMenu(widget.optionsButton);
    widget.optionsButton->setMenu(optionsMenu);

    m_matchCaseMenuEntry = optionsMenu->addAction(tr("Match case"));
    m_matchCaseMenuEntry->setCheckable(true);
    m_matchCaseMenuEntry->setChecked(true);
    connect(m_matchCaseMenuEntry, &QAction::toggled, this, &SearchBar::searchCriteriaChanged);


    m_useRegularExpressionMenuEntry = optionsMenu->addAction(tr("Regular expression"));
    m_useRegularExpressionMenuEntry->setCheckable(true);
    connect(m_useRegularExpressionMenuEntry, &QAction::toggled, this, &SearchBar::searchCriteriaChanged);

    m_highlightMatchesMenuEntry = optionsMenu->addAction(tr("Highlight all matches"));
    m_highlightMatchesMenuEntry->setCheckable(true);
    m_highlightMatchesMenuEntry->setChecked(true);
    connect(m_highlightMatchesMenuEntry, &QAction::toggled, this, &SearchBar::highlightMatchesChanged);

    retranslateUi();
}

SearchBar::~SearchBar() {
}

QString SearchBar::searchText()
{
    return widget.searchTextEdit->text();
}


bool SearchBar::useRegularExpression()
{
    return m_useRegularExpressionMenuEntry->isChecked();
}

bool SearchBar::matchCase()
{
    return m_matchCaseMenuEntry->isChecked();
}

bool SearchBar::highlightAllMatches()
{
    return m_highlightMatchesMenuEntry->isChecked();
}

void SearchBar::show()
{
    QWidget::show();
    widget.searchTextEdit->setFocus();
    widget.searchTextEdit->selectAll();
}

void SearchBar::hide()
{
    QWidget::hide();
    if (QWidget *p = parentWidget())
    {
        p->setFocus(Qt::OtherFocusReason); // give the focus to the parent widget on hiding
    }
}

void SearchBar::noMatchFound()
{
    QPalette palette;
    palette.setColor(widget.searchTextEdit->backgroundRole(), QColor(255, 128, 128));
    widget.searchTextEdit->setPalette(palette);
}


void SearchBar::keyReleaseEvent(QKeyEvent* keyEvent)
{
    if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
    {
        if (keyEvent->modifiers() == Qt::ShiftModifier)
        {
            Q_EMIT findPrevious();
        }
        else
        {
            Q_EMIT findNext();
        }
    }
    else if (keyEvent->key() == Qt::Key_Escape)
    {
        hide();
    }
}

void SearchBar::clearBackgroundColor()
{
    widget.searchTextEdit->setPalette(QWidget::window()->palette());
}

void SearchBar::setText(const QString &text)
{
    return widget.searchTextEdit->setText(text);
}

void SearchBar::retranslateUi(void) {
    widget.retranslateUi(this);
    m_matchCaseMenuEntry->setText(tr("Match case"));
    m_useRegularExpressionMenuEntry->setText(tr("Regular expression"));
    m_highlightMatchesMenuEntry->setText(tr("Highlight all matches"));
    widget.closeButton->setIcon(QFontIcon::icon(QChar(0xf00d)));
    widget.findPreviousButton->setIcon(QFontIcon::icon(QChar(0xf053)));
    widget.findNextButton->setIcon(QFontIcon::icon(QChar(0xf054)));
    widget.optionsButton->setIcon(QFontIcon::icon(QChar(0xf142)));
}
