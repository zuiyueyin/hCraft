/* 
 * hCraft - A custom Minecraft server.
 * Copyright (C) 2012-2013	Jacob Zhitomirsky (BizarreCake)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "slot/blocks.hpp"
#include "slot/items.hpp"
#include "util/cistring.hpp"
#include "physics/blocks/physics_block.hpp"
#include <cstring>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cctype>


namespace hCraft {
	
	block_info::block_info ()
	{
		this->id = 0;
		this->blast_resistance = 0.0f;
		this->opacity = 0;
		this->luminance = 0;
		this->max_stack = 64;
		this->opaque = false;
		this->state = BS_SOLID;
		std::strcpy (this->name, "unknown");
	}
	
	block_info::block_info (unsigned short id, const char *name,
		float blast_resistance, char opacity, char luminance, char max_stack,
		bool opaque, block_state state)
	{
		this->id = id;
		this->blast_resistance = blast_resistance;
		this->opacity = opacity;
		this->luminance = luminance;
		this->max_stack = max_stack;
		this->opaque = opaque;
		this->state = state;
		std::strcpy (this->name, name);
	}
	
	
//----
	
	static std::vector<block_info> block_list {
		{ 0x00, "air", 0.0f, 0, 0, 64,  false, BS_SOLID },
		{ 0x01, "stone", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x02, "grass", 3.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x03, "dirt", 2.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x04, "cobble", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x05, "wood", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x06, "sapling", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x07, "bedrock", 18000000.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x08, "water", 500.0f, 3, 0, 64, true, BS_FLUID },
		{ 0x09, "still-water", 500.0f, 3, 0, 64, true, BS_FLUID },
		{ 0x0A, "lava", 0.0f, 0, 15, 64, false, BS_FLUID },
		{ 0x0B, "still-lava", 500.0f, 0, 15, 64, false, BS_FLUID },
		{ 0x0C, "sand", 2.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x0D, "gravel", 3.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x0E, "gold-ore", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x0F, "iron-ore", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x10, "coal-ore", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x11, "trunk", 10.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x12, "leaves", 1.0f, 1, 0, 64, true, BS_SOLID },
		{ 0x13, "sponge", 3.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x14, "glass", 1.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x15, "lapis-ore", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x16, "lapis-block", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x17, "dispenser", 17.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x18, "sandstone", 4.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x19, "note-block", 4.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x1A, "bed", 1.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x1B, "powered-rail", 3.5f, 0, 0, 64, false, BS_SOLID },
		{ 0x1C, "detector-rail", 3.5f, 0, 0, 64, false, BS_SOLID },
		{ 0x1D, "sticky-piston", 2.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x1E, "cobweb", 20.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x1F, "tall-grass", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x20, "dead-bush", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x21, "piston", 2.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x22, "piston-extension", 2.5f, 0, 0, 64, false, BS_SOLID },
		{ 0x23, "wool", 4.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x24, "piston-move", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x25, "dandelion", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x26, "rose", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x27, "brown-mushroom", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x28, "red-mushroom", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x29, "gold-block", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x2A, "iron-block", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x2B, "dslab", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x2C, "slab", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x2D, "bricks", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x2E, "tnt", 0.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x2F, "bookshelf", 7.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x30, "mossy-cobble", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x31, "obsidian", 6000.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x32, "torch", 0.0f, 0, 14, 64, false, BS_SOLID },
		{ 0x33, "fire", 0.0f, 0, 15, 64, false, BS_SOLID },
		{ 0x34, "monster-spawner", 25.0f, 15, 0, 64, false, BS_SOLID },
		{ 0x35, "stairs", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x36, "chest", 12.5f, 15, 0, 64, false, BS_SOLID },
		{ 0x37, "redstone-wire", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x38, "diamond-ore", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x39, "diamond-block", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x3A, "workbench", 12.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x3B, "wheat-block", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x3C, "farmland", 3.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x3D, "furnace", 17.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x3E, "burning-furnace", 17.5f, 15, 13, 64, true, BS_SOLID },
		{ 0x3F, "sign-post", 5.0f, 0, 0, 16, false, BS_SOLID },
		{ 0x40, "wooden-door", 15.0f, 0, 0, 1, false, BS_SOLID },
		{ 0x41, "ladder", 2.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x42, "rail", 3.5f, 0, 0, 64, false, BS_SOLID },
		{ 0x43, "cobble-stairs", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x44, "wall-sign", 5.0f, 0, 0, 16, false, BS_SOLID },
		{ 0x45, "lever", 2.5f, 0, 0, 64, false, BS_SOLID },
		{ 0x46, "stone-pressure-plate", 2.5f, 0, 0, 64, false, BS_SOLID },
		{ 0x47, "wooden-door", 25.0f, 0, 0, 1, false, BS_SOLID },
		{ 0x48, "wooden-pressure-plate", 2.5f, 0, 0, 64, false, BS_SOLID },
		{ 0x49, "redstone-ore", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x4A, "glowing-redstone-ore", 15.0f, 15, 9, 64, true, BS_SOLID },
		{ 0x4B, "inactive-redstone-torch", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x4C, "redstone-torch", 0.0f, 0, 7, 64, false, BS_SOLID },
		{ 0x4D, "stone-button", 2.5f, 0, 0, 64, false, BS_SOLID },
		{ 0x4E, "snow-cover", 0.5f, 15, 0, 64, false, BS_SOLID },
		{ 0x4F, "ice", 2.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x50, "snow-block", 1.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x51, "cactus", 2.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x52, "clay-block", 3.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x53, "sugar-cane", 0.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x54, "jukebox", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x55, "fence", 15.0f, 0, 0, 64, false, BS_SOLID },
		{ 0x56, "pumpkin", 5.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x57, "netherrack", 2.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x58, "soulsand", 2.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x59, "glowstone", 1.5f, 15, 15, 64, true, BS_SOLID },
		{ 0x5A, "nether-portal", 0.0f, 0, 11, 64, false, BS_SOLID },
		{ 0x5B, "jack-o-lantern", 15.0f, 15, 15, 64, true, BS_SOLID },
		{ 0x5C, "cake-block", 2.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x5D, "redstone-repeater", 0.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x5E, "redstone-repeater-on", 0.0f, 15, 9, 64, true, BS_SOLID },
		{ 0x5F, "stained-glass", 1.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x60, "trapdoor", 15.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x61, "monster-egg", 3.75f, 15, 0, 64, true, BS_SOLID },
		{ 0x62, "stone-brick", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x63, "huge-brown-mushroom", 1.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x64, "huge-red-mushroom", 1.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x65, "iron-bars", 30.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x66, "glass-pane", 1.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x67, "melon-block", 5.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x68, "pumpkin-stem", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x69, "melon-stem", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x6A, "vines", 1.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x6B, "fence-gate", 15.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x6C, "brick-stairs", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x6D, "stone-brick-stairs", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x6E, "mycelium", 2.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x6F, "lily-pad", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x70, "nether-brick", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x71, "nether-brick-fence", 30.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x72, "nether-brick-stairs", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x73, "nether-wart", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x74, "enchantment-table", 6000.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x75, "brewing-stand", 2.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x76, "cauldron", 10.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x77, "end-portal", 18000000.0f, 0, 15, 64, true, BS_SOLID },
		{ 0x78, "end-portal-block", 18000000.0f, 15, 1, 64, true, BS_SOLID },
		{ 0x79, "endstone", 45.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x7A, "dragon-egg", 45.0f, 0, 1, 64, true, BS_SOLID },
		{ 0x7B, "redstone-lamp", 1.5f, 15, 0, 64, true, BS_SOLID },
		{ 0x7C, "redstone-lamp-on", 1.5f, 15, 15, 64, true, BS_SOLID },
		{ 0x7D, "wooden-dslab", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x7E, "wooden-slab", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x7F, "cocoa-block", 15.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x80, "sandstone-stairs", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x81, "emerald-ore", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x82, "ender-chest", 3000.0f, 0, 7, 64, true, BS_SOLID },
		{ 0x83, "tripwire-hook", 2.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x84, "tripwire", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x85, "emerald-block", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x86, "spruce-stairs", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x87, "birch-stairs", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x88, "jungle-stairs", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x89, "command-block", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x8A, "beacon", 15.0f, 0, 15, 64, true, BS_SOLID },
		{ 0x8B, "cobble-wall", 30.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x8C, "flower-pot", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x8D, "carrot-block", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x8E, "potato-block", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x8F, "wooden-button", 2.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x90, "mob-head", 5.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x91, "anvil", 6000.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x92, "trapped-chest", 12.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x93, "light-weighted-pressure-plate", 2.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x94, "heavy-weighted-pressure-plate", 2.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x95, "redstone-comparator", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x96, "redstone-comparator-on", 0.0f, 0, 9, 64, true, BS_SOLID },
		{ 0x97, "daylight-sensor", 1.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x98, "redstone-block", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x99, "quartz-ore", 15.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x9A, "hopper", 15.0f, 0, 0, 64, true, BS_SOLID },
		{ 0x9B, "quartz-block", 4.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x9C, "quartz-stairs", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x9D, "activator-rail", 3.5f, 0, 0, 64, true, BS_SOLID },
		{ 0x9E, "dropper", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0x9F, "stained-clay", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0xA0, "stained-glass-pane", 1.5f, 0, 0, 64, true, BS_SOLID },
		{ 0xA1, "leaves2", 1.0f, 1, 0, 64, true, BS_SOLID },
		{ 0xA2, "trunk2", 10.0f, 15, 0, 64, true, BS_SOLID },
		{ 0xA3, "acacia-stairs", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0xA4, "dark-oak-stairs", 30.0f, 15, 0, 64, true, BS_SOLID },
		{},
		{},
		{},
		{},
		{},
		{ 0xAA, "hay-bale", 10.0f, 15, 0, 64, true, BS_SOLID },
		{ 0xAB, "carpet", 0.0f, 0, 0, 64, true, BS_SOLID },
		{ 0xAC, "hardened-clay", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0xAD, "coal-block", 30.0f, 15, 0, 64, true, BS_SOLID },
		{ 0xAE, "packed-ice", 10.0f, 15, 0, 64, true, BS_SOLID },
	};
	
	static std::unordered_map<cistring, int> name_map {
		{ "air", 0x00 },
		{ "stone", 0x01 },
		{ "grass", 0x02 },
		{ "dirt", 0x03 },
		{ "cobble", 0x04 },
		{ "wood", 0x05 },
		{ "sapling", 0x06 },
		{ "bedrock", 0x07 },
		{ "water", 0x08 },
		{ "still-water", 0x09 },
		{ "lava", 0x0A },
		{ "still-lava", 0x0B },
		{ "sand", 0x0C },
		{ "gravel", 0x0D },
		{ "gold-ore", 0x0E },
		{ "iron-ore", 0x0F },
		{ "coal-ore", 0x10 },
		{ "trunk", 0x11 },
		{ "leaves", 0x12 },
		{ "sponge", 0x13 },
		{ "glass", 0x14 },
		{ "lapis-ore", 0x15 },
		{ "lapis-block", 0x16 },
		{ "dispenser", 0x17 },
		{ "sandstone", 0x18 },
		{ "note-block", 0x19 },
		{ "bed", 0x1A },
		{ "powered-rail", 0x1B },
		{ "detector-rail", 0x1C },
		{ "sticky-piston", 0x1D },
		{ "cobweb", 0x1E },
		{ "tall-grass", 0x1F },
		{ "dead-bush", 0x20 },
		{ "piston", 0x21 },
		{ "piston-extension", 0x22 },
		{ "wool", 0x23 },
		{ "piston-move", 0x24 },
		{ "dandelion", 0x25 },
		{ "rose", 0x26 },
		{ "brown-mushroom", 0x27 },
		{ "red-mushroom", 0x28 },
		{ "gold-block", 0x29 },
		{ "iron-block", 0x2A },
		{ "dslab", 0x2B },
		{ "slab", 0x2C },
		{ "bricks", 0x2D },
		{ "tnt", 0x2E },
		{ "bookshelf", 0x2F },
		{ "mossy-cobble", 0x30 },
		{ "obsidian", 0x31 },
		{ "torch", 0x32 },
		{ "fire", 0x33 },
		{ "monster-spawner", 0x34 },
		{ "stairs", 0x35 },
		{ "chest", 0x36 },
		{ "redstone-wire", 0x37 },
		{ "diamond-ore", 0x38 },
		{ "diamond-block", 0x39 },
		{ "workbench", 0x3A },
		{ "wheat-block", 0x3B },
		{ "farmland", 0x3C },
		{ "furnace", 0x3D },
		{ "burning-furnace", 0x3E },
		{ "sign-post", 0x3F },
		{ "wooden-door", 0x40 },
		{ "ladder", 0x41 },
		{ "rail", 0x42 },
		{ "cobble-stairs", 0x43 },
		{ "wall-sign", 0x44 },
		{ "lever", 0x45 },
		{ "stone-pressure-plate", 0x46 },
		{ "iron-door", 0x47 },
		{ "wooden-pressure-plate", 0x48 },
		{ "redstone-ore", 0x49 },
		{ "glowing-redstone-ore", 0x4A },
		{ "inactive-redstone-torch", 0x4B },
		{ "redstone-torch", 0x4C },
		{ "stone-button", 0x4D },
		{ "snow-cover", 0x4E },
		{ "ice", 0x4F },
		{ "snow-block", 0x50 },
		{ "cactus", 0x51 },
		{ "clay-block", 0x52 },
		{ "sugar-cane", 0x53 },
		{ "jukebox", 0x54 },
		{ "fence", 0x55 },
		{ "pumpkin", 0x56 },
		{ "netherrack", 0x57 },
		{ "soulsand", 0x58 },
		{ "glowstone", 0x59 },
		{ "nether-portal", 0x5A },
		{ "jack-o-lantern", 0x5B },
		{ "cake-block", 0x5C },
		{ "redstone-repeater", 0x5D },
		{ "redstone-repeater-on", 0x5E },
		{ "stained-glass", 0x5F },
		{ "trapdoor", 0x60 },
		{ "mosnter-egg", 0x61 },
		{ "stone-bricks", 0x62 },
		{ "huge-brown-mushroom", 0x63 },
		{ "huge-red-mushroom", 0x64 },
		{ "iron-bars", 0x65 },
		{ "glass-pane", 0x66 },
		{ "melon-block", 0x67 },
		{ "pumpkin-stem", 0x68 },
		{ "melon-stem", 0x69 },
		{ "vines", 0x6A },
		{ "fence-gate", 0x6B },
		{ "brick-stairs", 0x6C },
		{ "stone-brick-stairs", 0x6D },
		{ "mycelium", 0x6E },
		{ "lily-pad", 0x6F },
		{ "nether-brick", 0x70 },
		{ "nether-brick-fence", 0x71 },
		{ "nether-brick-stairs", 0x72 },
		{ "nether-wart", 0x73 },
		{ "enchantment-table", 0x74 },
		{ "brewing-stand", 0x75 },
		{ "cauldron", 0x76 },
		{ "end-portal", 0x77 },
		{ "end-porta-block", 0x78 },
		{ "endstone", 0x79 },
		{ "dragon-egg", 0x7A },
		{ "redstone-lamp", 0x7B },
		{ "redstone-lamp-on", 0x7C },
		{ "wooden-dslab", 0x7D },
		{ "wooden-slab", 0x7E },
		{ "cocoa-block", 0x7F },
		{ "sandstone-stairs", 0x80 },
		{ "emerald-ore", 0x81 },
		{ "ender-chest", 0x82 },
		{ "tripwire-hook", 0x83 },
		{ "tripwire", 0x84 },
		{ "emerald-block", 0x85 },
		{ "spruce-stairs", 0x86 },
		{ "birch-stairs", 0x87 },
		{ "jungle-stairs", 0x88 },
		{ "command-block", 0x89 },
		{ "beacon", 0x8A },
		{ "cobble-wall", 0x8B },
		{ "flower-pot", 0x8C },
		{ "carrot-block", 0x8D },
		{ "potato-block", 0x8E },
		{ "wooden-button", 0x8F },
		{ "mob-head", 0x90 },
		{ "anvil", 0x91 },
		{ "trapped-chest", 0x92 },
		{ "light-weighted-pressure-plate", 0x93 },
		{ "heavy-weighted-pressure-plate", 0x94 },
		{ "redstone-comparator", 0x95 },
		{ "redstone-comparator-on", 0x96 },
		{ "daylight-sensor", 0x97 },
		{ "redstone-block", 0x98 },
		{ "quartz-ore", 0x99 },
		{ "hopper", 0x9A },
		{ "quartz-block", 0x9B },
		{ "quartz-stairs", 0x9C },
		{ "activator-rail", 0x9D },
		{ "dropper", 0x9E },
		{ "stained-clay", 0x9F },
		{ "stained-glass-pane", 0xA0 },
		{ "leaves2", 0xA1 },
		{ "trunk2", 0xA2 },
		{ "acacia-stairs", 0xA3 },
		{ "dark-oak-stairs", 0xA4 },
		{},
		{},
		{},
		{},
		{},
		{ "hay-block", 0xAA },
		{ "carpet", 0xAB },
		{ "hardened-clay", 0xAC },
		{ "coal-block", 0xAD },
		{ "packed-ice", 0xAE },
	};
	
	// A metadata value of 0x10 signifies that the drop mapping should be applied
	// on all possible meta values.
	static std::unordered_map<blocki, blocki, blocki_hash> drop_map {
		{ {BT_GRASS, 0x10}, {BT_DIRT, 0} },
		{ {BT_STONE, 0x10}, {BT_COBBLE, 0} },
		
		{ {BT_TALL_GRASS, 0x10}, {} },
		{ {BT_LEAVES, 0x10}, {} },
		
		// ores:
		{ {BT_COAL_ORE, 0x10}, {IT_COAL, 0} },
		{ {BT_REDSTONE_ORE, 0x10}, {IT_REDSTONE_DUST, 0} },
		{ {BT_GLOWING_REDSTONE_ORE, 0x10}, {IT_REDSTONE_DUST, 0} },
		{ {BT_DIAMOND_ORE, 0x10}, {IT_DIAMOND, 0} },
	};
	
	
	
	/* 
	 * Returns the block_info structure describing the block associated with the
	 * specified ID number.
	 */
	block_info*
	block_info::from_id (unsigned short id)
	{
		block_info *binf = nullptr;
		if (id < block_list.size ())
			binf = &block_list[id];
		
		// handle physics blocks
		if (!binf)
			{
				static std::vector<block_info> phblock_list;
				physics_block *ph = physics_block::from_id (id);
				if (ph)
					{
						// add it to our list if it's not already there
						if (ph->id () >= (int)phblock_list.size ())
							phblock_list.resize (ph->id () + 1);
						if (phblock_list[id].id == 0)
							{
								// fill in using the associated vanilla block
								phblock_list[id] = block_list[ph->vanilla_block ().id];
								phblock_list[id].id = id;
							}
						
						return &phblock_list[id];
					}
			}
		
		return binf;
	}
	
	/* 
	 * Returns the block info structure that corresponds with the given block
	 * name.
	 */
	block_info*
	block_info::from_name (const char *name)
	{
		int id = -1;
		
		auto itr = name_map.find (name);
		if (itr != name_map.end ())
			id = itr->second;
		else
			{
				// handle physics blocks
				physics_block *ph = physics_block::from_name (name);
				if (ph)
					id = ph->id ();
			}
			
		if (id != -1)
			return block_info::from_id (id);
		return nullptr;
	}
	
	/* 
	 * Attempts to find the block info structure associated with the specified
	 * string. Inputs such as "cobble" and "4" will both return the same result.
	 */
	block_info*
	block_info::from_id_or_name (const char *str)
	{
		// check if number
		bool is_num = true;
		const char *ptr = str;
		while (*ptr)
			if (!std::isdigit (*ptr++))
				{ is_num = false; break; }
		
		if (is_num)
			{
				std::istringstream ss (str);
				int num;
				ss >> num;
				return block_info::from_id (num);
			}
		return block_info::from_name (str);
	}
	
	
	
	/* 
	 * Gets the block that should be dropped after destroying blocks of type 
	 * @{bl}.
	 */
	blocki
	block_info::get_drop (blocki bl)
	{
		auto itr = drop_map.find (bl);
		if (itr == drop_map.end ())
			return bl;
		return itr->second;
	}
}

