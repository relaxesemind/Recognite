#include "parser.h"
#include <QRegExp>
#include <cmath>

InputModel Parser::inputModelFromFileDefault(const QString &path, int id)
{
    QFile inputFile(path);

    if (!inputFile.open(QIODevice::ReadOnly))
    {
        AppMessage("Ошибка входного файла","Не удалось распарсить входной файл");
        return InputModel();
    }

    QTextStream stream(&inputFile);
    QString alltext = stream.readAll();

    inputFile.close();

    if (stream.status() != QTextStream::Ok)
    {
        AppMessage("Ошибка входного файла","Не удалось распарсить входной файл");
        return InputModel();
    }

    QString lineSeparator = SettingsStorage::shared().lineSeparator;
    QString numberSeparator = SettingsStorage::shared().numberSeparator;

    QStringList linesInFile = alltext.split(lineSeparator);

    if (linesInFile.isEmpty())
    {
        return InputModel();

    }

    QVector<QVector<float>> matrix (linesInFile.count(),QVector<float>(linesInFile.count(),0.f));

    repeat(i,linesInFile.count())
    {
        QStringList numbers = linesInFile.at(i).split(numberSeparator);

        if (numbers.count() > linesInFile.count())
        {
            continue;
        }

        repeat(j,numbers.count())
        {
            QString number = numbers.at(j);
            float height = number.toFloat() * 1000; // перевод в нанометры
            matrix[i][j] = height;
        }
    }

    return InputModel(id,path,std::move(matrix));
}

InputModel Parser::inputModelFromFileWithCommas(const QString &path, int id)
{
    QFile inputFile(path);

    if (!inputFile.open(QIODevice::ReadOnly))
    {
        AppMessage("Ошибка входного файла","Не удалось распарсить входной файл");
        return InputModel();
    }

    QTextStream stream(&inputFile);
    QString alltext = stream.readAll();

    inputFile.close();

    if (stream.status() != QTextStream::Ok)
    {
        AppMessage("Ошибка входного файла","Не удалось распарсить входной файл");
        return InputModel();
    }

    QStringList list = alltext.split(QString(","));
    if (list.isEmpty())
    {
        return InputModel();
    }

    QVector<QVector<float>> matrix(1,QVector<float>{});
    int linesCounter = 1;

    for (int i = 0; i < list.count(); ++i)
    {
        QString str = list.at(i);

        if (str.isEmpty())
        {
            ++linesCounter;
            matrix.resize(linesCounter);
        }
        else
        {
            bool ok;
            float number = str.toFloat(&ok);

            if (ok)
            {
                matrix[linesCounter - 1].append(number * 1000);
            }
        }
    }

    return InputModel(id,path,std::move(matrix));
}

inline bool isPerfectSquare(int n)
{
    double s = std::sqrt(n);
    return (s - std::floor(s) < std::numeric_limits<double>::epsilon());
}

int sideOfSquare(int number)
{
    int copyLeft = number, copyRight = number;
    int diffLeft = 0, diffRight = 0;

    while (!isPerfectSquare(copyLeft))
    {
        --copyLeft;
        ++diffLeft;
    }

    while (!isPerfectSquare(copyRight))
    {
        ++copyRight;
        ++diffRight;
    }

    qDebug () << "left = " << copyLeft << " right = " << copyRight;

    return diffLeft < diffRight ?
                static_cast<int>(std::sqrt(copyLeft)) :
                static_cast<int>(std::sqrt(copyRight));
}

InputModel Parser::inputModelFromNanoScope(const QString &path, int id)
{
    QFile inputFile(path);

    if (!inputFile.open(QIODevice::ReadOnly))
    {
        AppMessage("Ошибка входного файла","Не удалось распарсить входной файл");
        return InputModel();
    }

    QTextStream stream(&inputFile);
    QString alltext = stream.readAll();

    inputFile.close();

    if (stream.status() != QTextStream::Ok)
    {
        AppMessage("Ошибка входного файла","Не удалось распарсить входной файл");
        return InputModel();
    }

    QStringList stringList = alltext.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    int textSize = stringList.size();
    int nearestSide = sideOfSquare(textSize);

    qDebug() << "text size = " << textSize;
    qDebug() << "side = " << nearestSide;

    QVector<QVector<float>> matrix(nearestSide,QVector<float>{});

    int i = 0, j = 0;

    for (auto it = stringList.begin(); it != stringList.end(); ++it)
    {
        QString value = *it;
        bool ok;
        float fValue = value.toFloat(&ok);

        if (ok)
        {
            if (j == nearestSide - 1)
            {
                ++i;
                j = 0;
            }

            if (i <= nearestSide - 1)
            {
                matrix[i].append(fValue);
            }
            else
            {
              //  qDebug () << i;
            }

            ++j;
        }
    }

    return InputModel(id,path,std::move(matrix));
}



















