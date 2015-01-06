/*
 * Copyright (c) 2012-2015 Daniele Bartolini and individual contributors.
 * License: https://github.com/taylor001/crown/blob/master/LICENSE
 */

#pragma once

#define NOMINMAX
#include <cstddef>
#include <stdint.h>
#include "config.h"

typedef uint32_t StringId32;
typedef uint64_t StringId64;

#define INVALID_ID 65535

struct Id
{
	uint16_t id;
	uint16_t index;

	void decode(uint32_t id_and_index)
	{
		id = (id_and_index & 0xFFFF0000) >> 16;
		index = id_and_index & 0xFFFF;
	}

	uint32_t encode() const
	{
		return (uint32_t(id) << 16) | uint32_t(index);
	}

	bool operator==(const Id& other)
	{
		return id == other.id && index == other.index;
	}

	bool operator!=(const Id& other)
	{
		return id != other.id || index != other.index;
	}
};
