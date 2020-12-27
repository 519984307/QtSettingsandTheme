/*****************************************************************************
 * settings.hpp
 *
 * Created: 12/20/2020 2020 by mguludag
 *
 * Copyright 2020 mguludag. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
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
#include <QThread>
#include <QObject>

class AutoPalette;

class Settings : private QObject {
    Q_OBJECT


public:
    enum class Palette { light, dark };
    enum class Theme { vista, classic, lightFusion, darkFusion };
    enum class Format { regFormat, iniFormat };

    static void init(Format format, const QString &name);

    ~Settings() {
        delete m_instance;
        delete m_settingsObj;
    }

    template <typename T>
    /*!
    \fn void writeSettings
    \brief Write a value in settings given group and key
    */
    static void writeSettings(const QString group, const QString key,
                              const T &option)
    {
        m_instance->m_settingsObj->beginGroup(group);
        m_instance->m_settingsObj->setValue(key, option);
        m_instance->m_settingsObj->endGroup();
    }

    static Theme loadStyle();
    static void setStyle(const Theme val);
    static QVariant readSettings(const QString group, const QString key);
    static void setAutoPalette(bool autoPalette);

private:
    static Settings *m_instance;
    static AutoPalette *mAutoPalette;
    QSettings *m_settingsObj = nullptr;
    static bool m_autoPalette;


    Settings() = delete;
    // Construct a Settings object
    Settings(Format format, const QString &name);
    static void changePalette(Palette _palette);
    static void callbackForSignal(bool b, Settings &s);
    static void connectionCallback(Settings &s);

signals:
    void autoPaletteSignal(bool);


private slots:
    static void bool2PaletteHelper(bool b);

};


class AutoPalette : public QObject {
    Q_OBJECT

    QThread *mThread=nullptr;
public:
    bool m_autoPalette;
    static constexpr auto  hSubKey{L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"};
    static constexpr auto valueName{L"AppsUseLightTheme"};

    AutoPalette(QObject *parent=nullptr);
    auto readIsLight(const std::wstring &strKeyName, const std::wstring &strValueName);
    void isLightEvent(bool &value);
    bool autoPalette() const;
    void setAutoPalette(bool autoPalette);

public slots:
    void isLightEventLoop(const bool &event);

signals:
    void notifyPalette(bool);

};

#endif // SETTINGS_HPP
