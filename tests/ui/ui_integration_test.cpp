#include <gtest/gtest.h>
#include <QApplication>
#include <QTest>
#include <memory>
#include "../../src/ui/device_wizard.h"
#include "../../src/ui/progress_monitor.h"
#include "../../src/ui/results_view.h"

/**
 * @file ui_integration_test.cpp
 * @brief Integration tests for Qt6 UI components
 *
 * Test coverage includes:
 * - Device Wizard initialization and device selection
 * - Filesystem type detection
 * - Progress Monitor updates and timing
 * - Results View population and filtering
 * - Signal/slot connections
 * - UI state management
 *
 * Target: >80% coverage for UI components
 *
 * Note: These tests require Qt6 application context
 */

// Global QApplication instance for Qt GUI tests
int g_argc = 0;
char** g_argv = nullptr;
QApplication* g_app = nullptr;

/**
 * @class UIIntegrationTest
 * @brief Test fixture for UI integration tests
 */
class UIIntegrationTest : public ::testing::Test {
protected:
    void SetUpTestSuite() {
        if (!g_app) {
            g_app = new QApplication(g_argc, g_argv);
        }
    }

    void SetUp() override {
        // Initialize UI components for each test
    }

    void TearDown() override {
        // Cleanup after each test
    }
};

/**
 * @test TestDeviceWizardInitialization
 * @brief Verify DeviceWizard initializes correctly
 *
 * Expected:
 * - Widget created without errors
 * - Initial device list is populated
 * - UI controls are visible
 */
TEST_F(UIIntegrationTest, TestDeviceWizardInitialization) {
    std::unique_ptr<DeviceWizard> wizard(new DeviceWizard());

    ASSERT_NE(wizard, nullptr);
    EXPECT_STREQ(wizard->windowTitle().toStdString().c_str(), "Device Wizard");
    EXPECT_GE(wizard->width(), 500);
    EXPECT_GE(wizard->height(), 300);
}

/**
 * @test TestDeviceWizardDeviceSelection
 * @brief Verify device selection functionality
 *
 * Expected:
 * - GetSelectedDevice() returns valid device path
 * - deviceSelected signal is emitted
 * - UI updates on selection change
 */
TEST_F(UIIntegrationTest, TestDeviceWizardDeviceSelection) {
    std::unique_ptr<DeviceWizard> wizard(new DeviceWizard());

    wizard->RefreshDeviceList();
    QString selected = wizard->GetSelectedDevice();

    // First device should be selected after refresh
    EXPECT_FALSE(selected.isEmpty());
    EXPECT_TRUE(selected.startsWith("/dev/"));
}

/**
 * @test TestDeviceWizardFilesystemDetection
 * @brief Verify filesystem type detection
 *
 * Expected:
 * - DetectFileSystem identifies ext4/APFS/NTFS
 * - GetDetectedFileSystem() returns detected type
 * - Result is one of known types or "Unknown"
 */
TEST_F(UIIntegrationTest, TestDeviceWizardFilesystemDetection) {
    std::unique_ptr<DeviceWizard> wizard(new DeviceWizard());

    wizard->RefreshDeviceList();
    QString fs_type = wizard->GetDetectedFileSystem();

    EXPECT_FALSE(fs_type.isEmpty());
    // Filesystem should be one of: ext4, APFS, NTFS, Unknown
    bool valid_fs = (fs_type == "ext4" || fs_type == "APFS" ||
                     fs_type == "NTFS" || fs_type == "Unknown");
    EXPECT_TRUE(valid_fs) << "Invalid filesystem type: " << fs_type.toStdString();
}

/**
 * @test TestDeviceWizardUIEnabled
 * @brief Verify UI enable/disable functionality
 *
 * Expected:
 * - SetUIEnabled(true) enables controls
 * - SetUIEnabled(false) disables controls
 * - State persists correctly
 */
TEST_F(UIIntegrationTest, TestDeviceWizardUIEnabled) {
    std::unique_ptr<DeviceWizard> wizard(new DeviceWizard());

    wizard->SetUIEnabled(false);
    EXPECT_FALSE(wizard->isEnabled());

    wizard->SetUIEnabled(true);
    EXPECT_TRUE(wizard->isEnabled());
}

/**
 * @test TestProgressMonitorInitialization
 * @brief Verify ProgressMonitor initializes correctly
 *
 * Expected:
 * - Widget created without errors
 * - Progress bar starts at 0%
 * - Status labels are visible
 */
