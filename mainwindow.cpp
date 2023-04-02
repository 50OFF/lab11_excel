#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardPaths>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinBox->setValue(2);
    ui->textEdit->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

static bool sorted = false;
static bool sorting = false;
static bool searching = false;
static bool generating = false;
static bool deleting = false;

static bool existed = false;
static bool changed = false;
static QString prevOpenedFileName = "";
static QString openedfileName = "new";
static QString FileWindowTitle = "Новый текстовый документ";
static QStringList namearr;

static int rows;


int MainWindow::MessageBox()
{
    QMessageBox msg;
    msg.setWindowTitle("Внимание!");
    msg.setText("Вы хотите сохранить изменения в текущем файле?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Yes);
    return msg.exec();
}


//плюс ряд
void MainWindow::on_plusRow_clicked()
{
    if (ui->table->rowCount() < 1000000)
    {
        ui->table->setRowCount(ui->table->rowCount()+1);
        ui->spinBox->setValue(ui->spinBox->value()+1);
    }

    ui->table->clearSelection();
    ui->labelOtl_2->setText(" ");
    ui->textEdit->clear();

    ui->labelMax->setText(" ");
    ui->labelMin->setText(" ");
    ui->labelAvg->setText(" ");
    ui->labelSum->setText(" ");
    ui->labelMul->setText(" ");
    ui->labelOtl->setText(" ");
}

//минус ряд
void MainWindow::on_minusRow_clicked()
{
    ui->labelMax->setText(" ");
    ui->labelMin->setText(" ");
    ui->labelAvg->setText(" ");
    ui->labelSum->setText(" ");
    ui->labelMul->setText(" ");
    ui->labelOtl->setText(" ");

    ui->textEdit->clear();
    ui->table->clearSelection();
    ui->labelOtl_2->setText(" ");

    int row = ui->table->rowCount();
    if(row>0)
    {
        ui->table->setRowCount(row-1);
        ui->spinBox->setValue(ui->spinBox->value()-1);
    }
}

//изменение кол-ва рядов
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->table->setRowCount(arg1);

    ui->table->clearSelection();

    ui->labelOtl_2->setText(" ");

    ui->textEdit->clear();
}

//сгенерировать
void MainWindow::on_random_clicked()
{
    ui->labelMax->setText(" ");
    ui->labelMin->setText(" ");
    ui->labelAvg->setText(" ");
    ui->labelSum->setText(" ");
    ui->labelMul->setText(" ");
    ui->labelOtl->setText(" ");

    ui->textEdit->clear();
    ui->table->clearSelection();
    ui->labelOtl_2->setText(" ");

    sorted = false;
    generating = true;

    int row=ui->table->rowCount();
    int column=ui->table->columnCount();

    int value;

    for (int i=0;i<row;i++)
    {
        for (int j=0;j<column;j++)
        {
            if (ui->table->item(i,j)==nullptr)
            {
                QTableWidgetItem * ti;
                ti =  new QTableWidgetItem();
                ui->table->setItem(i,j,ti);
            }
            value=rand()%201-100;
            ui->table->item(i,j)->setText(QString::number(value));
        }
    }
    generating = false;
}

//сумма
void MainWindow::on_sum_clicked()
{
    int row=ui->table->rowCount();
    int column=ui->table->columnCount();

    int sum=0;
    bool flag = true;

    if (row != 0)
    {
        for (int j=0; j<column; j++)
        {
            for (int i=0; i<row; i++)
            {
                if (ui->table->item(i,j)!=nullptr)
                {
                    sum+=ui->table->item(i,j)->text().toInt(&flag);
                    if(not flag)
                    {
                        ui->labelSum->setText("Ошибка!");
                        flag = false;
                        break;
                    }
                }
                else
                {
                    QTableWidgetItem * ti;
                    ti =  new QTableWidgetItem();
                    ui->table->setItem(i,j,ti);

                    ui->table->item(i,j)->setBackgroundColor(Qt::red);
                    ui->labelSum->setText("Ошибка!");
                    flag = false;
                }
                if(not flag)
                {
                    continue;
                }
            }
            if(not flag)
            {
                continue;
            }
        }
        if(flag)
        {
            ui->labelSum->setNum(sum);
        }
    }
    else
    {
        ui->labelOtl_2->setText("Ошибка! Нет значений");
    }
}

