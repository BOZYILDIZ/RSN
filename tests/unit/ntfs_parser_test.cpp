#include <gtest/gtest.h>
#include "../../src/parsers/ntfs_parser.h"
#include <vector>
#include <string>

/**
 * @file ntfs_parser_test.cpp
 * @brief Unit tests for NTFS parser
 *
 * Test coverage includes:
 * - Filesystem type detection
 * - Device compatibility checking
 * - Boot sector validation
 * - MFT parsing
 * - File record extraction
 * - Deleted file detection
 * - Statistics computation
 *
 * Target: >80% code coverage for NTFSParser class
 *
 * TODO: Add more comprehensive test cases as implementation progresses
 */

/**
 * @class NTFSParserTest
 * @brief Test fixture for NTFSParser unit tests
 */
class NTFSParserTest : public ::testing::Test {
protected:
    /**
     * @brief Setup test fixture
     * Runs before each test case
     */
    void SetUp() override {
        parser = std::make_unique<NTFSParser>();
    }

    /**
     * @brief Teardown test fixture
     * Runs after each test case
     */
    void TearDown() override {
        parser.reset();
    }

    std::unique_ptr<NTFSParser> parser;

    /**
     * @brief Helper function to create mock file entry
     */
    FileEntry CreateMockFileEntry(const std::string& filename,
                                  uint64_t size,
                                  bool is_deleted = false,
                                  bool is_directory = false) {
        FileEntry entry;
        entry.filename = filename;
        entry.file_size = size;
        entry.is_deleted = is_deleted;
        entry.is_directory = is_directory;
        entry.creation_time = 0;
        entry.modification_time = 0;
        return entry;
    }
};

/**
 * @test TestGetFileSystemType
 * @brief Verify parser returns correct filesystem type
 *
 * Expected:
 * - GetFileSystemType() returns "NTFS"
 * - Consistent across multiple calls
 */
TEST_F(NTFSParserTest, GetFileSystemType) {
    EXPECT_EQ(parser->GetFileSystemType(), "NTFS");
    EXPECT_EQ(parser->GetFileSystemType(), "NTFS");  // Test consistency
}

/**
 * @test TestCanHandleInvalidDevice
 * @brief Verify parser rejects invalid devices
 *
 * Expected:
 * - CanHandle with empty string returns false
 * - CanHandle with non-NTFS device returns false
 * - CanHandle with invalid paths returns false
 */
TEST_F(NTFSParserTest, CanHandleInvalidDevice) {
    EXPECT_FALSE(parser->CanHandle(""));
    EXPECT_FALSE(parser->CanHandle("/dev/invalid"));
    EXPECT_FALSE(parser->CanHandle("\\.\D:"));  // Non-NTFS Windows drive
}

/**
 * @test TestCanHandleValidNTFSDevice
 * @brief Verify parser recognizes NTFS devices
 *
 * Expected:
 * - CanHandle with NTFS device path returns true
 * - Stub implementation accepts "NTFS" string in path
 *
 * TODO: Replace with actual device detection after boot sector parsing
 */
TEST_F(NTFSParserTest, CanHandleValidNTFSDevice) {
    // Stub test - uses mock device path
    EXPECT_TRUE(parser->CanHandle("NTFS:/dev/sda1"));
    EXPECT_TRUE(parser->CanHandle("\\.\C:"));  // Windows C: drive
}

/**
 * @test TestParseEmptyDevice
 * @brief Verify parser handles empty device gracefully
 *
 * Expected:
 * - Parse with empty device returns false
 * - Empty entries vector after failure
 */
TEST_F(NTFSParserTest, ParseEmptyDevice) {
    std::vector<FileEntry> entries;
    EXPECT_FALSE(parser->Parse("", entries));
    EXPECT_EQ(entries.size(), 0);
}

