#pragma once

#include "Library.h"

namespace DpLib {
	class DpKey {
	public:
		enum State {
			IDLE,
			PRESS,
			HOLD,
			RELEASE
		};
		class KeyFSMinstance {
		public:
			KeyFSMinstance(State s, KeyFSMinstance* (DpKey::* u)(int vkey, std::chrono::steady_clock::time_point now)) {
				state = s;
				update = u;
			}
		public:
			State state = IDLE;
			KeyFSMinstance* (DpKey::* update)(int vkey, std::chrono::steady_clock::time_point now);
		};
	public:
		KeyFSMinstance* current = nullptr;
		std::vector < KeyFSMinstance* > StateTable;
		KeyFSMinstance* (DpKey::* update)(int vkey, std::chrono::steady_clock::time_point now);
	public:

		DpKey() {
			StateTable.push_back(new KeyFSMinstance(IDLE, &DpKey::updateIdle));
			StateTable.push_back(new KeyFSMinstance(PRESS, &DpKey::updatePress));
			StateTable.push_back(new KeyFSMinstance(HOLD, &DpKey::updateHold));
			StateTable.push_back(new KeyFSMinstance(RELEASE, &DpKey::updateRelease));
			current = StateTable[0];
			update = current->update;
		}
		~DpKey() {
			for (auto& s : StateTable) {
				delete s;
			}
		}
	public:
		void updateFSM(int vkey, std::chrono::steady_clock::time_point now) {
			current = (this->*update)(vkey, now);
			update = current->update;
		}
	public:

		std::chrono::steady_clock::time_point lastTime;
		std::chrono::steady_clock::duration holdTime;

	private:
		KeyFSMinstance* updateIdle(int vkey, std::chrono::steady_clock::time_point now) {
			if (GetAsyncKeyState(vkey) & 0x8000) {
				return StateTable[1];
			}
			return StateTable[0];
		}
		KeyFSMinstance* updatePress(int vkey, std::chrono::steady_clock::time_point now) {
			if (GetAsyncKeyState(vkey) & 0x8000) {
				lastTime = now;
				return StateTable[2];
			}
			return StateTable[3];
		}
		KeyFSMinstance* updateHold(int vkey, std::chrono::steady_clock::time_point now) {
			if (GetAsyncKeyState(vkey) & 0x8000) {
				holdTime = now - lastTime;
				return StateTable[2];
			}
			return StateTable[3];
		}
		KeyFSMinstance* updateRelease(int vkey, std::chrono::steady_clock::time_point now) {
			holdTime = now - lastTime;
			return StateTable[0];
		}

	public:
		int vkey;
		void run() {
			updateFSM(vkey, std::chrono::steady_clock::now());
		}
		bool isPressed() {
			return current->state == PRESS;
		}
		bool isHold() {
			return current->state == HOLD;
		}
		bool isReleased() {
			return current->state == RELEASE;
		}
		std::chrono::steady_clock::duration getHoldTime() {
			return holdTime;
		}
	};
}