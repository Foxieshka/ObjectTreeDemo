#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)    
{
    qDebug() << "[CREATE] MainWindow";
    setWindowTitle("Дерево объектов Qt: демонстрация");
    resize(500, 400);
    setupUi();
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
}

void MainWindow::setupUi()
{
    // Создаем корневой layout
    m_rootLayout = new QVBoxLayout(this);

    // Виджеты верхнего уровня (родитель - this)
    m_title = new LogLabel("Дерево объектов Qt: владелец и потомки", this);
    m_rootLayout->addWidget(m_title.data());

    m_input = new LogLineEdit(this);
    m_input->setPlaceholderText("Виджет-ребёнок главного окна");
    m_rootLayout->addWidget(m_input.data());

    m_status = new LogLabel("Статус: окно создано", this);
    m_status->setWordWrap(true);
    m_rootLayout->addWidget(m_status.data());

    // Создаем панель (родитель - this)
    m_panel = new LogPanel("Панель-владелец", this);
    m_rootLayout->addWidget(m_panel.data());

    // Внутри панели создаем свой layout
    m_panelLayout = new QVBoxLayout(m_panel.data());

    // Статическая кнопка (всегда в панели)
    LogButton *staticButton = new LogButton("Статическая кнопка внутри панели", m_panel.data());
    m_panelLayout->addWidget(staticButton);

    // Перемещаемая кнопка (сейчас в панели, родитель — m_panel)
    m_movable = new LogButton("Перемещаемый виджет (в панели)", m_panel.data());
    m_panelLayout->addWidget(m_movable.data());

    // Кнопки управления (родитель — this)
    m_moveButton = new LogButton("Переместить виджет: панель <-> окно", this);
    connect(m_moveButton.data(), &QPushButton::clicked,
            this, &MainWindow::moveMovableWidget);
    m_rootLayout->addWidget(m_moveButton.data());

    m_deleteButton = new LogButton("Удалить панель (deleteLater)", this);
    connect(m_deleteButton.data(), &QPushButton::clicked,
            this, &MainWindow::deletePanel);
    m_rootLayout->addWidget(m_deleteButton.data());

    LogButton *detachButton = new LogButton("Демо setParent(nullptr)", this);
    connect(detachButton, &QPushButton::clicked,
            this, &MainWindow::demoDetaching);
    m_rootLayout->addWidget(detachButton);

    m_rootLayout->addStretch();

    setStatus("Готово. Все виджеты созданы с родителями.");
}

void MainWindow::setStatus(const QString &text)
{
    if (!m_status.isNull()) {
        m_status->setText(text);
    }
}

// Перемещение виджета между панелью и окном
void MainWindow::moveMovableWidget(){
    if(m_movable.isNull()){
        setStatus("Перемещаемый виджет уже уничтожен.");
    }
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена. Вернуть виджет в неё невозможно.");
        return;
    }
    if(m_movable->parentWidget() == m_panel.data())
    {
        // Если виджет в панели, переносим в главное окно
        m_panelLayout->removeWidget(m_movable.data());
        m_movable->setParent(this);
        m_rootLayout->addWidget(m_movable.data());
        m_movable->setText("Перемещаемый виджет (вне панели)");
        setStatus("setParent(this): теперь главное окно — владелец. Панель больше не удалит виджет.");
    }
    else
    {
        // Сейчас виджет в окне → возвращаем в панель
        m_rootLayout->removeWidget(m_movable.data());
        m_movable->setParent(m_panel.data());
        m_panelLayout->addWidget(m_movable.data());
        m_movable->setText("Перемещаемый виджет (в панели)");
        setStatus("Виджет снова ребёнок панели. Теперь он будет удалён вместе с панелью.");
    }
    m_movable->show();
    qDebug() << "[SET_PARENT] Новый владелец:" << m_movable->parentWidget()->objectName();
}

// === Удаление панели ===
void MainWindow::deletePanel()
{
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена.");
        return;
    }
    if (!m_movable.isNull() && m_movable->parentWidget() == m_panel.data()) {
        setStatus("Панель будет удалена. Перемещаемый виджет — её ребёнок, поэтому будет уничтожен автоматически.");
    } else {
        setStatus("Панель будет удалена. Перемещаемый виджет не её ребёнок, поэтому останется жив.");
    }
    if (!m_deleteButton.isNull()) m_deleteButton->setEnabled(false);
    if (!m_moveButton.isNull())   m_moveButton->setEnabled(false);
    m_panel->deleteLater(); // Безопасное удаление: объект удалится, когда управление вернётся в цикл событий
    m_panel = nullptr;      // QPointer сам станет null, но так нагляднее
}

// === Демонстрация setParent(nullptr) ===
void MainWindow::demoDetaching()
{
    LogPanel *tempOwner = new LogPanel("TempOwner", this);
    LogLabel *child1 = new LogLabel("Ребёнок TempOwner", tempOwner);
    LogLabel *child2 = new LogLabel("Отвязанный ребёнок TempOwner", tempOwner);

    qDebug() << "[DEMO] Детей у tempOwner до detach:" << tempOwner->children().size();

    // Отвязываем child2 от родителя
    child2->setParent(nullptr);
    qDebug() << "[DEMO] Детей у tempOwner после detach:" << tempOwner->children().size();

    // child2 теперь без владельца → нужно удалить вручную
    child2->deleteLater();

    // Удаляем tempOwner — child1 удалится автоматически
    tempOwner->deleteLater();

    setStatus("Демо: setParent(nullptr) отвязал виджет, но его нужно удалить вручную (deleteLater).");
}