//максимальное
void MainWindow::on_max_clicked()
{
    int row=ui->table->rowCount();
    int column=ui->table->columnCount();

    int cur;
    int max=-100;
    bool flag = true;

    if (row != 0)
    {
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                if (ui->table->item(i,j)!=nullptr)
                {
                    cur = ui->table->item(i,j)->text().toInt(&flag);
                    if(not flag)
                    {
                        ui->labelMax->setText("Ошибка!");
                        break;
                    }
                    if(cur>max)
                    {
                        max=cur;
                    }
                }
                else
                {
                    QTableWidgetItem * ti;
                    ti =  new QTableWidgetItem();
                    ui->table->setItem(i,j,ti);

                    ui->table->item(i,j)->setBackgroundColor(Qt::red);
                    ui->labelMax->setText("Ошибка!");
                    flag = false;
                }
                if(not flag)
                {
                    continue;
                }
            }
            if(not flag)
            {
                continue;
            }
        }
        if(flag)
        {
            ui->labelMax->setNum(max);
        }
    }
    else
    {
        ui->labelOtl_2->setText("Ошибка! Нет значений");
    }
}

//минимальное
void MainWindow::on_min_clicked()
{
    int row=ui->table->rowCount();
    int column=ui->table->columnCount();

    int cur;
    int min=100;
    bool flag = true;

    if (row != 0)
    {
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                if(ui->table->item(i,j)!=nullptr)
                {
                    cur = ui->table->item(i,j)->text().toInt(&flag);
                    if(not flag)
                    {
                        ui->labelMin->setText("Ошибка!");
                        break;
                    }
                    if(cur<min)
                    {
                        min=cur;
                    }
                }
                else
                {
                    QTableWidgetItem * ti;
                    ti =  new QTableWidgetItem();
                    ui->table->setItem(i,j,ti);

                    ui->table->item(i,j)->setBackgroundColor(Qt::red);
                    ui->labelMin->setText("Ошибка!");
                    flag = false;
                }
                if(cur<min)
                {
                    min=cur;
                }
                if(not flag)
                {
                    continue;
                }
            }
            if(not flag)
            {
                continue;
            }
        }
        if(flag)
        {
            ui->labelMin->setNum(min);
        }
    }
    else
    {
        ui->labelOtl_2->setText("Ошибка! Нет значений");
    }
}

//среднее
void MainWindow::on_avg_clicked()
{
    int row=ui->table->rowCount();
    int column=ui->table->columnCount();

    double avg;
    double sum=0;
    double c=row*column;
    bool flag = true;

    if (row != 0)
    {
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                if (ui->table->item(i,j)!=nullptr)
                {
                    sum+=ui->table->item(i,j)->text().toInt(&flag);
                    if(not flag)
                    {
                        ui->labelAvg->setText("Ошибка!");
                        break;
                    }
                }
                else
                {
                    QTableWidgetItem * ti;
                    ti =  new QTableWidgetItem();
                    ui->table->setItem(i,j,ti);

                    ui->table->item(i,j)->setBackgroundColor(Qt::red);
                    ui->labelAvg->setText("Ошибка!");
                    flag = false;
                }
                if(not flag)
                {
                    continue;
                }
            }
            if(not flag)
            {
                continue;
            }
        }
        avg = sum / c;
        if(flag)
        {
            ui->labelAvg->setNum(avg);
        }
    }
    else
    {
        ui->labelOtl_2->setText("Ошибка! Нет значений");
    }
}

