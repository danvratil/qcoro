<!--
SPDX-FileCopyrightText: 2023 Daniel Vrátil <dvratil@kde.org>

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# Coroutine Features

!!! note "This feature is available since QCoro 0.10"

QCoro coroutines can be tuned at runtime to adjust their behavior.

## Obtaining Coroutine Features

```cpp
auto QCoro::thisCoro();
```

Current coroutine's features can be obtained by `co_await`ing on `QCoro::thisCoro()` to
obtain `QCoro::CoroutineFeatures` object:

```cpp
QCoro::Task<> myCoroutine() {
    QCoro::CoroutineFeatures &features = co_await QCoro::thisCoro();
    // use features here to tune behavior of this coroutine.
}
```

The `co_await` does not actually suspend the current coroutine, the features object is returned
immediatelly synchronously.

The following features can be configured for QCoro coroutines:

## `CoroutineFeatures::guardThis(QObject *)`

When coroutine is a member function of a QObject-derived class, it can happen that the `this` object
is deleted while the coroutine is suspended. When the coroutine is resumed, the program would crash
when it would try to dereference `this` due to use-after-free. To prevent this, a coroutine can
guard the `this` pointer. If the object is destroyed while the coroutine is suspended, it will terminate
immediatelly after resumption.

```cpp
QCoro::Task<> MyButton::onButtonClicked()
{
    auto &features = co_await QCoro::thisCoro();
    features.guardThis(this);

    setLabel(tr("Downloading..."));
    const auto result = co_await fetchData();
    // If the button is destroyed while the coroutine is suspended waiting for the `fetchData()` coroutine,
    // it will immediately terminate here once `fetchData()` finishes.
    // If `this` is still a valid pointer at this point, the coroutine will continue as usual.

    setLabel(tr("Done"));
}
```

