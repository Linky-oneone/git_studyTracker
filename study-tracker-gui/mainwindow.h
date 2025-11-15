#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QDateEdit;
class QSpinBox;
class QLineEdit;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onAddRecord();      // 添加记录
    void onShowRecords();    // 查看记录列表
    void onShowTotal();      // 统计总时长

private:
    QString logFilePath() const;

    QDateEdit *m_dateEdit;
    QSpinBox  *m_minutesSpin;
    QLineEdit *m_subjectEdit;
    QTextEdit *m_output;
};

#endif // MAINWINDOW_H
