#pragma once
#include "../../src/math/types/vector.h"
#include "../../src/math/types/matrix3x4.h"
#include "../../src/virtual/virtual.h"
typedef unsigned char byte;

// thx qo0!
enum EHitboxIndex : int
{
	HITBOX_INVALID = -1,
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_STOMACH,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};

class CStudioBone
{
public:

	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int		 			parent;		// parent bone
	int					bonecontroller[6];	// bone controller index, -1 == none

	// default values
	Vector				pos;
	float				quat[4];
	float				rot[3];
	// compression scale
	Vector				posscale;
	Vector				rotscale;

	matrix3x4_t			poseToBone;
	float				qAlignment[4];
	int					flags;
	int					proctype;
	int					procindex;		// procedural rule
	mutable int			physicsbone;	// index into physically simulated bone
	inline void* pProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((byte*)this) + procindex); };
	int					surfacepropidx;	// index into string tablefor property name
	inline char* const pszSurfaceProp(void) const { return ((char*)this) + surfacepropidx; }
	inline int			GetSurfaceProp(void) const { return surfacepropLookup; }

	int					contents;		// See BSPFlags.h for the contents flags
	int					surfacepropLookup;	// this index must be cached by the loader, not saved in the file
	int					unused[7];		// remove as appropriate
};

class CStudioBoundingBox
{
public:
	int bone;
	int group;
	Vector bbMin;
	Vector bbMax;
	int hitboxNameIndex;
	Vector offsetRotation;
	float capsuleRadius;
	int unused[4];
};

class CStudioHitboxSet
{
public:
	int nameIndex;
	int numHitboxes;
	int hitboxIndex;


	inline const char* GetName() const noexcept
	{
		return reinterpret_cast<char*>(std::uintptr_t(this) + nameIndex);
	}

	inline CStudioBoundingBox* GetHitbox(int index) noexcept
	{
		if (index < 0 || index >= numHitboxes)
			return nullptr;

		return reinterpret_cast<CStudioBoundingBox*>(reinterpret_cast<std::uint8_t*>(this) + hitboxIndex) + index;
	}
};

class CStudioHdr
{
public:
	inline CStudioBone* GetBone(int index) noexcept
	{
		if (index < 0 || index >= numBones)
			return nullptr;

		return reinterpret_cast<CStudioBone*>(std::uintptr_t(this) + boneIndex) + index;
	}

	inline CStudioHitboxSet* GetHitboxSet(int index) noexcept
	{
		if (index < 0 || index >= numHitboxSets)
			return nullptr;

		return reinterpret_cast<CStudioHitboxSet*>(std::uintptr_t(this) + hitboxSetIndex) + index;
	}

	int id;
	int version;
	int checksum;
	char name[64];
	int length;

	Vector eyePosition;
	Vector illumPosition;
	Vector hullMin;
	Vector hullMax;
	Vector bbMin;
	Vector bbMax;

	int flags;
	int numBones;
	int boneIndex;
	int numBoneControllers;
	int boneControllerIndex;
	int numHitboxSets;
	int hitboxSetIndex;
};

class CModel
{
public:
	std::int32_t handle;
	char name[260];
	std::int32_t loadFlags;
	std::int32_t serverCount;
	std::int32_t type;
	std::int32_t flags;
	Vector mins;
	Vector maxs;
	float radius;
};

class IVModelInfo
{
public:
	enum ERenderFlags : std::uint32_t
	{
		RENDER_FLAGS_DISABLE_RENDERING = 0x01,
		RENDER_FLAGS_HASCHANGED = 0x02,
		RENDER_FLAGS_ALTERNATE_SORTING = 0x04,
		RENDER_FLAGS_RENDER_WITH_VIEWMODELS = 0x08,
		RENDER_FLAGS_BLOAT_BOUNDS = 0x10,
		RENDER_FLAGS_BOUNDS_VALID = 0x20,
		RENDER_FLAGS_BOUNDS_ALWAYS_RECOMPUTE = 0x40,
		RENDER_FLAGS_IS_SPRITE = 0x80,
		RENDER_FLAGS_FORCE_OPAQUE_PASS = 0x100,
	};

	const char* GetModelName(const CModel* model) noexcept
	{
		return vt::Call<const char*>(this, 3, model);
	}

	CStudioHdr* GetStudioModel(const CModel* model) noexcept
	{
		return vt::Call<CStudioHdr*>(this, 32, model);
	}
};