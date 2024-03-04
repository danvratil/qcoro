// SPDX-FileCopyrightText: 2023 Daniel Vrátil <dvratil@kde.org>
//
// SPDX-License-Identifier: MIT


/*
 * Do NOT include this file directly - include the QCoroTask header instead!
 */

#pragma once

#include "../qcorotask.h"

namespace QCoro {

// \cond internal
namespace detail {
struct ThisCoroPromise
{
private:
    ThisCoroPromise() = default;
    friend ThisCoroPromise QCoro::thisCoro();
};
} // namespace detail

// \endcond

inline auto thisCoro() -> detail::ThisCoroPromise
{
    return detail::ThisCoroPromise{};
}

inline void CoroutineFeatures::guardThis(QObject *obj)
{
    if (obj == nullptr) {
        mGuardedThis.reset();
    } else {
        mGuardedThis.emplace(obj);
    }
}

inline auto CoroutineFeatures::guardedThis() const -> const std::optional<QPointer<QObject>> &
{
    return mGuardedThis;
}

inline std::shared_ptr<CoroutineFeatures> CoroutineFeatures::create()
{
    struct ConstructableCoroutineFeatures : CoroutineFeatures {};
    return std::make_shared<ConstructableCoroutineFeatures>();
}

} // namespace QCoro
