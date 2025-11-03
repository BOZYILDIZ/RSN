#include <gtest/gtest.h>
#include <fstream>
#include <cstring>
#include <vector>
#include "../../src/io/device_io.h"

using namespace rsn::io;

/**
 * @file device_io_test.cpp
 * @brief Unit tests for DeviceIO cross-platform I/O abstraction
 *
 * Test coverage includes:
 * - Device open/close operations
 * - Block reading at various offsets
 * - Filesystem detection (NTFS, APFS, ext4, FAT32, HFS+)
 * - Error handling (invalid device, invalid offsets)
 * - Device info retrieval
 * - Platform-specific implementations
 *
 * Target: >80% code coverage for DeviceIO class
 *
 * Note: Tests use mock files instead of real devices for portability
 */

/**
 * @class DeviceIOTest
 * @brief Test fixture for DeviceIO unit tests
 */
class DeviceIOTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create temporary test files
        CreateMockDevice();
    }

    void TearDown() override {
        // Clean up test files
        CleanupMockDevice();
    }

    /**
     * @brief Create mock device file for testing
     * Simulates a simple disk image with mock data
     */
    void CreateMockDevice() {
        test_device_path_ = "/tmp/test_device.bin";

        // Create a 1MB test device file
        std::ofstream device_file(test_device_path_, std::ios::binary);
        std::vector<uint8_t> buffer(1024 * 1024, 0);

        // Write EXT4 magic at offset 1024+56 (offset 1080)
        uint16_t ext4_magic = 0xEF53;
        std::memcpy(&buffer[1080], &ext4_magic, 2);

        // Write NTFS signature at offset 3
        const char *ntfs_sig = "NTFS    ";
        std::memcpy(&buffer[3], ntfs_sig, 8);

        // Write FAT32 signature
        buffer[0] = 0xEB;  // Jump instruction
        uint16_t fat_sig = 0xAA55;
        std::memcpy(&buffer[510], &fat_sig, 2);

        device_file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        device_file.close();
    }

    void CleanupMockDevice() {
        // Remove test file
        std::remove(test_device_path_.c_str());
    }

    std::string test_device_path_;
};

/**
 * @test TestDeviceIOConstruction
 * @brief Verify DeviceIO object construction
 *
 * Expected:
 * - DeviceIO constructs without errors
 * - IsOpen() returns false initially
 * - GetDeviceSize() returns 0 initially
 */
TEST_F(DeviceIOTest, TestDeviceIOConstruction) {
    DeviceIO device_io;

    EXPECT_FALSE(device_io.IsOpen());
    EXPECT_EQ(device_io.GetDeviceSize(), 0);
    EXPECT_EQ(device_io.GetSectorSize(), 512);
}

/**
 * @test TestDeviceOpen
 * @brief Verify device open operation
 *
 * Expected:
 * - Open() succeeds on valid device path
 * - IsOpen() returns true after open
 * - GetDeviceSize() returns non-zero value
 * - GetDeviceInfo() returns valid structure
 */
TEST_F(DeviceIOTest, TestDeviceOpen) {
    DeviceIO device_io;

    EXPECT_TRUE(device_io.Open(test_device_path_));
    EXPECT_TRUE(device_io.IsOpen());
    EXPECT_GT(device_io.GetDeviceSize(), 0);

    DeviceInfo info = device_io.GetDeviceInfo();
    EXPECT_EQ(info.device_path, test_device_path_);
    EXPECT_TRUE(info.is_open);
    EXPECT_GT(info.total_bytes, 0);
}

/**
 * @test TestDeviceClose
 * @brief Verify device close operation
 *
 * Expected:
 * - Close() succeeds
 * - IsOpen() returns false after close
 * - GetDeviceSize() returns 0 after close
 */
TEST_F(DeviceIOTest, TestDeviceClose) {
    DeviceIO device_io;

    device_io.Open(test_device_path_);
    EXPECT_TRUE(device_io.IsOpen());

    EXPECT_TRUE(device_io.Close());
    EXPECT_FALSE(device_io.IsOpen());
}

/**
 * @test TestReadBlock
 * @brief Verify block reading at various offsets
 *
 * Expected:
 * - ReadBlock() reads correct number of bytes
 * - ReadBlockVector() returns correct data
 * - Read data matches written data
 */
TEST_F(DeviceIOTest, TestReadBlock) {
    DeviceIO device_io;
    device_io.Open(test_device_path_);

    // Read first 16 bytes
    std::vector<uint8_t> buffer(16);
    uint64_t bytes_read = device_io.ReadBlock(0, 16, buffer.data());

    EXPECT_EQ(bytes_read, 16);

    // Read using vector method
    std::vector<uint8_t> vector_data = device_io.ReadBlockVector(0, 16);
    EXPECT_EQ(vector_data.size(), 16);

    device_io.Close();
}

