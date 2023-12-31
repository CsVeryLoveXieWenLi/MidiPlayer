/**
 * @brief 初始化
 */

#pragma once

#include <unordered_map>
#include <string>
#include <future>

#include <llapi/mc/CommandOrigin.hpp>
#include <llapi/mc/CommandOutput.hpp>

#include <llapi/LoggerAPI.h>
#include <llapi/EventAPI.h>
#include <llapi/DynamicCommandAPI.h>

#include "static.h"
#include "output.h"
#include "task.h"
#include "midi.h"
#include "packet.h"

using namespace std;
using namespace PacketUtils;

/**
 * @brief 内部回调 实现功能
 */
namespace _CallBackAndFunc {
	// 音符命名空间
	vector<string> NOTE_NAMES{"air", "midi.piano.A0", "midi.piano.Bb0", "midi.piano.B0", "midi.piano.C1", "midi.piano.Db1", "midi.piano.D1", "midi.piano.Eb1", "midi.piano.E1", "midi.piano.F1", "midi.piano.Gb1", "midi.piano.G1", "midi.piano.Ab1", "midi.piano.A1", "midi.piano.Bb1", "midi.piano.B1", "midi.piano.C2", "midi.piano.Db2", "midi.piano.D2", "midi.piano.Eb2", "midi.piano.E2", "midi.piano.F2", "midi.piano.Gb2", "midi.piano.G2", "midi.piano.Ab2", "midi.piano.A2", "midi.piano.Bb2", "midi.piano.B2", "midi.piano.C3", "midi.piano.Db3", "midi.piano.D3", "midi.piano.Eb3", "midi.piano.E3", "midi.piano.F3", "midi.piano.Gb3", "midi.piano.G3", "midi.piano.Ab3", "midi.piano.A3", "midi.piano.Bb3", "midi.piano.B3", "midi.piano.C4", "midi.piano.Db4", "midi.piano.D4", "midi.piano.Eb4", "midi.piano.E4", "midi.piano.F4", "midi.piano.Gb4", "midi.piano.G4", "midi.piano.Ab4", "midi.piano.A4", "midi.piano.Bb4", "midi.piano.B4", "midi.piano.C5", "midi.piano.Db5", "midi.piano.D5", "midi.piano.Eb5", "midi.piano.E5", "midi.piano.F5", "midi.piano.Gb5", "midi.piano.G5", "midi.piano.Ab5", "midi.piano.A5", "midi.piano.Bb5", "midi.piano.B5", "midi.piano.C6", "midi.piano.Db6", "midi.piano.D6", "midi.piano.Eb6", "midi.piano.E6", "midi.piano.F6", "midi.piano.Gb6", "midi.piano.G6", "midi.piano.Ab6", "midi.piano.A6", "midi.piano.Bb6", "midi.piano.B6", "midi.piano.C7", "midi.piano.Db7", "midi.piano.D7", "midi.piano.Eb7", "midi.piano.E7", "midi.piano.F7", "midi.piano.Gb7", "midi.piano.G7", "midi.piano.Ab7", "midi.piano.A7", "midi.piano.Bb7", "midi.piano.B7", "midi.piano.C8"};

	// 全局正在播放的玩家队列
	MidiTask MIDI_TASKS;

