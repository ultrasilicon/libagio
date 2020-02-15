#include "file.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <climits>
#include <fstream>
#include <filesystem>

#define TEST_FILE_RAND_POSTFIX_SIZE 16
#define TEST_FILE_BUFFER_SIZE 65536

using namespace std;
using namespace Agio;

namespace FileTestHelper
{
  static string testName()
  {
    return testing::UnitTest::GetInstance()->current_test_info()->name();
  }

  static string randomStr(const size_t& length)
  {
      string str(length,0);
      generate_n(str.begin(), length,  []() {
          const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyz";
          return charset[ static_cast<unsigned long>(rand()) % (sizeof(charset) - 1) ];
      });
      return str;
  }

  class TestFile
  {
  public:
    TestFile(const string& name, const size_t& size)
      : size_(size)
      , name_(name + '_' + randomStr(TEST_FILE_RAND_POSTFIX_SIZE))
    {
      ofstream file;
      file.open(getDir());
      for(size_t i = size_ / TEST_FILE_BUFFER_SIZE; i > 0; -- i)
        file << randomStr(TEST_FILE_BUFFER_SIZE);
      file << randomStr(size_ % TEST_FILE_BUFFER_SIZE);
      file.close();
    }

    string readAll()
    {
      ifstream file;
      file.open(getDir());
      string buffer;
      string line;
      while(file){
        getline(file, line);
        buffer += line;
      }
      file.close();
      return buffer;
    }

    string getDir() const
    {
      return dir_ + name_;
    }

  private:
    size_t size_;
    string name_;
    const string dir_ = filesystem::temp_directory_path();
  };
}

TEST(FileSync, Read)
{  
}

TEST(FileSync, ReadAll)
{
  using namespace FileTestHelper;

  for(int i : { 1, 3/*, 32, 33, 128, 1023, 65536, 1000000*/})
    {
      TestFile* stlFile = new TestFile(testName(), i);
      File* agioFile = new File(stlFile->getDir(), Loop::defaultLoop());

      printf("STL Read: %s\n", stlFile->readAll().c_str());
      printf("Agio Read: %s\n", agioFile->readAll().c_str());

      string stlContent = stlFile->readAll();
      string agioContent = agioFile->readAll();
      EXPECT_EQ(stlContent, agioContent);
    }
}

TEST(FileSync, Write)
{
  EXPECT_EQ("", string());
}

TEST(FileSync, WriteAll)
{
  EXPECT_EQ("", string());
}

TEST(FileAsync, Read)
{
  EXPECT_EQ("", string());
}

TEST(FileAsync, ReadAll)
{
  EXPECT_EQ("", string());
}

TEST(FileAsync, Write)
{
  EXPECT_EQ("", string());
}

TEST(FileAsync, WriteAll)
{
  EXPECT_EQ("", string());
}




