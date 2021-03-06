#include "adaptor.h"
#include "yexception.h"

#include <library/unittest/registar.h>

struct TOnCopy : yexception {
};

struct TOnMove : yexception {
};

struct TState {
    explicit TState() {
    }

    TState(const TState&) {
        ythrow TOnCopy();
    }

    TState(TState&&) {
        ythrow TOnMove();
    }

    void operator=(const TState&) {
        ythrow TOnCopy();
    }

    void rbegin() const {
    }

    void rend() const {
    }
};

Y_UNIT_TEST_SUITE(TReverseAdaptor) {
    Y_UNIT_TEST(ReadTest) {
        TVector<int> cont = {1, 2, 3};
        TVector<int> etalon = {3, 2, 1};
        size_t idx = 0;
        for (const auto& x : Reversed(cont)) {
            UNIT_ASSERT_VALUES_EQUAL(etalon[idx++], x);
        }
    }

    Y_UNIT_TEST(WriteTest) {
        TVector<int> cont = {1, 2, 3};
        TVector<int> etalon = {3, 6, 9};
        size_t idx = 0;
        for (auto& x : Reversed(cont)) {
            x *= x + idx++;
        }
        idx = 0;
        for (auto& x : cont) {
            UNIT_ASSERT_VALUES_EQUAL(etalon[idx++], x);
        }
    }

    Y_UNIT_TEST(InnerTypeTest) {
        using TStub = TVector<int>;
        TStub stub;
        const TStub cstub;

        using namespace NPrivate;
        UNIT_ASSERT_TYPES_EQUAL(decltype(Reversed(stub)), TReverseImpl<TStub&>);
        UNIT_ASSERT_TYPES_EQUAL(decltype(Reversed(cstub)), TReverseImpl<const TStub&>);
    }

    Y_UNIT_TEST(CopyMoveTest) {
        TState lvalue;
        const TState clvalue;
        UNIT_ASSERT_NO_EXCEPTION(Reversed(lvalue));
        UNIT_ASSERT_NO_EXCEPTION(Reversed(clvalue));
    }
}
