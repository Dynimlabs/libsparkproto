// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause

#pragma once

//
// SPARK_API
//
#if (defined(_WIN32))
    #define SPARK_API __declspec(dllexport)
#else
    #define SPARK_API __attribute__ ((visibility ("default")))
#endif

// 
// SPARK_DEPRECATED
//
// A macro expanding to a compiler to mark a class/function as deprecated.
//
#if defined(SPARK_NO_DEPRECATED)
    #define SPARK_DEPRECATED
#elif defined(_GNUC_)
    #define SPARK_DEPRECATED __attribute__((deprecated))
#elif defined(__clang__)
    #define SPARK_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
    #define SPARK_DEPRECATED __declspec(deprecated)
#else
    #define SPARK_DEPRECATED
#endif
