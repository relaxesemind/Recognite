#include "parser.h"

InputModel Parser::inputModelFromFile(const QString &path, int id)
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



