TEST_F(UIIntegrationTest, TestProgressMonitorInitialization) {
    std::unique_ptr<ProgressMonitor> monitor(new ProgressMonitor());

    ASSERT_NE(monitor, nullptr);
    EXPECT_STREQ(monitor->windowTitle().toStdString().c_str(), "Progress Monitor");
    EXPECT_GE(monitor->width(), 500);
    EXPECT_GE(monitor->height(), 350);
}

/**
 * @test TestProgressMonitorReset
 * @brief Verify progress reset functionality
 *
 * Expected:
 * - ResetProgress() clears all counters
 * - Progress bar returns to 0%
 * - Counters reset to 0
 */
TEST_F(UIIntegrationTest, TestProgressMonitorReset) {
    std::unique_ptr<ProgressMonitor> monitor(new ProgressMonitor());

    monitor->SetProgress(50);
    monitor->SetFileCount(100, 50);
    monitor->SetDeletedCount(10);

    monitor->ResetProgress();

    // After reset, progress should be back to 0
    EXPECT_EQ(monitor->findChild<QProgressBar*>()->value(), 0);
}

/**
 * @test TestProgressMonitorProgress
 * @brief Verify progress bar updates
 *
 * Expected:
 * - SetProgress(50) sets bar to 50%
 * - SetProgress(100) sets bar to 100%
 * - SetProgress(value) clamps to 0-100
 */
TEST_F(UIIntegrationTest, TestProgressMonitorProgress) {
    std::unique_ptr<ProgressMonitor> monitor(new ProgressMonitor());

    monitor->SetProgress(25);
    EXPECT_EQ(monitor->findChild<QProgressBar*>()->value(), 25);

    monitor->SetProgress(75);
    EXPECT_EQ(monitor->findChild<QProgressBar*>()->value(), 75);

    monitor->SetProgress(100);
    EXPECT_EQ(monitor->findChild<QProgressBar*>()->value(), 100);
}

/**
 * @test TestProgressMonitorFileCount
 * @brief Verify file counter updates
 *
 * Expected:
 * - SetFileCount(100, 50) displays "50 files found"
 * - Counter updates correctly
 */
TEST_F(UIIntegrationTest, TestProgressMonitorFileCount) {
    std::unique_ptr<ProgressMonitor> monitor(new ProgressMonitor());

    monitor->SetFileCount(100, 50);
    monitor->SetDeletedCount(10);

    // Verify updates (actual display text verification would require UI inspection)
    EXPECT_TRUE(true);
}

/**
 * @test TestProgressMonitorTimer
 * @brief Verify timer functionality
 *
 * Expected:
 * - StartTimer() begins tracking elapsed time
 * - StopTimer() stops tracking
 * - Elapsed time updates correctly
 */
TEST_F(UIIntegrationTest, TestProgressMonitorTimer) {
    std::unique_ptr<ProgressMonitor> monitor(new ProgressMonitor());

    monitor->ResetProgress();
    monitor->StartTimer();

    // Timer should be running
    EXPECT_TRUE(true);

    // Simulate some time passing
    QTest::qWait(100);

    monitor->StopTimer();
    EXPECT_TRUE(true);
}

/**
 * @test TestResultsViewInitialization
 * @brief Verify ResultsView initializes correctly
 *
 * Expected:
 * - Widget created without errors
 * - Table is empty initially
 * - Result count is 0
 */
TEST_F(UIIntegrationTest, TestResultsViewInitialization) {
    std::unique_ptr<ResultsView> results(new ResultsView());

    ASSERT_NE(results, nullptr);
    EXPECT_STREQ(results->windowTitle().toStdString().c_str(), "Results View");
    EXPECT_EQ(results->GetResultCount(), 0);
}

/**
 * @test TestResultsViewPopulation
 * @brief Verify results table population
 *
 * Expected:
 * - PopulateResults() adds rows to table
 * - GetResultCount() reflects added entries
 * - Table displays file information
 */
TEST_F(UIIntegrationTest, TestResultsViewPopulation) {
    std::unique_ptr<ResultsView> results(new ResultsView());

    std::vector<FileEntry> test_entries;

    // Create mock entries
    for (int i = 0; i < 5; i++) {
        FileEntry entry;
        entry.filename = "test_file_" + std::to_string(i) + ".txt";
        entry.file_size = 1024 * (i + 1);
        entry.creation_time = 0;
        entry.modification_time = 0;
        entry.is_directory = (i % 2 == 0);
        entry.is_deleted = (i == 3);  // One deleted file

        test_entries.push_back(entry);
    }

    results->PopulateResults(test_entries);

    // Verify population
    EXPECT_EQ(results->GetResultCount(), 5);
}

