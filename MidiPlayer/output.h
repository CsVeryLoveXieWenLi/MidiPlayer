/**
 * @brief 输出
 */

#pragma once

#include <llapi/LoggerAPI.h>

using namespace std;

/**
 * @brief 格式化输出
 */
namespace CmdOutUtils {
	string PREFIX = "§r§l[MidiPlayer]§r";

	/**
	 * @brief 成功 -> §a
	 */
	void success(ServerPlayer* player, const string& message) {
		string data = format("{}§l§a {}", PREFIX, message);
		player->sendTextPacket(data);
	}

	/**
	 * @brief 失败 -> §c
	 */
	void error(ServerPlayer* player, const string& message) {
		string data = format("{}§l§c {}", PREFIX, message);
		player->sendTextPacket(data);
	}

	/**
	 * @brief 警告 -> §b
	 */
	void warn(ServerPlayer* player, const string& message) {
		string data = format("{}§l§b {}", PREFIX, message);
		player->sendTextPacket(data);
	}
}
