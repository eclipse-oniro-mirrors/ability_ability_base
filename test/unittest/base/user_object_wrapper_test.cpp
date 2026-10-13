/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>


#define private public
#define protected public
#include "user_object_wrapper.h"
#include "user_object_base.h"
#undef private
#undef protected

using namespace OHOS;
using namespace OHOS::AAFwk;
using testing::ext::TestSize;

namespace OHOS {
namespace AAFwk {
namespace {
const std::string SPLIT = "#";
};

class UserObjectTestImpl : public UserObjectBase {
public:
    UserObjectTestImpl() : UserObjectBase("UserObjectTestImpl") {}
    ~UserObjectTestImpl() = default;

    std::string ToString() const override
    {
        return strData_;
    }

    void Parse(const std::string &str) override
    {
        parseCalled_ = true;
        parsed_ = str;
    }

    bool ParseWithResult(const std::string &str) override
    {
        Parse(str);
        return parseResult_;
    }

    void DeepCopy(std::shared_ptr<UserObjectBase> &other) override {}

    bool Equals(std::shared_ptr<UserObjectBase> &other) override
    {
        return true;
    }

    std::string strData_ = "user_object_test_data";
    std::string parsed_;
    bool parseCalled_ = false;
    bool parseResult_ = true;
};

class RegisteredLegacyObject : public UserObjectBase {
public:
    RegisteredLegacyObject() : UserObjectBase("RegisteredLegacyObject") {}
    ~RegisteredLegacyObject() = default;

    std::string ToString() const override
    {
        return strData_;
    }

    void Parse(const std::string &str) override
    {
        parsed_ = str;
    }

    void DeepCopy(std::shared_ptr<UserObjectBase> &other) override {}

    bool Equals(std::shared_ptr<UserObjectBase> &other) override
    {
        return true;
    }

    std::string strData_ = "legacy_object_data";
    std::string parsed_;
};
REGISTER_USER_OBJECT_BASE(RegisteredLegacyObject);

class RegisteredFailingObject : public UserObjectBase {
public:
    RegisteredFailingObject() : UserObjectBase("RegisteredFailingObject") {}
    ~RegisteredFailingObject() = default;

    std::string ToString() const override
    {
        return strData_;
    }

    void Parse(const std::string &str) override {}

    bool ParseWithResult(const std::string &str) override
    {
        return false;
    }

    void DeepCopy(std::shared_ptr<UserObjectBase> &other) override {}

    bool Equals(std::shared_ptr<UserObjectBase> &other) override
    {
        return true;
    }

