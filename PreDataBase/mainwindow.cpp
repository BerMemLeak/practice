#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Исходное состояние виджетов
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    /*
     * Выделим память под необходимые объекты. Все они наследники
     * QObject, поэтому воспользуемся иерархией.
    */

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    //Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    /*
     * Устанавливаем данные для подключениея к БД.
     * Поскольку метод небольшой используем лямбда-функцию.
     */
    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });



    /*
     *  Сигнал для подключения к БД
     */
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    //Отобразим диалоговое окно. Какой метод нужно использовать?
    dataDb->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered()
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");


       auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
       QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }

}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{
    QSqlDatabase db = QSqlDatabase::database(DB_NAME);
    QString filterText = ui->cb_category->currentText();

    qDebug() << "=== НАЖАТА КНОПКА ПОЛУЧИТЬ ===";
    qDebug() << "Выбран фильтр:" << filterText;

    if (filterText == "Все" || filterText.isEmpty()) {
        QSqlTableModel *model = new QSqlTableModel(this, db);
        model->setTable("film");

        if (!model->select()) {
            qDebug() << "Ошибка SELECT:" << model->lastError().text();
            return;
        }
//вот тут хз хз , по идее сначала надо скрыть, а потом отображать
        ui->tb_result->setModel(model);

        for (int i = 0; i < model->columnCount(); ++i) {
            if (i != 1 && i != 2) {
                ui->tb_result->setColumnHidden(i, true);
            }
        }

        model->setHeaderData(1, Qt::Horizontal, "Название фильма");
        model->setHeaderData(2, Qt::Horizontal, "Описание фильма");

        qDebug() << "Загружено фильмов (Все):" << model->rowCount();
    }
    else {
        QString queryStr;
        queryStr = "SELECT title, description FROM film f "
                   "JOIN film_category fc on f.film_id = fc.film_id "
                   "JOIN category c on c.category_id = fc.category_id "
                   "WHERE c.name = ";
        if (filterText == "Комедия") {queryStr  +="'Comedy'";}
        else if (filterText == "Ужасы") {queryStr += "'Horror'";}

        QSqlQueryModel *queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(queryStr, db);

        if (queryModel->lastError().isValid()) {
            qDebug() << "ОШИБКА SQL ЗАПРОСА:" << queryModel->lastError().text();
            return;
        }

        queryModel->setHeaderData(0, Qt::Horizontal, "Название фильма");
        queryModel->setHeaderData(1, Qt::Horizontal, "Описание фильма");
        ui->tb_result->setModel(queryModel);
    }
}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */

/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }

}




void MainWindow::on_pb_clear_clicked()
{
    ui->tb_result->setModel(nullptr);
}