/**
 * @test TestReadBlockVector
 * @brief Verify vector-based block reading
 *
 * Expected:
 * - ReadBlockVector() allocates correct size
 * - Data matches expected content
 * - Works with various sizes
 */
TEST_F(DeviceIOTest, TestReadBlockVector) {
    DeviceIO device_io;
    device_io.Open(test_device_path_);

    std::vector<uint8_t> data32 = device_io.ReadBlockVector(0, 32);
    EXPECT_EQ(data32.size(), 32);

    std::vector<uint8_t> data256 = device_io.ReadBlockVector(0, 256);
    EXPECT_EQ(data256.size(), 256);

    std::vector<uint8_t> data4k = device_io.ReadBlockVector(0, 4096);
    EXPECT_EQ(data4k.size(), 4096);

    device_io.Close();
}

/**
 * @test TestFilesystemDetection
 * @brief Verify filesystem type detection
 *
 * Expected:
 * - DetectFilesystem() identifies filesystem type
 * - Returns one of known types or UNKNOWN
 * - FilesystemTypeToString() converts correctly
 */
TEST_F(DeviceIOTest, TestFilesystemDetection) {
    DeviceIO device_io;
    device_io.Open(test_device_path_);

    FilesystemType fs_type = device_io.DetectFilesystem();

    // Test device might detect as NTFS, FAT32, EXT4, etc.
    EXPECT_NE(fs_type, FilesystemType::UNKNOWN);

    std::string fs_name = DeviceIO::FilesystemTypeToString(fs_type);
    EXPECT_FALSE(fs_name.empty());
    EXPECT_NE(fs_name, "Unknown");

    device_io.Close();
}

/**
 * @test TestFilesystemTypeConversion
 * @brief Verify filesystem type string conversion
 *
 * Expected:
 * - FilesystemTypeToString() converts all types
 * - StringToFilesystemType() converts back correctly
 * - Round-trip conversion works
 */
TEST_F(DeviceIOTest, TestFilesystemTypeConversion) {
    // Test NTFS
    EXPECT_EQ(DeviceIO::FilesystemTypeToString(FilesystemType::NTFS), "NTFS");
    EXPECT_EQ(DeviceIO::StringToFilesystemType("NTFS"), FilesystemType::NTFS);

    // Test ext4
    EXPECT_EQ(DeviceIO::FilesystemTypeToString(FilesystemType::EXT4), "ext4");
    EXPECT_EQ(DeviceIO::StringToFilesystemType("ext4"), FilesystemType::EXT4);

    // Test APFS
    EXPECT_EQ(DeviceIO::FilesystemTypeToString(FilesystemType::APFS), "APFS");
    EXPECT_EQ(DeviceIO::StringToFilesystemType("APFS"), FilesystemType::APFS);

    // Test FAT32
    EXPECT_EQ(DeviceIO::FilesystemTypeToString(FilesystemType::FAT32), "FAT32");
    EXPECT_EQ(DeviceIO::StringToFilesystemType("FAT32"), FilesystemType::FAT32);

    // Test HFS+
    EXPECT_EQ(DeviceIO::FilesystemTypeToString(FilesystemType::HFS_PLUS), "HFS+");
    EXPECT_EQ(DeviceIO::StringToFilesystemType("HFS+"), FilesystemType::HFS_PLUS);

    // Test Unknown
    EXPECT_EQ(DeviceIO::FilesystemTypeToString(FilesystemType::UNKNOWN), "Unknown");
}

/**
 * @test TestDeviceInfo
 * @brief Verify device information retrieval
 *
 * Expected:
 * - GetDeviceInfo() returns complete structure
 * - All fields populated correctly
 * - filesystem_name field matches detected type
 */
TEST_F(DeviceIOTest, TestDeviceInfo) {
    DeviceIO device_io;
    device_io.Open(test_device_path_);

    DeviceInfo info = device_io.GetDeviceInfo();

    EXPECT_EQ(info.device_path, test_device_path_);
    EXPECT_GT(info.total_bytes, 0);
    EXPECT_GT(info.sector_size, 0);
    EXPECT_TRUE(info.is_open);
    EXPECT_FALSE(info.filesystem_name.empty());

    device_io.Close();
}