//произведение
void MainWindow::on_mul_clicked()
{
    int row=ui->table->rowCount();
    int column=ui->table->columnCount();

    int mul=1;
    bool flag = true;

    if (row != 0)
    {
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<column;j++)
            {
                if (ui->table->item(i,j)!=nullptr)
                {
                    mul*=ui->table->item(i,j)->text().toInt(&flag);
                    if(not flag)
                    {
                        ui->labelMul->setText("Ошибка!");
                        break;
                    }
                }
                else
                {
                    QTableWidgetItem * ti;
                    ti =  new QTableWidgetItem();
                    ui->table->setItem(i,j,ti);

                    ui->table->item(i,j)->setBackgroundColor(Qt::red);
                    ui->labelMul->setText("Ошибка!");
                    flag = false;
                }
                if(not flag)
                {
                    continue;;
                }
            }
            if(not flag)
            {
                continue;
            }
        }
        if(flag)
        {
            ui->labelMul->setNum(mul);
        }
    }
    else
    {
        ui->labelOtl_2->setText("Ошибка! Нет значений");
    }
}

//изменение ячейки
void MainWindow::on_table_cellChanged(int row, int column)
{
    if (not generating and not sorting and not deleting and not searching)
    {
        ui->labelOtl->setText(" ");
        ui->labelOtl_2->setText(" ");

        if (not searching)
        {
            ui->textEdit->clear();
        }

        if (not sorting)
        {
            ui->labelMax->setText(" ");
            ui->labelMin->setText(" ");
            ui->labelAvg->setText(" ");
            ui->labelSum->setText(" ");
            ui->labelMul->setText(" ");
        }


        bool flag;
        int cur;

        cur = ui->table->item(row, column)->text().toInt(&flag);
        int i = row;
        int j = column;

        if(not flag)
        {
            ui->table->item(i, j)->setBackgroundColor(Qt::red);
        }
        else
        {
            ui->table->item(i, j)->setBackgroundColor(Qt::white);
        }
    }
}

//быстрая сортировка
void quicksort(int *arr, int row)
{
    int i=0, j=row-1;
    int mid = arr[row/2];

    do
    {
        //Пробегаем элементы, ищем те, которые нужно перекинуть в другую часть
        //В левой части массива пропускаем(оставляем на месте) элементы, которые меньше центрального
        while(arr[i] < mid)
        {
            i++;
        }

        //В правой части пропускаем элементы, которые больше центрального
        while(arr[j] > mid)
        {
            j--;
        }

        //Меняем элементы местами
        if (i <= j)
        {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;

            i++;
            j--;
        }
    }
    while (i <= j);

    //Рекурсивные вызовы, если осталось, что сортировать
        if(j > 0)
        {
            //"Левый кусок"
            quicksort(arr, j + 1);
        }
        if (i < row)
        {
            //"Првый кусок"
            quicksort(&arr[i], row - i);
        }
}

