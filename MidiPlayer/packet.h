/**
 * @brief 网络包
 */

#pragma once

#include <condition_variable>
#include <mutex>

#include <llapi/mc/BinaryStream.hpp>
#include <llapi/mc/MinecraftPackets.hpp>
#include <llapi/mc/Packet.hpp>

using namespace std;

/**
 * @brief 封包
 */
namespace PacketUtils {
	/**
	 * @brief 立即播放声音
	 */
	void sendPlaySoundPacket(ServerPlayer* player, string& soundName, Vec3& position, float volume, float pitch)
	{
		BinaryStream bin;

		bin.writeString(soundName);

		bin.writeVarInt(position.x * 8);
		bin.writeUnsignedVarInt(position.y * 8);
		bin.writeVarInt(position.z * 8);

		bin.writeFloat(volume);
		bin.writeFloat(pitch);

		shared_ptr<Packet> packet = MinecraftPackets::createPacket(MinecraftPacketIds::PlaySound);
		packet->read(bin);

		if (player == nullptr || packet == nullptr) {
			return;
		}
		player->sendNetworkPacket(*packet);
	}

	/**
	 * @brief 停止播放声音
	 */
	void sendStopSoundPacket(ServerPlayer* player, string& soundName, bool stopAll)
	{
		BinaryStream bin;
		bin.writeString(soundName);
		bin.writeBool(stopAll);

		shared_ptr<Packet> packet = MinecraftPackets::createPacket(MinecraftPacketIds::StopSound);
		packet->read(bin);

		if (player == nullptr || packet == nullptr) {
			return;
		}
		player->sendNetworkPacket(*packet);
	}

	/**
	 * @brief 发包队列
	 */
	class TaskPacket
	{
	public:
		/**
		 * @brief 初始化
		 */
		TaskPacket() {
			close = false;
			thread(bind(&TaskPacket::handler, this)).detach();
		};

		/**
		 * @brief 析构
		 */
		~TaskPacket() {
			close = true;
		};

		/**
		 * @brief 提交 -> 立即播放声音
		 */
		void submitPlaySoundTask(ServerPlayer* player, string& soundName, Vec3& position, float volume, float pitch) {
			packaged_task<void()> task(
				bind(sendPlaySoundPacket, player, soundName, position, volume, pitch)
			);

			{
				lock_guard locker(lock);
				tasks.push_back(move(task));
			}

			cv.notify_one();
		};

		/**
		 * @brief 提交 -> 停止播放声音
		 */
		void submitStopSoundTask(ServerPlayer* player, string& soundName, bool stopAll) {
			packaged_task<void()> task(
				bind(sendStopSoundPacket, player, soundName, stopAll)
			);

			{
				lock_guard locker(lock);
				tasks.push_back(move(task));
			}

			cv.notify_one();
		};

	private:
		condition_variable cv;
		mutex lock;
		bool close;

		deque<packaged_task<void()>> tasks;

		/**
		 * @brief 线程执行函数
		 */
		void handler() {
			packaged_task<void()> task;

			while (true)
			{
				{
					if (this->close) {
						return;
					}
					if (this->tasks.empty()) {
						continue;
					}

					task = move(this->tasks.front());

					this->tasks.pop_front();
				}

				task();
			}
		};
	};

}
