#pragma once
#include "../../src/virtual/virtual.h"
#include "centity.h"
#include "../../src/math/types/vector.h"

class ITraceFilter
{
public:
	enum ETraceType : std::int32_t
	{
		TRACE_EVERYTHING = 0,
		TRACE_WORLD_ONLY,
		TRACE_ENTITIES_ONLY,
		TRACE_EVERYTHING_FILTER_PROPS
	};

	virtual bool ShouldHitEntity(CEntity* entity, std::int32_t contentsMask) = 0;
	virtual std::int32_t GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	CTraceFilter(CEntity* entity) noexcept : skipEntity(entity) { }

	bool ShouldHitEntity(CEntity* entity, std::int32_t contentsMask) override
	{
		return entity != this->skipEntity;
	}

	std::int32_t GetTraceType() const override
	{
		return TRACE_EVERYTHING;
	}

private:
	const CEntity* skipEntity;
};

class CRay
{
public:
	CRay(const Vector& start, const Vector& end) noexcept :
		start(start), delta(end - start)
	{
		isSwept = delta.x || delta.y || delta.z;
	}

	Vector start;
	float pad0{ };
	Vector delta;
	float pad1{ };
	Vector startOffset{ };
	float pad2{ };
	Vector extents{ };
	float pad3{ };
	const uintptr_t* skip;
	bool isRay{ true };
	bool isSwept;
};

class CPlane
{
public:
	Vector normal;
	float distance;
	std::byte type;
	std::byte signBits;
	std::byte pad[2];
};

class CSurface
{
public:
	const char* name;
	short surfaceProps;
	std::uint16_t flags;
};

class CTrace
{
public:
	Vector startPos;
	Vector endPos;
	CPlane plane;

	float fraction;

	int contents;
	std::uint16_t dispFlags;

	bool allSolid;
	bool startSolid;

	float fractionLeftSolid;
	CSurface surface;

	int hitgroup;

	short physicsBone;
	std::uint16_t worldSurfaceIndex;

	CEntity* entity;
	int hitbox;
};

// interface itself
class IEngineTraceClient
{
public:
	void TraceRay(const CRay& ray, std::uint32_t mask, const CTraceFilter& filter, CTrace& trace) noexcept
	{
		vt::Call<void>(this, 5, std::cref(ray), mask, std::cref(filter), std::ref(trace));
	}
};;