#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace algo
{
    class tokens
    {
    public:
        bool empty() { return true; }
    };
}

namespace algo { namespace spec
{		
    TEST_CLASS(can_recognize_tokens)
	{
	public:
		
        TEST_METHOD(should_not_have_any_tokens_when_default_constructed)
		{
            tokens testSubject;
            Assert::IsTrue(testSubject.empty());
		}

	};
}}