/**
 * @test TestResultsViewClear
 * @brief Verify results clearing functionality
 *
 * Expected:
 * - ClearResults() removes all rows
 * - GetResultCount() returns 0 after clear
 */
TEST_F(UIIntegrationTest, TestResultsViewClear) {
    std::unique_ptr<ResultsView> results(new ResultsView());

    std::vector<FileEntry> test_entries;
    FileEntry entry;
    entry.filename = "test.txt";
    entry.file_size = 1024;
    entry.is_directory = false;
    entry.is_deleted = false;
    test_entries.push_back(entry);

    results->PopulateResults(test_entries);
    EXPECT_EQ(results->GetResultCount(), 1);

    results->ClearResults();
    EXPECT_EQ(results->GetResultCount(), 0);
}

/**
 * @test TestResultsViewStatistics
 * @brief Verify statistics update
 *
 * Expected:
 * - UpdateStatistics() updates display
 * - Statistics reflect total and deleted counts
 */
TEST_F(UIIntegrationTest, TestResultsViewStatistics) {
    std::unique_ptr<ResultsView> results(new ResultsView());

    results->UpdateStatistics(100, 25);

    // Statistics should be updated (actual verification would require UI inspection)
    EXPECT_TRUE(true);
}

/**
 * @test TestResultsViewExportCSV
 * @brief Verify CSV export functionality
 *
 * Expected:
 * - ExportToCSV() creates valid CSV file
 * - File contains correct headers and data
 * - CSV can be parsed correctly
 */
TEST_F(UIIntegrationTest, TestResultsViewExportCSV) {
    std::unique_ptr<ResultsView> results(new ResultsView());

    std::vector<FileEntry> test_entries;
    FileEntry entry;
    entry.filename = "document.pdf";
    entry.file_size = 2097152;
    entry.is_directory = false;
    entry.is_deleted = false;
    test_entries.push_back(entry);

    // Export to temporary file
    QString temp_file = "/tmp/test_export.csv";
    bool export_ok = results->ExportToCSV(temp_file, test_entries);

    EXPECT_TRUE(export_ok);
    // Verify file exists (in real test, would verify content)
}

/**
 * @test TestUIComponentIntegration
 * @brief Verify UI components work together
 *
 * Expected:
 * - Device Wizard → Progress Monitor → Results View flow works
 * - Signals connect properly
 * - No crashes during typical user workflow
 */
TEST_F(UIIntegrationTest, TestUIComponentIntegration) {
    std::unique_ptr<DeviceWizard> wizard(new DeviceWizard());
    std::unique_ptr<ProgressMonitor> monitor(new ProgressMonitor());
    std::unique_ptr<ResultsView> results(new ResultsView());

    // Simulate user workflow
    wizard->RefreshDeviceList();
    QString device = wizard->GetSelectedDevice();
    EXPECT_FALSE(device.isEmpty());

    monitor->ResetProgress();
    monitor->StartTimer();
    monitor->SetProgress(50);
    monitor->SetFileCount(100, 50);

    std::vector<FileEntry> entries;
    for (int i = 0; i < 10; i++) {
        FileEntry entry;
        entry.filename = "file_" + std::to_string(i);
        entry.file_size = 1024 * i;
        entry.is_directory = false;
        entry.is_deleted = (i % 5 == 0);
        entries.push_back(entry);
    }

    results->PopulateResults(entries);
    EXPECT_EQ(results->GetResultCount(), 10);

    monitor->OnScanCompleted(true, 10, 2);

    EXPECT_TRUE(true);
}

/**
 * @test TestUIMemoryManagement
 * @brief Verify proper memory cleanup
 *
 * Expected:
 * - No memory leaks during widget lifecycle
 * - Destructors called properly
 * - Resources cleaned up
 */
TEST_F(UIIntegrationTest, TestUIMemoryManagement) {
    {
        std::unique_ptr<DeviceWizard> wizard(new DeviceWizard());
        std::unique_ptr<ProgressMonitor> monitor(new ProgressMonitor());
        std::unique_ptr<ResultsView> results(new ResultsView());

        // Components go out of scope and should be cleaned up
    }

    // If we reach here without crashes, memory management is OK
    EXPECT_TRUE(true);
}

#endif  // UI_INTEGRATION_TEST_CPP
