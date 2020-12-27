/*****************************************************************************
 * settings.cpp
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
#include "settings.hpp"
#include <string>
#include <windows.h>

Settings *Settings::m_instance = nullptr;
bool Settings::m_autoPalette=false;
AutoPalette* Settings::mAutoPalette=nullptr;


void Settings::init(Settings::Format format, const QString &name) {
    if (!m_instance){
        m_instance = new Settings(format, name);
        mAutoPalette = new AutoPalette(m_instance);
        connect(mAutoPalette,SIGNAL(notifyPalette(bool)),m_instance,SLOT(bool2PaletteHelper(bool)));
        connectionCallback(*m_instance);
    }
}

/*!
\fn int loadStyle
\brief Loads current application style from settings file or registry
*/
Settings::Theme Settings::loadStyle() {
    int val;
    m_instance->m_settingsObj->beginGroup("Style");
    val = m_instance->m_settingsObj
            ->value("Theme", static_cast<int>(Theme::lightFusion))
            .toInt(); // default theme is lightFusion
    m_instance->m_settingsObj->endGroup();
    return static_cast<Theme>(val);
}

/*!
\fn void setStyle
\brief Apply a given style to application
*/
void Settings::setStyle(const Settings::Theme val) {
    switch (val) {
    case Theme::vista:
        qApp->setStyle(QStyleFactory::create("windowsvista"));
        changePalette(Palette::light);
        break;
    case Theme::classic:
        qApp->setStyle(QStyleFactory::create("windows"));
        changePalette(Palette::light);
        break;
    case Theme::lightFusion:
        qApp->setStyle(QStyleFactory::create("Fusion"));
        changePalette(Palette::light);
        break;
    case Theme::darkFusion:
        qApp->setStyle(QStyleFactory::create("Fusion"));
        changePalette(Palette::dark);
        break;
    default:
        break;
    }
}

/*!
\fn QVariant readSettings
\brief Read a value stored in settings given group and key
*/
QVariant Settings::readSettings(const QString group, const QString key) {
    QVariant val;
    m_instance->m_settingsObj->beginGroup(group);
    val = m_instance->m_settingsObj->value(key);
    m_instance->m_settingsObj->endGroup();
    return val;
}

void Settings::setAutoPalette(bool autoPalette)
{
    m_autoPalette = autoPalette;
    callbackForSignal(m_autoPalette, *m_instance);
}

Settings::Settings(Settings::Format format, const QString &name) {
    (static_cast<bool>(format))
            ? m_settingsObj = new QSettings(name, QSettings::IniFormat)
            : m_settingsObj = new QSettings(name, QApplication::applicationName());
}

/*!
\fn void changePalette
\brief Change the color palette between light or dark
*/
void Settings::changePalette(Palette _palette) {
    static QPalette Palette;
    if (static_cast<bool>(_palette)) {
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
        Palette.setColor(QPalette::Disabled, QPalette::WindowText,
                         QColor(164, 166, 168));
        Palette.setColor(QPalette::Disabled, QPalette::ButtonText,
                         QColor(164, 166, 168));
        Palette.setColor(QPalette::Disabled, QPalette::HighlightedText,
                         QColor(164, 166, 168));
        Palette.setColor(QPalette::Disabled, QPalette::Base, QColor(68, 68, 68));
        Palette.setColor(QPalette::Disabled, QPalette::Window, QColor(68, 68, 68));
        Palette.setColor(QPalette::Disabled, QPalette::Highlight,
                         QColor(68, 68, 68));
    } else
        Palette = qApp->style()->standardPalette();
    QToolTip::setPalette(Palette);
    qApp->setPalette(Palette);
}

void Settings::callbackForSignal(bool b, Settings& s)
{
    s.autoPaletteSignal(b);
}

void Settings::connectionCallback(Settings &s)
{
    connect(&s,SIGNAL(autoPaletteSignal(bool)), mAutoPalette, SLOT(isLightEventLoop(bool)));
}

void Settings::bool2PaletteHelper(bool b)
{
    changePalette(static_cast<Palette>(b));
}

bool AutoPalette::autoPalette() const
{
    return m_autoPalette;
}

void AutoPalette::setAutoPalette(bool autoPalette)
{
    m_autoPalette = autoPalette;
}

AutoPalette::AutoPalette(QObject *parent) : QObject(parent)
{

}

auto AutoPalette::readIsLight(const std::wstring &strKeyName, const std::wstring &strValueName)
{
    HKEY hKey;
    DWORD dwBufferSize(sizeof(DWORD));
    DWORD nResult(0);
    LONG nError = -1;
    if(RegOpenKeyExW(HKEY_CURRENT_USER,
                     strKeyName.c_str()
                     ,0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS){
        nError = ::RegQueryValueExW(hKey,
                                    strValueName.c_str(),
                                    0,
                                    NULL,
                                    reinterpret_cast<LPBYTE>(&nResult),
                                    &dwBufferSize);
        RegCloseKey(hKey);
    }
    return (ERROR_SUCCESS == nError) ? nResult : -1;
}

void AutoPalette::isLightEvent(bool &value)
{
    DWORD  dwFilter = REG_NOTIFY_CHANGE_ATTRIBUTES |
            REG_NOTIFY_CHANGE_LAST_SET;
    HKEY hMainKey{HKEY_CURRENT_USER};
    HKEY hKey;
    HANDLE hEvent;
    LONG lErrorCode;

    lErrorCode = RegOpenKeyEx(hMainKey, hSubKey, 0, KEY_NOTIFY, &hKey);

    if(lErrorCode != ERROR_SUCCESS) return;


    // Create an event.
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


    if(hEvent){
        lErrorCode = RegNotifyChangeKeyValue(hKey, TRUE, dwFilter, hEvent, TRUE);
        if(lErrorCode != ERROR_SUCCESS) return;
        if(WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED) return;
        value=static_cast<bool>(readIsLight(hSubKey,valueName));
        RegCloseKey(hKey);
        CloseHandle(hEvent);
    }
    return;
}

void AutoPalette::isLightEventLoop(const bool &event)
{
    if(mThread){
        mThread->quit();
    }
    else {
        mThread=QThread::create([&]{
            auto islight = static_cast<bool>(readIsLight(hSubKey,valueName));
            emit notifyPalette(!islight);
            auto temp=islight;
            while (event) {
                temp=islight;
                isLightEvent(islight);
                if(temp!=islight) emit notifyPalette(!islight);
            };
        });
    }
    event ? mThread->start() : mThread->quit();
}
