/**
 * @brief MIDI
 */

#pragma once

#include <vector>
#include <string>

#include <llapi/utils/NetworkHelper.h>

#include "midi/MidiFile.h"

using namespace std;
using namespace smf;

/**
 * @brief MIDI工具
 */
namespace MidiUtils {
	/**
	 * @brief file [path, url] -> 编译MIDI文件数据到PlaySound -> [note, time]
	 */
	vector<int> parse(string& path) {
		MidiFile file;
		vector<int> result;

		// 读取文件
		file.read(path);

		if (!file.status()) {
			return result;
		}

		// 合并音轨 -> 计算真实时间(s)
		file.joinTracks();
		file.doTimeAnalysis();

		// 计算到 PlaySound
		MidiEvent* event;

		MidiEventList* tracks = &file[0];
		int length = tracks->size();

		for (int index = 0; index < length; index++) {
			event = &(*tracks)[index];

			// 只记录音符按下
			if (!event->isNoteOn()) continue;

			// 事件0 音高1 力度2
			int8_t note = (int8_t)(*event)[1];

			// 绝对时间 ms -> 至少为50ms
			int32_t absolute_time = (int32_t)round(event->seconds * 1000);
			// if (absolute_time < 50) absolute_time = 50;

			result.push_back(note);
			result.push_back(absolute_time);
		}

		return result;

	}
}