//сортировка
void MainWindow::on_sort_clicked()
{
    int row = ui->table->rowCount();
    int column = ui->table->columnCount();

    ui->table->clearSelection();
    ui->labelOtl_2->setText(" ");
    ui->textEdit->clear();

    sorting = true;

    for(int j=0; j<column; j++)
    {
        int c = 0;
        int ci;
        bool flag = true;

        //выделение памяти
        int *arr;

        try
        {
            arr = new int[row];
        }
        catch (std::bad_alloc&)
        {
            ui->labelOtl->setText("Ошибка, невозможно выделить память");
            flag=false;
        }

        if (flag)
        {
            //создание массива
            for (int i=0;i<row;i++)
            {
                if (ui->table->item(i,j) != nullptr)
                {
                    ci = ui->table->item(i,j)->text().toInt(&flag);
                    if (not flag)
                    {
                        ui->labelOtl->setText("Ошибка! Некорректное значение.");
                        break;
                    }
                    arr[i] = ci;
                    c += 1;
                }
                else
                {
                    QTableWidgetItem * ti;
                    ti =  new QTableWidgetItem();
                    ui->table->setItem(i,j,ti);

                    ui->table->item(i,j)->setBackgroundColor(Qt::red);
                    ui->labelOtl->setText("Ошибка!");
                    delete []arr;
                    flag = false;
                }
            }
            if(not flag)
            {
                delete []arr;
                continue;
            }
            //сортировка
            if(ui->comboboxSort->currentText() == "Пузырек")
            {
                int temp;

                for (int i = 0; i < c - 1; i++)
                {
                    for (int j = 0; j < c - i - 1; j++)
                    {
                        if (arr[j] > arr[j + 1])
                        {
                            temp = arr[j];
                            arr[j] = arr[j + 1];
                            arr[j + 1] = temp;
                        }
                    }
                }
            }
            else if(ui->comboboxSort->currentText() == "Гномья")
            {
                int i;
                int temp;

                i = 0;
                while (i < c)
                {
                    if (i == 0 or arr[i - 1] <= arr[i])
                        ++i;
                    else
                    {
                        temp = arr[i];
                        arr[i] = arr[i - 1];
                        arr[i - 1] = temp;
                        --i;
                    }
                }
            }
            else if(ui->comboboxSort->currentText() == "Быстрая")
            {
                quicksort(arr, row);
            }
            else if(ui->comboboxSort->currentText() == "Расческа")
            {
                int step = row;
                bool prz = true;

                while(step > 1 or prz)
                {
                    if(step > 1)
                    {
                        step = step / 1.247;
                    }
                    prz = false;
                    int i = 0;
                    while(i + step < row)
                    {
                        if(arr[i] > arr[i + step])
                        {
                            int tmp = arr[i];
                            arr[i] = arr[i + step];
                            arr[i + step] = tmp;
                            prz = true;
                        }
                        i = i + 1;
                    }
                }
            }
            else if(ui->comboboxSort->currentText() == "Обезьянья")
            {
                if (ui->table->rowCount() > 10)
                {
                    ui->labelOtl->setText("Слишком много значений");
                    delete []arr;
                    return;
                }
                while(true)
                {
                    flag = true;
                    for(int i = 0; i < row - 1; i++)
                        {
                            if(arr[i] > arr[i + 1])
                            {
                                flag = false;
                                break;
                            }
                        }
                    if(flag)
                    {
                        break;
                    }
                    else
                    {
                        for(int i = 0; i < row - 1; i++)
                        {
                            int tmpn = rand() % row;
                            int tmp = arr[tmpn];
                            arr[tmpn] = arr[i];
                            arr[i] = tmp;
                        }
                    }
                }
            }
            //вывод
            QString curv;
            for (int i=0;i<row;i++)
            {
                curv = QString::number(arr[i]);
                ui->table->item(i,j)->setText(curv);
            }
            delete []arr;
        }
        else
        {
            delete []arr;
            return;
        }
    }
    sorting = false;
    sorted = true;
}

//удаление дубликатов
void MainWindow::on_del_clicked()
{
    int row = ui->table->rowCount();
    int column = ui->table->columnCount();

    ui->table->clearSelection();
    ui->labelOtl_2->setText(" ");

    deleting = true;

    for(int j=0; j<column; j++)
    {
        int ci;
        bool flag = true;
        int new_length = row;

        //выделение памяти
        int *arr;

        try
        {
            arr = new int[row];
        }
        catch (std::bad_alloc&)
        {
            ui->labelOtl->setText("Ошибка, невозможно выделить память");
            flag=false;
        }

        if (flag)
        {
            //создание массива
            for (int i=0;i<row;i++)
            {
                if (ui->table->item(i,j) != nullptr)
                {
                    ci = ui->table->item(i,j)->text().toInt(&flag);
                    if (not flag)
                    {
                        ui->labelOtl->setText("Ошибка!");
                        break;
                    }
                    arr[i] = ci;
                }
                else
                {
                    QTableWidgetItem * ti;
                    ti =  new QTableWidgetItem();
                    ui->table->setItem(i,j,ti);

                    ui->table->item(i,j)->setBackgroundColor(Qt::red);
                    ui->labelOtl->setText("Ошибка!");
                    delete []arr;
                    flag = false;
                }
            }
            if(not flag)
            {
                delete []arr;
                continue;
            }

            //проверка сортировки
            for (int i=0; i<row-1; i++)
            {
                if (arr[i] > arr[i+1])
                {
                    sorted = false;
                }
            }

            if (sorted)
            {
                //удаление дубликатов
                int *newarr;
                int c = 0;
                try
                {
                    newarr = new int[new_length];
                }
                catch (std::bad_alloc&)
                {
                    ui->labelOtl->setText("Ошибка, невозможно выделить память");
                    flag=false;
                }


                for (int i = 0; i < new_length-1; i++)
                {
                    if (arr[i] != arr[i+1] and arr[i] != newarr[c - 1])
                    {
                        newarr[c] = arr[i];
                        c++;
                    }
                }

                //вывод
                QString curv;

                for (int i=0;i<new_length;i++)
                {
                    curv = QString::number(newarr[i]);
                    ui->table->item(i,j)->setText(curv);
                }
                ui->table->setRowCount(new_length);
                ui->spinBox->setValue(new_length);
                delete []arr;
                delete []newarr;
            }
            else
            {
                ui->labelOtl->setText("Массив не отсортирован");
                delete []arr;
            }
        }
    }
    deleting = false;
}