    std::string strData_ = "failing_object_data";
};
REGISTER_USER_OBJECT_BASE(RegisteredFailingObject);

class AAfWKUserObjectWrapperTest : public testing::Test {
public:
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.number: UserObjectWrapperTest_GetValue_001
 * @tc.name: GetValue
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_GetValue_001, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    ErrCode result = useObjectValue.GetValue(value);
    EXPECT_EQ(ERR_OK, result);
}

/**
 * @tc.number: UserObjectWrapperTest_ToString_001
 * @tc.name: ToString
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_ToString_001, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    EXPECT_EQ("", useObjectValue.ToString());
}

/**
 * @tc.number: UserObjectWrapperTest_Box_001
 * @tc.name: Box
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Box_001, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    EXPECT_EQ(nullptr, useObjectValue.Box(value));
}

/**
 * @tc.number: UserObjectWrapperTest_Unbox_001
 * @tc.name: Unbox
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Unbox_001, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    sptr<IUserObject> object = new (std::nothrow) UserObject(value);
    EXPECT_EQ(value, useObjectValue.Unbox(object));
}

/**
 * @tc.number: UserObjectWrapperTest_Unbox_002
 * @tc.name: Unbox
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Unbox_002, TestSize.Level1)
{
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    sptr<IUserObject> object = new (std::nothrow) UserObject(value);
    object = nullptr;
    EXPECT_EQ(nullptr, useObjectValue.Unbox(object));
}

/**
 * @tc.number: UserObjectWrapperTest_Parse_001
 * @tc.name: Parse
 * @tc.desc:
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Parse_001, TestSize.Level1)
{
    std::string str = "";
    std::shared_ptr<UserObjectBase> value = nullptr;
    UserObject useObjectValue(value);
    EXPECT_EQ(nullptr, useObjectValue.Parse(str));
}

/**
 * @tc.number: UserObjectWrapperTest_Parse_002
 * @tc.name: Parse
 * @tc.desc: UserObject::Parse returns object for legacy subclass without ParseWithResult override
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Parse_002, TestSize.Level1)
{
    sptr<IUserObject> result = UserObject::Parse("RegisteredLegacyObject#object_content");
    EXPECT_NE(nullptr, result);
    std::shared_ptr<UserObjectBase> value = UserObject::Unbox(result.GetRefPtr());
    EXPECT_NE(nullptr, value);
    auto *legacy = static_cast<RegisteredLegacyObject *>(value.get());
    EXPECT_EQ("object_content", legacy->parsed_);
}

/**
 * @tc.number: UserObjectWrapperTest_Parse_003
 * @tc.name: Parse
 * @tc.desc: UserObject::Parse returns nullptr when subclass ParseWithResult fails
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Parse_003, TestSize.Level1)
{
    EXPECT_EQ(nullptr, UserObject::Parse("RegisteredFailingObject#object_content"));
}

/**
 * @tc.number: UserObjectWrapperTest_ParseWithResult_001
 * @tc.name: ParseWithResult
 * @tc.desc: default ParseWithResult calls Parse and returns true for legacy subclasses
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_ParseWithResult_001, TestSize.Level1)
{
    class LegacyUserObject : public UserObjectBase {
    public:
        LegacyUserObject() : UserObjectBase("LegacyUserObject") {}
        ~LegacyUserObject() = default;
        std::string ToString() const override
        {
            return strData_;
        }
        void Parse(const std::string &str) override
        {
            parseCalled_ = true;
            parsed_ = str;
        }
        void DeepCopy(std::shared_ptr<UserObjectBase> &other) override {}
        bool Equals(std::shared_ptr<UserObjectBase> &other) override
        {
            return true;
        }
        std::string strData_ = "legacy_data";
        std::string parsed_;
        bool parseCalled_ = false;
    };

    LegacyUserObject object;
    EXPECT_TRUE(object.ParseWithResult("legacy_data"));
    EXPECT_TRUE(object.parseCalled_);
    EXPECT_EQ("legacy_data", object.parsed_);
}

/**
 * @tc.number: UserObjectWrapperTest_Unmarshalling_001
 * @tc.name: Unmarshalling
 * @tc.desc: Unmarshalling returns false when reading string from parcel fails
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Unmarshalling_001, TestSize.Level1)
{
    Parcel parcel;
    UserObjectTestImpl object;
    EXPECT_FALSE(object.Unmarshalling(parcel));
    EXPECT_FALSE(object.parseCalled_);
}

/**
 * @tc.number: UserObjectWrapperTest_Unmarshalling_002
 * @tc.name: Unmarshalling
 * @tc.desc: Unmarshalling returns true and parses content after Marshalling
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Unmarshalling_002, TestSize.Level1)
{
    Parcel parcel;
    UserObjectTestImpl object;
    EXPECT_TRUE(object.Marshalling(parcel));
    UserObjectTestImpl target;
    EXPECT_TRUE(target.Unmarshalling(parcel));
    EXPECT_TRUE(target.parseCalled_);
    EXPECT_EQ(object.strData_, target.parsed_);
}

/**
 * @tc.number: UserObjectWrapperTest_Unmarshalling_003
 * @tc.name: Unmarshalling
 * @tc.desc: Unmarshalling returns false when ParseWithResult fails
 */
HWTEST_F(AAfWKUserObjectWrapperTest, UserObjectWrapperTest_Unmarshalling_003, TestSize.Level1)
{
    Parcel parcel;
    UserObjectTestImpl object;
    EXPECT_TRUE(object.Marshalling(parcel));
    UserObjectTestImpl target;
    target.parseResult_ = false;
    EXPECT_FALSE(target.Unmarshalling(parcel));
    EXPECT_TRUE(target.parseCalled_);
}
}
}