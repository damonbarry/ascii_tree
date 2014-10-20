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
    TEST_CLASS(CanRecognizeTokens)
	{
	public:
		
        TEST_METHOD(ShouldNotHaveAnyTokensWhenDefaultConstructed)
		{
            tokens testSubject;
            Assert::IsTrue(testSubject.empty());
		}

	};
}}
