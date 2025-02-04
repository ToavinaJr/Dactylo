#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mistakes(0), isPracticeActive(false)
{
    practiceTexts = {
        "The quick brown fox jumps over the lazy dog.",
        "Pack my box with five dozen liquor jugs.",
        "How vexingly quick daft zebras jump!",
        "The five boxing wizards jump quickly."
    };

    // Initialize text formats
    correctFormat.setBackground(QColor(100, 255, 100, 50));  // Light green
    correctFormat.setForeground(Qt::white);

    errorFormat.setBackground(QColor(255, 100, 100));  // Light red
    errorFormat.setForeground(Qt::white);

    setupUi();
    setWindowTitle("Dactylo - Typing Practice");
    setMinimumSize(800, 600);
}

void MainWindow::setupUi()
{
    // Style du fond de la fenêtre principale
    setStyleSheet("QMainWindow { background-color: #2C2C2C; }");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    // Ajout d'un titre stylisé
    QLabel *titleLabel = new QLabel("Dactylo - Typing Practice", this);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #4CAF50; margin-bottom: 20px;");
    layout->addWidget(titleLabel);

    // Text display avec style amélioré
    textToType = new QLabel(this);
    textToType->setWordWrap(true);
    textToType->setFont(QFont("Arial", 16));
    textToType->setAlignment(Qt::AlignCenter);
    textToType->setStyleSheet(
        "QLabel {"
        "   background-color: #363636;"
        "   border-radius: 10px;"
        "   padding: 20px;"
        "   color: #E0E0E0;"
        "   border: 2px solid #4CAF50;"
        "}"
        );
    layout->addWidget(textToType);

    // Typing area avec style moderne
    typingArea = new QTextEdit(this);
    typingArea->setFont(QFont("Arial", 16));
    typingArea->setStyleSheet(
        "QTextEdit {"
        "   background-color: #1E1E1E;"
        "   border: 2px solid #3C3C3C;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "   color: #FFFFFF;"
        "   selection-background-color: #404040;"
        "}"
        "QTextEdit:focus {"
        "   border: 2px solid #4CAF50;"
        "}"
    );

    typingArea->setMinimumHeight(150);
    connect(typingArea, &QTextEdit::textChanged, this, &MainWindow::checkTyping);
    typingArea->setEnabled(false);
    layout->addWidget(typingArea);

    // Stats layout dans un cadre stylisé
    QFrame *statsFrame = new QFrame(this);
    statsFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #363636;"
        "   border-radius: 10px;"
        "   padding: 15px;"
        "   margin: 10px 0px;"
        "}"
        );
    QHBoxLayout *statsLayout = new QHBoxLayout(statsFrame);
    statsLayout->setSpacing(20);

    // Style commun pour les labels de statistiques
    QString statsLabelStyle =
        "QLabel {"
        "   background-color: #2C2C2C;"
        "   padding: 10px 20px;"
        "   border-radius: 5px;"
        "   color: #E0E0E0;"
        "}";

    wpmLabel = new QLabel("WPM: 0", this);
    accuracyLabel = new QLabel("Accuracy: 100%", this);
    mistakesLabel = new QLabel("Mistakes: 0", this);

    QFont statsFont("Arial", 14);
    for (QLabel* label : {wpmLabel, accuracyLabel, mistakesLabel}) {
        label->setFont(statsFont);
        label->setStyleSheet(statsLabelStyle);
        statsLayout->addWidget(label);
    }

    layout->addWidget(statsFrame);

    // Button layout avec style moderne
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   border: none;"
        "   color: white;"
        "   padding: 12px 25px;"
        "   border-radius: 5px;"
        "   font-size: 14px;"
        "   min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #666666;"
        "}";

    startButton = new QPushButton("Start Practice", this);
    resetButton = new QPushButton("Reset", this);

    startButton->setStyleSheet(buttonStyle);
    resetButton->setStyleSheet(buttonStyle);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startPractice);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetPractice);

    resetButton->setEnabled(false);

    buttonLayout->addStretch();
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    // Affichage du texte fantôme (ghost mode)
    ghostTextLabel = new QLabel(this);
    ghostTextLabel->setWordWrap(true);
    ghostTextLabel->setFont(QFont("Arial", 16));
    ghostTextLabel->setAlignment(Qt::AlignCenter);
    ghostTextLabel->setStyleSheet(
        "QLabel {"
        "   color: #757575;"  // Gris clair
        "   background-color: transparent;"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "}"
        );
    layout->addWidget(ghostTextLabel);
}


