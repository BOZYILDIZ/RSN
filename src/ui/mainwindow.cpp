#include "mainwindow.h"
#include "../core/recovery_engine.h"
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , recovery_engine_(std::make_unique<RecoveryEngine>())
{
  setWindowTitle("RecoverySoftNetz — Data Recovery");
  setGeometry(100, 100, 800, 600);

  CreateUI();
  ConnectSignals();
}

MainWindow::~MainWindow()
{
}

void MainWindow::CreateUI()
{
  // Central widget
  QWidget* central = new QWidget(this);
  QVBoxLayout* layout = new QVBoxLayout(central);

  // Title
  QLabel* title = new QLabel("RecoverySoftNetz", this);
  title->setStyleSheet("font-size: 24px; font-weight: bold;");
  layout->addWidget(title);

  // Placeholder text
  QLabel* placeholder = new QLabel(
    "🚀 Bootstrap Phase\n\n"
    "Core recovery engine initialized.\n"
    "Phase 1 implementation in progress...\n\n"
    "Features coming in Phase 1:\n"
    "• Device selection\n"
    "• File system scanning\n"
    "• Recovery algorithms\n"
    "• Results visualization",
    this
  );
  placeholder->setStyleSheet("margin: 20px;");
  layout->addWidget(placeholder);

  layout->addStretch();

  setCentralWidget(central);
}

void MainWindow::ConnectSignals()
{
  // TODO: Connect recovery engine signals to UI updates
  // - Progress updates
  // - Error handling
  // - Results display
}
