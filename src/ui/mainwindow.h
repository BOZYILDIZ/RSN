#pragma once

#include <QMainWindow>
#include <memory>

class RecoveryEngine;

/**
 * @brief Main application window for RecoverySoftNetz
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /**
   * @brief Constructor
   *
   * @param parent Parent widget
   */
  explicit MainWindow(QWidget* parent = nullptr);

  /**
   * @brief Destructor
   */
  ~MainWindow();

private:
  // UI setup
  void CreateUI();
  void ConnectSignals();

  // Member variables
  std::unique_ptr<RecoveryEngine> recovery_engine_;

private slots:
  // TODO: Add slots for UI actions
  // - Device selection
  // - Start/stop scan
  // - Results viewing
  // - Export functionality
};