//поиск
void MainWindow::on_search_clicked()
{
    bool flag;
    int row = ui->table->rowCount();
    int column = ui->table->columnCount();
    int srch;

    ui->textEdit->clear();
    ui->labelOtl->setText(" ");
    ui->table->clearSelection();
    ui->labelOtl_2->setText(" ");

    searching = true;

    srch = ui->searchValue->text().toInt(&flag);

    if (flag)
    {
        for(int j=0; j<column; j++)
        {
            int ci;
            bool flag = true;
            int srch = ui->searchValue->text().toInt(&flag);
            bool sorted = true;
            int c = 0;

            if (flag)
            {
                //выделение памяти
                int *arr;

                try
                {
                    arr = new int[row];
                }
                catch (std::bad_alloc&)
                {
                    ui->labelOtl->setText("Ошибка, невозможно выделить память");
                    flag=false;
                }

                //создание массива
                if (flag)
                {
                    for (int i=0;i<row;i++)
                    {
                        if (ui->table->item(i,j) != nullptr)
                        {
                            ci = ui->table->item(i,j)->text().toInt(&flag);
                            if (not flag)
                            {
                                ui->labelOtl->setText("Ошибка! Некорректное значение.");
                                flag = false;
                            }
                            arr[i] = ci;
                        }
                        else
                        {
                            QTableWidgetItem * ti;
                            ti =  new QTableWidgetItem();
                            ui->table->setItem(i,j,ti);

                            ui->table->item(i,j)->setBackgroundColor(Qt::red);
                            ui->labelOtl->setText("Ошибка! Пустая ячейка");
                            delete []arr;
                            flag = false;
                        }
                    }
                    if(not flag)
                    {
                        delete []arr;
                        continue;
                    }
                }

                //проверка сортировки
                for (int i=0; i<row-1; i++)
                {
                    if (arr[i] > arr[i+1])
                    {
                        sorted = false;
                        delete []arr;
                        break;
                    }
                }

                //поиск
                if (sorted)
                {
                    bool flag = false;
                    int l = 0;
                    int r = row - 1;
                    int mid;

                    while ((l <= r) and (flag != true))
                    {
                        mid = (l + r) / 2;

                        if (arr[mid] == srch)
                            flag = true;
                        if (arr[mid] > srch)
                            r = mid - 1;
                        else
                            l = mid + 1;
                    }
                    if (flag)
                    {
                        //ui->labelOtl_2->setText("Найдено в ячейке " + QString::number(mid + 1) + "," + QString::number(j+1));
                        std::cout << "found in cell " << mid+1 << "," << j+1 << std::endl;
                        ui->table->item(mid,j)->setSelected(true);
                        ui->textEdit->append("Найдено в ячейке " + QString::number(mid + 1));
                        c++;

                        int amid = mid + 1;
                        while (arr[amid] == arr[mid])
                        {
                            std::cout << "found in cell " << amid+1 << "," << j+1 << std::endl;
                            ui->table->item(amid,j)->setSelected(true);
                            ui->textEdit->append("Найдено в ячейке " + QString::number(amid + 1));
                            amid += 1;
                            c++;
                        }

                        amid = mid - 1;
                        while (arr[amid] == arr[mid])
                        {
                            std::cout << "found in cell " << amid+1 << "," << j+1 << std::endl;
                            ui->table->item(amid,j)->setSelected(true);
                            ui->textEdit->append("Найдено в ячейке " + QString::number(amid + 1));
                            amid -= 1;
                            c++;
                        }
                    }
                    else
                    {
                        ui->labelOtl_2->setText("Не найдено");
                    }
                }
                else
                {
                    flag = false;
                    for (int i=0; i<row; i++)
                    {
                        if (arr[i] == srch)
                        {
                            std::cout << "found in cell " << i+1 << "," << j+1 << std::endl;
                            ui->table->item(i,j)->setSelected(true);
                            flag = true;
                            ui->textEdit->append("Найдено в ячейке " + QString::number(i + 1));
                            c++;
                        }
                    }
                    if (not flag)
                    {
                        ui->labelOtl_2->setText("Не найдено");
                    }
                }
                if (c > 0)
                {
                    ui->labelOtl_2->setText("Найдено " + QString::number(c) + " значений");
                }
                delete []arr;
            }
        }
    }
    else
    {
        ui->labelOtl->setText("Некорретное искомое значение");
    }
    searching = false;
}

