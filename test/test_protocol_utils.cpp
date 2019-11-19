#include "protocol_utils.h"
#include "variant.h"
#include "agio.h"
#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <climits>

using namespace std;
using namespace Agio::ProtocolUtils;

namespace Helper
{
  using variant_t = Agio::Variant<
      bool
      , int8_t
      , int16_t
      , int32_t
      , int64_t
      , uint8_t
      , uint16_t
      , uint32_t
      , uint64_t
      , std::string
  >;

  struct Packet
  {
    std::vector<variant_t> data;
    uint8_t msgType;
  };


  char* getScopedString()
  {
    //! [6:hello?]
    char s[] = "hello?";
    char* stream = CXX_MALLOC_CSTR(sizeof(uint32_t) + strlen(s));
    char* pos = stream;

    auto header = strlen(s);
    memcpy(pos, &header, sizeof(uint32_t));
    pos += sizeof(uint32_t);

    strcpy(pos, s);
    pos += strlen(s);

    return stream;
  }

  char* getScopedInt()
  {
    //! [4:56636]
    int v = 65535;
    char* stream = CXX_MALLOC_CSTR(sizeof(uint32_t) + sizeof(v));
    char* pos = stream;

    auto header = sizeof(v);
    memcpy(pos, &header, sizeof(uint32_t));
    pos += sizeof(uint32_t);

    memcpy(pos, &v, sizeof(v));
    pos += sizeof(v);

    return stream;
  }

  char* getMultiLayerPacket()
  {
    //! [10:[6:hello?]]
    char s[] = "hello?";
    char* stream = CXX_MALLOC_CSTR( sizeof(uint32_t) + sizeof(uint32_t) + strlen(s) );
    char* pos = stream;

    uint32_t header;

    header =
        sizeof(uint32_t) + strlen(s); // first cell
    memcpy(pos, &header, sizeof(uint32_t));
    pos += sizeof(uint32_t);

    header = strlen(s);
    memcpy(pos,  (char*)&header, sizeof(uint32_t));
    pos += sizeof(uint32_t);

    strcpy(pos, s);
    pos += strlen(s);

    return stream;
  }

  char* getMultiLayerMultiCellStrPacket()
  {
    //! [19:[6:hello?][5:world]]
    char s1[] = "hello?";
    char s2[] = "world";
    char* stream = CXX_MALLOC_CSTR(sizeof(uint32_t) // main header
                                  + sizeof(uint32_t) + strlen(s1) // cell1 total
                                  + sizeof(uint32_t) + strlen(s2) // cell2 total
                                  );
    char* pos = stream;

    uint32_t header;

    header = // main header
        sizeof(uint32_t) + strlen(s1) // cell1 total
        + sizeof(uint32_t) + strlen(s2); // cell2 total
    memcpy(pos, &header, sizeof(uint32_t));
    pos += sizeof(header);

    // cell1 header
    header = strlen(s1);
    memcpy(pos, &header, sizeof(uint32_t));
    pos += sizeof(header);

    // cell1 data
    strcpy(pos, s1);
    pos += strlen(s1);

    // cell2 header
    header = strlen(s2);
    memcpy(pos, &header, sizeof(uint32_t));
    pos += sizeof(header);

    // cell1 data
    strcpy(pos, s2);
    pos += strlen(s2);

    return stream;
  }

  std::vector<char> getMultilayerMultiCellMixedPacket()
  {
    //! [20:[6:hello,][6:world!]]
    std::vector<char> stream(sizeof(uint32_t));
    size_t pos{};

    Packet pk {{std::string{"hello,"}, std::string{"world!"}, std::string{"duckhacks"}, uint32_t{2019}}, 100};

    *(uint32_t*)&stream[0] = (4 + 6) + (4 + 6) + (4 + 9) + 4 + 1; // set main header
    pos += sizeof(uint32_t); // skip main header

    insertVal(stream, pos, pk.msgType);
    insertStr(stream, pos, pk.data[0].get<std::string>());
    insertStr(stream, pos, pk.data[1].get<std::string>());
    insertStr(stream, pos, pk.data[2].get<std::string>());
    insertVal(stream, pos, pk.data[3].get<uint32_t>());
    return stream;
  }
}


TEST(RedeemStr, SingleLayer)
{
  char* stream = Helper::getScopedString();
  auto end = scopeEnd<uint32_t>(stream);

  EXPECT_EQ("hello?", string(redeemStr<uint32_t>(stream, end)));
}

