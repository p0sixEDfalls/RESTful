#include <gtest/gtest.h>
#include "../src/server/httpserv.h"

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

TEST(ProcessParameters, test1)
{
    vector<string> vArgs = {"arg1", "arg2", "arg3"};
    ASSERT_FALSE(ProcessParameters(vArgs));
}

TEST(JSONParser, CreateIterfacesJSON)
{
    CreateJSON creator;
    string tStr = "{\"interfaces\":[\"lo\",\"enp3s0\",\"docker0\",\"br-2a7ce7262544\",\"br-4e5bdab2a166\"]}";
    vector<string> interfaces = {"lo", "enp3s0", "docker0", "br-2a7ce7262544", "br-4e5bdab2a166"};
    creator.CreateIterfacesJSON(interfaces);
    string res = creator.GetJSON();
    ASSERT_STREQ(tStr.c_str(), res.c_str());
}

TEST(JSONParser, CreateMessageJSON)
{
   CreateJSON creator;
    string tStr = "{\"message\":\"test\"}";
    string error = "test";
    creator.CreateMessageJSON(error);
    string res = creator.GetJSON();
    ASSERT_STREQ(tStr.c_str(), res.c_str());
}

TEST(JSONParser, CreateErrorJSON)
{
    CreateJSON creator;
    string tStr = "{\"error\":\"test\"}";
    string error = "test";
    creator.CreateErrorJSON(error);
    string res = creator.GetJSON();
    ASSERT_STREQ(tStr.c_str(), res.c_str());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
