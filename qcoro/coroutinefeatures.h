// SPDX-FileCopyrightText: 2023 Daniel Vrátil <dvratil@kde.org>
//
// SPDX-License-Identifier: MIT

#pragma once

#include "coroutine.h"

#include <optional>
#include <QPointer>

namespace QCoro {

/*! \cond internal */
namespace detail {
class TaskPromiseBase;
struct ThisCoroPromise;
} // namespace detail
/*! \endcond */


//! A special coroutine that returns features of the current coroutine
/*!
 * A special coroutine that can be co_awaited inside a current QCoro coroutine
 * to obtain QCoro::CoroutineFeatures object that allows the user to tune
 * certain features and behavior of the current coroutine.
 *
 * @see QCoro::CoroutineFeatures
 */
auto thisCoro() -> detail::ThisCoroPromise;

//! Features of the current coroutine
/*!
 * Allows configuring behavior of the current coroutine.
 *
 * Use `co_await QCoro::thisCoro()` to obtain the current coroutine's Features
 * and modify them.
 *
 * @see QCoro::thisCoro()
 */
class CoroutineFeatures
{
public:
    CoroutineFeatures(const CoroutineFeatures &) = delete;
    CoroutineFeatures(CoroutineFeatures &&) = delete;
    CoroutineFeatures &operator=(const CoroutineFeatures &) = delete;
    CoroutineFeatures &operator=(CoroutineFeatures &&) = delete;
    ~CoroutineFeatures() = default;

    //! Bind the coroutine lifetime to the lifetime of the given object
    /*!
     * Watches the given \c obj object. If the object is destroyed while the
     * current coroutine is suspended, the coroutine will be destroyed immediately
     * after resuming to prevent a use-after-free "this" pointer.
     *
     * \param obj QObject to observe its lifetime. Pass `nullptr` to stop observing.
     */
    void guardThis(QObject *obj);

    //! Returns the currently guarded QObject
    /*!
     * Returns the currently guarded QObject set by guardThis(). If no object was being
     * guarded, the returned `std::optional` is empty. If the returned `std::optional`
     * is not empty, then an object was specified to be guarded. When the QPointer inside
     * the `std::optional` is empty it means that the guarded object has already been
     * destroyed. Otherwise pointer to the guarded QObject is obtained.
     */
    auto guardedThis() const -> const std::optional<QPointer<QObject>> &;

private:
    std::optional<QPointer<QObject>> mGuardedThis;

    explicit CoroutineFeatures() = default;
    static std::shared_ptr<CoroutineFeatures> create();
    friend class QCoro::detail::TaskPromiseBase;
};

} // namespace QCoro

#include "impl/coroutinefeatures.h"
