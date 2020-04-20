#include "file.h"
#include "test_utils.h"

#include <gtest/gtest.h>
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



  template <typename SizeT>
  static pair<bool, SizeT> smemcmp (const char *s1, const char *s2, SizeT size)
  {
      for(SizeT i = 0; i < size; ++ i)
          if(((char*)s1)[i] != ((char*)s2)[i])
              return {false, i};
      return {true, -1};
  }

  class TestFile
  {
  public:
    TestFile(const string& name, const size_t& size)
      : size_(size)
      , name_(name + '_' + TestUtils::Random::randomName(TEST_FILE_RAND_POSTFIX_SIZE))
      , dir_(filesystem::temp_directory_path().string() + '/')
    {
      ofstream file;
      file.open(getPath());
      string s;
      for(size_t i = size_ / TEST_FILE_BUFFER_SIZE; i > 0; -- i)
        file << TestUtils::Random::randomStr(TEST_FILE_BUFFER_SIZE);
      file << TestUtils::Random::randomStr(size_ % TEST_FILE_BUFFER_SIZE);
      file.close();
    }

    char* readAll()
    {
      FILE *f;
      f = fopen(getPath().data(), "r");
      if(f == nullptr)
        throw ios_base::failure("failed to open file " + getPath() + " for read");

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

    string getPath() const
    {
      return dir_ + name_;
    }

    void rm()
    {
      remove(getPath().c_str());
    }

  private:
    size_t size_;
    string name_;
    string dir_;
  };
}

TEST(FileSync, Read)
{  
}

TEST(FileSync, ReadAll)
{
  using namespace FileTestHelper;
  srand(time(nullptr));

  const size_t bufferSize = ASIO_FILE_READ_BUF_SIZE;
  for(size_t i : { bufferSize, bufferSize + 1, 3ul, 33ul, 61440ul, 61441ul, 65536ul, 66344ul, 66345ul, 10000000ul })
    {
      TestFile* stlFile = new TestFile(testName(), i);
      cout << "Test file: " << stlFile->getPath() << ", size: " << i << "b" << endl;
      char* stlData = stlFile->readAll();

      File* agioFile = new File(stlFile->getPath(), Loop::defaultLoop());
      Buffer* agioData = agioFile->readAll();
      agioFile->close(Sync);

      auto [result, index] = smemcmp(stlData, agioData->data(), i);
      if(!result)
              cout << "diff at:" << index << endl;
      EXPECT_EQ(true, result);

      stlFile->rm();
      delete stlFile;
      free(stlData);
      delete agioFile;
      delete agioData;
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




