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

    QChar lineSeparator = StaticModel::shared().lineSeparator;
    QChar numberSeparator = StaticModel::shared().numberSeparator;

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
            float height = number.toFloat() * 1000;
            matrix[i][j] = height;
        }
    }

   return InputModel(id,path,std::move(matrix));
}



















