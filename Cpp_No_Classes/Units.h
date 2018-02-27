#pragma once

#include "MathTypes.h"
#include <cstdint>

namespace Graphics
{
	struct _Graphics;
}

namespace Units
{
	enum class Team
	{
		Red,Blue
	};
	enum class Job
	{
		Builder, Farmer, Fisher, Gatherer, FootSoldier, Mounted, Naval, Scout		
	};
	enum class JobState
	{
		// Builder,	Farmer,	   Fisher,	  Gatherer,	   Scount
		isBuilding, isFarming, isFishing, isGathering, isScouting,

		// Common
		isAttacking, 
		// Hasn't been assigned, or job no longer valid
		isIdle, 
		// Task complete, looking for next task
		isSearching, 
		// Picking up supplies for task 
		// ex: fishing pole broke, going to market to get a new one
		isPurchasing, 
		
		// Used when carrying resources or returning to job,
		// but still working
		isTransitioning
	};
	enum class Buffs
	{
		// Econo-buffs
		Agriculture, Technology, Labor,

		// Military-buffs
		Assassin, Horsebreader, Sailor, Eagleeye
	};

	template<Job j>struct traits{};

	template<>struct traits<Job::Builder>
	{
		static constexpr float max_speed = 310.f;
		static constexpr float attack = 6.f;
		static constexpr float base_hp = 25.f;
		static constexpr float armor = 0.f;
		static constexpr Job job = Job::Builder;
	};
	template<>struct traits<Job::Farmer>
	{
		static constexpr float max_speed = 300.f;
		static constexpr float attack = 5.f;
		static constexpr float base_hp = 20.f;
		static constexpr float armor = 0.f;
		static constexpr Job job = Job::Farmer;
	};
	template<>struct traits<Job::Fisher>
	{
		static constexpr float max_speed = 270.f;
		static constexpr float attack = 4.f;
		static constexpr float base_hp = 15.f;
		static constexpr float armor = 0.f;
		static constexpr Job job = Job::Fisher;
	};
	template<>struct traits<Job::Gatherer>
	{
		static constexpr float max_speed = 350.f;
		static constexpr float attack = 4.f;
		static constexpr float base_hp = 10.f;
		static constexpr float armor = 5.f;
		static constexpr Job job = Job::Gatherer;
	};
	template<>struct traits<Job::FootSoldier>
	{
		static constexpr float max_speed = 275.f;
		static constexpr float attack = 10.f;
		static constexpr float base_hp = 50.f;
		static constexpr float armor = 15.f;
		static constexpr Job job = Job::FootSoldier;
	};
	template<>struct traits<Job::Mounted>
	{
		static constexpr float max_speed = 375.f;
		static constexpr float attack = 20.f;
		static constexpr float base_hp = 50.f;
		static constexpr float armor = 25.f;
		static constexpr Job job = Job::Mounted;
	};
	template<>struct traits<Job::Naval>
	{
		static constexpr float max_speed = 325.f;
		static constexpr float attack = 20.f;
		static constexpr float base_hp = 50.f;
		static constexpr float armor = 5.f;
		static constexpr Job job = Job::Naval;
	};
	template<>struct traits<Job::Scout>
	{
		static constexpr float max_speed = 325.f;
		static constexpr float attack = 10.f;
		static constexpr float base_hp = 25.f;
		static constexpr float armor = 0.f;
		static constexpr Job job = Job::Scout;
	};
	
	struct vTable
	{
		void (*Update)( _Unit& unit, float dt );
	};

	struct _Unit
	{
		_Unit( float Attack, float HP, float Armor, Job _Job, Team _Team );

		static constexpr Math::Rect::_Rect<int32_t> rect = { -16,-32,16,32 };
		static constexpr size_t max_aniframes = 16u;
		
		float speed		= 0.f;
		float attack	= traits<Job::Builder>::attack;
		float hp		= traits<Job::Builder>::base_hp;
		float armor		= traits<Job::Builder>::armor;
		size_t aniframe = 0;
		const _Unit* p_attacker = nullptr;

		Math::Vector::_Vec2<float> position = { 400.f,300.f };
		Math::Vector::_Vec2<float> velocity = { 0.f,0.f };

		Team team = Team::Red;
		Job job = Job::Builder;
		JobState jobstate = JobState::isIdle;
		Buffs buff = Buffs::Agriculture;
		vTable vtable;
	};

	void Update( _Unit& unit, float dt );
	void SetAttackingUnit( _Unit& source, const _Unit& pAttacker );

	void Draw( Graphics::_Graphics& gfx, const _Unit& unit );
	bool IsAlive( const _Unit& unit );
	void ClampToScreen( _Unit& unit );
}

namespace Units::Builder
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Farmer
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Fisher
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Gatherer
{
	void Update( _Unit& unit, float dt );
}
namespace Units::FootSoldier
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Mounted
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Naval
{
	void Update( _Unit& unit, float dt );
}
namespace Units::Scout
{
	void Update( _Unit& unit, float dt );
}