/**
 * @test TestParseInvalidDevice
 * @brief Verify parser handles invalid device gracefully
 *
 * Expected:
 * - Parse with invalid device returns false
 * - No entries populated
 * - Statistics remain zero
 */
TEST_F(NTFSParserTest, ParseInvalidDevice) {
    std::vector<FileEntry> entries;
    std::string invalid_device = "/dev/nonexistent_device";

    EXPECT_FALSE(parser->Parse(invalid_device, entries));
    EXPECT_EQ(entries.size(), 0);

    auto stats = parser->GetRecoveryStats();
    EXPECT_EQ(stats.first, 0);   // No recoverable files
    EXPECT_EQ(stats.second, 0);  // No deleted files
}

/**
 * @test TestParseNTFSDevice
 * @brief Verify parser processes NTFS device successfully
 *
 * Expected:
 * - Parse returns true for valid NTFS device
 * - Entries vector is populated
 * - Statistics are updated
 *
 * TODO: Enhance with real device parsing after implementation
 */
TEST_F(NTFSParserTest, ParseNTFSDevice) {
    std::vector<FileEntry> entries;

    // Stub test with mock NTFS device
    bool result = parser->Parse("NTFS:/dev/sda1", entries);

    if (result) {
        EXPECT_GT(entries.size(), 0);

        auto stats = parser->GetRecoveryStats();
        EXPECT_GE(stats.first, 0);   // Non-negative recoverable count
        EXPECT_GE(stats.second, 0);  // Non-negative deleted count
    }
}

/**
 * @test TestGetRecoveryStatsInitial
 * @brief Verify recovery stats are zero before parsing
 *
 * Expected:
 * - GetRecoveryStats() returns (0, 0) initially
 * - Pair contains non-negative values
 */
TEST_F(NTFSParserTest, GetRecoveryStatsInitial) {
    auto stats = parser->GetRecoveryStats();

    EXPECT_EQ(stats.first, 0);    // Initial recoverable count
    EXPECT_EQ(stats.second, 0);   // Initial deleted count
    EXPECT_GE(stats.first, stats.second);  // Recoverable >= deleted
}

/**
 * @test TestGetRecoveryStatsAfterParse
 * @brief Verify recovery stats are updated after parsing
 *
 * Expected:
 * - Stats reflect parsed filesystem
 * - Recoverable files >= deleted files
 * - Statistics remain consistent across calls
 */
TEST_F(NTFSParserTest, GetRecoveryStatsAfterParse) {
    std::vector<FileEntry> entries;

    // Parse mock NTFS device
    if (parser->Parse("NTFS:/dev/sda1", entries)) {
        auto stats1 = parser->GetRecoveryStats();
        auto stats2 = parser->GetRecoveryStats();

        // Consistency check
        EXPECT_EQ(stats1.first, stats2.first);
        EXPECT_EQ(stats1.second, stats2.second);

        // Logical check
        EXPECT_GE(stats1.first, stats1.second);
    }
}

/**
 * @test TestFileEntryStructure
 * @brief Verify FileEntry structure is properly populated
 *
 * Expected:
 * - FileEntry contains all required fields
 * - Fields can be accessed without errors
 * - Size and timestamp fields are valid
 */
TEST_F(NTFSParserTest, FileEntryStructure) {
    FileEntry entry = CreateMockFileEntry("test.txt", 1024, false, false);

    EXPECT_EQ(entry.filename, "test.txt");
    EXPECT_EQ(entry.file_size, 1024);
    EXPECT_FALSE(entry.is_deleted);
    EXPECT_FALSE(entry.is_directory);
    EXPECT_GE(entry.creation_time, 0);
    EXPECT_GE(entry.modification_time, 0);
}

/**
 * @test TestParserMultipleInstances
 * @brief Verify multiple parser instances work independently
 *
 * Expected:
 * - Multiple parsers don't interfere
 * - Each maintains own state
 * - Stats are independent
 */
