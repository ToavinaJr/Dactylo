#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QElapsedTimer>
#include <QVector>
#include <QTextCharFormat>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void startPractice();
    void resetPractice();
    void checkTyping();

private:
    void setupUi();
    void updateStats();

    QLabel *textToType;
    QTextEdit *typingArea;
    QLabel *wpmLabel;
    QLabel *accuracyLabel;
    QLabel *mistakesLabel;
    QLabel *ghostTextLabel;
    QPushButton *startButton;
    QPushButton *resetButton;

    QString currentText;
    QElapsedTimer startTime;
    int mistakes;
    bool isPracticeActive;
    QVector<QString> practiceTexts;

    QString currentTypedText;
    QTextCharFormat correctFormat;
    QTextCharFormat errorFormat;
};

#endif
