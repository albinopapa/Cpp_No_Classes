#pragma once

#include "Entity.h"
#include "MathTypes.h"
#include <cstdint>

namespace Framework::Graphics
{
	struct _Graphics;
}
namespace Game
{
	namespace Grid
	{
		struct _Cell;
		struct _Grid;
	}
	namespace Units
	{
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

		struct vTable_Unit
		{
			void (*Update)( struct _Unit& unit, float dt, Grid::_Grid& grid  );
		};

		struct _Unit
		{
			template<class Traits>
			_Unit( Traits _traits, Team _team )
				:
				_Unit( _traits.attack, _traits.base_hp, _traits.armor, _traits.job, _team )
			{

			}
			_Unit( float Attack, float HP, float Armor, Job _Job, Team _Team );

			operator _Entity&( );
			operator const _Entity&( )const;

			static constexpr Math::Rect::_Rect<float> rect = { -16,-32,16,32 };
			static constexpr size_t max_aniframes = 16u;

			vTable_Unit vtable;
			_Entity entity;
			float speed		= 0.f;
			float attack	= traits<Job::Builder>::attack;
			float hp		= traits<Job::Builder>::base_hp;
			float armor		= traits<Job::Builder>::armor;
			size_t aniframe = 0;
			const _Unit* p_attacker = nullptr;
			Job job = Job::Builder;
			JobState jobstate = JobState::isIdle;
			Buffs buff = Buffs::Agriculture;
		};

		template<Job _job>
		_Unit CreateUnit( Team team )
		{
			return _Unit(
				traits<_job>::attack,
				traits<_job>::base_hp,
				traits<_job>::armor,
				traits<_job>::job,
				team
			);
		}


		void Update( _Unit& unit, float dt, Grid::_Grid& grid );
		void ClampToScreen( _Unit& unit );
		void Draw( Framework::Graphics::_Graphics& gfx, const _Unit& unit );

		// Getters
		Vec2f GetPosition( const _Unit& unit );
		Job GetJob( const _Unit& unit );
		bool IsAlive( const _Unit& unit );
		bool IsIdle( const _Unit& unit );

		// Setters
		void SetAttackingUnit( _Unit& source, const _Unit& pAttacker );
		void SetPosition( _Unit& unit, const Vec2f position );

	}

	namespace Buildings
	{
		enum class Type
		{
			Hut,House,Mansion,Tent,
			Butcher,Market,Warehouse,
			Barracks,Stable,Port
		};

		template<Type T> struct traits{};
		template<> struct traits<Type::Hut>
		{
			static constexpr int32_t fire_resistance = -10;
			static constexpr int32_t melee_resistance = 25;
			static constexpr int32_t projectile_resistance = 25;
			static constexpr int32_t width = 2, height = 2;
			static constexpr int32_t max_hp = 1000;
		};
		template<> struct traits<Type::House>
		{
			static constexpr int32_t fire_resistance = 10;
			static constexpr int32_t melee_resistance = 100;
			static constexpr int32_t projectile_resistance = 125;
			static constexpr int32_t width = 3, height = 3;
			static constexpr int32_t max_hp = 1500;
		};
		template<> struct traits<Type::Mansion>
		{
			static constexpr int32_t fire_resistance = 20;
			static constexpr int32_t melee_resistance = 400;
			static constexpr int32_t projectile_resistance = 500;
			static constexpr int32_t width = 5, height = 5;
			static constexpr int32_t max_hp = 2500;
		};
		template<> struct traits<Type::Tent>
		{
			static constexpr int32_t fire_resistance = 0;
			static constexpr int32_t melee_resistance = 0;
			static constexpr int32_t projectile_resistance = 0;
			static constexpr int32_t width = 1, height = 1;
			static constexpr int32_t max_hp = 500;
		};
		template<> struct traits<Type::Butcher>
		{
			static constexpr int32_t fire_resistance = 20;
			static constexpr int32_t melee_resistance = 200;
			static constexpr int32_t projectile_resistance = 500;
			static constexpr int32_t width = 3, height = 3;
			static constexpr int32_t max_hp = 2500;
		};
		template<> struct traits<Type::Market>
		{
			static constexpr int32_t fire_resistance = 20;
			static constexpr int32_t melee_resistance = 40;
			static constexpr int32_t projectile_resistance = 10;
			static constexpr int32_t width = 5, height = 5;
			static constexpr int32_t max_hp = 1500;
		};
		template<> struct traits<Type::Warehouse>
		{
			static constexpr int32_t fire_resistance = 100;
			static constexpr int32_t melee_resistance = 600;
			static constexpr int32_t projectile_resistance = 1000;
			static constexpr int32_t width = 5, height = 5;
			static constexpr int32_t max_hp = 3000;
		};
		template<> struct traits<Type::Barracks>
		{
			static constexpr int32_t fire_resistance = 100;
			static constexpr int32_t melee_resistance = 1000;
			static constexpr int32_t projectile_resistance = 1000;
			static constexpr int32_t width = 5, height = 5;
			static constexpr int32_t max_hp = 3000;
		};
		template<> struct traits<Type::Stable>
		{
			static constexpr int32_t fire_resistance = 50;
			static constexpr int32_t melee_resistance = 1000;
			static constexpr int32_t projectile_resistance = 200;
			static constexpr int32_t width = 3, height = 3;
			static constexpr int32_t max_hp = 2000;
		};
		template<> struct traits<Type::Port>
		{
			static constexpr int32_t fire_resistance = 100;
			static constexpr int32_t melee_resistance = 2000;
			static constexpr int32_t projectile_resistance = 1500;
			static constexpr int32_t width = 3, height = 3;
			static constexpr int32_t max_hp = 2000;
		};


		struct _Building
		{
			_Building() = default;
			_Building( Team _team, Type _type );
			_Building( 
				int32_t _fire_resistance,
				int32_t _melee_resistance,
				int32_t _projectile_resistance,
				int32_t _width, 
				int32_t _height,
				int32_t _hp,
				Team _team, 
				Type _type );

			operator _Entity&();
			operator const _Entity&( )const
			{
				return entity;
			}

			_Entity entity = { EntityCategory::Building };

			Type type;
			int32_t fire_resistance;
			int32_t melee_resistance;
			int32_t projectile_resistance;
			int32_t width, height;
			int32_t hp;
		};
	}
}