#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H

#include <QString>

enum ParseMode
{
    defaultMode = 0,
    commasMode = 1,
    mode1024 = 2
};

class SettingsStorage
{
public:
    SettingsStorage(SettingsStorage const&) = delete;
    SettingsStorage& operator=(SettingsStorage const&) = delete;
    static SettingsStorage& shared()
    {
        static SettingsStorage instance;
        return instance;
    }

private:
    SettingsStorage();

public:

    float derivativeStability; /* если разница производных меньше этой константы они считаются равными */
    int traverseWalkStep; /* размер шага в пикселях для алгоритма расширения области пика объекта (поиск истинной высоты) */
    QString numberSeparator, lineSeparator;
    ParseMode parseMode;
};

#endif // SETTINGSSTORAGE_H
