#include "mainwindow.h"

#include <QDateEdit>
#include <QSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
#include <QDate>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringConverter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_dateEdit(nullptr),
    m_minutesSpin(nullptr),
    m_subjectEdit(nullptr),
    m_output(nullptr)
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    // 日期
    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat("yyyy-MM-dd");

    // 时长（分钟）
    m_minutesSpin = new QSpinBox(this);
    m_minutesSpin->setRange(1, 24 * 60);
    m_minutesSpin->setValue(60);

    // 科目 / 备注
    m_subjectEdit = new QLineEdit(this);

    // 输出区域
    m_output = new QTextEdit(this);
    m_output->setReadOnly(true);

    // 按钮
    auto *addBtn   = new QPushButton(tr("添加记录"), this);
    auto *showBtn  = new QPushButton(tr("查看记录列表"), this);
    auto *totalBtn = new QPushButton(tr("统计总时长"), this);

    connect(addBtn,  &QPushButton::clicked, this, &MainWindow::onAddRecord);
    connect(showBtn, &QPushButton::clicked, this, &MainWindow::onShowRecords);
    connect(totalBtn,&QPushButton::clicked, this, &MainWindow::onShowTotal);

    // 表单布局
    auto *formLayout = new QGridLayout();
    formLayout->addWidget(new QLabel(tr("日期：")), 0, 0);
    formLayout->addWidget(m_dateEdit,               0, 1);
    formLayout->addWidget(new QLabel(tr("时长（分钟）：")), 1, 0);
    formLayout->addWidget(m_minutesSpin,                 1, 1);
    formLayout->addWidget(new QLabel(tr("科目 / 备注：")), 2, 0);
    formLayout->addWidget(m_subjectEdit,                 2, 1);

    // 按钮布局
    auto *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(showBtn);
    btnLayout->addWidget(totalBtn);
    btnLayout->addStretch();

    // 总体布局
    auto *mainLayout = new QVBoxLayout(central);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_output);

    setWindowTitle(tr("学习打卡工具（GUI版）"));
    resize(600, 400);
}

MainWindow::~MainWindow() = default;

QString MainWindow::logFilePath() const
{
    // 使用和可执行文件同目录的 study_log.txt
    const QString dir = QCoreApplication::applicationDirPath();
    return QDir(dir).filePath("study_log.txt");
}

// 添加记录
void MainWindow::onAddRecord()
{
    const QString dateStr = m_dateEdit->date().toString("yyyy-MM-dd");
    const int minutes = m_minutesSpin->value();
    const QString subject = m_subjectEdit->text().trimmed();

    if (subject.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("科目 / 备注不能为空。"));
        return;
    }

    QFile file(logFilePath());
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, tr("错误"), tr("无法打开记录文件。"));
        return;
    }

    QTextStream out(&file);
    // Qt6：默认就是 UTF-8，其实这行可以不要
    // out.setCodec("UTF-8");
    out.setEncoding(QStringConverter::Utf8);
    // 格式与你原来的命令行版保持一致：日期 分钟 科目...
    out << dateStr << " " << minutes << " " << subject << "\n";
    file.close();

    m_output->append(
        tr("已添加：%1 | %2 分钟 | %3")
            .arg(dateStr)
            .arg(minutes)
            .arg(subject)
        );
    m_subjectEdit->clear();
}

// 查看记录列表
void MainWindow::onShowRecords()
{
    QFile file(logFilePath());
    if (!file.exists()) {
        QMessageBox::information(this, tr("提示"), tr("当前还没有任何记录。"));
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("错误"), tr("无法读取记录文件。"));
        return;
    }

    QTextStream in(&file);
    // in.setCodec("UTF-8");
    in.setEncoding(QStringConverter::Utf8);

    m_output->clear();
    m_output->append(tr("--- 所有学习记录 ---"));

    while (!in.atEnd()) {
        const QString line = in.readLine();
        if (line.trimmed().isEmpty())
            continue;

        // 直接把原始行展示出来即可
        m_output->append(line);
    }

    file.close();
    m_output->append(tr("---------------------\n"));
}

// 统计总时长
void MainWindow::onShowTotal()
{
    QFile file(logFilePath());
    if (!file.exists()) {
        QMessageBox::information(this, tr("提示"), tr("当前还没有任何记录。"));
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("错误"), tr("无法读取记录文件。"));
        return;
    }

    QTextStream in(&file);
    // in.setCodec("UTF-8");
    in.setEncoding(QStringConverter::Utf8);

    int total = 0;

    while (!in.atEnd()) {
        const QString line = in.readLine();
        if (line.trimmed().isEmpty())
            continue;

        // 格式：date minutes subject...
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() < 2)
            continue;

        bool ok = false;
        int minutes = parts.at(1).toInt(&ok);
        if (ok && minutes > 0)
            total += minutes;
    }

    file.close();

    const double hours = total / 60.0;
    QString msg = tr("总学习时长：%1 分钟（约 %2 小时）")
                      .arg(total)
                      .arg(QString::number(hours, 'f', 1));

    m_output->append(msg);
    QMessageBox::information(this, tr("统计结果"), msg);
}