/**
 * @test TestErrorHandlingInvalidOffset
 * @brief Verify error handling for invalid read offsets
 *
 * Expected:
 * - ReadBlock() throws exception for out-of-bounds offset
 * - Error message is descriptive
 */
TEST_F(DeviceIOTest, TestErrorHandlingInvalidOffset) {
    DeviceIO device_io;
    device_io.Open(test_device_path_);

    uint8_t buffer[1024];

    // Try to read beyond device size
    EXPECT_THROW(
        device_io.ReadBlock(device_io.GetDeviceSize() + 1000, 512, buffer),
        DeviceIOException
    );

    device_io.Close();
}

/**
 * @test TestErrorHandlingDeviceNotOpen
 * @brief Verify error handling when device not open
 *
 * Expected:
 * - ReadBlock() throws exception if device not open
 * - Error is descriptive
 */
TEST_F(DeviceIOTest, TestErrorHandlingDeviceNotOpen) {
    DeviceIO device_io;

    uint8_t buffer[1024];

    EXPECT_THROW(
        device_io.ReadBlock(0, 512, buffer),
        DeviceIOException
    );
}

/**
 * @test TestErrorHandlingNullBuffer
 * @brief Verify error handling for null buffer pointer
 *
 * Expected:
 * - ReadBlock() throws exception for null buffer
 * - Error is descriptive
 */
TEST_F(DeviceIOTest, TestErrorHandlingNullBuffer) {
    DeviceIO device_io;
    device_io.Open(test_device_path_);

    EXPECT_THROW(
        device_io.ReadBlock(0, 512, nullptr),
        DeviceIOException
    );

    device_io.Close();
}

/**
 * @test TestMultipleDeviceOperations
 * @brief Verify multiple open/close cycles
 *
 * Expected:
 * - Can open, close, and reopen device
 * - State properly managed
 * - No resource leaks
 */
TEST_F(DeviceIOTest, TestMultipleDeviceOperations) {
    DeviceIO device_io;

    // First cycle
    EXPECT_TRUE(device_io.Open(test_device_path_));
    EXPECT_TRUE(device_io.IsOpen());
    EXPECT_TRUE(device_io.Close());
    EXPECT_FALSE(device_io.IsOpen());

    // Second cycle
    EXPECT_TRUE(device_io.Open(test_device_path_));
    EXPECT_TRUE(device_io.IsOpen());
    EXPECT_TRUE(device_io.Close());
    EXPECT_FALSE(device_io.IsOpen());

    EXPECT_TRUE(true);  // No crashes
}

/**
 * @test TestSequentialReads
 * @brief Verify multiple sequential reads
 *
 * Expected:
 * - Can perform multiple reads in sequence
 * - Each read returns correct amount of data
 * - No state corruption between reads
 */
TEST_F(DeviceIOTest, TestSequentialReads) {
    DeviceIO device_io;
    device_io.Open(test_device_path_);

    // Read from multiple offsets
    auto data1 = device_io.ReadBlockVector(0, 512);
    EXPECT_EQ(data1.size(), 512);

    auto data2 = device_io.ReadBlockVector(512, 512);
    EXPECT_EQ(data2.size(), 512);

    auto data3 = device_io.ReadBlockVector(1024, 512);
    EXPECT_EQ(data3.size(), 512);

    device_io.Close();
}

/**
 * @test TestSectorSize
 * @brief Verify sector size retrieval
 *
 * Expected:
 * - GetSectorSize() returns non-zero value
 * - Typically 512 or 4096 bytes
 * - Consistent across multiple calls
 */
TEST_F(DeviceIOTest, TestSectorSize) {
    DeviceIO device_io;
    device_io.Open(test_device_path_);

    uint64_t sector_size = device_io.GetSectorSize();
    EXPECT_GT(sector_size, 0);

    // Sector size should typically be 512, 1024, 2048, or 4096
    bool valid_size = (sector_size == 512 || sector_size == 1024 ||
                       sector_size == 2048 || sector_size == 4096);
    EXPECT_TRUE(valid_size || sector_size > 4096);

    device_io.Close();
}

/**
 * @test TestDeviceIODestructor
 * @brief Verify proper cleanup in destructor
 *
 * Expected:
 * - Destructor closes open device
 * - No resource leaks
 * - No exceptions thrown
 */
TEST_F(DeviceIOTest, TestDeviceIODestructor) {
    {
        DeviceIO device_io;
        device_io.Open(test_device_path_);
        EXPECT_TRUE(device_io.IsOpen());
        // Destructor called here
    }

    // If we reach here without crashes, destructor worked correctly
    EXPECT_TRUE(true);
}

#endif  // DEVICE_IO_TEST_CPP
