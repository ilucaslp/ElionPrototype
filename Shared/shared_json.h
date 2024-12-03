#ifndef __SHARED_JSON_H__
#define __SHARED_JSON_H__

#pragma once

#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
#define RAPIDJSON_NEON
#else
#if defined(RAPIDJSON_FORCESSE4)
#define RAPIDJSON_SSE42
#else
#define RAPIDJSON_SSE2
#endif
#endif

#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>
#include <rapidjson\encodedstream.h>
#include <rapidjson\stringbuffer.h>
#include <rapidjson\reader.h>
#include <rapidjson\writer.h>

#ifdef UNICODE
typedef rapidjson::GenericDocument<rapidjson::UTF16LE<>> JsonDocument;
typedef JsonDocument::MemberIterator JsonIterator;
typedef rapidjson::GenericValue<rapidjson::UTF16LE<>> JsonValue;
#else
typedef rapidjson::Document JsonDocument;
typedef JsonDocument::MemberIterator JsonIterator;
typedef rapidjson::Value JsonValue;
#endif

#endif