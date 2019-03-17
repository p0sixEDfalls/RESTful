#include <gtest/gtest.h>
#include "../src/client/util.h"

TEST(ParseParameters, test1)
{
    char * argv[3];
    argv[0] = "arg1";
    argv[1] = "arg2";
    argv[2] = "arg3";

    vector<string> vArgs = {"arg1", "arg2", "arg3"};
    vector<string> vArgs1 = ParseParameters(3, argv);

    ASSERT_STREQ(vArgs[0].c_str(), vArgs1[0].c_str());
    ASSERT_STREQ(vArgs[1].c_str(), vArgs1[1].c_str());
    ASSERT_STREQ(vArgs[2].c_str(), vArgs1[2].c_str());
}

TEST(ExtractJSON, test1)
{
    string html = "fghfgjtryrtfgjhh{\"error\":\"test\"}dhjhfjyjtyj{\"errortreth\"gdfggr\"}re";
    string tStr = "{\"error\":\"test\"}";
    ExtractJSON(html);
    ASSERT_STREQ(html.c_str(), tStr.c_str());
}

TEST(ProcessParameters, test1)
{
    string versionstr = "v1.0.0";
    vector<string> args = {"client", "--version"};
    string res = ProcessParameters(args);
    ASSERT_STREQ(versionstr.c_str(), res.c_str());
}

TEST(JSONParser, ParseVersionJSON)
{
    JSONParser parser;
    string versionjson = "{\"version\":\"v1\"}";
    string tStr = "v1.0.0";
    parser.ParseVersionJSON(versionjson);
    string res = parser.GetParsedJSON();
    ASSERT_STREQ(tStr.c_str(), res.c_str());
}

TEST(JSONParser, ParseInterfacesJSON)
{
    JSONParser parser;
    string versionjson = "{\"interfaces\":[\"lo\",\"enp3s0\",\"docker0\",\"br-2a7ce7262544\",\"br-4e5bdab2a166\"]}";
    string tStr = "lo,enp3s0,docker0,br-2a7ce7262544,br-4e5bdab2a166";
    parser.ParseInterfacesJSON(versionjson);
    string res = parser.GetParsedJSON();
    ASSERT_STREQ(tStr.c_str(), res.c_str());
}

TEST(JSONParser, ParseMessageJSON)
{
    JSONParser parser;
    string versionjson = "{\"message\":\"test\"}";
    string tStr = "test";
    parser.ParseMessageJSON(versionjson);
    string res = parser.GetParsedJSON();
    ASSERT_STREQ(tStr.c_str(), res.c_str());
}

TEST(JSONParser, ParseErrorJSON)
{
    JSONParser parser;
    string versionjson = "{\"error\":\"test\"}";
    string tStr = "test";
    parser.ParseErrorJSON(versionjson);
    string res = parser.GetParsedJSON();
    ASSERT_STREQ(tStr.c_str(), res.c_str());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