TEST(RedeemStr, SingleLayerOverRedeem)
{
  char* stream = Helper::getScopedString();
  auto end = scopeEnd<uint32_t>(stream);

  EXPECT_EQ("hello?", string(redeemStr<uint32_t>(stream, end)));
  for(int i = 0; i < 10; ++ i)
    {
      EXPECT_EQ("", string(redeemStr<uint32_t>(stream, end)));
      EXPECT_EQ("", string(redeemStr<uint16_t>(stream, end)));
      EXPECT_EQ("", string(redeemStr<uint8_t>(stream, end)));
    }
}

TEST(RedeemStr, MultiLayer)
{
  char* stream = Helper::getMultiLayerPacket();
  auto begin = scopeBegin<uint32_t>(stream);
  auto end = scopeEnd<uint32_t>(stream);

  EXPECT_EQ(string("hello?"), string(redeemStr<uint32_t>(begin, end)));
}

TEST(RedeemStr, MultiLayerMultiCell)
{
  char* stream = Helper::getMultiLayerMultiCellStrPacket();
  auto p = scopeBegin<uint32_t>(stream);
  auto end = scopeEnd<uint32_t>(stream);

  for(std::string s : {"hello?", "world"})
    EXPECT_EQ(s, string(redeemStr<uint32_t>(p, end)));
}

TEST(RedeemStr, MultiLayerMultiCellOverRedeem)
{
  char* stream = Helper::getMultiLayerMultiCellStrPacket();
  auto p = scopeBegin<uint32_t>(stream);
  auto end = scopeEnd<uint32_t>(stream);

  for(std::string s : {"hello?", "world", "", "", ""})
    EXPECT_EQ(s, string(redeemStr<uint32_t>(p, end)));
}

TEST(RedeemVal, SingleLayer)
{
  char* stream = Helper::getScopedInt();
  auto begin = scopeBegin<uint32_t>(stream);
  auto end = scopeEnd<uint32_t>(stream);

  EXPECT_EQ(65535, redeemVal<int>(begin, end));
}

TEST(PacketMixedRedeem, MultiLayerMultiCellPacket)
{
  std::vector<char> stream = Helper::getMultilayerMultiCellMixedPacket();
  auto p = scopeBegin<uint32_t>(stream.data());
  auto end = scopeEnd<uint32_t>(stream.data());

  EXPECT_EQ(uint8_t{100}, uint8_t(redeemVal<uint8_t>(p, end)));
  EXPECT_EQ(string("hello,"), string(redeemStr<uint32_t>(p, end)));
  EXPECT_EQ(string("world!"), string(redeemStr<uint32_t>(p, end)));
  EXPECT_EQ(string("duckhacks"), string(redeemStr<uint32_t>(p, end)));
  EXPECT_EQ(uint32_t{2019}, uint32_t(redeemVal<uint32_t>(p, end)));
}


TEST(MessageParser, Experiment)
{
  MessageScheme<
      int32_t
      > schemeHeartbeat{0};

  MessageScheme<
      std::string
      > schemeAuthReq{1};

  MessageScheme<
      std::string
      > schemeAuthRes{2};

  MessageScheme<
      std::string
      > schemeLoginReq{3};

  MessageScheme<
      bool,
      std::string
      > schemeLoginRes{4};

  MessageScheme<
      std::string,
      int32_t,
      std::string
      > schemeMsgTxt{5};

  auto* parser = make_parser(schemeHeartbeat,
                        schemeAuthReq,
                        schemeAuthRes,
                        schemeLoginReq,
                        schemeLoginRes,
                        schemeMsgTxt);
  Packet* p = new Packet{ {std::string{"hello?"}, int32_t{54}, std::string{"msg"}}, 5};
  std::vector<char> buffer = parser->encode(p);


}

//TEST(Encode, MultiLayerMultiCellPacket)
//{
//    Packet pk {{std::string{"hello,"}}, 1};

//    iovec v = ParseEngine::encode(&pk);

//    std::vector<char> stream((char*)v.iov_base, (char*)v.iov_base + v.iov_len);

//    auto p = scopeBegin<uint32_t>(stream.data());
//    auto end = scopeEnd<uint32_t>(stream.data());
//    EXPECT_EQ(uint8_t{1}, uint8_t(redeemVal<uint8_t>(p, end)));
//    EXPECT_EQ(string("hello,"), string(redeemStr<uint32_t>(p, end)));
//}


