#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVBoxLayout>
#include <QPointer>
#include <QWidget>
#include "widgets.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void moveMovableWidget(); // Переместить виджет панель <-> окно
    void deletePanel(); // Удалить панель
    void demoDetaching(); // Демо setParent(nullptr)

private:
    Ui::MainWindow *ui;

private:
    void setupUi();
    void setStatus(const QString &text);

    // QPointer — "умный" указатель, который автоматически
    // становится null при удалении объекта
    QPointer<QVBoxLayout> m_rootLayout;
    QPointer<QVBoxLayout> m_panelLayout;

    QPointer<LogLabel> m_title;
    QPointer<LogLineEdit> m_input; // Поле ввода
    QPointer<LogLabel> m_status; // Статус
    QPointer<LogPanel> m_panel; // Панель
    QPointer<LogButton> m_movable; // Передвигаемый виджет
    QPointer<LogButton> m_moveButton; // Кнопка, перемещающая виджет
    QPointer<LogButton> m_deleteButton;
};
#endif // MAINWINDOW_H
