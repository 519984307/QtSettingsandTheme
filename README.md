# QtSettings and Fusion Dark Theme
Qt read write create settings and set windows, classic, fusion and dark themes

## How to use?
You include header file named "settings.hpp" into your qt project then you have to construct **Settings** object for how settings are stored in computer. __Settings* yourobj = new Settings(Settings::regformat, "yourappname")__ is example for store setting in registry under "yourappname" key. Other option is **Settings::iniformat** that stores the settings in "yourappname" file. Change themes simply type **your_obj->setTheme(Settings::darkFusion);** for Fusion Dark theme. Other theme choices in the table.

| Theme | Enum value | 
|---|---|
| Default | Settings::vista |
| Classic | Settings::classic |
| Fusion Light | Settings::lightFusion |
| Fusion Dark | Settings::darkFusion |

And you can read settings with **your_obj->readSettings(QString group, QString key);** function. This function reads settings.ini file for given QString group and QString key argument. If these group and key found in settings.ini file then function returns key's value in QVariant type.

You can write settings with **your_obj->writeSettings(QString group, QString key, T option);** fuction. This function creates settings.ini file and writes group, key and its value (T type option). 

## Example settings.ini file 
>
> **[Group]**
>
> **Key**=*value*

## Fusion Dark theme screenshots:

![ss1](https://raw.githubusercontent.com/cheytacllc/QtSettingsandTheme/master/2018-09-14_192745.jpg)
![ss2](https://raw.githubusercontent.com/cheytacllc/QtSettingsandTheme/master/2018-09-14_192822.jpg)

