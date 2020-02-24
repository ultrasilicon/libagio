#include "file.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <exception>


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
          const char charset[] = " 0123456789abcdefghijklmnopqrstuvwxyz\0\n\t";
          return charset[ static_cast<unsigned long>(rand()) % (sizeof(charset) - 1) ];
      });
      return str;
  }

  static string randomName(const size_t& length)
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
      , name_(name + '_' + randomName(TEST_FILE_RAND_POSTFIX_SIZE))
    {
      ofstream file;
      file.open(getDir());
      string s;
      for(size_t i = size_ / TEST_FILE_BUFFER_SIZE; i > 0; -- i)
        file << randomStr(TEST_FILE_BUFFER_SIZE);
      file << randomStr(size_ % TEST_FILE_BUFFER_SIZE);
      file.close();
    }

    char* readAll()
    {
      FILE *f;
      f = fopen(getDir().data(), "r");
      if(f == nullptr)
        throw ios_base::failure("failed to open file " + getDir() + " for read");

      fseek(f, 0L, SEEK_END);
      long fileSize = ftell(f);
      fseek(f, 0L, SEEK_SET);

      char *buffer;
      buffer = (char*)calloc(fileSize, sizeof(char));
      if(buffer == nullptr)
        throw bad_alloc();
      fread(buffer, sizeof(char), fileSize, f);
      fclose(f);

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
  srand(time(nullptr));


//  for(size_t i : { 0ul, 3ul, 32ul, 33ul, 128ul, 1023ul, 65536ul, 100000000ul })
  for(size_t i : { 1023ul })
    {
      TestFile* stlFile = new TestFile(testName(), i);
      cout << stlFile->getDir() << "  " << i << endl;
      char* stlData = stlFile->readAll();

      File* agioFile = new File(stlFile->getDir(), Loop::defaultLoop());
      char* agioData = agioFile->readAll().data();
      agioFile->close(Mode::Sync);


      int r = memcmp(stlData, agioData, i);
      EXPECT_EQ(0, r);
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




