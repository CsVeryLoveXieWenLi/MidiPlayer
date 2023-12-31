/**
 * @brief 任务队列
 */

#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

using namespace std;

/**
 * @brief 播放队列
 */
class MidiTask
{
private:
	// unordered_map || vector || unordered_set
	unordered_set<string> list;
	mutex threadLock;
	
public:
	/**
	 * @brief 析构
	 */
	~MidiTask() {
		list.clear();
	};

	/**
	 * @brief 玩家是否在播放队列内
	 */
	bool in(string& xuid)
	{
		return list.find(xuid) != list.end();
	};

	/**
	 * @brief 向播放队列中添加玩家 -> 玩家必须不存在队列中
	 */
	void add(string& xuid)
	{
		threadLock.lock();
		list.insert(xuid);
		threadLock.unlock();
	};

	/**
	 * @brief 从播放队列中删除玩家 -> 玩家必须存在于队列
	 */
	void del(string& xuid)
	{
		threadLock.lock();
		list.erase(xuid);
		threadLock.unlock();
	};

	/**
	 * @brief 从播放队列中删除玩家 -> 安全的删除 -> 玩家不存在不删除
	 */
	void delSafe(string& xuid)
	{
		if (in(xuid)) {
			del(xuid);
		}
	};

	/**
	 * @brief 向播放队列中添加玩家 -> 安全的添加 -> 玩家存在不添加
	 */
	void addSafe(string& xuid)
	{
		if (!in(xuid)) {
			add(xuid);
		}
	};
};