	/**
	 * @brief 命令功能 -> midi/mid -> play
	 */
	void CommandMidiPlay(ServerPlayer* player, string&& xuid, string&& path) {
		xuid = (string)xuid;
		path = (string)path;

		if (MIDI_TASKS.in(xuid)) {
			CmdOutUtils::error(player, "播放失败 你正在播放别的MIDI");
			return;
		}

		// 解析文件
		vector<int> midi = MidiUtils::parse(path);
		size_t midi_length = midi.size();

		if (midi_length == 0) {
			CmdOutUtils::error(player, "播放失败 MIDI文件不对劲");
			return;
		}

		// IO函数 防止玩家离开
		if (player == nullptr) {
			return;
		}

		// 播放
		MIDI_TASKS.add(xuid);
		CmdOutUtils::success(player, "播放成功 正在播放MIDI");
		
		auto start = clock();
		auto now = start;

		TaskPacket send;
		
		for (size_t index = 0; index < midi_length; index += 2)
		{
			int note = midi[index];
			string sound = NOTE_NAMES[note - 20];

			int atime = midi[index + 1];

			while (true)
			{
				// TIME?
				now = clock();
				if (now - start >= atime) {
					break;
				}

				// PLAY IN?
				if (!MIDI_TASKS.in(xuid))
				{
					return;
				}

				// CPU AIR
				// this_thread::sleep_for(chrono::nanoseconds(1));
			}

			if (player == nullptr) {
				MIDI_TASKS.delSafe(xuid);
				return;
			}

			Vec3 pos = player->getPosition();
			send.submitPlaySoundTask(player, sound, pos, 1.0, 1.0);
			// sendPlaySoundPacket(player, sound, pos, 1.0, 1.0);
		}

		// STOP
		MIDI_TASKS.delSafe(xuid);
	}

	/**
	 * @brief 命令功能 -> midi/mid -> stop
	 */
	void CommandMidiStop(ServerPlayer* player, string& xuid) {
		if (MIDI_TASKS.in(xuid)) {
			MIDI_TASKS.del(xuid);
			CmdOutUtils::success(player, "停止播放成功");
		}
		else
		{
			CmdOutUtils::error(player, "停止播放失败 你没有在播放MIDI");
		}
	}

	/**
	 * @brief 命令回调 -> midi/mid
	 */
	void CommandMidi(
		DynamicCommand const& command, 
		CommandOrigin const& origin, 
		CommandOutput& output,
		unordered_map<string, DynamicCommand::Result>& results
	) {
		auto player = origin.getPlayer();

		if (player == nullptr) {
			CmdOutUtils::error(player, "仅限玩家使用该命令");
			return;
		}

		string xuid = player->getXuid();
		string func = results.at("func").get<string>();

		if (func.compare("play") == 0) {

			string path = results.at("path").get<string>();
			thread(CommandMidiPlay, player, move(xuid), move(path)).detach();

		}else if (func.compare("stop") == 0) {

			CommandMidiStop(player, xuid);

		}
	}
}

/**
 * @brief 初始化工具
 */
namespace InitUtils {
	/**
	 * @brief 初始化 -> 注册命令
	 */
	void RegisterCommand() {
		using ParamType = DynamicCommand::ParameterType;
		using Param = DynamicCommand::ParameterData;

		// 创建命令
		unique_ptr<DynamicCommandInstance> command = DynamicCommand::createCommand(
			PLUGIN_COMMAND_NAME, 
			PLUGIN_INTRODUCTION, 
			CommandPermissionLevel::Any, 
			{ (CommandFlagValue)0x80 }, 
			{ (CommandFlagValue)1 }
		);

		// 别名
		command->setAlias(PLUGIN_COMMAND_NAME_A);

		// 枚举
		command->setEnum("play", { "play" });
		command->setEnum("stop", { "stop" });

		// 参数
		// CommandParameterOption::EnumAutocompleteExpansion -> 必选
		command->mandatory("path", ParamType::String, CommandParameterOption::EnumAutocompleteExpansion);

		command->mandatory("func", ParamType::Enum, "play", CommandParameterOption::EnumAutocompleteExpansion);
		command->mandatory("func", ParamType::Enum, "stop", CommandParameterOption::EnumAutocompleteExpansion);

		// 重载命令 -> 组合
		command->addOverload({ "play", "path" });
		command->addOverload({ "stop" });

		// 回调 处理功能
		command->setCallback(&_CallBackAndFunc::CommandMidi);

		// 装载命令 -> why move? -> this is unique_ptr
		DynamicCommand::setup(move(command));
	}

	/**
	 * @brief 初始化 -> 注册监听
	 */
	void RegisterEvent() {
		Event::PlayerLeftEvent::subscribe([](const Event::PlayerLeftEvent& event) {
			string xuid = event.mXUID;
			_CallBackAndFunc::MIDI_TASKS.delSafe(xuid);
			return true;
		});
	}
}