//открытие файла
void MainWindow::on_fileOpen_clicked()
{
    //если что-то открыто
    if(openedfileName != "new" and changed)
    {
        int reply = MessageBox();
        if(reply == QMessageBox::Yes)
        {
            //перезапись нового содержимого
            QFile file(openedfileName);
            file.open(QIODevice::WriteOnly);
            //сделать построчную запись
            file.close();
        }
        else if(reply == QMessageBox::Cancel)
        {
            return;
        }
        //открытие
        prevOpenedFileName = openedfileName;
        openedfileName = nullptr;
        openedfileName = QFileDialog::getOpenFileName(this, tr("Открытие"), "C:/Users/Nikita/Documents/", tr("Текстовые документы(*txt)"));
        if (openedfileName.isNull())
        {
            openedfileName = prevOpenedFileName;
            prevOpenedFileName = "";
            return;
        }
        ui->textEdit->setText("");
        QFile file(openedfileName);
        namearr = openedfileName.split("/");
        FileWindowTitle = namearr[namearr.size() - 1].left(namearr[namearr.size() - 1].indexOf("."));
        file.open(QIODevice::ReadOnly);
        //сделать построчный вывод
        QTextStream writeStream(&file);
        writeStream.setCodec("UTF-8");
        int cell;
        rows = writeStream.readLine().toInt();
        ui->table->setRowCount(rows);
        int curcell = 0;
        while (curcell<rows)
        {
            cell = writeStream.readLine().toInt();
            if (ui->table->item(curcell,0)==nullptr)
            {
                QTableWidgetItem * ti;
                ti =  new QTableWidgetItem();
                ui->table->setItem(curcell,0,ti);
            }
            ui->table->item(curcell,0)->setText(QString::number(cell));
            curcell++;
        }
        setWindowTitle(FileWindowTitle);
        file.close();
        changed = false;
        existed = true;

    }
    //если новый файл
    else if (openedfileName == "new" and changed)
    {
        int reply = MessageBox();
        if(reply == QMessageBox::Yes)
        {
            //сохранение
            QString newfileName = nullptr;
            newfileName = QFileDialog::getSaveFileName(this, tr("Сохранение"), "C:/Users/Nikita/Documents/Новый текстовый документ", "Текстовые документы(*.txt)");
            if (newfileName.isNull())
            {
                return;
            }
            QFile file(newfileName);
            openedfileName = newfileName;
            file.open(QIODevice::WriteOnly);
            //сделать построчную запись
            file.close();
        }
        else if(reply == QMessageBox::Cancel)
        {
            return;
        }
        //открытие
        prevOpenedFileName = openedfileName;
        openedfileName = nullptr;
        openedfileName = QFileDialog::getOpenFileName(this, tr("Открытие"), "C:/Users/Nikita/Documents/", tr("Текстовые документы(*txt)"));
        if (openedfileName.isNull())
        {
            openedfileName = prevOpenedFileName;
            prevOpenedFileName = "";
            return;
        }
        ui->textEdit->setText("");
        QFile file(openedfileName);
        namearr = openedfileName.split("/");
        FileWindowTitle = namearr[namearr.size() - 1].left(namearr[namearr.size() - 1].indexOf("."));
        file.open(QIODevice::ReadOnly);
        //сделать построчный вывод
        QTextStream writeStream(&file);
        writeStream.setCodec("UTF-8");
        int cell;
        rows = writeStream.readLine().toInt();
        ui->table->setRowCount(rows);
        int curcell = 0;
        while (curcell<rows)
        {
            cell = writeStream.readLine().toInt();
            if (ui->table->item(curcell,0)==nullptr)
            {
                QTableWidgetItem * ti;
                ti =  new QTableWidgetItem();
                ui->table->setItem(curcell,0,ti);
            }
            ui->table->item(curcell,0)->setText(QString::number(cell));
            curcell++;
        }
        file.close();
        setWindowTitle(FileWindowTitle);
        changed = false;
        existed = true;
    }
    else
    {
        prevOpenedFileName = openedfileName;
        openedfileName = nullptr;
        openedfileName = QFileDialog::getOpenFileName(this, tr("Открытие"), "C:/Users/Nikita/Documents/", tr("Текстовые документы(*txt)"));
        if (openedfileName.isNull())
        {
            openedfileName = prevOpenedFileName;
            prevOpenedFileName = "";
            return;
        }
        ui->textEdit->setText("");
        QFile file(openedfileName);
        namearr = openedfileName.split("/");
        FileWindowTitle = namearr[namearr.size() - 1].left(namearr[namearr.size() - 1].indexOf("."));
        file.open(QIODevice::ReadOnly);
        //сделать построчный вывод
        QTextStream writeStream(&file);
        writeStream.setCodec("UTF-8");
        int cell;
        rows = writeStream.readLine().toInt();
        ui->table->setRowCount(rows);
        int curcell = 0;
        while (curcell<rows)
        {
            cell = writeStream.readLine().toInt();
            if (ui->table->item(curcell,0)==nullptr)
            {
                QTableWidgetItem * ti;
                ti =  new QTableWidgetItem();
                ui->table->setItem(curcell,0,ti);
            }
            ui->table->item(curcell,0)->setText(QString::number(cell));
            curcell++;
        }
        setWindowTitle(FileWindowTitle);
        file.close();
        changed = false;
        existed = true;
    }
}


void MainWindow::on_fileSave_clicked()
{
    QString newfileName = nullptr;
    newfileName = QFileDialog::getSaveFileName(this, tr("Сохранение"), "C:/Users/Nikita/Documents/Новый текстовый документ", "Текстовые документы(*.txt)");
    if (newfileName.isNull() or !newfileName.endsWith(".txt"))
    {
        if (!newfileName.isNull())
        {
            QMessageBox::warning(this, "Внимание!", "Файл сохраняется в неверном формате.");
        }
        return;
    }
    QFile file(newfileName);
    openedfileName = newfileName;
    namearr = openedfileName.split("/");
    FileWindowTitle = namearr[namearr.size() - 1].left(namearr[namearr.size() - 1].indexOf("."));
    file.open(QIODevice::WriteOnly);
    QTextStream writeStream(&file);
    writeStream.setCodec("UTF-8");
    //построчная запись
    rows = ui->table->rowCount();
    int curcell = 0;
    writeStream << rows << endl;
    writeStream.flush();
    while (curcell < rows)
    {
        writeStream << ui->table->item(curcell, 0)->text() << endl;
        writeStream.flush();
        curcell++;
    }
    file.close();
    setWindowTitle(FileWindowTitle);
    existed = true;
    changed = false;
}