void MainWindow::startPractice()
{
    int randomIndex = QRandomGenerator::global()->bounded(practiceTexts.size());
    currentText = practiceTexts[randomIndex];
    textToType->setText(currentText);
    typingArea->clear();
    typingArea->setEnabled(true);
    startButton->setEnabled(false);
    resetButton->setEnabled(true);
    mistakes = 0;
    isPracticeActive = true;
    startTime.start();
    typingArea->setFocus();
    ghostTextLabel->setText(currentText);
}

void MainWindow::resetPractice()
{
    typingArea->clear();
    typingArea->setEnabled(false);
    startButton->setEnabled(true);
    resetButton->setEnabled(false);
    textToType->clear();
    wpmLabel->setText("WPM: 0");
    accuracyLabel->setText("Accuracy: 100%");
    mistakesLabel->setText("Mistakes: 0");
    isPracticeActive = false;
}

void MainWindow::checkTyping()
{
    if (!isPracticeActive) return;

    currentTypedText = typingArea->toPlainText();
    QString referenceText = currentText.left(currentTypedText.length());

    // Désactiver temporairement les signaux pour éviter les appels récursifs
    typingArea->blockSignals(true);

    QTextDocument *doc = typingArea->document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);
    cursor.setCharFormat(QTextCharFormat()); // Réinitialiser le format

    mistakes = 0;

    for (int i = 0; i < currentText.length(); ++i) {
        QTextCharFormat format;

        if (i < currentTypedText.length()) {
            if (currentTypedText[i] == currentText[i]) {
                format.setBackground(QColor(100, 255, 100, 50)); // Vert clair
                format.setForeground(Qt::white);
            } else {
                format.setBackground(QColor(255, 100, 100)); // Rouge clair
                format.setForeground(Qt::white);
                mistakes++;
            }
        } else {
            // Mode ghost pour le texte non encore tapé (gris clair)
            format.setForeground(QColor(200, 200, 200));
        }

        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
        cursor.mergeCharFormat(format);
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 0);
    }

    // Restaurer le curseur à sa position initiale
    QTextCursor newCursor = typingArea->textCursor();
    newCursor.setPosition(currentTypedText.length());
    typingArea->setTextCursor(newCursor);

    // Réactiver les signaux
    typingArea->blockSignals(false);

    updateStats();

    // Vérifier si l'utilisateur a terminé
    if (currentTypedText.length() == currentText.length()) {
        isPracticeActive = false;
        typingArea->setEnabled(false);
        startButton->setEnabled(true);
    }
}

void MainWindow::updateStats()
{
    QString typedText = typingArea->toPlainText();
    if (typedText.isEmpty()) return;

    // Calculate WPM
    float minutes = startTime.elapsed() / 60000.0f;
    int words = typedText.split(" ", Qt::SkipEmptyParts).size();
    int wpm = minutes > 0 ? static_cast<int>(words / minutes) : 0;

    // Calculate accuracy
    float accuracy = 100.0f - (static_cast<float>(mistakes) / typedText.length() * 100.0f);

    // Update labels
    wpmLabel->setText(QString("WPM: %1").arg(wpm));
    accuracyLabel->setText(QString("Accuracy: %1%").arg(QString::number(accuracy, 'f', 1)));
    mistakesLabel->setText(QString("Mistakes: %1").arg(mistakes));
}
