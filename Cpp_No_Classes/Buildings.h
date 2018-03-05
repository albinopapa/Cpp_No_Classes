#pragma once

#include <cstdint>

namespace Game
{
	namespace Buildings
	{
		enum class Type
		{
			Hut, House, Mansion, Tent,
			Butcher, Market, Warehouse,
			Barracks, Stable, Port
		};

		template<Type T> struct traits
		{};
		template<> struct traits<Type::Hut>
		{
			static constexpr int32_t fire_resistance = -10;
			static constexpr int32_t melee_resistance = 25;
			static constexpr int32_t projectile_resistance = 25;
			static constexpr int32_t width = 2, height = 2;
			static constexpr int32_t max_hp = 1000;
			static constexpr int32_t num_occupants = 2;
		};
		template<> struct traits<Type::House>
		{
			static constexpr int32_t fire_resistance = 10;
			static constexpr int32_t melee_resistance = 100;
			static constexpr int32_t projectile_resistance = 125;
			static constexpr int32_t width = 3, height = 3;
			static constexpr int32_t max_hp = 1500;
			static constexpr int32_t num_occupants = 4;
		};
		template<> struct traits<Type::Mansion>
		{
			static constexpr int32_t fire_resistance = 20;
			static constexpr int32_t melee_resistance = 400;
			static constexpr int32_t projectile_resistance = 500;
			static constexpr int32_t width = 5, height = 5;
			static constexpr int32_t max_hp = 2500;
			static constexpr int32_t num_occupants = 8;
		};
		template<> struct traits<Type::Tent>
		{
			static constexpr int32_t fire_resistance = 0;
			static constexpr int32_t melee_resistance = 0;
			static constexpr int32_t projectile_resistance = 0;
			static constexpr int32_t width = 1, height = 1;
			static constexpr int32_t max_hp = 500;
			static constexpr int32_t num_occupants = 1;
		};
		template<> struct traits<Type::Butcher>
		{
			static constexpr int32_t fire_resistance = 20;
			static constexpr int32_t melee_resistance = 200;
			static constexpr int32_t projectile_resistance = 500;
			static constexpr int32_t width = 3, height = 3;
			static constexpr int32_t max_hp = 2500;
			static constexpr int32_t num_occupants = 3;
		};
		template<> struct traits<Type::Market>
		{
			static constexpr int32_t fire_resistance = 20;
			static constexpr int32_t melee_resistance = 40;
			static constexpr int32_t projectile_resistance = 10;
			static constexpr int32_t width = 5, height = 5;
			static constexpr int32_t max_hp = 1500;
			static constexpr int32_t num_occupants = 20;
		};
		template<> struct traits<Type::Warehouse>
		{
			static constexpr int32_t fire_resistance = 100;
			static constexpr int32_t melee_resistance = 600;
			static constexpr int32_t projectile_resistance = 1000;
			static constexpr int32_t width = 5, height = 5;
			static constexpr int32_t max_hp = 3000;
			static constexpr int32_t num_occupants = 30;
		};
		template<> struct traits<Type::Barracks>
		{
			static constexpr int32_t fire_resistance = 100;
			static constexpr int32_t melee_resistance = 1000;
			static constexpr int32_t projectile_resistance = 1000;
			static constexpr int32_t width = 5, height = 5;
			static constexpr int32_t max_hp = 3000;
			static constexpr int32_t num_occupants = 30;
		};
		template<> struct traits<Type::Stable>
		{
			static constexpr int32_t fire_resistance = 50;
			static constexpr int32_t melee_resistance = 1000;
			static constexpr int32_t projectile_resistance = 200;
			static constexpr int32_t width = 3, height = 3;
			static constexpr int32_t max_hp = 2000;
			static constexpr int32_t num_occupants = 6;
		};
		template<> struct traits<Type::Port>
		{
			static constexpr int32_t fire_resistance = 100;
			static constexpr int32_t melee_resistance = 2000;
			static constexpr int32_t projectile_resistance = 1500;
			static constexpr int32_t width = 3, height = 3;
			static constexpr int32_t max_hp = 2000;
			static constexpr int32_t num_occupants = 30;
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
				int32_t num_occupants,
				Team _team,
				Type _type );

			operator _Entity&( );
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