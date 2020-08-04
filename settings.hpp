/*****************************************************************************
 * settings.hpp
 *
 * Created: 8/4/2020 2020 by mguludag
 *
 * Copyright 2020 mguludag. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>
#include <QSettings>
#include <QStyle>
#include <QStyleFactory>
#include <QToolTip>

class Settings
{
    QSettings *m_settingsObj = nullptr;

public:
    enum Palette { light, dark };
    enum Theme { vista, classic, lightFusion, darkFusion };
    enum Format { regFormat, iniFormat };

    //Construct a Settings object
    Settings(bool format, const QString name)
    {
        (format) ? m_settingsObj = new QSettings(name, QSettings::IniFormat)
                 : m_settingsObj = new QSettings(name, QApplication::applicationName());
    }

    ~Settings() { delete m_settingsObj; }

    /*!
    \fn int loadStyle
    \brief Loads current application style from settings file or registry
    */
    int loadStyle()
    {
        int val;
        m_settingsObj->beginGroup("Style");
        val = m_settingsObj->value("Theme", lightFusion).toInt(); //default theme is lightFusion
        m_settingsObj->endGroup();
        return val;
    }

    /*!
    \fn void setStyle
    \brief Apply a given style to application
    */
    void setStyle(const int val)
    {
        switch (val) {
        case vista:
            qApp->setStyle(QStyleFactory::create("windowsvista"));
            changePalette(Settings::light);
            break;
        case classic:
            qApp->setStyle(QStyleFactory::create("windows"));
            changePalette(Settings::light);
            break;
        case lightFusion:
            qApp->setStyle(QStyleFactory::create("Fusion"));
            changePalette(Settings::light);
            break;
        case darkFusion:
            qApp->setStyle(QStyleFactory::create("Fusion"));
            changePalette(Settings::dark);
            break;
        default:
            break;
        }
    }

    /*!
    \fn QVariant readSettings
    \brief Read a value stored in settings given group and key
    */
    QVariant readSettings(const QString group, const QString key)
    {
        QVariant val;
        m_settingsObj->beginGroup(group);
        val = m_settingsObj->value(key);
        m_settingsObj->endGroup();
        return val;
    }

    template<class T>
    /*!
    \fn void writeSettings
    \brief Write a value in settings given group and key
    */
    void writeSettings(const QString group, const QString key, const T option)
    {
        m_settingsObj->beginGroup(group);
        m_settingsObj->setValue(key, option);
        m_settingsObj->endGroup();
    }

    /*!
    \fn void changePalette
    \brief Change the color palette between light or dark
    */
    void changePalette(bool _palette)
    {
        QPalette Palette;
        if (_palette) {
            Palette.setColor(QPalette::Window, QColor(53, 53, 53));
            Palette.setColor(QPalette::WindowText, Qt::white);
            Palette.setColor(QPalette::Base, QColor(25, 25, 25));
            Palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
            Palette.setColor(QPalette::ToolTipBase, QColor(53, 53, 53));
            Palette.setColor(QPalette::ToolTipText, Qt::white);
            Palette.setColor(QPalette::Text, Qt::white);
            Palette.setColor(QPalette::Button, QColor(53, 53, 53));
            Palette.setColor(QPalette::ButtonText, Qt::white);
            Palette.setColor(QPalette::BrightText, Qt::red);
            Palette.setColor(QPalette::Link, QColor(42, 130, 218));
            Palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            Palette.setColor(QPalette::HighlightedText, Qt::black);
            Palette.setColor(QPalette::Disabled, QPalette::Text, QColor(164, 166, 168));
            Palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(164, 166, 168));
            Palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(164, 166, 168));
            Palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(164, 166, 168));
            Palette.setColor(QPalette::Disabled, QPalette::Base, QColor(68, 68, 68));
            Palette.setColor(QPalette::Disabled, QPalette::Window, QColor(68, 68, 68));
            Palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(68, 68, 68));
        } else
            Palette = qApp->style()->standardPalette();
        QToolTip::setPalette(Palette);
        qApp->setPalette(Palette);
    }
};

#endif // SETTINGS_HPP