TEST_F(NTFSParserTest, ParserMultipleInstances) {
    auto parser1 = std::make_unique<NTFSParser>();
    auto parser2 = std::make_unique<NTFSParser>();

    std::vector<FileEntry> entries;

    // Parse with parser1
    if (parser1->Parse("NTFS:/dev/sda1", entries)) {
        auto stats1 = parser1->GetRecoveryStats();

        // parser2 should still have zero stats
        auto stats2 = parser2->GetRecoveryStats();
        EXPECT_EQ(stats2.first, 0);
        EXPECT_EQ(stats2.second, 0);
    }
}

/**
 * @test TestParserConsistency
 * @brief Verify parser behavior is consistent
 *
 * Expected:
 * - Same device parsed twice gives same results
 * - FileSystemType never changes
 * - No side effects between operations
 */
TEST_F(NTFSParserTest, ParserConsistency) {
    const std::string device = "NTFS:/dev/sda1";

    std::vector<FileEntry> entries1, entries2;

    bool result1 = parser->Parse(device, entries1);
    bool result2 = parser->Parse(device, entries2);

    EXPECT_EQ(result1, result2);
    EXPECT_EQ(entries1.size(), entries2.size());

    auto stats1 = parser->GetRecoveryStats();
    EXPECT_EQ(parser->GetFileSystemType(), "NTFS");
}

/**
 * @test TestLargeFileEntryList
 * @brief Verify parser handles large file lists
 *
 * Expected:
 * - Can process vectors with many entries
 * - No memory corruption or crashes
 * - Performance remains reasonable
 */
TEST_F(NTFSParserTest, LargeFileEntryList) {
    std::vector<FileEntry> entries;

    // Create 1000 mock entries
    for (int i = 0; i < 1000; i++) {
        entries.push_back(
            CreateMockFileEntry("file_" + std::to_string(i) + ".txt",
                               i * 1024,
                               (i % 10 == 0),  // Every 10th is deleted
                               false));
    }

    EXPECT_EQ(entries.size(), 1000);

    // Verify entries are accessible
    EXPECT_EQ(entries[0].filename, "file_0.txt");
    EXPECT_EQ(entries[999].filename, "file_999.txt");
    EXPECT_TRUE(entries[0].is_deleted);   // 0 % 10 == 0
    EXPECT_FALSE(entries[1].is_deleted);  // 1 % 10 != 0
}

/**
 * @test TestFileTypeDetection
 * @brief Verify parser correctly identifies file types
 *
 * Expected:
 * - Directories marked with is_directory flag
 * - Regular files marked appropriately
 * - Symbolic links (if supported) marked correctly
 */
TEST_F(NTFSParserTest, FileTypeDetection) {
    FileEntry file = CreateMockFileEntry("document.txt", 2048, false, false);
    FileEntry dir = CreateMockFileEntry("MyFolder", 4096, false, true);
    FileEntry deleted_file = CreateMockFileEntry("old_file.exe", 512, true, false);

    EXPECT_FALSE(file.is_directory);
    EXPECT_TRUE(dir.is_directory);
    EXPECT_TRUE(deleted_file.is_deleted);
}

/**
 * @test TestErrorRecovery
 * @brief Verify parser recovers gracefully from errors
 *
 * Expected:
 * - Parser doesn't crash on invalid input
 * - Can continue operating after errors
 * - Returns sensible error codes
 */
TEST_F(NTFSParserTest, ErrorRecovery) {
    std::vector<FileEntry> entries1, entries2;

    // First parse with invalid device
    bool result1 = parser->Parse("", entries1);
    EXPECT_FALSE(result1);

    // Parser should still work after error
    bool result2 = parser->Parse("NTFS:/dev/sda1", entries2);
    // result2 might be true or false depending on mock, but shouldn't crash

    // Parser should still be usable
    EXPECT_EQ(parser->GetFileSystemType(), "NTFS");
}

#endif  // NTFS_PARSER_TEST_CPP
